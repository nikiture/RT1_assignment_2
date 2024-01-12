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


/*static char getinput () {
	char in;
	std::cin>>in;
	return in;
}*/			
	

assignment_2_2023::Pos_vel pub_msg;

void process_input_odom (const nav_msgs::Odometry::ConstPtr& msg) {
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
//assignment_2_2023::PlanningFeedback curr_pose;
//geometry_msgs::Twist curr_vel;
	ros::Subscriber sub;
	actionlib::SimpleActionClient<assignment_2_2023::PlanningAction> move_plan ("/reaching_goal", true);
//nav_msgs::Odometry odom_pos;


	ROS_INFO ("connecting to server...");
	move_plan.waitForServer();
	
	pub = nh.advertise <assignment_2_2023::Pos_vel> ("/act_pos_vel", 1);
	
	
	sub = nh.subscribe ("/odom",1, process_input_odom);
	
	//values for non-blocking cin
	//std::chrono::milliseconds timeout (1000/80);
	
	
	//values to be read from input
	
	/*Targ.position.x = 0;
	Targ.position.y = 0;
	double des_yaw = 0;
	tf2::Quaternion q;
	q.setRPY (0, 0, des_yaw);
	double qx,qy,qz,qw;*/
	/*Rotation targ_rot = RPY (0.0, 0.0, des_yaw);
	targ_rot.GetQuaternion (Targ.orientation.x, Targ.orientation.y, Targ.orientation.z, Targ.orientation.w);
	*/
	//2 threads used: one (thread1) handles user input for goal modifications, the other one publishes the custom message
	//pthread_t thread1, thread2;
	
	/*int res1 = pthread_create (&thread1, NULL, update_desired_pose, (void *) &move_plan);
	int res2 = pthread_create (&thread2, NULL, update_custom_message, (void *) &pub);*/
	ROS_INFO ("connection successful, sending goal");
	assignment_2_2023::PlanningGoal Goal;
	geometry_msgs::Pose Targ;
	
	ros::param::get ("/des_x", Targ.position.x);
	ros::param::get ("/des_y", Targ.position.y);
	
	/*double des_yaw;
	ros::param::get ("/des_yaw", des_yaw);
	tf2::Quaternion q;
	q.setRPY (0, 0, des_yaw);
	
	Targ.orientation.x = q.getX();
	Targ.orientation.y = q.getY();
	Targ.orientation.z = q.getZ();
	Targ.orientation.w = q.getW();*/
	Goal.target_pose.pose = Targ;
	
	//ROS_INFO ("insert for how long you want the action to run; write 0 to let it run until finished");
	


	move_plan.sendGoal (Goal);
	bool quit = false;
	
	//char input = n;
	//std::future<char> input_value;
	ROS_INFO ("action in process. To stop it press q");
	
	while (ros::ok()) {
		if (move_plan.waitForResult(ros::Duration (1/(4 * des_rate))))
			break;
		/*input_value = std::async (getinput());
		if (input_value.wait_for (timeout) == std::future_status::ready) {
			if (quit = (input_value.get() == 'q')) move_plan.cancelGoal();
		}*/
			
		pub.publish (pub_msg);
		rate.sleep();
	}
	if (quit) 
		ROS_INFO ("action stopped!");
	else
		ROS_INFO ("target position reached!");
	
	exit (EXIT_SUCCESS);
	
	return 0;
}






			


/*void * update_desired_pose (void * in) {
	ros::Rate rate (20.0);
	char input;
	actionlib::SimpleActionClient<assignment_2_2023::PlanningAction> * move_plan_ptr = (actionlib::SimpleActionClient<assignment_2_2023::PlanningAction> *) in;
	
	assignment_2_2023::PlanningGoal Goal;
	geometry_msgs::Pose Targ;
	tf2::Quaternion q;
	double des_yaw = 0, des_x = 0, des_y = 0;
	while (ros::ok () && !move_plan_ptr->getState().isDone()) {
		std::cout<<"press p to insert a new target position or q to interrupt the current target reaching"<<std::endl;
		std::cin>>input;
		switch (input) {
			case 'q':
				
				if (move_plan_ptr->getState().isDone()) {
					ros::ROS_INFO ("the target has already finished, choose a new target position\n");
				}
				else move_plan_ptr->cancelGoal();
				
				break;
			case 'p':
				
				ros::ROS_INFO ("please insert in order the desired x position, the desired y position and the desired yaw\n");
				std::cin>>des_x>>des_y>>des_yaw;
				
				
				Targ.position.x = des_x;
				Targ.position.y = des_y;
				q.setRPY (0, 0, des_yaw);
				Targ.orientation.x = q.getX();
				Targ.orientation.y = q.getY();
				Targ.orientation.z = q.getZ();
				Targ.orientation.w = q.getW();
				Goal.target_pose.pose = Targ;
				
				
				move_plan_ptr->sendGoal (Goal);
				break;
			default: 
				ros::ROS_INFO ("input character not valid, please insert a valid one\n");
				break;
		}
		rate.sleep ();
	}
	exit (EXIT_SUCCESS);
}

void * update_custom_message (void * in){
	ros::Publisher * pub_ptr = (ros::Publisher *) in;
	//here the node reads the actual position from feedback of action message (Planning) and velocity from the odometry message
	//and they are written on the custom message Pos_vel
	
	ros::Rate rate (20.0);
	while (ros::ok()){
		pub_ptr->publish (pub_msg);
		rate.sleep();
	}
	
}*/	
	
