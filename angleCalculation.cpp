#include <angleCalculation.h>


XnSkeletonJointPosition userJointPosition[10];
float angles[10][2];
double filteredAngles[10][2];
float filterAngles[10][2][10];
XnSkeletonJointOrientation userJointOrientation[10];
static long count=0;

void userAngleData(XnUserID user)
{
    findUserOrdinates(user);
}

void findUserOrdinates(XnUserID user)
{
    g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(user, XN_SKEL_HEAD,userJointPosition[HEAD]);
    g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(user,XN_SKEL_LEFT_SHOULDER,userJointPosition[LEFT_SHOULDER]);
   // g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(user,XN_SKEL_TORSO,userJointPosition[HI]);

    ordinates(user,XN_SKEL_LEFT_SHOULDER,LEFT_SHOULDER);
    ordinates(user,XN_SKEL_RIGHT_SHOULDER,RIGHT_SHOULDER);
    ordinates(user,XN_SKEL_LEFT_ELBOW,LEFT_ELBOW);
    ordinates(user,XN_SKEL_RIGHT_ELBOW,RIGHT_ELBOW);
    ordinates(user,XN_SKEL_RIGHT_HIP,RIGHT_HIP);
    ordinates(user,XN_SKEL_LEFT_HIP,LEFT_HIP);

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
        angles[loc][Y_ANGLE]=acosf((float)col1.Y)*57.295;
        angles[loc][Z_ANGLE]=asinf((float)col1.Z)*57.295;
        averagingFilter(loc);
        break;
    case XN_SKEL_RIGHT_SHOULDER:
        angles[loc][Y_ANGLE]=(acosf((float)col1.Y)*57.295);
        angles[loc][Z_ANGLE]=(acosf((float)col1.Z)*57.295);
        averagingFilter(loc);
        break;
    case XN_SKEL_LEFT_ELBOW:
        angles[loc][Y_ANGLE]=acosf((float)col1.Y)*57.295;
        angles[loc][Z_ANGLE]=abs(acosf((float)col1.Z)*57.295);
        averagingFilter(loc);
        break;
    case XN_SKEL_RIGHT_ELBOW:
        angles[loc][Y_ANGLE]=acosf((float)col1.Y)*57.295;
        angles[loc][Z_ANGLE]=abs(acosf((float)col1.Z)*57.295);
        averagingFilter(loc);
        break;
    }


    write_data();

}
void averagingFilter(int loc)
{
    double angleSum[2];
    for(int i=0; i<9; i++)
    {
        filterAngles[loc][Y_ANGLE][i]=filterAngles[loc][Y_ANGLE][i+1];
        filterAngles[loc][Z_ANGLE][i]=filterAngles[loc][Z_ANGLE][i+1];
    }
    filterAngles[loc][Y_ANGLE][9]=angles[loc][Y_ANGLE];
    filterAngles[loc][Z_ANGLE][9]=angles[loc][Z_ANGLE];

    for(int i=0; i<10; i++)
    {
        angleSum[Y_ANGLE]+=filterAngles[loc][Y_ANGLE][i];
        angleSum[Z_ANGLE]+=filterAngles[loc][Z_ANGLE][i];
    }
    filteredAngles[loc][Y_ANGLE]=angleSum[Y_ANGLE]/10;
    filteredAngles[loc][Z_ANGLE]=angleSum[Z_ANGLE]/10;
}

void write_data()
{
    printf("\033[2J");
    printf("Left Shoulder: %f \t %f\n",filteredAngles[LEFT_SHOULDER][Y_ANGLE],filteredAngles[LEFT_SHOULDER][Z_ANGLE]);
    printf("Left Elbow: %f \t %f\n",filteredAngles[LEFT_ELBOW][Y_ANGLE],filteredAngles[LEFT_ELBOW][Z_ANGLE]);
    printf("Right Shoulder: %f \t %f\n",filteredAngles[RIGHT_SHOULDER][Y_ANGLE],filteredAngles[RIGHT_SHOULDER][Z_ANGLE]);
    printf("Right Elbow: %f \t %f\n",filteredAngles[RIGHT_ELBOW][Y_ANGLE],filteredAngles[RIGHT_ELBOW][Z_ANGLE]);
    printf("count:%d\n",count);
    count++;
    if(count>150){
    setAngles();
    count =0;}
}
