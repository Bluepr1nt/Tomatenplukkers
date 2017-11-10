#include "stdafx.h"
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <stdio.h>
#include <cmath>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>
#include <string>
#include "Algorithms.h"

using namespace cv;
using namespace std;

const bool SOBEL = true;
const bool LINEDETECTION = false;


int main(int argc, char** argv)
{
	// Lees de afbeelding in
	Mat image = imread("PlantSchool.jpg", CV_LOAD_IMAGE_COLOR);   
	if (!image.data)                           
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	imshow("Original image", image);

	Mat dst, cdst,gray_image;
	cvtColor(image, gray_image, CV_BGR2GRAY);
	Canny(image, dst, 50, 200, 3);
	cvtColor(dst, cdst, CV_GRAY2BGR);

	imshow("gray image", gray_image);

	imshow("Hough Line Transform", TomatenAlgorithms::houghLineTransform(image));



	Mat deltaImageX = gray_image.clone();
	Mat deltaImageY = gray_image.clone();


	int x, p, y;
	for(int j=0; j<gray_image.cols; j++)
	{
		for (int i=0; i < gray_image.rows; i++)
		{			
			p = static_cast<int>(gray_image.at<uchar>(i,j));

			if (j != gray_image.cols-1)
			{
				x = static_cast<int>(gray_image.at<uchar>(i, (j+1)));
			}
			if(i != gray_image.rows-1)
			{
				y = static_cast<int>(gray_image.at<uchar>((i + 1), j));
			}
			
			//cout << "pixelwaarde(" << i << "," << j << ") = " << p  << endl;
			//hier komt een mooi algoritme wat de delta bepaald;
			
			deltaImageX.at<uchar>(i,j) = abs(p-x);
			deltaImageY.at<uchar>(i, j) = abs(p - y);

		}
	}


	imshow("deltaImageX", deltaImageX);
	imshow("deltaImageY", deltaImageY);

	//save image as bmp
	imwrite("deltaImageX.bmp", deltaImageX);

	//
	Mat deltaImageXFiltered = gray_image.clone();
	x, y, p = 0;
	for (int j = 0; j<gray_image.cols; j++)
	{
		for (int i = 0; i < gray_image.rows; i++)
		{
			p = static_cast<int>(gray_image.at<uchar>(i, j));

			if (j != gray_image.cols - 1)
			{
				x = static_cast<int>(gray_image.at<uchar>(i, (j + 1)));
			}
			
			//filter noise
			if (abs(p - x) < 30)
				p = 0;
			else
				p = abs(p - x);
			
			deltaImageXFiltered.at<uchar>(i, j) = p;

		}
	}

	imshow("detlaImageXFiltered", deltaImageXFiltered);

	//save image as bmp
	imwrite("deltaImageXFiltered.bmp", deltaImageXFiltered);

	//save deltaImageXFiltered + Sobel
	//imwrite("deltasobel.bmp", deltaImageXFiltered.mul(grad));

	waitKey(0);

	return 0;
}