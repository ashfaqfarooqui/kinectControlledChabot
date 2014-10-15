#include "handdetect.h"


using namespace cv;
using namespace std;
using namespace xn;
void findHand()
{
    int range=50;
    pos start,eend;
    g_DepthGenerator.ConvertRealWorldToProjective(1,&skeletonPoints[XN_SKEL_LEFT_HAND],&skeletonPoints[XN_SKEL_LEFT_HAND]);
    start.x=skeletonPoints[XN_SKEL_LEFT_HAND].X-range;
    if(start.x<0)
    {
        start.x=0;
    }

    start.y=skeletonPoints[XN_SKEL_LEFT_HAND].Y-range;
    if(start.y<0)
    {
        start.y=0;
    }
    eend.x=skeletonPoints[XN_SKEL_LEFT_HAND].X+range;
    if(eend.x>640)
    {
        eend.x=640;
    }
    eend.y=skeletonPoints[XN_SKEL_LEFT_HAND].Y+range;
    if(eend.y>480)
    {
        eend.y=480;
    }
    //cout<<"start:"<<start.x<<","<<start.y<<endl;
    //cout<<"end:"<<eend.x<<","<<eend.y<<endl;
    int a=0;
    int b=0;
    for(int i=start.y;i<eend.y;i++,b++)
    {
       a=0;
        for(int j=start.x;j<eend.x;j++,a++)
        {
            handLeft[a][b]=depth_map[i][j];
        }
    }


 //writeDepthMap(start,eend,"HandLeft_data");

 if(calib!=true)
 {
    cnt++;
    if(cnt>20)
        calib=true;
 }
 if(calib==true)
 {

 Mat A=Mat(480,640,CV_16U,depth_map);
 //A.convertTo(A,CV_8U);
rectangle(A,cvPoint(start.x,start.y),cvPoint(eend.x,eend.y),CV_RGB(0,0,255),3,8,0);
  //blur( A, A, Size(3,3) );
//Canny(A,A,10,30,3);
imshow("Full",A);
 cvWaitKey(10);


}

 //   ImageMetaData imageData;
   // XnSkeletonJointPosition pos;


   // cout<<"enter"<<endl;
   // imageGenerator.StartGenerating();

// imageGenerator.GetMetaData(imageData);
 //if(imageGenerator.IsCapabilitySupported(XN_CAPABILITY_ALTERNATIVE_VIEW_POINT)){
  //  cout<<"supported"<<endl;
//imageGenerator.GetAlternativeViewPointCap().SetViewPoint(g_DepthGenerator);
//}
  //  const XnGrayscale16Pixel* img=imageData.Grayscale16Data();
  //for (int a=0; a<XN_1080P_Y_RES; a++)
    //{
      //  for(int b=0; b<XN_1080P_X_RES; b++)
        //{
          //  image[a][b]=img[a*XN_1080P_X_RES  + b];
        //}

    //}
   // B=Mat(480,640,CV_16U);

    //Mat A=Mat(XN_1080P_Y_RES,XN_1080P_X_RES,CV_16U,image);
   //Size s=cvSize(640,480);
//cout<<"constructing hand"<<endl;
   // g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(user, XN_SKEL_LEFT_SHOULDER,pos);
  // struct pos{
  // int x;
   //int y;}start,endd;

    //if(userJointPosition[0].fConfidence>0.5)
    //{
      //  start.y=userJointPosition[0].position.Y-100;
        //if(start.y<0)
        //{
          //  start.y=0;
        //}
        //start.x=userJointPosition[0].position.X-100;
         //if(start.x<0)
        //{
          //  start.x=0;
        //}

         //endd.y=userJointPosition[0].position.Y+100;
        //if(start.y>XN_1080P_Y_RES)
        //{
          //  start.y=XN_1080P_Y_RES;
        //}
        //endd.x=userJointPosition[0].position.X+100;
         //if(start.x>XN_1080P_X_RES)
        //{
         //   start.x=XN_1080P_Y_RES;
     //   }

   // }


//cout<<img<<endl;
//    A=Mat(imageData.XRes(),imageData.YRes(),CV_8UC3, );
//cout<<A<<endl;
//imshow("name",A);


//cout<<"enter"<<endl;
//cvNamedWindow("aimg");

//  Mat grImage;

//VideoCapture capture;
//cvNamedWindow("name",CV_WINDOW_AUTOSIZE);
//cv::VideoCapture kinect;
//capture.open(CV_CAP_OPENNI);
//  if( !capture.grab() )
//       {
    //   cout << "Can not grab images." << endl;
//            return -1;
//       }

//capture.retrieve( grImage, CV_CAP_OPENNI_BGR_IMAGE );



//imshow("name",grImage);
//cvWaitKey(1);

//cvShowImage("name",bgrImage);

    // IplImage* img=new IplImage(bgrImage);
    // cvShowImage("window",bgrImage);

}
