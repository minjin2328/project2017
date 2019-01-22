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

	//////////////////////////////////////////// 이미지 불러오기 /////////////////////////////////

	cv::Mat image1 = cv::imread("C:/Users/TaeWon/Documents/Visual Studio 2017/OpenCVTest/33_ggg50.jpg"); //RGB이미지를 읽어들인다.
	
	///////////////////////csv 파일 생성///////////////////////////////

	FILE *out;

	out = fopen("output.csv", "w");      //오류가 나는 경우 프로젝트 속성 - 전처리기 - 전처리기 정의에"_CRT_SECURE_NO_WARNINGS"를 넣어주세요.

	fprintf(out, "Y,X,pixY,pixX,Name\n");
																									 
	//////////////////////////////////////////// 보도 블럭 검출 /////////////////////////////////
	

	Mat detected_pavement= find_colors(image1, 12, 38, 27, 255, 100, 130, 30, 255);  //컬러 추출 (입력 이미지 , int _minHue01, int _maxHue01, int _minSat01, int _maxSat01, int _minHue02, int _maxHue02, int _minSat02, int _maxSat02) 순 2가지 색상으로 검출 안할경우는 앞 뒤를 같은색으로 설정

	/*imwrite("C:/Users/TaeWon/Documents/Visual Studio 2017/OpenCVTest/pavements.jpg", detected_pavement);*/  //보도블럭 이미지 저장


	 //////////////////////////////////////////// 볼라드 검출  /////////////////////////////////
	Mat th_img01 = preprocessing(image1);  //전처리를 위한 코드

	/*imwrite("C:/Users/TaeWon/Documents/Visual Studio 2017/OpenCVTest/find_bolad_mask.jpg", th_img01);*/

	cv::Mat circle_image = image1.clone();                                                  //원본에서 색검출하면 원본이 훼손됨으로 복사 필수 
	Mat circle01 = find_circles(th_img01, circle_image, 80, 120, 0, 255, 0, 20);				//find_circle(Mat th_img, Mat _image, int min, int max, int B, int G, int R, int H)

	imwrite("C:/Users/TaeWon/Documents/Visual Studio 2017/OpenCVTest//find_bolad1.jpg", circle01);   //원으로 볼라드 검출 



	//////////////////////////////////////////// 검출된 볼라드 원 색상 검출  /////////////////////////////////
	
	Mat detected_bolad = find_colors(circle01, 48, 62, 200, 255, 48, 62, 200, 255);  //컬러 추출 (입력 이미지 , int _minHue01, int _maxHue01, int _minSat01, int _maxSat01, int _minHue02, int _maxHue02, int _minSat02, int _maxSat02) 순 2가지 색상으로 검출 안할경우는 앞 뒤를 같은색으로 설정
	
	/*imwrite("C:/Users/TaeWon/Documents/Visual Studio 2017/OpenCVTest//bolad_circle.jpg", detected_bolad); */         //검출된 원 색으로 검출
	//////////////////////////////////////////// 검출된 볼라드 색상 검출  ///////////////////////////////// 

	Mat bolad_point = find_points(detected_bolad, out, "bolad1");   //중심점 검출  (이미지명,csv명, csv에 표시할 장애물 명 )




	//////////csv 파일 닫기/////////////////////
	fclose(out);  // csv 불러온것은 반드시 닫아줘야 한다.





}	