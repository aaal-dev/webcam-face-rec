#include "facedetector.hpp"

FaceDetector::FaceDetector()
{
	face_classifier.load("../../../data/haarcascade_frontalface_alt.xml");
	landmarksDetector = new LandmarksDetector("../../../data/shape_predictor_68_face_landmarks.dat");
}

FaceDetector::~FaceDetector(){}

template <typename T>
std::vector<Face> FaceDetector::detect_faces(const cv::Mat &frame)
{
	std::vector<Face> faces;
	std::vector<cv::Rect> faces_rects;
	face_classifier.detectMultiScale(frame, faces_rects, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE | cv::CASCADE_FIND_BIGGEST_OBJECT, cv::Size(150, 150));
	for (auto &face_rect : faces_rects)
	{
		Face face;
		face.frame = frame;
		face.face_rect = face_rect;
		face.landmarks = landmarksDetector->detect_landmarks<T>(frame, face_rect);
		face.nose_base = get_nose_base(face);
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

cv::Point FaceDetector::get_mouth_base(const Face &face)
{
	cv::Point mouthLeft = face.landmarks[48];
	cv::Point mouthRight = face.landmarks[54];
	return cv::Point((mouthLeft.x+mouthRight.x)/2, (mouthLeft.y+mouthRight.y)/2);
}
