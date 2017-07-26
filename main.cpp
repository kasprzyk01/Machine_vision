#include "mainwindow.h"
#include <QApplication>
#include "opencv2/opencv.hpp"
#include<stdio.h>


using namespace std;
using namespace cv;

#define PI 3.14159

int main( int argc, char** argv )

{
    IplImage* obraz=cvLoadImage("01.jpg",0);
    cvThreshold(obraz,obraz,0,255,CV_THRESH_OTSU);
    cvErode(obraz,obraz);
    cvErode(obraz,obraz);
    cvSaveImage("05.jpg", obraz, 0);
    cvShowImage("oknoo",obraz);
    //waitKey(0);

    double max=0;//znalezienie największej monety(5zł) w celu
    //określenia powierzchni innych monet
    double min=200;//minimalna powierzchnia do wykrywania
    //zapobiega wykrywaniu zbyt małych powierzchni, które
    //nie mogą być monetą
    double max2=3000;//maksymalny błąd
    double licznik=0;//licznik całkowitej wartosci pieniedzy

    IplImage* obraz2=cvLoadImage("02.jpg",0);
   // cvErode(obraz2,obraz2);
    //tworzenie nagłówka i alokacja danych
    IplImage* obraz3  = cvCreateImage( cvGetSize(obraz2), 8, 3 );//rozmiar, głębokość bitowa,liczba kanałów na piksel
    //magazyn dla struktur dynamicznych- CvSeq
    CvMemStorage* miejsce = cvCreateMemStorage(0);
    //struktura CvSeq
    CvSeq* aktualny_kontur = 0;
    cvThreshold( obraz2, obraz2, 0, 255, CV_THRESH_BINARY );
    //odwrócenie
    cvXorS(obraz2, cvScalar(255, 0, 0, 0), obraz2,NULL);
    //dylatacja
    cvDilate(obraz2, obraz2, NULL, 2);
    //cvDilate(obraz2, obraz2, NULL, 2);
    //znajdowanie konturów
    cvFindContours( obraz2, miejsce, &aktualny_kontur, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));
    //zerowanie do wyświetlenia
    cvZero( obraz3 );

    //pętla po wsyzstkich konturach- znalezienie największej monety(5zł)
    for( 0; aktualny_kontur != 0; aktualny_kontur = aktualny_kontur->h_next )
    {
        //contour area
        double aktualna_powierzchnia = fabs(cvContourArea(aktualny_kontur, CV_WHOLE_SEQ, 0));
        //sprawdzenie czy powierzchnia jest większa od minimalnej
        if (aktualna_powierzchnia < min)
            continue;
        //opisywanie konturu przez prostokąt
        CvRect prostokat = ((CvContour *)aktualny_kontur)->rect;
        //szerokosc opisanego prostokata
        int A = prostokat.width / 2;
        //wysokosc opisanego prostokata
        int B = prostokat.height / 2;
        //powierzchnia przyblizona polem elipsy
        double przyblizona_pow = PI * A*B;
        double roznica;//roznica miedzy rzeczywista powierzchnia a przyblizona
        roznica= fabs(aktualna_powierzchnia - przyblizona_pow);
        if (roznica > max2)
            continue;
        if(przyblizona_pow>max){
            max=przyblizona_pow;
        }}
    //ponowne wyszukanie konturów
    cvFindContours( obraz2, miejsce, &aktualny_kontur, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));
    cvZero( obraz3 );

    //pętla po wsyzstkich konturach- rysowanie
    for( 0; aktualny_kontur != 0; aktualny_kontur = aktualny_kontur->h_next ){
        //contour area
        double aktualna_powierzchnia = fabs(cvContourArea(aktualny_kontur, CV_WHOLE_SEQ, 0));
        //sprawdzenie czy powierzchnia jest większa od minimalnej
        if (aktualna_powierzchnia < min)
            continue;
        //opisywanie konturu przez prostokąt
        CvRect prostokat = ((CvContour *)aktualny_kontur)->rect;
        int A = prostokat.width / 2;
        int B = prostokat.height / 2;
        double przyblizona_pow  = PI * A*B;
        double roznica;
        roznica= fabs(aktualna_powierzchnia - przyblizona_pow);
        if (roznica > max2)
            continue;

        CvScalar color0= CV_RGB(255,0,0);       //czerwony-     5zl  24mm=100%
        CvScalar color1= CV_RGB(255,255,0);     //zolty-        1zl  23/24=95,83%
        CvScalar color2 = CV_RGB(0,0,255);      //niebieski-    2zl  21,5/24=89,58%
        CvScalar color3= CV_RGB(0,255,0);       //zielony-      50gr 20,5/24=85,41%
        CvScalar color4= CV_RGB(128,128,128);   //szary-        5gr  19,5/24=81,16%
        CvScalar color5= CV_RGB(147,246,100);   //seledynowy-    20gr 18,5/24=77,08%
        CvScalar color6= CV_RGB(188,226,127);   //miętowy       2gr  17,5/24=72,91%
        CvScalar color7= CV_RGB(255,165,0);     //pomarańczowy  10gr 16,5/24=68,75%
        CvScalar color8= CV_RGB(255,165,0);     //biały         1gr  15,5/24=64,58%



        if(przyblizona_pow>=0.97*max ){//5zl

            cvDrawContours( obraz3, aktualny_kontur, color0, color0, -1, CV_FILLED, 8, cvPoint(0,0));
            licznik=licznik+5;
        }
        if(przyblizona_pow>0.91*max && przyblizona_pow<0.97*max){//1zl

            cvDrawContours( obraz3, aktualny_kontur, color1, color1, -1, CV_FILLED, 8, cvPoint(0,0));
            licznik=licznik+1;
        }
        if(przyblizona_pow>0.82*max && przyblizona_pow<=0.91*max){//2zl

            cvDrawContours(obraz3, aktualny_kontur, color2, color2, -1, CV_FILLED, 8, cvPoint(0,0));
            licznik=licznik+2;
        }
        if(przyblizona_pow>0.78*max && przyblizona_pow<=0.82*max){//50gr

            cvDrawContours( obraz3, aktualny_kontur, color3, color3, -1, CV_FILLED, 8, cvPoint(0,0));
            licznik=licznik+0.5;
        }

        if(przyblizona_pow>0.66*max && przyblizona_pow<=0.78*max){//5gr

            cvDrawContours( obraz3, aktualny_kontur, color4, color4, -1, CV_FILLED, 8, cvPoint(0,0));
            licznik=licznik+0.05;
        }
        if(przyblizona_pow>0.63*max && przyblizona_pow<=0.66*max){//20gr

            cvDrawContours( obraz3, aktualny_kontur, color5, color5, -1, CV_FILLED, 8, cvPoint(0,0));
            licznik=licznik+0.2;
        }
        if(przyblizona_pow>0.59*max && przyblizona_pow<=0.63*max){//2gr

            cvDrawContours( obraz3, aktualny_kontur, color6, color6, -1, CV_FILLED, 8, cvPoint(0,0));
            licznik=licznik+0.02;
        }
        if(przyblizona_pow>0.57*max && przyblizona_pow<=0.59*max){//10gr

            cvDrawContours( obraz3, aktualny_kontur, color7, color7, -1, CV_FILLED, 8, cvPoint(0,0));
            licznik=licznik+0.1;
        }
        if(przyblizona_pow>0.50*max && przyblizona_pow<=0.57*max){//1gr

            cvDrawContours( obraz3, aktualny_kontur, color8, color8, -1, CV_FILLED, 8, cvPoint(0,0));
            licznik=licznik+0.01;
        }


    }


    std::cout<<"max= "<<max<<endl;
    std::cout<<"suma pieniedzy to: "<<licznik<<"zl"<<endl;
    cvShowImage("okno",obraz3);
    cvSaveImage("11.jpg",obraz3);
    waitKey(0);


    return 0;
}





