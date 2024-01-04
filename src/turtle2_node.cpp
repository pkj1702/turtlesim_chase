#include <stdio.h>
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
ros::Publisher pub2;

void cb_pose1(turtlesim::Pose msg){
    pose1.position.x = msg.x;
    pose1.position.y = msg.y;
}

void cb_pose2(turtlesim::Pose msg){
    dist = sqrt(pow(pose1.position.x - pose2.position.x,2)+pow(pose1.position.y - pose2.position.y,2));
    pose2.position.x = msg.x;
    pose2.position.y = msg.y;
    cmd_vel2.linear.x = (pose1.position.x - pose2.position.x)*2/dist; 
    cmd_vel2.linear.y = (pose1.position.y - pose2.position.y)*2/dist;
    //Prevent dist=0
    if(dist<=0.01){
        cmd_vel2.linear.x = 0;
        cmd_vel2.linear.y = 0;
    }
    pub2.publish(cmd_vel2);
}

int main(int argc, char** argv){
    ros::init(argc,argv, "turtle2_node");
    ros::NodeHandle nh;
    ros::Rate rate(20);
    ros::Subscriber sub1 = nh.subscribe("/turtle1/pose", 10, cb_pose1);
    ros::Subscriber sub2 = nh.subscribe("/turtle2/pose", 10, cb_pose2);
    
    pub2 = nh.advertise<geometry_msgs::Twist>("turtle2/cmd_vel", 10);
    //Turtle2 spawn
    ros::ServiceClient spawnClient = nh.serviceClient<turtlesim::Spawn>("spawn");
    turtlesim::Spawn spawn;
    spawn.request.x = 2;
    spawn.request.y = 2;
    spawn.request.theta = 0;

    //Repeat until turtlesim turns on
    while(!spawnClient.call(spawn)){
        ROS_ERROR("Failed to spawn turtle2");
    }
    ROS_INFO("Successfully spawned turtle2");
    ros::spin();
}