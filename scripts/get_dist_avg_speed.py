#! /usr/bin/env python
## 
# \file get_dist_avg_speed.py
# \brief server node providing the current distance from the target and the average speed of the simulated robot
# \author Torre Nicolò
#
# \param[in] avg_win integer value representing the averaging window's width (the number of speed element on which the average is computed)
#
# \param [in] des_pos_x x-coordinate of the goal position
#
#
# \param [in] des_pos_y y-coordinate of the goal position
#
# \details
#
# Subscribes to: <BR>
# /act_pos_vel
#
# Service: <BR>
# /Dist_avg_speed
#
# description: 
# This node is a server node subscribing to the /act_pos_vel topic (from which it receives the current position and velocity of the simulated robot). When a request is received this node provides the
# requester with the current distance fromthe target and the average velocity. the server can be called from the /Dist_avg_speed service

import rospy
from assignment_2_2023.srv import Distance_AvgSpeed, Distance_AvgSpeedResponse
from assignment_2_2023.msg import Pos_vel
import math



w = rospy.get_param('avg_win')
curr_x = 0
curr_y = 0
curr_vx = []
curr_vy = []

##
# \brief callback function for /act_pos_vel topic's messages
# \param msg message received from the topic
# return nothing
#
# This function is a callback for the /act_pos_vel topic and receives the custom message Pos_vel. When called it stores the current x and y positions in two global variables and adds the current velocity's
# components to two lists of size w, removing the first values inserted in the list if the list contains more than avg_win element 

def callback(msg):
	global curr_x, curr_y
	curr_x = msg.x
	curr_y = msg.y
	if (len (curr_vx) >= w): #for the purpose of the server only the last w speeds are to be stored
		curr_vx.pop (0)
		curr_vy.pop (0)
	curr_vx.append (msg.vel_x)
	curr_vy.append (msg.vel_y)
	

##
# \brief service request callback function
# \param receives the custom service message Distance_AvgSpeed, empty on the request side
# \return returns a response to the service request containing the x-y components of the distance from the target and of the average velocity
#
# this function is called whenever a service request is received. When called the distance from the target and the average speed are computed (both split in their x and y components) and sent to the 
# requester through the response to the request

def requestresponse (req):
	global curr_x, curr_y
	targ_x = rospy.get_param('des_pos_x')
	targ_y = rospy.get_param('des_pos_y')
	dist_x = abs(targ_x - curr_x)
	dist_y = abs(targ_y - curr_y)
	avg_x = 0
	avg_y = 0
	for v in curr_vx:
		avg_x += v/w
	for v in curr_vy:
		avg_y += v/w
	return Distance_AvgSpeedResponse (dist_x, dist_y, avg_x, avg_y)
	




def main ():
	rospy.init_node ('Dist_AvgSpeed_Service')
	rospy.Subscriber ('/act_pos_vel', Pos_vel, callback)
	serv = rospy.Service ('get_dist_avg_speed', Distance_AvgSpeed, requestresponse)
	print ("Distance and avg speed service active")
	rospy.spin()
	
#if __name__ == '__main__':
#	main()

main ()	
