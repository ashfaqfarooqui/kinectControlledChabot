#include "handdetect.h"


using namespace cv;
using namespace std;
using namespace xn;
void findHand(int hand)
{
    int range;


    if(calib!=true)
    {
        cnt++;
        if(cnt>20)
            calib=true;
    }
    if(calib==true)
    {
        pos start,eend;
        g_DepthGenerator.ConvertRealWorldToProjective(1,&skeletonPoints[hand],&skeletonPoints[hand]);
        if(skeletonPoints[hand].Z<2000)
        {
            range=55;
        }
        else if(skeletonPoints[hand].Z>2000)
        {
            range=40;
        }
        start.x=skeletonPoints[hand].X-range;
        if(start.x<0)
        {
            start.x=0;
        }

        start.y=skeletonPoints[hand].Y-range;
        if(start.y<0)
        {
            start.y=0;
        }
        eend.x=skeletonPoints[hand].X+range;
        if(eend.x>XN_VGA_X_RES)
        {
            eend.x=XN_VGA_X_RES;
        }
        eend.y=skeletonPoints[hand].Y+range;
        if(eend.y>XN_VGA_Y_RES)
        {
            eend.y=XN_VGA_Y_RES;
        }
        Mat view=Mat(XN_VGA_Y_RES,XN_VGA_X_RES,CV_16U,depth_map);
        try
        {
            Mat handImg=Mat(view,Rect(Point(start.x,start.y),Point(eend.x,eend.y)));
            handImg.convertTo(handImg,CV_32F);
            threshold(handImg,handImg,(skeletonPoints[hand].Z+50)*10,255,THRESH_BINARY_INV);
            handImg.convertTo(handImg,CV_8U);
            rectangle(view,cvPoint(start.x,start.y),cvPoint(eend.x,eend.y),CV_RGB(0,0,255),3,8,0);
            // blur( handImg, handImg, Size(3,3) );
            //  Canny(handImg,handImg,10,30,3);
            /*Mat hist;
            int histSize=64;
            calcHist(&handImg, 1, 0, Mat(), hist, 1, &histSize, 0);
            imshow("hist",hist);
            cout<<hist<<endl;
            cout<<&hist<<endl;*/
            double area=calculateArea(handImg,eend.x-start.x,eend.y-start.y);

            if(hand==XN_SKEL_LEFT_HAND)
            {
                imshow("LeftHand",handImg);
                LeftHandState=isHandOpen(area,range);

            }
            if(hand==XN_SKEL_RIGHT_HAND)
            {
                imshow("RightHand",handImg);
                RightHandState=isHandOpen(area,range);
            }
            imshow("Viewer",view);

            cvWaitKey(10);
        }
        catch(exception e)
        {
            cerr<<"roi out of range"<<endl;
        }



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
void generateDepthMap()
{


    int i,j=0;
    const XnDepthPixel* pDepthMap;
    DepthMetaData depthMD;
    g_DepthGenerator.GetMetaData(depthMD);
    pDepthMap= g_DepthGenerator.GetDepthMap();

    for (int a=0; a<XN_VGA_Y_RES; a++)
    {
        for(int b=0; b<XN_VGA_X_RES; b++)
        {

            depth_map[a][b]=(pDepthMap[(a*XN_VGA_X_RES + b)])*10;
        }

    }

}
double calculateArea(Mat img,int col,int row)
{


    Mat A = Mat::ones(row, col, CV_8U);
    return A.dot(img);
}
bool isHandOpen(double area,int range)
{
    /* cout<<"\033[2J"<<endl;
       cout<<area/(range*255)<<endl;
       cout<<(range)<<endl;*/
    return (range==40 && area/(range*range*255)>.8)||(range==55 && area/(range*255)>40)?true:false;
}
