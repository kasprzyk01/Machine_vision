#include "mainwindow.h"
#include <QApplication>
#include "opencv2/opencv.hpp"

int main(int argc, char *argv[])
{
    Mat obraz=imread("10.jpg",0);

        pyrDown(obraz,obraz);
        threshold(obraz,obraz,1,255,CV_THRESH_OTSU);
        erode(obraz,obraz,Mat());
        threshold(obraz,obraz,0,255,CV_THRESH_OTSU);
        imshow("okno",obraz);
}
