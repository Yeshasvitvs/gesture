#include <ros/ros.h>
#include <geometry_msgs/Pose.h>
#include <std_msgs/Char.h>
#include <sensor_msgs/JointState.h>
#include <moveit/move_group_interface/move_group.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>
#include <moveit_msgs/PlanningScene.h>
#include <moveit_msgs/DisplayTrajectory.h>
#include <stdio.h>
#include <iostream>


using namespace std;


bool left_success;
bool right_success;
double left_planning_time;
double right_planning_time;

geometry_msgs::Pose targetPose; //Target pose variable
ros::Subscriber gesture_sub; //Gesture subscriber
char p;

//Subscriber callback function
void gestureCallback(std_msgs::Char gesture_rec)
{
	//cout<<"Gesture received"<<endl;
	p = gesture_rec.data;
	
	
}


int main(int argc, char** argv)
{

	ros::init(argc,argv,"gesture");
	ros::NodeHandle nh;
	ros::AsyncSpinner spinner(1); // Use 4 threads
    	spinner.start();
   	
	//Move group variables
	moveit::planning_interface::MoveGroup left_group("left_arm");
	moveit::planning_interface::MoveGroup right_group("right_arm");

	//Plan variables
	moveit::planning_interface::MoveGroup::Plan left_plan;
	moveit::planning_interface::MoveGroup::Plan right_plan;
	
	moveit::planning_interface::PlanningSceneInterface planning_scene_interface;

	gesture_sub = nh.subscribe<std_msgs::Char>("/gesture_detected",1,gestureCallback);
		
	ros::Rate rate(10);
	while(ros::ok())
	{
	
		std::cout << "Inside ROS OK!" << std::endl;//Debug Code
  		left_planning_time = 0;
		right_planning_time = 0;	
		
		//right_group.setRandomTarget();
		//right_group.move();  	
		
  		cout<<"Have to perform gesture" << endl;
  	
 		if(p == 65)//char A
 		{ 
 			std::cout<<"Received Gesture A "<<std::endl;//Debug Code
 			
 			targetPose.position.x=0.8;	//Set the end effector pose values for each gesture
			targetPose.position.y=0.3;
			targetPose.position.z=-0.1;
			//targetPose.orientation.x=0.0115213171449;
			//targetPose.orientation.y=0.999834934573;
			//targetPose.orientation.z=0.0139595637967;
			targetPose.orientation.w=1.0;
			
	}
  	
		else if(p == 66)//char B
 		{ 
 			std::cout<<"Received Gesture B "<<std::endl;//Debug Code
 			targetPose.position.x=0.80;	//Set the end effector pose values for each gesture
			targetPose.position.y=-0.00;
			targetPose.position.z=-0.17;
			targetPose.orientation.x=0.0;
			targetPose.orientation.y=-0.0;
			targetPose.orientation.z=0.0;
			targetPose.orientation.w=-0.0;
 	 	}
		else if(p == 67)//char C
 		{ 
 			std::cout<<"Received Gesture C "<<std::endl;//Debug Code
 			targetPose.position.x=0.69;	//Set the end effector pose values for each gesture
			targetPose.position.y=-0.35;
			targetPose.position.z=-0.19;
			targetPose.orientation.x=0.0;
			targetPose.orientation.y=0.0;
			targetPose.orientation.z=0.0;
			targetPose.orientation.w=0.0;
 	 	}


	
  		left_group.setPoseTarget(targetPose);
  		right_group.setPoseTarget(targetPose);
  		
  		left_planning_time = ros::Time::now().toSec();
		left_success = left_group.plan(left_plan);
		std::cout << "Left plan success : " << left_success << std::endl;//Debug Code
		left_planning_time = ros::Time::now().toSec() - left_planning_time;
		
  		right_planning_time = ros::Time::now().toSec();
		right_success = right_group.plan(right_plan);
		std::cout << "Right plan success : " << right_success << std::endl;//Debug Code
		right_planning_time = ros::Time::now().toSec() - right_planning_time;


		std::cout << "Left planning time "<< left_planning_time << std::endl;//Debug Code
		std::cout << "Right planning time "<< right_planning_time << std::endl;//Debug Code
		
		if( left_success == false && right_success == false ){

			std::cout << "Moving right arm to Random pose "<<std::endl;
			//right_group.setRandomTarget();
			//right_group.move();
		}
		else if(right_planning_time < left_planning_time){

			if(right_success == true ){ 
			
				right_group.move(); 
			
			}
			else { 
				left_group.move(); 
			}
		
		}
		else{ 
			
  		  	if(left_success == true ){ left_group.move(); }
			else { right_group.move(); }
  		}
  	
  	rate.sleep() ;
  	ros::waitForShutdown();
	}
	return 0;
}	
