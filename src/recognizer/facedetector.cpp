#include "facedetector.hpp"

FaceDetector::FaceDetector()
{
	face_classifier.load("../../../data/haarcascade_frontalface_alt.xml");
	eye_classifier.load("../../../data/haarcascade_eye.xml");
	lefteye_classifier.load("../../../data/haarcascade_lefteye_2splits.xml");
	righteye_classifier.load("../../../data/haarcascade_righteye_2splits.xml");
	landmarksDetector = new LandmarksDetector("../../../data/shape_predictor_68_face_landmarks.dat");
}

FaceDetector::~FaceDetector(){}

template <>
std::vector<Face> FaceDetector::detect_faces<dlib::bgr_pixel>(const cv::Mat &frame)
{
	std::vector<Face> faces;
	std::vector<cv::Rect> faces_rects;
	face_classifier.detectMultiScale(frame, faces_rects, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE | cv::CASCADE_FIND_BIGGEST_OBJECT, cv::Size(150, 150));
	for (auto &face_rect : faces_rects)
	{
		Face face;
		face.frame = frame;
		face.face_rect = face_rect;
		face.landmarks = landmarksDetector->detect_landmarks<dlib::bgr_pixel>(frame, face_rect);
		face.nose_base = get_nose_base(face);
		face.nose_tip = get_nose_tip(face);
		face.mouth_base = get_mouth_base(face);
		face.rotation = detect_face_gaze(face);
		
		cv::Mat face_roi = frame( face_rect );
		eye_classifier.detectMultiScale(face_roi, face.eyes_rects, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE | cv::CASCADE_FIND_BIGGEST_OBJECT, cv::Size(15, 15));
		std::vector<cv::Rect> lefteye_rects;
		lefteye_classifier.detectMultiScale(face_roi, face.lefteye_rects, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE | cv::CASCADE_FIND_BIGGEST_OBJECT, cv::Size(15, 15));
		std::vector<cv::Rect> righteye_rects;
		righteye_classifier.detectMultiScale(face_roi, face.righteye_rects, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE | cv::CASCADE_FIND_BIGGEST_OBJECT, cv::Size(15, 15));
		
		faces.push_back(face);
	}
	return faces;
}

template <>
std::vector<Face> FaceDetector::detect_faces<unsigned char>(const cv::Mat &frame)
{
	std::vector<Face> faces;
	std::vector<cv::Rect> faces_rects;
	face_classifier.detectMultiScale(frame, faces_rects, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE | cv::CASCADE_FIND_BIGGEST_OBJECT, cv::Size(150, 150));
	for (auto &face_rect : faces_rects)
	{
		Face face;
		face.frame = frame;
		face.face_rect = face_rect;
		face.landmarks = landmarksDetector->detect_landmarks<unsigned char>(frame, face_rect);
		face.nose_base = get_nose_base(face);
		face.nose_tip = get_nose_tip(face);
		face.mouth_base = get_mouth_base(face);
		face.rotation = detect_face_gaze(face);
		faces.push_back(face);
	}
	return faces;
}

cv::Point FaceDetector::get_nose_base(const Face &face)
{
	cv::Point leftEye = face.landmarks[36];
	cv::Point rightEye = face.landmarks[45];
	return cv::Point((leftEye.x+rightEye.x)/2, (leftEye.y+rightEye.y)/2);
}

cv::Point FaceDetector::get_nose_tip(const Face& face)
{
	return face.landmarks[30];
}

cv::Point FaceDetector::get_mouth_base(const Face &face)
{
	cv::Point mouthLeft = face.landmarks[48];
	cv::Point mouthRight = face.landmarks[54];
	return cv::Point((mouthLeft.x+mouthRight.x)/2, (mouthLeft.y+mouthRight.y)/2);
}

std::array<float,3> FaceDetector::detect_face_gaze(const Face &face)
{
	std::array<cv::Point3d, 6> reference_points;
	reference_points[0] = cv::Point3d(0.0f, 0.0f, 0.0f);           // Nose tip
	reference_points[1] = cv::Point3d(0.0f, -330.0f, -65.0f);      // Chin
	reference_points[2] = cv::Point3d(-225.0f, 170.0f, -135.0f);   // Left eye left corner
	reference_points[3] = cv::Point3d(225.0f, 170.0f, -135.0f);    // Right eye right corner
	reference_points[4] = cv::Point3d(-150.0f, -150.0f, -125.0f);  // Left Mouth corner
	reference_points[5] = cv::Point3d(150.0f, -150.0f, -125.0f);   // Right mouth corner
	
	std::array<cv::Point2d, 6> rigging_landmarks;
	rigging_landmarks[0] = cv::Point2d(face.landmarks[30]);    // Nose tip
	rigging_landmarks[1] = cv::Point2d(face.landmarks[8]);     // Chin
	rigging_landmarks[2] = cv::Point2d(face.landmarks[45]);    // Left eye left corner
	rigging_landmarks[3] = cv::Point2d(face.landmarks[36]);    // Right eye right corner
	rigging_landmarks[4] = cv::Point2d(face.landmarks[54]);    // Left Mouth corner
	rigging_landmarks[5] = cv::Point2d(face.landmarks[48]);    // Right mouth corner
	
	double focal_length = face.frame.cols;
	cv::Point2d center = cv::Point2d(face.frame.cols / 2, face.frame.rows / 2);
	cv::Mat camera_matrix = (cv::Mat_<double>(3, 3) << focal_length, 0, center.x, 0, focal_length, center.y, 0, 0, 1);
	cv::Mat dist_coeffs = cv::Mat::zeros(4, 1, cv::DataType<double>::type); // Assuming no lens distortion
	
	cv::Mat rotation_vector;
	cv::Mat translation_vector;
	
	cv::solvePnP(reference_points, rigging_landmarks, camera_matrix, dist_coeffs, rotation_vector, translation_vector);
	
	cv::Mat rot_mat;
	cv::Rodrigues(rotation_vector, rot_mat);
	
	double x, y, z;
	double sy = sqrt(rot_mat.at<double>(0,0) * rot_mat.at<double>(0,0) +  rot_mat.at<double>(1,0) * rot_mat.at<double>(1,0) );
	
	bool singular = sy < 1e-6;
	
	if (!singular)
	{
		x = atan2(rot_mat.at<double>(2,1) , rot_mat.at<double>(2,2));
		y = atan2(-rot_mat.at<double>(2,0), sy);
		z = atan2(rot_mat.at<double>(1,0), rot_mat.at<double>(0,0));
	} else {
		x = atan2(-rot_mat.at<double>(1,2), rot_mat.at<double>(1,1));
		y = atan2(-rot_mat.at<double>(2,0), sy);
		z = 0;
	}
	return {(float)x, (float)y, (float)z};
}