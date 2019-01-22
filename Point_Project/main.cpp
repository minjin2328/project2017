#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <vector>

#include "find_circle.h"
#include "find_color.h"
#include "center_point.h"

using namespace cv;

int main() {

	//////////////////////////////////////////// �̹��� �ҷ����� /////////////////////////////////

	cv::Mat image1 = cv::imread("C:/Users/TaeWon/Documents/Visual Studio 2017/OpenCVTest/33_ggg50.jpg"); //RGB�̹����� �о���δ�.
	
	///////////////////////csv ���� ����///////////////////////////////

	FILE *out;

	out = fopen("output.csv", "w");      //������ ���� ��� ������Ʈ �Ӽ� - ��ó���� - ��ó���� ���ǿ�"_CRT_SECURE_NO_WARNINGS"�� �־��ּ���.

	fprintf(out, "Y,X,pixY,pixX,Name\n");
																									 
	//////////////////////////////////////////// ���� �� ���� /////////////////////////////////
	

	Mat detected_pavement= find_colors(image1, 12, 38, 27, 255, 100, 130, 30, 255);  //�÷� ���� (�Է� �̹��� , int _minHue01, int _maxHue01, int _minSat01, int _maxSat01, int _minHue02, int _maxHue02, int _minSat02, int _maxSat02) �� 2���� �������� ���� ���Ұ��� �� �ڸ� ���������� ����

	/*imwrite("C:/Users/TaeWon/Documents/Visual Studio 2017/OpenCVTest/pavements.jpg", detected_pavement);*/  //������ �̹��� ����


	 //////////////////////////////////////////// ����� ����  /////////////////////////////////
	Mat th_img01 = preprocessing(image1);  //��ó���� ���� �ڵ�

	/*imwrite("C:/Users/TaeWon/Documents/Visual Studio 2017/OpenCVTest/find_bolad_mask.jpg", th_img01);*/

	cv::Mat circle_image = image1.clone();                                                  //�������� �������ϸ� ������ �Ѽյ����� ���� �ʼ� 
	Mat circle01 = find_circles(th_img01, circle_image, 80, 120, 0, 255, 0, 20);				//find_circle(Mat th_img, Mat _image, int min, int max, int B, int G, int R, int H)

	imwrite("C:/Users/TaeWon/Documents/Visual Studio 2017/OpenCVTest//find_bolad1.jpg", circle01);   //������ ����� ���� 



	//////////////////////////////////////////// ����� ����� �� ���� ����  /////////////////////////////////
	
	Mat detected_bolad = find_colors(circle01, 48, 62, 200, 255, 48, 62, 200, 255);  //�÷� ���� (�Է� �̹��� , int _minHue01, int _maxHue01, int _minSat01, int _maxSat01, int _minHue02, int _maxHue02, int _minSat02, int _maxSat02) �� 2���� �������� ���� ���Ұ��� �� �ڸ� ���������� ����
	
	/*imwrite("C:/Users/TaeWon/Documents/Visual Studio 2017/OpenCVTest//bolad_circle.jpg", detected_bolad); */         //����� �� ������ ����
	//////////////////////////////////////////// ����� ����� ���� ����  ///////////////////////////////// 

	Mat bolad_point = find_points(detected_bolad, out, "bolad1");   //�߽��� ����  (�̹�����,csv��, csv�� ǥ���� ��ֹ� �� )




	//////////csv ���� �ݱ�/////////////////////
	fclose(out);  // csv �ҷ��°��� �ݵ�� �ݾ���� �Ѵ�.





}	