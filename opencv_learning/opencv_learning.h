#pragma once
#include <iostream>


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#include <opencv2/libraw/libraw.h>



#define R 2
#define G 1
#define B 0

#define RGGB 0
#define BGGR 1
#define GRBG 2
#define GBRG 3


void GettingStartedwithImages(void);
void ReadARW(void);
cv::Mat raw2mat(char* file);
cv::Mat rawprocess(cv::Mat mat);
cv::Mat bayer2rgb(cv::Mat bayer, int flag=0);
cv::Mat rggb2rgb(cv::Mat bayer);
cv::Mat bggr2rgb(cv::Mat bayer); 
cv::Mat grbg2rgb(cv::Mat bayer);
cv::Mat gbrg2rgb(cv::Mat bayer);