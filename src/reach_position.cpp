/**
*\file reach_position.cpp
*\brief node used as action client to the action server implementation of the bug0 algorithm
*\author Torre Nicolò
*\param [in] des_x indicates the x-coordinate of the goal position
*\param [in] des_y indicates the y-coordinate of the goal position
*
*\details
* Subscribes to: <BR>
* /odom
*
* Publishes to: <BR>
* /act_pos_vel
*
* Action client to: <BR>
* /reaching goal
*
* Description:
* This node uses the desired position received as input and sends a actionlib goal message to the bug0 action server, while also reading from the odometry the current position and linear velocity and
* publishing these values on a custom message. As of now no methods to cancel the goal before it is reached are implemented.
*
*/



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




#define des_rate 20.0 ///< Frequency of the custom message publication		
	

assignment_2_2023::Pos_vel pub_msg; ///< Custom message published periodically by the node, modified evry time a message is received from the /odom topic

/*!
* \brief Callback function for the /odom topic
* \param msg pointer to the message received from the /odom topic
* 
* This function when called copies the values for current position and velocity contained in the odometry message in the Pos_vel custom message to be later published

*/
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


	
	
