#! /usr/bin/env python
import rospy
from assignment_2_2023.srv import Distance_AvgSpeed, Distance_AvgSpeedResponse
from assignment_2_2023.msg import Pos_vel
import math



w = rospy.get_param('avg_win')
curr_x = 0
curr_y = 0
curr_vx = []
curr_vy = []
def callback(msg):
	curr_x = msg.x
	curr_y = msg.y
	if (len (curr_vx) >= w):
		curr_vx.pop (0)
		curr_vy.pop (0)
	curr_vx.append (msg.vel_x)
	curr_vy.append (msg.vel_y)
	


def requestresponse (req):
	targ_x = rospy.get_param('des_pos_x')
	targ_y = rospy.get_param('des_pos_y')
	dist_x = targ_x - curr_x
	dist_y = targ_y - curr_y
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
	serv = rospy.Service ('/Dist_avg_speed', Distance_AvgSpeed, requestresponse)
	rospy.spin()
	
if __name__ == '__main__':
	main()
	
