#pragma once

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>
#include <math.h>
#include <string.h>

using namespace cv;
using namespace std;

class SquareAreaCheck
{


public:
    static const int thresh=150;
    static const int N=5;
    
    SquareAreaCheck();
    ~SquareAreaCheck();




    // finds a cosine of angle between vectors
// from pt0->pt1 and from pt0->pt2
    static double angle(Point pt1_in, Point pt2_in, Point pt0_in);
    // returns sequence of squares detected on the image.
    // the sequence is stored in the specified memory storage
    static void findSquares(const Mat& image_in, vector<vector<Point> >& squares_out);
    // the function draws all the squares in the image
    static void drawSquares(Mat& image_in, const vector<vector<Point> >& squares_in);
};

