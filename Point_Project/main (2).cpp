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
#include "resize.h"
#include "cannyEdge.h"
#include "find_polygon.h"
using namespace cv;

int main() {
	

	/////////////////////////////////본코드 //////////////////////////
	////csv 파일 생성////
	FILE *out;
	out = fopen("output.csv", "w");                     //오류가 나는 경우 프로젝트 속성 - 전처리기 - 전처리기 정의에"_CRT_SECURE_NO_WARNINGS"를 넣어주세요.
	fprintf(out, "Y,X,pixY,pixX,Name,No,PtColor\n");
	
	
	
	//// 이미지 불러오기 ////													
	cv::Mat image_0 = cv::imread("ggg50.jpg"); //RGB이미지를 읽어들인다.

	/////이미지 리사이즈 ////
	Mat image_R = image_resize(image_0);
	//imwrite("resize_image.jpg", image_R);              //0.33배로 축소한 이미지 저장
				   ////외각선 추출용 캐니엣지////
	Mat image_R_canny = Canny_Edge(image_R,170,150,3);
	imwrite("image_R_canny.jpg", image_R_canny);             //리사이즈 한 이미지 캐니엣지 - 인도 외각 추출용
	
	//// 보도 블럭 검출 ////
	Mat detected_pavement= find_colors(image_R, 12, 38, 27, 255, 100, 130, 30, 255);  //컬러 추출 (입력 이미지 , int _minHue01, int _maxHue01, int _minSat01, int _maxSat01, int _minHue02, int _maxHue02, int _minSat02, int _maxSat02) 순 2가지 색상으로 검출 안할경우는 앞 뒤를 같은색으로 설정

	//imwrite("pavements.jpg", detected_pavement);  //보도블럭 이미지 저장
				////보도 블럭 캐니엣지////
	Mat pavements_canny = Canny_Edge(detected_pavement,30,40,3);
	imwrite("pavements_canny.jpg", pavements_canny);        //보도블럭 캐니엣지 

	
	//// 볼라드 검출  /////
	Mat preprocessing_image = preprocessing(image_R,130);  //전처리를 위한 코드 (이미지명, 임계값)

	// imwrite("find_bolad_mask.jpg", th_img01);

	cv::Mat image_bolad = image_R.clone();                                                  //원본에서 색검출하면 원본이 훼손됨으로 복사 필수
	Mat bolad_circle = find_circles(preprocessing_image, image_bolad, 80, 120, 0, 255, 0, 20);				//find_circle(Mat th_img, Mat _image, int min, int max, int B, int G, int R, int H)

	//imwrite("find_bolad.jpg", bolad_circle);   //원으로 볼라드 검출



	//// 검출된 볼라드 원 색상 검출  ////

	Mat detected_bolad = find_colors(bolad_circle, 48, 62, 200, 255, 48, 62, 200, 255);  //컬러 추출 (입력 이미지 , int _minHue01, int _maxHue01, int _minSat01, int _maxSat01, int _minHue02, int _maxHue02, int _minSat02, int _maxSat02) 순 2가지 색상으로 검출 안할경우는 앞 뒤를 같은색으로 설정
	imwrite("bolad_circle.jpg", detected_bolad);        //검출된 원의 색으로 검출   -볼라드 벡터라이징용
	
	///// 검출된 볼라드 색상 검출  /////

	Mat bolad_point = find_points(detected_bolad, out, "bolad",16,16);   //중심점 검출  (이미지명,csv명, csv에 표시할 장애물 명 )


	//////////csv 파일 닫기/////////////////////
	 fclose(out);  // csv 불러온것은 반드시 닫아줘야 한다.
	
	
				   

	////////////////////////////////////////////  항공측점 검출 - 색상: 화이트 - 도형 - 색상 :그린 - XYpoint/////////////////////////////////

	FILE *skypointXY;
	skypointXY = fopen("SkyPointXY.csv", "w");                     //오류가 나는 경우 프로젝트 속성 - 전처리기 - 전처리기 정의에"_CRT_SECURE_NO_WARNINGS"를 넣어주세요.
	fprintf(skypointXY, "Y,X,pixY,pixX,Name,No,PtColor\n");

	Mat detected_sky = find_colors(image_R, 90, 130, 0, 30, 90, 130, 0, 30);  //컬러 추출 (입력 이미지 , int _minHue01, int _maxHue01, int _minSat01, int _maxSat01, int _minHue02, int _maxHue02, int _minSat02, int _maxSat02) 순 2가지 색상으로 검출 안할경우는 앞 뒤를 같은색으로 설정   0, 360, 0, 255, 0, 360, 0, 255

	Mat skypoint = find_polygons(detected_sky);
	// imwrite("sky_point.jpg", skypoint);
	Mat sky_poly = find_colors(skypoint, 48, 62, 200, 255, 48, 62, 200, 255);  //컬러 추출 (입력 이미지 , int _minHue01, int _maxHue01, int _minSat01, int _maxSat01, int _minHue02, int _maxHue02, int _minSat02, int _maxSat02) 순 2가지 색상으로 검출 안할경우는 앞 뒤를 같은색으로 설정
	// imwrite("sky_poly.jpg", sky_poly);
	Mat sky_point = find_points(sky_poly, skypointXY, "sky", 5, 40);   //중심점 검출  (이미지명,csv명, csv에 표시할 장애물 명 , 모서리점 최소,최대값)
	// imwrite("sky.jpg", detected_sky);  //보도블럭 이미지 저장
	fclose(skypointXY);  // csv 불러온것은 반드시 닫아줘야 한다.


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	 // 주석 풀고 해도 오류는 안남 다만 두개가 연동이 안됨 /**/만 지우면 연동 됩니다.			


				  


}	