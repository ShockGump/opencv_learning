#include "SquareAreaCheck.h"

SquareAreaCheck::SquareAreaCheck()
{
}

SquareAreaCheck::~SquareAreaCheck() {}



// finds a cosine of angle between vectors
// from pt0->pt1 and from pt0->pt2
double SquareAreaCheck::angle(Point pt1_in,Point pt2_in, Point pt0_in)
{
    double dx1 = pt1_in.x - pt0_in.x;
    double dy1 = pt1_in.y - pt0_in.y;
    double dx2 = pt2_in.x - pt0_in.x;
    double dy2 = pt2_in.y - pt0_in.y;
    return (dx1 * dx2 + dy1 * dy2) / sqrt((dx1 * dx1 + dy1 * dy1) * (dx2 * dx2 + dy2 * dy2) + 1e-10);
}
// returns sequence of squares detected on the image.
// the sequence is stored in the specified memory storage
 void SquareAreaCheck::findSquares(const Mat& image_in, vector<vector<Point> >& squares_out){
    squares_out.clear();

    //s    Mat pyr, timg, gray0(image.size(), CV_8U), gray;

        // down-scale and upscale the image to filter out the noise
        //pyrDown(image, pyr, Size(image.cols/2, image.rows/2));
        //pyrUp(pyr, timg, image.size());


        // blur will enhance edge detection
    Mat timg(image_in);
    medianBlur(image_in, timg, 9);
    Mat gray0(timg.size(), CV_8U), gray;

    vector<vector<Point> > contours;

    // find squares in every color plane of the image
    for (int c = 0; c < 3; c++)
    {
        int ch[] = { c, 0 };
        mixChannels(&timg, 1, &gray0, 1, ch, 1);

        // try several threshold levels
        {
            // hack: use Canny instead of zero threshold level.
            // Canny helps to catch squares with gradient shading
            {
                // apply Canny. Take the upper threshold from slider
                // and set the lower to 0 (which forces edges merging)
                Canny(gray0, gray, 5, thresh, 5);
                // dilate canny output to remove potential
                // holes between edge segments
                dilate(gray, gray, Mat(), Point(-1, -1));
            }
            {
                // apply threshold if l!=0:
                //     tgray(x,y) = gray(x,y) < (l+1)*255/N ? 255 : 0
                gray = gray0 >= 3 * 255 / N;
            }

            // find contours and store them all as a list
            findContours(gray, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

            vector<Point> approx;

            // test each contour
            for (size_t i = 0; i < contours.size(); i++)
            {
                // approximate contour with accuracy proportional
                // to the contour perimeter
                approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true) * 0.02, true);

                // square contours should have 4 vertices after approximation
                // relatively large area (to filter out noisy contours)
                // and be convex.
                // Note: absolute value of an area is used because
                // area may be positive or negative - in accordance with the
                // contour orientation
                if (approx.size() == 4 &&
                    fabs(contourArea(Mat(approx))) > 1000 &&
                    isContourConvex(Mat(approx)))
                {
                    double maxCosine = 0;

                    for (int j = 2; j < 5; j++)
                    {
                        // find the maximum cosine of the angle between joint edges
                        double cosine = fabs(angle(approx[j % 4], approx[j - 2], approx[j - 1]));
                        maxCosine = MAX(maxCosine, cosine);
                    }

                    // if cosines of all angles are small
                    // (all angles are ~90 degree) then write quandrange
                    // vertices to resultant sequence
                    if (maxCosine < 0.3)
                        squares_out.push_back(approx);
                }
                printf("i=%d\t\n", i);
            }
        }
    }
}


// the function draws all the squares in the image
void SquareAreaCheck::drawSquares(Mat& image_in, const vector<vector<Point> >& squares_in)
{
    for (size_t i = 0; i < squares_in.size(); i++)
    {
        const Point* p = &squares_in[i][0];

        int n = (int)squares_in[i].size();
        //dont detect the border
        if (p->x > 3 && p->y > 3)
            polylines(image_in, &p, &n, 1, true, Scalar(0, 255, 0), 3, CV_AA);
    }
    imshow("Square Detection", image_in);
}