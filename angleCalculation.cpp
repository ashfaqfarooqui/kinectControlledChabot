#include <angleCalculation.h>



using namespace std;
using namespace cv;
using namespace xn;


ImageGenerator imageGenerator;
XnSkeletonJointPosition userJointPosition[10];
float angles[10][3];
double filteredAngles[10][3];
float filterAngles[10][3][10];
XnSkeletonJointOrientation userJointOrientation[10];
XnPoint3D skeletonPoints[24];
uint16_t depth_map[480][640];



float angle[15];
float filterAngle[15][10];

bool LeftHandState=false;
bool RightHandState=false;





//static long count=0;

void userAngleData(XnUserID user)
{
    pos zero,res;
    zero.x=0;
    zero.y=0;
    res.x=640;
    res.y=480;
    //findUserOrdinates(user);
    //checkSocketConnection();
    if(g_UserGenerator.GetSkeletonCap().IsTracking(user))
    {
        createSkeletonPointCloud(user);

        angle[LEFT_HIP_SHOULDER_ELBOW]=movingAverageFilter(calculateAngleBetweenPoints(skeletonPoints[XN_SKEL_LEFT_HIP],skeletonPoints[XN_SKEL_LEFT_SHOULDER],skeletonPoints[XN_SKEL_LEFT_ELBOW]),LEFT_HIP_SHOULDER_ELBOW);
        angle[LEFT_HIP_SHOULDER_HAND]=movingAverageFilter(calculateAngleBetweenPoints(skeletonPoints[XN_SKEL_LEFT_HIP],skeletonPoints[XN_SKEL_LEFT_SHOULDER],skeletonPoints[XN_SKEL_LEFT_HAND]),LEFT_HIP_SHOULDER_HAND);
        angle[LEFT_SHOULDER_ELBOW_HAND]=movingAverageFilter(calculateAngleBetweenPoints(skeletonPoints[XN_SKEL_LEFT_SHOULDER],skeletonPoints[XN_SKEL_LEFT_ELBOW],skeletonPoints[XN_SKEL_LEFT_HAND]),LEFT_SHOULDER_ELBOW_HAND)-70;
        angle[RIGHT_HIP_SHOULDER_ELBOW]=movingAverageFilter(calculateAngleBetweenPoints(skeletonPoints[XN_SKEL_RIGHT_HIP],skeletonPoints[XN_SKEL_RIGHT_SHOULDER],skeletonPoints[XN_SKEL_RIGHT_ELBOW]),RIGHT_HIP_SHOULDER_ELBOW);
        angle[RIGHT_HIP_SHOULDER_HAND]=movingAverageFilter(calculateAngleBetweenPoints(skeletonPoints[XN_SKEL_RIGHT_HIP],skeletonPoints[XN_SKEL_RIGHT_SHOULDER],skeletonPoints[XN_SKEL_RIGHT_HAND]),RIGHT_HIP_SHOULDER_HAND);
        angle[RIGHT_SHOULDER_ELBOW_HAND]=movingAverageFilter(calculateAngleBetweenPoints(skeletonPoints[XN_SKEL_RIGHT_SHOULDER],skeletonPoints[XN_SKEL_RIGHT_ELBOW],skeletonPoints[XN_SKEL_RIGHT_HAND]),RIGHT_SHOULDER_ELBOW_HAND)-70;

        ordinates(user,XN_SKEL_RIGHT_HIP,RIGHT_HIP);
        ordinates(user,XN_SKEL_LEFT_HIP,LEFT_HIP);


        //sendData(SHOULDER_PACKET,angle[LEFT_HIP_SHOULDER_ELBOW],angle[RIGHT_HIP_SHOULDER_ELBOW],createdSocket);
        generateDepthMap();
        //sendData(ELBOW_PACKET,angle[LEFT_SHOULDER_ELBOW_HAND],angle[RIGHT_SHOULDER_ELBOW_HAND],createdSocket);
        findHand(XN_SKEL_LEFT_HAND);
        //sendData(HAND_PACKET,angle[LEFT_HIP_SHOULDER_HAND],angle[RIGHT_HIP_SHOULDER_HAND],createdSocket);
         generateDepthMap();
        //sendData(HIP_PACKET,angles[LEFT_HIP][X_ANGLE],angles[RIGHT_HIP][X_ANGLE],createdSocket);
         findHand(XN_SKEL_RIGHT_HAND);
        // sendData(CLAW_PACKET,LeftHandState,RightHandState,createdSocket);
        write_data();
    }

}
float calculateAngleBetweenPoints(XnVector3D joint1,XnVector3D joint2,XnVector3D joint3)
{
    XnVector3D v1,v2;
    v1=calculateVectorBetween(joint2,joint1);
    v2=calculateVectorBetween(joint2,joint3);

    return calculateAngleBetweenVectors(v1,v2);
}

XnVector3D calculateVectorBetween(XnVector3D vector1,XnVector3D vector2)
{
    XnVector3D retVector;
    retVector.X=vector1.X-vector2.X;
    retVector.Y=vector1.Y-vector2.Y;
    retVector.Z=vector1.Z-vector2.Z;

    return retVector;
}
float calculateAngleBetweenVectors(XnVector3D vector1,XnVector3D vector2)
{
    double magV1=(double)magnitude(vector1);
    double magV2=(double)magnitude(vector2);
    XnVector3D v1;
    v1.X=vector1.X/magV1;
    v1.Y=vector1.Y/magV1;
    v1.Z=vector1.Z/magV1;
    XnVector3D v2;
    v2.X=vector2.X/magV2;
    v2.Y=vector2.Y/magV2;
    v2.Z=vector2.Z/magV2;



    double dotProd=dotProduct(v1,v2);

    return ((acosf((dotProd)))*57.295);
}
float magnitude(XnVector3D a)
{

    return sqrt((a.X*a.X)+(a.Y*a.Y)+(a.Z*a.Z));

}
float dotProduct(XnVector3D a,XnVector3D b)
{
    return ((a.X*b.X)+(a.Y*b.Y)+(a.Z*b.Z));
}

void findUserOrdinates(XnUserID user)
{
    // g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(user, XN_SKEL_LEFT_HAND,userJointPosition[0]);
    // g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(user,XN_SKEL_LEFT_SHOULDER,userJointPosition[LEFT_SHOULDER]);
    // g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(user,XN_SKEL_TORSO,userJointPosition[HI]);

    ordinates(user,XN_SKEL_LEFT_SHOULDER,LEFT_SHOULDER);
    ordinates(user,XN_SKEL_RIGHT_SHOULDER,RIGHT_SHOULDER);
    ordinates(user,XN_SKEL_LEFT_ELBOW,LEFT_ELBOW);
    ordinates(user,XN_SKEL_RIGHT_ELBOW,RIGHT_ELBOW);
    ordinates(user,XN_SKEL_RIGHT_HIP,RIGHT_HIP);
    ordinates(user,XN_SKEL_LEFT_HIP,LEFT_HIP);


}
void createSkeletonPointCloud(XnUserID user)
{
    storePointCloud(user,XN_SKEL_HEAD);
    storePointCloud(user,XN_SKEL_LEFT_SHOULDER);
    storePointCloud(user,XN_SKEL_LEFT_ELBOW);
    storePointCloud(user,XN_SKEL_LEFT_HIP);
    storePointCloud(user,XN_SKEL_LEFT_HAND);

    storePointCloud(user,XN_SKEL_RIGHT_SHOULDER);
    storePointCloud(user,XN_SKEL_RIGHT_ELBOW);
    storePointCloud(user,XN_SKEL_RIGHT_HAND);
    storePointCloud(user,XN_SKEL_RIGHT_HIP);
}
void storePointCloud(XnUserID user,XnSkeletonJoint joint)
{
    XnSkeletonJointPosition jointPosition;
    g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(user,joint,jointPosition);

    if(jointPosition.fConfidence>0.5)
    {
        skeletonPoints[joint]=jointPosition.position;
    }
}
void ordinates(XnUserID user,XnSkeletonJoint joint,int loc)
{

    g_UserGenerator.GetSkeletonCap().GetSkeletonJointOrientation(user, joint,userJointOrientation[loc]);
    XnVector3D col1 = xnCreatePoint3D(userJointOrientation[loc].orientation.elements[0], userJointOrientation[loc].orientation.elements[3], userJointOrientation[loc].orientation.elements[6]);
    XnVector3D col2 = xnCreatePoint3D(userJointOrientation[loc].orientation.elements[1], userJointOrientation[loc].orientation.elements[4], userJointOrientation[loc].orientation.elements[7]);
    XnVector3D col3 = xnCreatePoint3D(userJointOrientation[loc].orientation.elements[2], userJointOrientation[loc].orientation.elements[5], userJointOrientation[loc].orientation.elements[8]);
    switch(joint)
    {
    case XN_SKEL_LEFT_SHOULDER:
        angles[loc][X_ANGLE]=(asinf((float)col1.X)*57.295);
        angles[loc][Y_ANGLE]=(acosf((float)col1.Y)*57.295);
        angles[loc][Z_ANGLE]=(asinf((float)col1.Z)*57.295);
        averagingFilter(loc);
        //printf("angle:")
        break;
    case XN_SKEL_RIGHT_SHOULDER:
        angles[loc][X_ANGLE]=180-(acosf((float)col1.X)*57.295);
        angles[loc][Y_ANGLE]=180-(acosf((float)col1.Y)*57.295);
        angles[loc][Z_ANGLE]=180-(acosf((float)col1.Z)*57.295);
        averagingFilter(loc);
        break;
    case XN_SKEL_LEFT_ELBOW:
        angles[loc][X_ANGLE]=(asinf((float)col1.X)*57.295);
        angles[loc][Y_ANGLE]=(acosf((float)col1.Y)*57.295);
        angles[loc][Z_ANGLE]=(asinf((float)col1.Z)*57.295);
        averagingFilter(loc);
        break;
    case XN_SKEL_RIGHT_ELBOW:
        angles[loc][X_ANGLE]=180-(acosf((float)col1.Y)*57.295);
        angles[loc][Y_ANGLE]=180-(acosf((float)col1.Y)*57.295);
        angles[loc][Z_ANGLE]=180-(acosf((float)col1.Z)*57.295);
        averagingFilter(loc);
        break;
    case XN_SKEL_LEFT_HIP:
        angles[loc][X_ANGLE]=acosf((float)col1.X)*57.295;
        angles[loc][Y_ANGLE]=acosf((float)col1.Y)*57.295;
        angles[loc][Z_ANGLE]=acosf((float)col1.Z)*57.295;
        averagingFilter(loc);
        break;
    case XN_SKEL_RIGHT_HIP:
        angles[loc][X_ANGLE]=acosf((float)col1.X)*57.295;
        angles[loc][Y_ANGLE]=acosf((float)col1.Y)*57.295;
        angles[loc][Z_ANGLE]=acosf((float)col1.Z)*57.295;
        averagingFilter(loc);
        break;

    }
}
void averagingFilter(int loc)
{
    double angleSum[3];
    angleSum[0]=angleSum[1]=0;
    for(int i=0; i<9; i++)
    {
        filterAngles[loc][X_ANGLE][i]=filterAngles[loc][X_ANGLE][i+1];
        filterAngles[loc][Y_ANGLE][i]=filterAngles[loc][Y_ANGLE][i+1];
        filterAngles[loc][Z_ANGLE][i]=filterAngles[loc][Z_ANGLE][i+1];
    }
    filterAngles[loc][X_ANGLE][9]=angles[loc][X_ANGLE];
    filterAngles[loc][Y_ANGLE][9]=angles[loc][Y_ANGLE];
    filterAngles[loc][Z_ANGLE][9]=angles[loc][Z_ANGLE];

    for(int i=0; i<10; i++)
    {
        angleSum[X_ANGLE]+=filterAngles[loc][X_ANGLE][i];
        angleSum[Y_ANGLE]+=filterAngles[loc][Y_ANGLE][i];
        angleSum[Z_ANGLE]+=filterAngles[loc][Z_ANGLE][i];
    }
    filteredAngles[loc][X_ANGLE]=(angleSum[X_ANGLE]/10);
    filteredAngles[loc][Y_ANGLE]=(angleSum[Y_ANGLE]/10);
    filteredAngles[loc][Z_ANGLE]=(angleSum[Z_ANGLE]/10);
}

int movingAverageFilter(float angle,int loc)
{
    double angleSum=0;
    for(int i=0; i<9; i++)
    {
        filterAngle[loc][i]=filterAngle[loc][i+1];
    }
    filterAngle[loc][9]=angle;
    for(int i=0; i<9; i++)
    {
        angleSum+=filterAngle[loc][i];
    }
    return angleSum/10;
}

void write_data()
{
    //printf("\033[2J");
    /*
        printf("Left Shoulder: %.2f \t %.2f \t %.2f\n",filteredAngles[LEFT_SHOULDER][X_ANGLE],filteredAngles[LEFT_SHOULDER][Y_ANGLE],filteredAngles[LEFT_SHOULDER][Z_ANGLE]);
        printf("Left Elbow: %.2f \t%.2f \t %.2f\n",filteredAngles[LEFT_ELBOW][X_ANGLE],filteredAngles[LEFT_ELBOW][Y_ANGLE],filteredAngles[LEFT_ELBOW][Z_ANGLE]);
        printf("Right Shoulder:%.2f \t %.2f \t %.2f\n",filteredAngles[RIGHT_SHOULDER][Z_ANGLE],filteredAngles[RIGHT_SHOULDER][Y_ANGLE],filteredAngles[RIGHT_SHOULDER][Z_ANGLE]);
        printf("Right Elbow: %.2f \t%.2f \t %.2f\n",filteredAngles[RIGHT_ELBOW][X_ANGLE],filteredAngles[RIGHT_ELBOW][Y_ANGLE],filteredAngles[RIGHT_ELBOW][Z_ANGLE]);
        printf(" Hip:%.2f \t %.2f \t %.2f\n",(filteredAngles[LEFT_HIP][X_ANGLE]+filteredAngles[RIGHT_HIP][X_ANGLE])/2,(filteredAngles[LEFT_HIP][Y_ANGLE]+filteredAngles[RIGHT_HIP][Y_ANGLE])/2,(filteredAngles[LEFT_HIP][Z_ANGLE]+filteredAngles[RIGHT_HIP][Z_ANGLE])/2);
    */

    // printf("count:%d\n",count);
    //count++;
    //if(count>150){
    //setAngles();
    // count =0;}

    cout<<"\033[2J"<<endl;
    cout<<"Left Hip-Shoulder-elbow:"<<angle[LEFT_HIP_SHOULDER_ELBOW]<<endl;
    cout<<"LEFT_HIP_SHOULDER_HAND:"<<angle[LEFT_HIP_SHOULDER_HAND]<<endl;
    cout<<"LEFT_SHOULDER_ELBOW_HAND:"<<angle[LEFT_SHOULDER_ELBOW_HAND]<<endl;
    cout<<"RIGHT Hip-Shoulder-elbow:"<<angle[RIGHT_HIP_SHOULDER_ELBOW]<<endl;
    cout<<"RIGHT_HIP_SHOULDER_HAND:"<<angle[RIGHT_HIP_SHOULDER_HAND]<<endl;
    cout<<"RIGHT_SHOULDER_ELBOW_HAND:"<<angle[RIGHT_SHOULDER_ELBOW_HAND]<<endl;
    cout<<"Left hand open:"<<LeftHandState<<endl;
    cout<<"Right hand open:"<<RightHandState<<endl;

}

/*
void writeHandToFile()
{
     FILE *p = NULL;

    char *file="HandData";

    char buffer[150];
    size_t len=150;
    p = fopen(file,"wb");

    if(p==NULL)
    {
        printf("NOT succesful \n");
    }
    for (int a=0; a<150; a++)
    {
        for(int b=0;b<150;b++)
        {
            if(handLeft[a][b]==0)
            sprintf(buffer," 0000");
            else if(handLeft[a][b]<1000)
            sprintf(buffer," 0%i\0",handLeft[a][b]);
            else
            sprintf(buffer," %i\0",handLeft[a][b]);
            len=strlen(buffer);
            fwrite(buffer,len,1,p);
        }
        fwrite("\n",1,1,p);

    }

    fclose(p);
}
*/

void writeDepthMap(pos from, pos to, char *file)
{
    FILE *p = NULL;

    // char *file="depth_map";

    char buffer[800];
    size_t len=800;
    p = fopen(file,"wb");

    if(p==NULL)
    {
        printf("NOT succesful \n");
    }
    int i=0;
    int j=0;
    for (int a=from.y; a<to.y; a++)
    {
        j=0;
        for(int b=from.x; b<to.x; b++)
        {
            if(depth_map[a][b]==0)
                sprintf(buffer," 0000");
            else if(depth_map[a][b]<1000)
                sprintf(buffer," 0%i\0",depth_map[a][b]);
            else
                sprintf(buffer," %i\0",depth_map[a][b]);
            len=strlen(buffer);
            fwrite(buffer,len,1,p);
        }
        fwrite("\n",1,1,p);
    }
    fclose(p);

}
