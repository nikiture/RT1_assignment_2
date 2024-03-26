#! /usr/bin/env python
##
# \file last_target.py
# \brief server node providing the service of with returning the last target sent as request to the bug 0 action server
# \author Torre Nicolò
# \details
#
# \param [in] des_pos_x x-coordinate of the last sent goal's position
#
# \param [in] des_pos_y y-coordinate of the last sent goal's position
#
# Service: <BR>
# /get_last_target
#
# description: 
# This node is a server providing, when requested, the last target position sent to the bug 0 action server. The requests and the responses are sent through the /get_last_target service using the Last_target custom service message

import rospy
from std_srvs.srv import *
from assignment_2_2023.srv import Last_target, Last_targetResponse




##
# \brief service request callback function
# \param receives the custom service message Last_target, empty on the request side
# \return returns the x-y coordinates of the last target requested to the bug 0 actionserver
# this function is called when a request is sent to the node, and the target position's coordinates are read from ROS' parameter server and sent back to the caller


def print_targ_pos(req):
#when called the server does this
	targ_x = rospy.get_param('des_pos_x')
	targ_y = rospy.get_param('des_pos_y')
	return Last_targetResponse (targ_x, targ_y);



def main ():
	rospy.init_node ('last_target')
	serv = rospy.Service ('/get_last_target', Last_target, print_targ_pos)
	rospy.spin ()
	
		
		
if __name__ == '__main__':
	main()
