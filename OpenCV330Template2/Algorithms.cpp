#include "stdafx.h"
#include "Algorithms.h"

//https://docs.opencv.org/2.4/doc/tutorials/imgproc/imgtrans/hough_lines/hough_lines.html
Mat TomatenAlgorithms::houghLineTransform(Mat source)
{

	Mat dst, cdst, gray_image;
	cvtColor(source, gray_image, CV_BGR2GRAY);
	Canny(source, dst, 50, 200, 3);
	cvtColor(dst, cdst, CV_GRAY2BGR);

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

	return cdst;
}


//https://docs.opencv.org/2.4.13.3/doc/tutorials/imgproc/imgtrans/sobel_derivatives/sobel_derivatives.html
Mat TomatenAlgorithms::sobel(Mat grayImage) {

	Mat grad;
	Mat grad_x = TomatenAlgorithms::sobelX(grayImage);
	Mat grad_y = TomatenAlgorithms::sobelY(grayImage);

	addWeighted(grad_x, 0.5, grad_y, 0.5, 0, grad);
	return grad;
}

Mat TomatenAlgorithms::sobelX(Mat grayImage)
{
	Mat grad_x, abs_grad_x;

	int scale = 10;
	int delta = 0;
	int ddepth = CV_16S;

	/// Gradient X
	//Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
	Sobel(grayImage, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x);

	return abs_grad_x;
}

Mat TomatenAlgorithms::sobelY(Mat grayImage)
{
	Mat grad_y, abs_grad_y;

	int scale = 10;
	int delta = 0;
	int ddepth = CV_16S;

	/// Gradient Y
	//Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
	Sobel(grayImage, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);

	return abs_grad_y;
}

void TomatenAlgorithms::saveMatAsBMP(string name, Mat source)
{
	imwrite(name + ".bmp", source);
}
