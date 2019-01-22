
#pragma once

#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

Mat preprocessing(Mat img)
{
	int coin_no = 20;
	CV_Assert(img.data);
	Mat gray, th_img;

	cvtColor(img, gray, CV_BGR2GRAY);
	GaussianBlur(gray, gray, Size(7, 7), 2, 2);

	threshold(gray, th_img, 130, 255, THRESH_BINARY_INV | THRESH_OTSU);
	morphologyEx(th_img, th_img, MORPH_OPEN, Mat(), Point(-1, -1), 1);

	return th_img;
}


vector<RotatedRect> find_holl(Mat img, int min, int max)
{
	vector<vector<Point> > contours;
	findContours(img.clone(), contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	vector<RotatedRect> circles1;
	for (int i = 0; i < (int)contours.size(); i++)
	{
		RotatedRect mr = minAreaRect(contours[i]);
		mr.angle = (mr.size.width + mr.size.height) / 4.0f;

		if (mr.angle > min && mr.angle < max) circles1.push_back(mr);
	}
	return circles1;
}

cv::Mat find_circles(Mat th_img, Mat _image, int min, int max, int B, int G, int R, int H)
{
Mat circle_image = _image;

vector<RotatedRect> circles1 = find_holl(th_img, min, max);

for (int i = 0; i<circles1.size(); i++)
{
	float radius = circles1[i].angle;
	circle(circle_image, circles1[i].center, radius, Scalar(B, G, R), H);

}


		waitKey();
return circle_image;
};
