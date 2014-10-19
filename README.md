
#Controlling the Chabot via a Kinect

Chabot, is a humanoid  robot built at Chalmers as a bachalor thesis [link](http://publications.lib.chalmers.se/records/fulltext/199997/199997.pdf).

This project uses a kinect sensor to control the Chabot. OpenNI and OpenCV are used to detect and track a human. The angles are measured using vector calculations and then filtered. which are then sent over a TCP socket to the the robot code to be
deciphered, checked for range and redundancy of data before being sent to the robot.

##File Structure

Required Libraries contains all the libraries to run the kinect. Use this guide [link](http://mitchtech.net/ubuntu-kinect-openni-primesense/) to install the kinect drivers, and required files.
ChabotContol- has the project files and code for the kinect.
Chabot-Hardware interface has the files for the chabot.
