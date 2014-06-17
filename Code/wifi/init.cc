#include "robot.h"
#include "stage.h"

using namespace Stg;

#define MAX_ROBOT 1000

int PositionUpdate( Model* mod, Robot* robot );
void MessageProcess(WifiMessageBase* Message);
int LaserUpdate( Model* mod, Robot* robot );

Robot* Rs[MAX_ROBOT];

// Stage calls this when the model starts up
extern "C" int Init( Model* mod, CtrlArgs* args )
{
	Robot* robot = new Robot(mod);
	printf("in Init : the robot id is %d\n", robot->id);
	Rs[robot->id] = robot;

	robot->position->AddUpdateCallback( (stg_model_callback_t)PositionUpdate, robot );
	robot->laser->AddUpdateCallback( (stg_model_callback_t)LaserUpdate, robot);
	robot->wifi->comm.SetReceiveMsgFn(MessageProcess);

	robot->wifi->Subscribe(); // start the wifi updates
	robot->laser->Subscribe(); // starts the laser updates
	robot->position->Subscribe(); // starts the position updates

	return 0; //ok
}

// call the robot's loop function
int PositionUpdate( Model* mod, Robot* robot )
{
	robot->do_loop();
	return 0; // run again
}

void MessageProcess(WifiMessageBase* message)
{
	if(message == NULL)
	{
		printf("init.cc : in MessageProcess() - message is null\n");
		return;
	}

	Robot* robot = Rs[message->GetRecipientId()];
	if(robot == NULL)
	{
		printf("init.cc : in MessageProcess() - robot is null\n");
	}

	MapMessage* map_message = dynamic_cast<MapMessage*>(message);
	if(map_message == NULL)
	{
		printf("init.cc : in MessageProcess() - map_message is null\n");
	}
	robot->MessageProcess(map_message);
}

int LaserUpdate( Model* mod, Robot* robot )
{
	return 0;
}
