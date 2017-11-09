#include "stdafx.h"
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include <stdio.h>
#include <cmath>

// The Basic Image Container // Part 2
// Uitleg: http://docs.opencv.org/doc/tutorials/core/mat_the_basic_image_container/mat_the_basic_image_container.html
// Jan Oostindie, dd 22-1-2015

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    // met commandline argument wordt de filenaam doorgegeven
	// zie: project properties - configuration properties - debugging - command arguments
	if (argc != 2)
	{
		cout << "NB! Geef als command argument volledige padnaam van de imagefile mee" << endl;
		return -1;
	}
	else cout << "De imagefile = " << argv[1] << endl;

	// Lees de afbeelding in
	Mat image;
	image = imread(argv[1], CV_LOAD_IMAGE_COLOR);   
	if (!image.data)                           
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}
	imshow("Origional image", image);

	Mat dst, cdst,gray_image;
	cvtColor(image, gray_image, CV_BGR2GRAY);
	Canny(image, dst, 50, 200, 3);
	cvtColor(dst, cdst, CV_GRAY2BGR);

	imshow("gray image", gray_image);

#if 0
	vector<Vec2f> lines;
	HoughLines(dst, lines, 1, CV_PI / 180, 100, 0, 0);

	for (size_t i = 0; i < lines.size(); i++)
	{
		float rho = lines[i][0], theta = lines[i][1];
		Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a*rho, y0 = b*rho;
		pt1.x = cvRound(x0 + 1000 * (-b));
		pt1.y = cvRound(y0 + 1000 * (a));
		pt2.x = cvRound(x0 - 1000 * (-b));
		pt2.y = cvRound(y0 - 1000 * (a));
		line(cdst, pt1, pt2, Scalar(0, 0, 255), 3, CV_AA);
	}
#else
	vector<Vec4i> lines;
	HoughLinesP(dst, lines, 1, CV_PI / 180, 50, 50, 10);
	for (size_t i = 0; i < lines.size(); i++)
	{
		Vec4i l = lines[i];
		line(cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3, CV_AA);
	}
#endif
	imshow("source", dst);
	imshow("detected lines", cdst);


	/// Generate grad_x and grad_y
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;

	int scale =10;
	int delta = 0;
	int ddepth = CV_16S;

	/// Gradient X
	//Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
	Sobel(gray_image, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x);

	
	/// Gradient Y
	//Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
	Sobel(gray_image, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);

	Mat grad;
	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

	imshow("Sobel X", grad_x);
	imshow("Sobel Y", grad_y);
	imshow("ABS", grad);


	Mat deltaImage = gray_image.clone();

	for(int j=0; j<gray_image.cols; j++)
	{
		for (int i=0; i < gray_image.rows; i++)
		{			
			int p = static_cast<int>(gray_image.at<uchar>(i,j));

			//cout << "pixelwaarde(" << i << "," << j << ") = " << p  << endl;
			//hier komt een mooi algoritme wat de delta bepaald;

			deltaImage.at<uchar>(i,j) = p;
		}
	}

	imshow("detlaImage", deltaImage);
	waitKey(0);

	return 0;
}