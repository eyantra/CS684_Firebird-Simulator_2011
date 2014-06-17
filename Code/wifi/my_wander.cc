/*
 * my_wander.cc
 *
 *  Created on: May 27, 2011
 *      Author: Anirban Basumallik
 */

#include "stage.hh"
#define THRESHOLD 0.5

using namespace Stg;

// The structure that represents the robot
typedef struct
{
	ModelPosition* pos;
	ModelLinefollower* linefollower;
} robot_t;

// The callback functions needed for handling the changes in the sensor values
int LinefollowerUpdate( ModelLinefollower* mod, robot_t* robot );
int PositionUpdate( Model* mod, robot_t* robot );

// The global variables needed to control the movement of the robot
double forwardSpeed = 0.7, turnSpeed = 0.0;

// Stage calls this when the model starts up
extern "C" int Init( Model* mod, CtrlArgs* args )
{
	// Declare a robot instance
	robot_t* robot = new robot_t;

	// Bind the robot's sensors local instance to the internal representation
	robot->pos = (ModelPosition*)mod;
	robot->linefollower = (ModelLinefollower*)mod->GetUnusedModelOfType( "linefollower" );

	// Bind the local function such that it is called in every loop of stage
	robot->linefollower->AddUpdateCallback( (stg_model_callback_t)LinefollowerUpdate, robot );
	// robot->pos->AddUpdateCallback( (stg_model_callback_t)PositionUpdate, robot );

	robot->linefollower->Subscribe(); // starts the linefollower updates
	robot->pos->Subscribe(); // starts the position updates

	return 0; //ok
}

int stop()
{
	forwardSpeed = 0;
	turnSpeed = 0;
	return 0;
}

int forward()
{
	forwardSpeed = 0.7;
	turnSpeed = 0;
	return 0;
}

int myLeft()
{
	forwardSpeed = 0.1;
	turnSpeed = 40;
	return 0;
}

int myRight()
{
	forwardSpeed = 0.1;
	turnSpeed = -40;
	return 0;
}


// inspect the linefollower data and decide what to do
int LinefollowerUpdate( ModelLinefollower* rgr, robot_t* robot )
{
	double left, middle, right;
	// uint32_t sensor_count = rgr->sensors.size(); // To count the no. of sensors

//	// Uncomment the following to print the values as read from the sensors
//	printf("Values:");
//	for( unsigned int i=0; i < sensor_count; i++ )
//	{
//		ModelLinefollower::Sensor& s = rgr->sensors[i];
//		printf( " %f", s.value );
//	}
//	printf("\n");

	// access the sensors
	left = rgr->sensors[0].value;
	middle = rgr->sensors[1].value;
	right = rgr->sensors[2].value;

	// Algorithm goes here
	if(left > THRESHOLD && middle > THRESHOLD && right > THRESHOLD)
		stop();
	else if(left > THRESHOLD && right < THRESHOLD)
		myRight();
	else if(right > THRESHOLD && left < THRESHOLD)
		myLeft();
	else if (left < THRESHOLD && right < THRESHOLD)
		forward();
	else
		forward();
	// Algorithm till here

	// Set the speed of the motor here
	robot->pos->SetSpeed( forwardSpeed, 0, dtor(turnSpeed) );
	return 0;
}

int PositionUpdate( Model* mod, robot_t* robot )
{
	Pose pose = robot->pos->GetPose();
	printf( "Pose: [%.2f %.2f %.2f %.2f]\n",pose.x, pose.y, pose.z, pose.a );
	return 0; // run again
}
