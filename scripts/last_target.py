#! /usr/bin/env python
import rospy
from std_srvs.srv import *
from assignment_2_2023.srv import Last_target, Last_targetResponse






#when called the server does this
def print_targ_pos(req):
	targ_x = rospy.get_param('des_pos_x')
	targ_y = rospy.get_param('des_pos_y')
	return Last_targetResponse (targ_x, targ_y);



def main ():
	rospy.init_node ('last_target')
	serv = rospy.Service ('get_last_target', Last_target, print_targ_pos)
	rospy.spin ()
	
		
		
if __name__ == '__main__':
	main()
