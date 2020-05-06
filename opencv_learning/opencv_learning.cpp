#include "opencv_learning.h"



using namespace cv;
using namespace std;

int main(void)
{
	//GettingStartedwithImages();
    //ReadARW();
    char  imageName[] = "C:\\Users\\Administrator\\Downloads\\50cfead969df9\\RAW\\1.ARW";
    Mat raw = bayer2rgb(raw2mat(imageName),RGGB);
    Mat imgrgb(raw.rows,raw.cols,CV_8UC3);
    cv::namedWindow("test", WINDOW_NORMAL);
    for (int f = 0b100000000000; f < 0b100000000000000; f++)
    {
        printf("f=%d\n", f);
        for (int i = 0; i < raw.rows; i++)
            for (int j = 0; j < raw.cols; j++)
            {
                imgrgb.ptr<Vec3b>(i)[j][R] = raw.ptr<Vec3w>(i)[j][R] * 0b100000000 / f;
                imgrgb.ptr<Vec3b>(i)[j][G] = raw.ptr<Vec3w>(i)[j][G] * 0b100000000 / f;
                imgrgb.ptr<Vec3b>(i)[j][B] = raw.ptr<Vec3w>(i)[j][B] * 0b100000000 / f;
            }
        //cvtColor(raw, imgrgb, CV_BayerRG2BGR, 3);
       // for (int i = 0; i < imgrgb.rows; i++)
         //   for (int j = 0; j < imgrgb.cols; j++)
           //     printf("i=%d\tR=%d\tG=%d\tB=%d\n", i, imgrgb.ptr<Vec3w>(i)[j][R], imgrgb.ptr<Vec3w>(i)[j][G], imgrgb.ptr<Vec3w>(i)[j][B]);
        cv::imshow("test", imgrgb);
        waitKey(10); // Wait for a keystroke in the window
    }
    int k = waitKey(0); // Wait for a keystroke in the window
}

void GettingStartedwithImages(void) 
{
    std::string image_path ="C:\\Users\\Administrator\\Desktop\\test.jpg";
    Mat img = imread(image_path, IMREAD_COLOR);
    if (img.empty())
    {
        std::cout << "Could not read the image: " << image_path << std::endl;
    }
    imshow("Display window", img);
    int k = waitKey(0); // Wait for a keystroke in the window
    if (k == 's')
    {
        imwrite("C:\\Users\\Administrator\\Desktop\\starry_night.png", img);
    }
}

void ReadARW(void)
{

    // Load image
    std::string imageName = "C:\\Users\\Administrator\\Downloads\\50cfead969df9\\RAW\\2.ARW";
    //std::string imageName = "C:\\Users\\Administrator\\Desktop\\test.jpg";


    //// Raw data to Gray using opencv 
    //cv::Mat grayImage;
    //cv::cvtColor(rawImage, grayImage, cv::COLOR_BayerGB2GRAY/*cv::COLOR_BayerGB2BGR_EA*///);
    FILE* filePointer;

    fopen_s(&filePointer, imageName.c_str(), "rb+");
    //把文件的位置指针移到文件尾
    fseek(filePointer, 0L, SEEK_END);
    //获取文件长度;
    int length = ftell(filePointer);
    fseek(filePointer, 0L, SEEK_SET);

    int  rawDataHeight = 2648;
    int  rawDataWidth = 3968;


    if (filePointer == NULL)
    {
        std::cout << "Can not open the raw data!\n";
    }
    int l = sizeof(ushort);
    //cv::Mat rawData(rawDataHeight, rawDataWidth, CV_8UC1, cv::Scalar(0));

    // read imgae data
    cv::Mat rawData(rawDataHeight, rawDataWidth, CV_16UC1, cv::Scalar(0));
        for (int i = 0; i < rawDataHeight; i++)
        {
            uchar* pointer = rawData.ptr<uchar>(i);
            fread(pointer, sizeof(uchar), rawDataWidth, filePointer);
        }

        fclose(filePointer);

        cv::namedWindow("test", WINDOW_NORMAL);
        cv::imshow("test", rawData);
        int k = waitKey(0); // Wait for a keystroke in the window
}

cv::Mat raw2mat(char* file)
{
    // Let us create an image processor
    LibRaw iProcessor;

    // Open the file and read the metadata
    int ret=iProcessor.open_file(file);

    // The metadata are accessible through data fields of the class
    printf("Image size: %d x %d\n", iProcessor.imgdata.sizes.width, iProcessor.imgdata.sizes.height);

    // Let us unpack the image
    ret=iProcessor.unpack();
     cv::Mat rawData(iProcessor.imgdata.sizes.raw_height, iProcessor.imgdata.sizes.raw_width, CV_16UC1,Scalar(0));
    // And let us print its dump; the data are accessible through data fields of the class
    //rawdata in *iProcessor.imgdata.rawdata.raw_image
   for (int i = 0; i < iProcessor.imgdata.sizes.raw_height * iProcessor.imgdata.sizes.raw_width; i++)
       rawData.ptr<ushort>(i / (iProcessor.imgdata.sizes.raw_width) )[i % (iProcessor.imgdata.sizes.raw_width)] = iProcessor.imgdata.rawdata.raw_image[i];
    // Finally, let us free the image processor for work with the next image
    iProcessor.recycle();
    return rawData;
}

Mat rawprocess(Mat mat)
{
    Mat res;
    res = mat;
    return res;
}

cv::Mat bayer2rgb(cv::Mat bayer, int flag) 
{
    Mat res(bayer.rows, bayer.cols, CV_16UC3, Scalar(255, 255, 255));
    switch (flag)
    {
    case RGGB:
        res = rggb2rgb(bayer);
        break;
    case BGGR:
        res = bggr2rgb(bayer);
        break;
    case GRBG:
        res = grbg2rgb(bayer);
        break;
    case GBRG:
        res = gbrg2rgb(bayer);
        break;
    default:
        break;
    }
    return res;
}
cv::Mat rggb2rgb(cv::Mat bayer)
{
    Mat res(bayer.rows, bayer.cols, CV_16UC3, Scalar(255, 255, 255));
    //R(R,G,B)
    for (int i = 0; i < res.rows; i = i + 2)
        for (int j = 0; j < res.cols; j = j + 2)
        {
            //R(R)
            res.ptr<Vec3w>(i)[j][R] = bayer.ptr<ushort>(i)[j];//
            if ((i > 0) && (j > 0))
            {
                //R(G)
                res.ptr<Vec3w>(i)[j][G] = (bayer.ptr<ushort>(i - 1)[j] + bayer.ptr<ushort>(i + 1)[j] + bayer.ptr<ushort>(i)[j - 1] + bayer.ptr<ushort>(i)[j + 1]) / 4;//
                //R(B)
                res.ptr<Vec3w>(i)[j][B] = (bayer.ptr<ushort>(i - 1)[j - 1] + bayer.ptr<ushort>(i + 1)[j - 1] + bayer.ptr<ushort>(i - 1)[j + 1] + bayer.ptr<ushort>(i + 1)[j + 1]) / 4;//
            }
            else if ((i == 0) && (j == 0))
            {
                //R(G)
                res.ptr<Vec3w>(i)[j][G] = (bayer.ptr<ushort>(i + 1)[j] + bayer.ptr<ushort>(i)[j + 1]) / 2;//
                //R(B)
                res.ptr<Vec3w>(i)[j][B] = bayer.ptr<ushort>(i + 1)[j + 1];//
            }
            else if (i == 0)
            {
                //R(G)
                res.ptr<Vec3w>(i)[j][G] = (bayer.ptr<ushort>(i + 1)[j] + bayer.ptr<ushort>(i)[j - 1] + bayer.ptr<ushort>(i)[j + 1]) / 3;//
                //R(B)
                res.ptr<Vec3w>(i)[j][B] = (bayer.ptr<ushort>(i + 1)[j - 1] + bayer.ptr<ushort>(i + 1)[j + 1]) / 2;//
            }
            else if ((j == 0))
            {
                //R(G)
                res.ptr<Vec3w>(i)[j][G] = (bayer.ptr<ushort>(i + 1)[j] + bayer.ptr<ushort>(i - 1)[j] + bayer.ptr<ushort>(i)[j + 1]) / 3;//
                //R(B)
                res.ptr<Vec3w>(i)[j][B] = (bayer.ptr<ushort>(i + 1)[j + 1] + bayer.ptr<ushort>(i - 1)[j + 1]) / 2;//
            }
        }

    //G(R,G,B)
    for (int i = 0; i < res.rows; i = i + 1)
        for (int j = (i + 1) % 2; j < res.cols; j = j + 2)
        {
            //G(R)
            if ((i < res.rows - 1) && (j < res.cols - 1) && ((i + 1) % 2))
            {
                res.ptr<Vec3w>(i)[j][R] = (bayer.ptr<ushort>(i)[j + 1] + bayer.ptr<ushort>(i)[j - 1]) / 2;//
            }
            else if ((i < res.rows - 1) && (j < res.cols - 1) && (!((i + 1) % 2)))
            {
                res.ptr<Vec3w>(i)[j][R] = (bayer.ptr<ushort>(i + 1)[j] + bayer.ptr<ushort>(i - 1)[j]) / 2;//
            }
            else if (i == res.rows - 1)
            {
                res.ptr<Vec3w>(i)[j][R] = bayer.ptr<ushort>(i - 1)[j];//
            }
            else if ((j == res.cols - 1))
            {
                res.ptr<Vec3w>(i)[j][R] = bayer.ptr<ushort>(i)[j - 1];//
            }

            //G(G)
            res.ptr<Vec3w>(i)[j][G] = bayer.ptr<ushort>(i)[j];//

            //G(B)
            if ((i > 0) && (j > 0) && ((i + 1) % 2))
            {
                res.ptr<Vec3w>(i)[j][B] = (bayer.ptr<ushort>(i - 1)[j] + bayer.ptr<ushort>(i + 1)[j]) / 2;//
            }
            else if ((i > 0) && (j > 0) && (!((i + 1) % 2)))
            {
                res.ptr<Vec3w>(i)[j][B] = (bayer.ptr<ushort>(i)[j + 1] + bayer.ptr<ushort>(i)[j - 1]) / 2;//
            }
            else if (i == 0)
            {
                res.ptr<Vec3w>(i)[j][B] = bayer.ptr<ushort>(i + 1)[j];//
            }
            else if (j == 0)
            {
                res.ptr<Vec3w>(i)[j][B] = bayer.ptr<ushort>(i)[j + 1];//
            }

        }
    //B(R,G,B)
    for (int i = 1; i < res.rows; i = i + 2)
        for (int j = 1; j < res.cols; j = j + 2)
        {
            //B(R)
            if ((i < res.rows - 1) && (j < res.cols - 1))
            {
                res.ptr<Vec3w>(i)[j][R] = (bayer.ptr<ushort>(i - 1)[j - 1] + bayer.ptr<ushort>(i + 1)[j - 1] + bayer.ptr<ushort>(i - 1)[j + 1] + bayer.ptr<ushort>(i + 1)[j + 1]) / 4;//
            }
            else if ((i == res.rows - 1) && (j == res.cols - 1))
            {
                res.ptr<Vec3w>(i)[j][R] = bayer.ptr<ushort>(i - 1)[j - 1];//
            }
            else if (i == res.rows - 1)
            {
                res.ptr<Vec3w>(i)[j][R] = (bayer.ptr<ushort>(i - 1)[j - 1] + bayer.ptr<ushort>(i - 1)[j + 1]) / 2;//
            }
            else if (j == res.cols - 1)
            {
                res.ptr<Vec3w>(i)[j][R] = (bayer.ptr<ushort>(i - 1)[j - 1] + bayer.ptr<ushort>(i + 1)[j - 1]) / 2;//
            }

            //B(G)
            if ((i < res.rows - 1) && (j < res.cols - 1))
            {
                res.ptr<Vec3w>(i)[j][G] = (bayer.ptr<ushort>(i - 1)[j] + bayer.ptr<ushort>(i + 1)[j] + bayer.ptr<ushort>(i)[j - 1] + bayer.ptr<ushort>(i)[j + 1]) / 4;//
            }
            else if ((i == res.rows - 1) && (j == res.cols - 1))
            {
                res.ptr<Vec3w>(i)[j][G] = (bayer.ptr<ushort>(i - 1)[j] + bayer.ptr<ushort>(i)[j - 1]) / 2;//
            }
            else if (i == res.rows - 1)
            {
                res.ptr<Vec3w>(i)[j][G] = (bayer.ptr<ushort>(i)[j - 1] + bayer.ptr<ushort>(i)[j + 1] + bayer.ptr<ushort>(i - 1)[j]) / 3;//
            }
            else if (j == res.cols - 1)
            {
                res.ptr<Vec3w>(i)[j][G] = (bayer.ptr<ushort>(i - 1)[j] + bayer.ptr<ushort>(i + 1)[j] + bayer.ptr<ushort>(i)[j - 1]) / 3;//
            }

            //B(B)
            res.ptr<Vec3w>(i)[j][B] = bayer.ptr<ushort>(i)[j];//
        }
    return res;
}
cv::Mat bggr2rgb(cv::Mat bayer)
{
    Mat res(bayer.rows, bayer.cols, CV_16UC3, Scalar(255, 255, 255));
    //B(R,G,B)
    for (int i = 0; i < res.rows; i = i + 2)
        for (int j = 0; j < res.cols; j = j + 2)
        {
            //B(B)
            res.ptr<Vec3w>(i)[j][B] = bayer.ptr<ushort>(i)[j];//
            if ((i > 0) && (j > 0))
            {
                //B(G)
                res.ptr<Vec3w>(i)[j][G] = (bayer.ptr<ushort>(i - 1)[j] + bayer.ptr<ushort>(i + 1)[j] + bayer.ptr<ushort>(i)[j - 1] + bayer.ptr<ushort>(i)[j + 1]) / 4;//
                //B(R)
                res.ptr<Vec3w>(i)[j][R] = (bayer.ptr<ushort>(i - 1)[j - 1] + bayer.ptr<ushort>(i + 1)[j - 1] + bayer.ptr<ushort>(i - 1)[j + 1] + bayer.ptr<ushort>(i + 1)[j + 1]) / 4;//
            }
            else if ((i == 0) && (j == 0))
            {
                //B(G)
                res.ptr<Vec3w>(i)[j][G] = (bayer.ptr<ushort>(i + 1)[j] + bayer.ptr<ushort>(i)[j + 1]) / 2;//
                //B(R)
                res.ptr<Vec3w>(i)[j][R] = bayer.ptr<ushort>(i + 1)[j + 1];//
            }
            else if (i == 0)
            {
                //B(G)
                res.ptr<Vec3w>(i)[j][G] = (bayer.ptr<ushort>(i + 1)[j] + bayer.ptr<ushort>(i)[j - 1] + bayer.ptr<ushort>(i)[j + 1]) / 3;//
                //B(R)
                res.ptr<Vec3w>(i)[j][R] = (bayer.ptr<ushort>(i + 1)[j - 1] + bayer.ptr<ushort>(i + 1)[j + 1]) / 2;//
            }
            else if ((j == 0))
            {
                //B(G)
                res.ptr<Vec3w>(i)[j][G] = (bayer.ptr<ushort>(i + 1)[j] + bayer.ptr<ushort>(i - 1)[j] + bayer.ptr<ushort>(i)[j + 1]) / 3;//
                //B(R)
                res.ptr<Vec3w>(i)[j][R] = (bayer.ptr<ushort>(i + 1)[j + 1] + bayer.ptr<ushort>(i - 1)[j + 1]) / 2;//
            }
        }

    //G(R,G,B)
    for (int i = 0; i < res.rows; i = i + 1)
        for (int j = (i + 1) % 2; j < res.cols; j = j + 2)
        {
            //G(B)
            if ((i < res.rows - 1) && (j < res.cols - 1) && ((i + 1) % 2))
            {
                res.ptr<Vec3w>(i)[j][B] = (bayer.ptr<ushort>(i)[j + 1] + bayer.ptr<ushort>(i)[j - 1]) / 2;//
            }
            else if ((i < res.rows - 1) && (j < res.cols - 1) && (!((i + 1) % 2)))
            {
                res.ptr<Vec3w>(i)[j][B] = (bayer.ptr<ushort>(i + 1)[j] + bayer.ptr<ushort>(i - 1)[j]) / 2;//
            }
            else if (i == res.rows - 1)
            {
                res.ptr<Vec3w>(i)[j][B] = bayer.ptr<ushort>(i - 1)[j];//
            }
            else if ((j == res.cols - 1))
            {
                res.ptr<Vec3w>(i)[j][B] = bayer.ptr<ushort>(i)[j - 1];//
            }

            //G(G)
            res.ptr<Vec3w>(i)[j][G] = bayer.ptr<ushort>(i)[j];//

            //G(R)
            if ((i > 0) && (j > 0) && ((i + 1) % 2))
            {
                res.ptr<Vec3w>(i)[j][R] = (bayer.ptr<ushort>(i - 1)[j] + bayer.ptr<ushort>(i + 1)[j]) / 2;//
            }
            else if ((i > 0) && (j > 0) && (!((i + 1) % 2)))
            {
                res.ptr<Vec3w>(i)[j][R] = (bayer.ptr<ushort>(i)[j + 1] + bayer.ptr<ushort>(i)[j - 1]) / 2;//
            }
            else if (i == 0)
            {
                res.ptr<Vec3w>(i)[j][R] = bayer.ptr<ushort>(i + 1)[j];//
            }
            else if (j == 0)
            {
                res.ptr<Vec3w>(i)[j][R] = bayer.ptr<ushort>(i)[j + 1];//
            }

        }
    //R(R,G,B)
    for (int i = 1; i < res.rows; i = i + 2)
        for (int j = 1; j < res.cols; j = j + 2)
        {
            //R(B)
            if ((i < res.rows - 1) && (j < res.cols - 1))
            {
                res.ptr<Vec3w>(i)[j][B] = (bayer.ptr<ushort>(i - 1)[j - 1] + bayer.ptr<ushort>(i + 1)[j - 1] + bayer.ptr<ushort>(i - 1)[j + 1] + bayer.ptr<ushort>(i + 1)[j + 1]) / 4;//
            }
            else if ((i == res.rows - 1) && (j == res.cols - 1))
            {
                res.ptr<Vec3w>(i)[j][B] = bayer.ptr<ushort>(i - 1)[j - 1];//
            }
            else if (i == res.rows - 1)
            {
                res.ptr<Vec3w>(i)[j][B] = (bayer.ptr<ushort>(i - 1)[j - 1] + bayer.ptr<ushort>(i - 1)[j + 1]) / 2;//
            }
            else if (j == res.cols - 1)
            {
                res.ptr<Vec3w>(i)[j][B] = (bayer.ptr<ushort>(i - 1)[j - 1] + bayer.ptr<ushort>(i + 1)[j - 1]) / 2;//
            }

            //R(G)
            if ((i < res.rows - 1) && (j < res.cols - 1))
            {
                res.ptr<Vec3w>(i)[j][G] = (bayer.ptr<ushort>(i - 1)[j] + bayer.ptr<ushort>(i + 1)[j] + bayer.ptr<ushort>(i)[j - 1] + bayer.ptr<ushort>(i)[j + 1]) / 4;//
            }
            else if ((i == res.rows - 1) && (j == res.cols - 1))
            {
                res.ptr<Vec3w>(i)[j][G] = (bayer.ptr<ushort>(i - 1)[j] + bayer.ptr<ushort>(i)[j - 1]) / 2;//
            }
            else if (i == res.rows - 1)
            {
                res.ptr<Vec3w>(i)[j][G] = (bayer.ptr<ushort>(i)[j - 1] + bayer.ptr<ushort>(i)[j + 1] + bayer.ptr<ushort>(i - 1)[j]) / 3;//
            }
            else if (j == res.cols - 1)
            {
                res.ptr<Vec3w>(i)[j][G] = (bayer.ptr<ushort>(i - 1)[j] + bayer.ptr<ushort>(i + 1)[j] + bayer.ptr<ushort>(i)[j - 1]) / 3;//
            }

            //R(R)
            res.ptr<Vec3w>(i)[j][R] = bayer.ptr<ushort>(i)[j];//
        }
    return res;
}
cv::Mat grbg2rgb(cv::Mat bayer)
{
    Mat res(bayer.rows, bayer.cols, CV_16UC3, Scalar(255, 255, 255));
    //R(R,G,B)
    for (int i = 0; i < res.rows-1; i = i + 2)
        for (int j = 1; j < res.cols; j = j + 2)
        {
            //R(R)
            res.ptr<Vec3w>(i)[j][R] = bayer.ptr<ushort>(i)[j];//
            if ((i > 0) && (j < res.rows-1))
            {
                //R(G)
                res.ptr<Vec3w>(i)[j][G] = (bayer.ptr<ushort>(i - 1)[j] + bayer.ptr<ushort>(i + 1)[j] + bayer.ptr<ushort>(i)[j - 1] + bayer.ptr<ushort>(i)[j + 1]) / 4;//
                //R(B)
                res.ptr<Vec3w>(i)[j][B] = (bayer.ptr<ushort>(i - 1)[j - 1] + bayer.ptr<ushort>(i + 1)[j - 1] + bayer.ptr<ushort>(i - 1)[j + 1] + bayer.ptr<ushort>(i + 1)[j + 1]) / 4;//
            }
            else if ((i == 0) && (j == res.cols-1))
            {
                //R(G)
                res.ptr<Vec3w>(i)[j][G] = (bayer.ptr<ushort>(i + 1)[j] + bayer.ptr<ushort>(i)[j - 1]) / 2;//
                //R(B)
                res.ptr<Vec3w>(i)[j][B] = bayer.ptr<ushort>(i + 1)[j - 1];//
            }
            else if (i == 0)
            {
                //R(G)
                res.ptr<Vec3w>(i)[j][G] = (bayer.ptr<ushort>(i + 1)[j] + bayer.ptr<ushort>(i)[j - 1] + bayer.ptr<ushort>(i)[j + 1]) / 3;//
                //R(B)
                res.ptr<Vec3w>(i)[j][B] = (bayer.ptr<ushort>(i + 1)[j - 1] + bayer.ptr<ushort>(i + 1)[j + 1]) / 2;//
            }
            else if ((j == res.cols-1))
            {
                //R(G)
                res.ptr<Vec3w>(i)[j][G] = (bayer.ptr<ushort>(i + 1)[j] + bayer.ptr<ushort>(i - 1)[j] + bayer.ptr<ushort>(i)[j - 1]) / 3;//
                //R(B)
                res.ptr<Vec3w>(i)[j][B] = (bayer.ptr<ushort>(i + 1)[j - 1] + bayer.ptr<ushort>(i - 1)[j - 1]) / 2;//
            }
        }

    //G(R,G,B)
    for (int i = 0; i < res.rows; i = i + 1)
        for (int j = i  % 2; j < res.cols; j = j + 2)
        {
            //G(R)
            if ((i < res.rows - 1) && (j >0) && (!(i % 2)))
            {
                res.ptr<Vec3w>(i)[j][R] = (bayer.ptr<ushort>(i)[j + 1] + bayer.ptr<ushort>(i)[j - 1]) / 2;//
            }
            else if ((i < res.rows - 1) && (j >0) && (i % 2))
            {
                res.ptr<Vec3w>(i)[j][R] = (bayer.ptr<ushort>(i + 1)[j] + bayer.ptr<ushort>(i - 1)[j]) / 2;//
            }
            else if (i == res.rows - 1)
            {
                res.ptr<Vec3w>(i)[j][R] = bayer.ptr<ushort>(i - 1)[j];//
            }
            else if ((j == 0))
            {
                res.ptr<Vec3w>(i)[j][R] = bayer.ptr<ushort>(i)[j + 1];//
            }

            //G(G)
            res.ptr<Vec3w>(i)[j][G] = bayer.ptr<ushort>(i)[j];//

            //G(B)
            if ((i >0) && (j < res.cols-1) && (!(i % 2)))
            {
                res.ptr<Vec3w>(i)[j][B] = (bayer.ptr<ushort>(i - 1)[j] + bayer.ptr<ushort>(i + 1)[j]) / 2;//
            }
            else if ((i >0) && (j < res.cols-1) && (i % 2))
            {
                res.ptr<Vec3w>(i)[j][B] = (bayer.ptr<ushort>(i)[j + 1] + bayer.ptr<ushort>(i)[j - 1]) / 2;//
            }
            else if (i == 0)
            {
                res.ptr<Vec3w>(i)[j][B] = bayer.ptr<ushort>(i + 1)[j];//
            }
            else if (j == res.cols-1)
            {
                res.ptr<Vec3w>(i)[j][B] = bayer.ptr<ushort>(i)[j - 1];//
            }

        }
    //B(R,G,B)
    for (int i = 1; i < res.rows; i = i + 2)
        for (int j = 0; j < res.cols-1; j = j + 2)
        {
            //B(R)
            if ((i < res.rows - 1) && (j >0))
            {
                res.ptr<Vec3w>(i)[j][R] = (bayer.ptr<ushort>(i - 1)[j - 1] + bayer.ptr<ushort>(i + 1)[j - 1] + bayer.ptr<ushort>(i - 1)[j + 1] + bayer.ptr<ushort>(i + 1)[j + 1]) / 4;//
            }
            else if ((i == res.rows - 1) && (j == 0))
            {
                res.ptr<Vec3w>(i)[j][R] = bayer.ptr<ushort>(i - 1)[j + 1];//
            }
            else if (i == res.rows - 1)
            {
                res.ptr<Vec3w>(i)[j][R] = (bayer.ptr<ushort>(i - 1)[j - 1] + bayer.ptr<ushort>(i - 1)[j + 1]) / 2;//
            }
            else if (j == 0)
            {
                res.ptr<Vec3w>(i)[j][R] = (bayer.ptr<ushort>(i - 1)[j + 1] + bayer.ptr<ushort>(i + 1)[j + 1]) / 2;//
            }

            //B(G)
            if ((i < res.rows - 1) && (j > 0))
            {
                res.ptr<Vec3w>(i)[j][G] = (bayer.ptr<ushort>(i - 1)[j] + bayer.ptr<ushort>(i + 1)[j] + bayer.ptr<ushort>(i)[j - 1] + bayer.ptr<ushort>(i)[j + 1]) / 4;//
            }
            else if ((i == res.rows - 1) && (j == 0))
            {
                res.ptr<Vec3w>(i)[j][G] = (bayer.ptr<ushort>(i - 1)[j] + bayer.ptr<ushort>(i)[j + 1]) / 2;//
            }
            else if (i == res.rows - 1)
            {
                res.ptr<Vec3w>(i)[j][G] = (bayer.ptr<ushort>(i)[j - 1] + bayer.ptr<ushort>(i)[j + 1] + bayer.ptr<ushort>(i - 1)[j]) / 3;//
            }
            else if (j == 0)
            {
                res.ptr<Vec3w>(i)[j][G] = (bayer.ptr<ushort>(i - 1)[j] + bayer.ptr<ushort>(i + 1)[j] + bayer.ptr<ushort>(i)[j + 1]) / 3;//
            }

            //B(B)
            res.ptr<Vec3w>(i)[j][B] = bayer.ptr<ushort>(i)[j];//
        }
    return res;

}
cv::Mat gbrg2rgb(cv::Mat bayer)
{
    Mat res(bayer.rows, bayer.cols, CV_16UC3, Scalar(255, 255, 255));
    //B(B,G,R)
    for (int i = 0; i < res.rows - 1; i = i + 2)
        for (int j = 1; j < res.cols; j = j + 2)
        {
            //B(B)
            res.ptr<Vec3w>(i)[j][B] = bayer.ptr<ushort>(i)[j];//
            if ((i > 0) && (j < res.rows - 1))
            {
                //B(G)
                res.ptr<Vec3w>(i)[j][G] = (bayer.ptr<ushort>(i - 1)[j] + bayer.ptr<ushort>(i + 1)[j] + bayer.ptr<ushort>(i)[j - 1] + bayer.ptr<ushort>(i)[j + 1]) / 4;//
                //B(R)
                res.ptr<Vec3w>(i)[j][R] = (bayer.ptr<ushort>(i - 1)[j - 1] + bayer.ptr<ushort>(i + 1)[j - 1] + bayer.ptr<ushort>(i - 1)[j + 1] + bayer.ptr<ushort>(i + 1)[j + 1]) / 4;//
            }
            else if ((i == 0) && (j == res.cols - 1))
            {
                //B(G)
                res.ptr<Vec3w>(i)[j][G] = (bayer.ptr<ushort>(i + 1)[j] + bayer.ptr<ushort>(i)[j - 1]) / 2;//
                //B(R)
                res.ptr<Vec3w>(i)[j][R] = bayer.ptr<ushort>(i + 1)[j - 1];//
            }
            else if (i == 0)
            {
                //B(G)
                res.ptr<Vec3w>(i)[j][G] = (bayer.ptr<ushort>(i + 1)[j] + bayer.ptr<ushort>(i)[j - 1] + bayer.ptr<ushort>(i)[j + 1]) / 3;//
                //B(R)
                res.ptr<Vec3w>(i)[j][R] = (bayer.ptr<ushort>(i + 1)[j - 1] + bayer.ptr<ushort>(i + 1)[j + 1]) / 2;//
            }
            else if ((j == res.cols - 1))
            {
                //B(G)
                res.ptr<Vec3w>(i)[j][G] = (bayer.ptr<ushort>(i + 1)[j] + bayer.ptr<ushort>(i - 1)[j] + bayer.ptr<ushort>(i)[j - 1]) / 3;//
                //B(R)
                res.ptr<Vec3w>(i)[j][R] = (bayer.ptr<ushort>(i + 1)[j - 1] + bayer.ptr<ushort>(i - 1)[j - 1]) / 2;//
            }
        }

    //G(B,G,R)
    for (int i = 0; i < res.rows; i = i + 1)
        for (int j = i % 2; j < res.cols; j = j + 2)
        {
            //G(B)
            if ((i < res.rows - 1) && (j > 0) && (!(i % 2)))
            {
                res.ptr<Vec3w>(i)[j][B] = (bayer.ptr<ushort>(i)[j + 1] + bayer.ptr<ushort>(i)[j - 1]) / 2;//
            }
            else if ((i < res.rows - 1) && (j > 0) && (i % 2))
            {
                res.ptr<Vec3w>(i)[j][B] = (bayer.ptr<ushort>(i + 1)[j] + bayer.ptr<ushort>(i - 1)[j]) / 2;//
            }
            else if (i == res.rows - 1)
            {
                res.ptr<Vec3w>(i)[j][B] = bayer.ptr<ushort>(i - 1)[j];//
            }
            else if ((j == 0))
            {
                res.ptr<Vec3w>(i)[j][B] = bayer.ptr<ushort>(i)[j + 1];//
            }

            //G(G)
            res.ptr<Vec3w>(i)[j][G] = bayer.ptr<ushort>(i)[j];//

            //G(R)
            if ((i > 0) && (j < res.cols - 1) && (!(i % 2)))
            {
                res.ptr<Vec3w>(i)[j][R] = (bayer.ptr<ushort>(i - 1)[j] + bayer.ptr<ushort>(i + 1)[j]) / 2;//
            }
            else if ((i > 0) && (j < res.cols - 1) && (i % 2))
            {
                res.ptr<Vec3w>(i)[j][R] = (bayer.ptr<ushort>(i)[j + 1] + bayer.ptr<ushort>(i)[j - 1]) / 2;//
            }
            else if (i == 0)
            {
                res.ptr<Vec3w>(i)[j][R] = bayer.ptr<ushort>(i + 1)[j];//
            }
            else if (j == res.cols - 1)
            {
                res.ptr<Vec3w>(i)[j][R] = bayer.ptr<ushort>(i)[j - 1];//
            }

        }
    //R(B,G,R)
    for (int i = 1; i < res.rows; i = i + 2)
        for (int j = 0; j < res.cols - 1; j = j + 2)
        {
            //R(B)
            if ((i < res.rows - 1) && (j > 0))
            {
                res.ptr<Vec3w>(i)[j][B] = (bayer.ptr<ushort>(i - 1)[j - 1] + bayer.ptr<ushort>(i + 1)[j - 1] + bayer.ptr<ushort>(i - 1)[j + 1] + bayer.ptr<ushort>(i + 1)[j + 1]) / 4;//
            }
            else if ((i == res.rows - 1) && (j == 0))
            {
                res.ptr<Vec3w>(i)[j][B] = bayer.ptr<ushort>(i - 1)[j + 1];//
            }
            else if (i == res.rows - 1)
            {
                res.ptr<Vec3w>(i)[j][B] = (bayer.ptr<ushort>(i - 1)[j - 1] + bayer.ptr<ushort>(i - 1)[j + 1]) / 2;//
            }
            else if (j == 0)
            {
                res.ptr<Vec3w>(i)[j][B] = (bayer.ptr<ushort>(i - 1)[j + 1] + bayer.ptr<ushort>(i + 1)[j + 1]) / 2;//
            }

            //R(G)
            if ((i < res.rows - 1) && (j > 0))
            {
                res.ptr<Vec3w>(i)[j][G] = (bayer.ptr<ushort>(i - 1)[j] + bayer.ptr<ushort>(i + 1)[j] + bayer.ptr<ushort>(i)[j - 1] + bayer.ptr<ushort>(i)[j + 1]) / 4;//
            }
            else if ((i == res.rows - 1) && (j == 0))
            {
                res.ptr<Vec3w>(i)[j][G] = (bayer.ptr<ushort>(i - 1)[j] + bayer.ptr<ushort>(i)[j + 1]) / 2;//
            }
            else if (i == res.rows - 1)
            {
                res.ptr<Vec3w>(i)[j][G] = (bayer.ptr<ushort>(i)[j - 1] + bayer.ptr<ushort>(i)[j + 1] + bayer.ptr<ushort>(i - 1)[j]) / 3;//
            }
            else if (j == 0)
            {
                res.ptr<Vec3w>(i)[j][G] = (bayer.ptr<ushort>(i - 1)[j] + bayer.ptr<ushort>(i + 1)[j] + bayer.ptr<ushort>(i)[j + 1]) / 3;//
            }

            //R(R)
            res.ptr<Vec3w>(i)[j][R] = bayer.ptr<ushort>(i)[j];//
        }
    return res;

}