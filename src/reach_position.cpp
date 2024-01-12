#include "ros/ros.h"
#include <iostream>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include "assignment_2_2023/PlanningAction.h"
#include "geometry_msgs/PoseStamped.h"
#include "geometry_msgs/Pose.h"
#include <tf2/LinearMath/Quaternion.h>
#include "geometry_msgs/Twist.h"
#include "assignment_2_2023/Pos_vel.h"
#include "nav_msgs/Odometry.h"




#define des_rate 20.0			
	

assignment_2_2023::Pos_vel pub_msg;

void process_input_odom (const nav_msgs::Odometry::ConstPtr& msg) {
//here the messages from the topic /odom are read and written in the custom message Pos_vel
	pub_msg.x = msg->pose.pose.position.x;
	pub_msg.y = msg->pose.pose.position.y;
	pub_msg.vel_x = msg->twist.twist.linear.x;
	pub_msg.vel_y = msg->twist.twist.linear.y;
}

int main (int argc, char ** argv) {
	
	
	ros::init (argc, argv, "reach_position");
	ros::NodeHandle nh;
	
	ros::Rate rate (des_rate);
	
	
	ros::Publisher pub;
	ros::Subscriber sub;
	actionlib::SimpleActionClient<assignment_2_2023::PlanningAction> move_plan ("/reaching_goal", true);


	ROS_INFO ("connecting to server...");
	move_plan.waitForServer();
	
	pub = nh.advertise <assignment_2_2023::Pos_vel> ("/act_pos_vel", 1);
	
	
	sub = nh.subscribe ("/odom",1, process_input_odom);
	
	ROS_INFO ("connection successful, sending goal");
	assignment_2_2023::PlanningGoal Goal;
	geometry_msgs::Pose Targ;
	
	ros::param::get ("/des_x", Targ.position.x);
	ros::param::get ("/des_y", Targ.position.y);

	Goal.target_pose.pose = Targ;
	


	move_plan.sendGoal (Goal);
	
	while (ros::ok()) {
		if (move_plan.waitForResult(ros::Duration (1/(4 * des_rate)))){ //this condition is true if the action server has finished (that is to say the target position has been reached
			ROS_INFO ("target position reached!");
			exit (EXIT_SUCCESS);
		}
			
		pub.publish (pub_msg);
		rate.sleep();
	}
	
	exit (EXIT_SUCCESS);
	
	return 0;
}


	
	
