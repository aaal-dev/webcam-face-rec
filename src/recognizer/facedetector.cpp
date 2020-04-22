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
		
		cv::Mat face_roi = frame( face_rect );
		eye_classifier.detectMultiScale(face_roi, face.eyes_rects, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE | cv::CASCADE_FIND_BIGGEST_OBJECT, cv::Size(30, 30));
		std::vector<cv::Rect> lefteye_rects;
		lefteye_classifier.detectMultiScale(face_roi, lefteye_rects, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE | cv::CASCADE_FIND_BIGGEST_OBJECT, cv::Size(30, 30));
		std::vector<cv::Rect> righteye_rects;
		righteye_classifier.detectMultiScale(face_roi, righteye_rects, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE | cv::CASCADE_FIND_BIGGEST_OBJECT, cv::Size(30, 30));
		
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
