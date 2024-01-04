#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <geometry_msgs/Twist.h>
#include <ros/ros.h>
#include <turtlesim/Spawn.h>
#include <turtlesim/Pose.h>
#include <geometry_msgs/Pose.h>
#include <cmath>

geometry_msgs::Pose pose1;
geometry_msgs::Pose pose2;
float dist = sqrt(pow(pose1.position.x - pose2.position.x,2)+pow(pose1.position.y - pose2.position.y,2));
geometry_msgs::Twist cmd_vel1;
geometry_msgs::Twist cmd_vel2;

//////////////////////////////////////////////////////////////////////////////////////////////////

int getKey(void){
  int ch;
  struct termios oldt;
  struct termios newt;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;

  newt.c_lflag &= ~(ICANON | ECHO);
  newt.c_iflag |= IGNBRK;
  newt.c_iflag &= ~(INLCR  | ICRNL | IXON  | IXOFF);
  newt.c_lflag &= ~(ICANON | ECHO  | ECHOK | ECHOE | ECHONL | ISIG | IEXTEN);
  newt.c_cc[VMIN] = 1;
  newt.c_cc[VTIME] = 0;
  tcsetattr(fileno(stdin), TCSANOW, &newt);

  ch = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

  return ch;
}

void printInfo(){
  puts("Remote Control turtle of turtlesim_node");
  puts("---------------------------------------");
  puts("               (forward)               ");
  puts("                   w                   ");
  puts("  (turn-left) a    s    d (turn-right) ");
  puts("                (back)                 ");
  puts("---------------------------------------");
  puts("### type Ctrl-C to quit                ");
  puts("");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv){

    ros::init(argc,argv, "turtle1_node");
    ros::NodeHandle nh;
    ros::Rate rate(20);
    ros::Publisher pub1 = nh.advertise<geometry_msgs::Twist>("turtle1/cmd_vel", 10);
    while(ros::ok()){
        printInfo();
        int input= getKey();
        if(input =='w'){
            cmd_vel1.linear.x=0;
            cmd_vel1.linear.y=2;
        }
        else if(input=='a'){
            cmd_vel1.linear.x=-2;
            cmd_vel1.linear.y=0;
        }
        else if(input=='s'){
            cmd_vel1.linear.x=0;
            cmd_vel1.linear.y=-2;
        }
        else if(input=='d'){
            cmd_vel1.linear.x=2;
            cmd_vel1.linear.y=0;
        }
        pub1.publish(cmd_vel1);
        ros::spinOnce();
    }
    // ros::Publisher pub1=nh.advertise<geometry_msgs::Twist>("turtle1/cmd_vel", 10);
    // ros::Subscriber turtle1_sub=nh.subscribe("/turtle1/pose", 10, cb_pose1);
    // ros::Subscriber turtle2_sub=nh.subscribe("/turtle2/pose", 10, cb_pose2);
    // ros::ServiceClient spawnClient = nh.serviceClient<turtlesim::Spawn>("spawn");
    // turtlesim::Spawn spawn;
    // spawn.request.x = 2;
    // spawn.request.y = 2;
    // spawn.request.theta = 0;
    // if (spawnClient.call(spawn))
    // {
    //     ROS_INFO("Successfully spawned turtle2");
    // }
    // else
    // {
    //     ROS_ERROR("Failed to spawn turtle2");
    //     return 1;
    // }
    // while(ros::ok()){
    //     printInfo();
    //     int input= getKey();
    //     if(input =='w'){
    //         cmd_vel1.linear.x=0;
    //         cmd_vel1.linear.y=2;
    //     }
    //     else if(input=='a'){
    //         cmd_vel1.linear.x=-2;
    //         cmd_vel1.linear.y=0;
    //     }
    //     else if(input=='s'){
    //         cmd_vel1.linear.x=0;
    //         cmd_vel1.linear.y=-2;
    //     }
    //     else if(input=='d'){
    //         cmd_vel1.linear.x=2;
    //         cmd_vel1.linear.y=0;
    //     }
    //     pub1.publish(cmd_vel1);
        //*******************************Method1************************************************
        // turtle2 only follows turtle1's previous position.
        // ros::spinOnce();
        // cmd_vel2.linear.x = (pose1.position.x - pose2.position.x); 
        // cmd_vel2.linear.y = (pose1.position.y - pose2.position.y);
        // pub2.publish(cmd_vel2);
        //**************************************************************************************
        //*******************************Method2************************************************
        //turtle2 follows turtle1 well
        //but turtle1 moves after turtle2 catches up with turtle1
        // ros::Duration(0.1).sleep();
        // ros::spinOnce();
        // while(dist>=0.1){
        //     cmd_vel2.linear.x = (pose1.position.x - pose2.position.x)*2/dist; 
        //     cmd_vel2.linear.y = (pose1.position.y - pose2.position.y)*2/dist;
        //     //make cmd_vel2=2m/s 
        //     pub2.publish(cmd_vel2);
        //     dist = sqrt(pow(pose1.position.x - pose2.position.x,2)+pow(pose1.position.y - pose2.position.y,2));
        //     ros::spinOnce();
        //     rate.sleep();
        // }
        //***************************************************************************************
    ros::spin();
    return 0;
}


