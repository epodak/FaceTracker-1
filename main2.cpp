/**
 * @file objectDetection.cpp
 * Remixed by Hazim Bitar
 * Based on code writtenby A. Huaman ( based in the classic facedetect.cpp in samples/c )
 * @brief A simplified version of facedetect.cpp, show how to load a cascade classifier and how to find objects (Face + eyes) in a video stream
 * Mdified to intercept X,Y of center of face object and sends them to Arduino Uno via serial
 */
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
//////#include "Tserial.h"
#include "myserialtest.cpp"

using namespace std;
using namespace cv;

/** Function Headers */
void detectAndDisplay( Mat frame, int fd );

/** Global variables */
//-- Note, either copy these two files from opencv/data/haarscascades to your current folder, or change these locations
String face_cascade_name = "/Users/pratu16x7/homebrew/Cellar/opencv/2.4.9/share/OpenCV/haarcascades/haarcascade_frontalface_alt.xml";
String eyes_cascade_name = "/Users/pratu16x7/homebrew/Cellar/opencv/2.4.9/share/OpenCV/haarcascades/haarcascade_eye_tree_eyeglasses.xml";
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;
string window_name = "Capture - Face detection";

// Serial to Arduino global declarations
//    int arduino_command;
//    myserialtest *ard;

int actualx, actualy;
int valx, valy;


int main( int argc, const char** argv )
{
  CvCapture* capture;
  Mat frame;
    int fd = 0;
    int baudrate = B57600;

//    ard = new myserialtest();
//    fd = ard->serialport_init("/dev/tty.usbmodem1421", baudrate);

  //-- 1. Load the cascades
  if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };
  if( !eyes_cascade.load( eyes_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };
 
  //-- 2. Read the video stream
  capture = cvCaptureFromCAM( -1 );
  if( capture )
  {
    while( true )
    {
      frame = cvQueryFrame( capture );
      //-- 3. Apply the classifier to the frame
      if( !frame.empty() )
       { detectAndDisplay( frame, fd ); }
      else
       { printf(" --(!) No captured frame -- Break!"); break; }
      
      int c = waitKey(10);
      if( (char)c == 'c' ) { break; } 
    }
  }
    
  return 0;
}

/**
 * @function detectAndDisplay
 */
void detectAndDisplay( Mat frame, int fd )
{
 
   std::vector<Rect> faces;
   Mat frame_gray;
    int rc1, rc2;

   cvtColor( frame, frame_gray, CV_BGR2GRAY );
   equalizeHist( frame_gray, frame_gray );
   //-- Detect faces
   face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );

   for( int i = 0; i < faces.size(); i++ )
    {
      Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );
      ellipse( frame, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 2, 8, 0 );
	  cout << "X:" << faces[i].x  <<  "  y:" << faces[i].y  << endl;
        
        actualx = faces[i].x;
        actualy = faces[i].y;
        
        valx = actualx/5;
        //rc1 = ard->serialport_writebyte(fd, (uint8_t)valx);
        
        valy = actualy/5;
        //rc2 = ard->serialport_writebyte(fd, (uint8_t)valy);

        Mat faceROI = frame_gray( faces[i] );
      std::vector<Rect> eyes;
    } 

   //-- Show what you got
   imshow( window_name, frame );

}
