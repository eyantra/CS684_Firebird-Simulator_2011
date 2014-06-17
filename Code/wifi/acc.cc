
#include "stage.hh"

using namespace Stg;

struct ModelGripper::config_t gcfg;

int gripperAction = 0;

bool gripperIsOpen() {
   return ( gcfg.paddles == ModelGripper::PADDLE_OPEN );
}

bool gripperIsClosed() {
   return ( gcfg.paddles == ModelGripper::PADDLE_CLOSED );
}

bool gripperIsOpening() {
   return ( gcfg.paddles == ModelGripper::PADDLE_OPENING );
}

bool gripperIsClosing() {
   return ( gcfg.paddles == ModelGripper::PADDLE_CLOSING );
}

bool gripperIsDown() {
   return ( gcfg.lift == ModelGripper::LIFT_DOWN );
}

bool gripperIsUp() {
   return ( gcfg.lift == ModelGripper::LIFT_UP );
}

bool gripperIsMovingUp() {
   return ( gcfg.lift == ModelGripper::LIFT_UPPING );
}

bool gripperIsMovingDown() {
   return ( gcfg.lift == ModelGripper::LIFT_DOWNING );
}

void gripperOpen() {
   gripperAction = 1;
}

void gripperClose() {
   gripperAction = 2;
}

void gripperUp() {
   gripperAction = 3;
}

void gripperDown() {
   gripperAction = 4;
}

bool gripperCanGrip() {
   return ( ( gcfg.beam[0] || gcfg.beam[1] ) != NULL );
}

bool gripperHasGripped() {
   return ( gcfg.gripped != NULL );
}

double left,middle,right,THRESHOLD =0.5;
float front_ir;


int opmode = 0;
Pose lastPickUpPos;
float targetx = 0, targety = 0;

// The structure that represents the robot
typedef struct
{
	ModelPosition* pos;
	ModelRanger* ranger;
	ModelLinefollower* linefollower;
	ModelGripper* gripper;
} robot_t;

// The callback functions needed for handling the changes in the sensor values
int LinefollowerUpdate( ModelLinefollower* mod, robot_t* robot );
int PositionUpdate( Model* mod, robot_t* robot );
int RangerUpdate( ModelRanger* mod, robot_t* robot);
int GripperUpdate (ModelGripper* mod, robot_t* robot);

// The global variables needed to control the movement of the robot
double forwardSpeed = 0.7, turnSpeed = 0.0;
float newDir;

// Stage calls this when the model starts up
extern "C" int Init( Model* mod, CtrlArgs* args )
{
	// Declare a robot instance
	robot_t* robot = new robot_t;
	
	// Bind the robot's sensors local instance to the internal representation
	robot->pos = (ModelPosition*)mod;
	robot->linefollower = (ModelLinefollower*)mod->GetUnusedModelOfType( "linefollower" );
	robot->ranger=(ModelRanger*)mod->GetChild("ranger:0");
	robot->gripper = (ModelGripper*)mod->GetUnusedModelOfType( "gripper" );
	// Bind the local function such that it is called in every loop of stage
	robot->linefollower->AddUpdateCallback( (stg_model_callback_t)LinefollowerUpdate, robot );
	 robot->pos->AddUpdateCallback( (stg_model_callback_t)PositionUpdate, robot );
	robot->ranger->AddUpdateCallback( (stg_model_callback_t)RangerUpdate,robot);
	robot->gripper->AddUpdateCallback( (stg_model_callback_t)GripperUpdate,robot);
	robot->linefollower->Subscribe(); // starts the linefollower updates
	robot->pos->Subscribe(); // starts the position updates
	robot->ranger->Subscribe();
	robot->gripper->Subscribe();
	
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
   if ( front_ir < 0.5 ){     
      return 0;
     }
         
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
	//double left, middle, right;
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
/*	printf("LEFT	%f\n",rgr->sensors[0].value);
	printf("MIDDLE	%f\n",rgr->sensors[1].value);
	printf("RIGHT	%f\n",rgr->sensors[2].value);
*/
	left = rgr->sensors[0].value;
	middle = rgr->sensors[1].value;
	right = rgr->sensors[2].value;

	// Algorithm goes here
	
	if ( opmode == 0 )
	{
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
		   
		if ( gripperCanGrip() ){
		   opmode = 1;
		}
   }
   else if ( opmode == 1 ) {
      gripperClose();
      opmode = 2;
   }
   else if ( opmode == 2 ) {
      if ( gripperHasGripped() ) {
         printf (" Going to drop object \n");
         opmode = 3;
      }
   }
   else if ( opmode == 3 ) {
      lastPickUpPos = robot->pos->GetPose();
      newDir = atan((targety-lastPickUpPos.y)/(targetx-lastPickUpPos.x));
      if ( targety > lastPickUpPos.y ){
         if ( targetx > lastPickUpPos.x ){
            newDir = newDir;         
         }
         else{
            newDir = newDir + 3.14;
         }
      }
      else{
         if ( targetx > lastPickUpPos.x ){
            newDir = newDir;         
         }
         else{
            newDir = -3.14 + newDir;
         }
      } 
         
      opmode = 4;
   }
   else if ( opmode == 4 ) {
      float curDir = robot->pos->GetPose().a;
      system("clear");
      printf (" newd : %f \t %f \t %f\n", newDir, curDir, newDir - curDir);
      if ( (newDir - curDir) > 0.05 || (newDir - curDir) < -0.05 ){
         turnSpeed = 20*sgn(newDir-curDir);
      }
      else{
         turnSpeed = 0;
         forward();
         opmode = 5;
      }
   }   
   else if ( opmode == 5 ) {
      float curx = robot->pos->GetPose().x, cury = robot->pos->GetPose().y ;
      if ( (targetx - curx) < 0.5 && (targetx - curx) > -0.5 && (targety - cury ) < 0.5 && (targety - cury ) > -0.5){
         stop();
         gripperOpen();
         printf("dropped \n");
         opmode = 6;
      }
      else{
         forward();
      }
   }
   else if ( opmode == 6 ) {
       float curx = robot->pos->GetPose().x, cury = robot->pos->GetPose().y ;   
       newDir = atan((lastPickUpPos.y - cury)/(lastPickUpPos.x-curx));
       if ( lastPickUpPos.y > cury ){
         if ( lastPickUpPos.x > curx ){
            newDir = newDir;         
         }
         else{
            newDir = newDir + 3.14;
         }
      }
      else{
         if ( lastPickUpPos.x > curx ){
            newDir = newDir;         
         }
         else{
            newDir = -3.14 + newDir;
         }
      } 
       printf("turn to : %f \n", newDir);       
       opmode = 7;
   }
   else if ( opmode == 7 ) {
      float curDir = robot->pos->GetPose().a;
//      system("clear");
      printf (" newd : %f \t %f \t %f\n", newDir, curDir, newDir - curDir);
      if ( (newDir - curDir) > 0.05 || (newDir - curDir) < -0.05 ){
         turnSpeed = 20*sgn(newDir-curDir);
      }
      else{
         turnSpeed = 0;
         forward();
         opmode = 8;
      }
   }   
   else if ( opmode == 8 ) {
      float curx = robot->pos->GetPose().x, cury = robot->pos->GetPose().y ;
      if ( (lastPickUpPos.x - curx) < 0.5 && (lastPickUpPos.x - curx) > -0.5 && (lastPickUpPos.y - cury) < 0.5 && (lastPickUpPos.y - cury) > -0.5 ){
         printf("back \n");
         stop();
         newDir = lastPickUpPos.a;
         opmode = 9;
      }
      else{
         forward();
      }
   }
   else if ( opmode == 9 ) {
      float curDir = robot->pos->GetPose().a;
 //     system("clear");
//      printf (" newd : %f \t %f \t %f\n", newDir, curDir, newDir - curDir);
      if ( (newDir - curDir) > 0.05 || (newDir - curDir) < -0.05 ){
         turnSpeed = 20*sgn(newDir-curDir);
      }
      else{
         turnSpeed = 0;
         forward();
         opmode = 0;
      }
   }   
   
   
   
//   printf("%d \n", opmode);
	// Algorithm till here

	// Set the speed of the motor here
	robot->pos->SetSpeed( forwardSpeed, 0, dtor(turnSpeed) );
	
return 0;
}

int RangerUpdate(ModelRanger* mod, robot_t* robot)
{ 
   for (int i=0;i<4;i++)
//      printf("ranger	%d : %f\n",i, mod->sensors[i].range);

front_ir = mod->sensors[0].range;
if (front_ir < 1.0)
{
	stop();
}
return 0;
}

int PositionUpdate( Model* mod, robot_t* robot )
{	robot->pos->SetSpeed( forwardSpeed, 0, dtor(turnSpeed) );
	Pose pose = robot->pos->GetPose();
//	printf( "Pose: [%.2f %.2f %.2f %.2f]\n",pose.x, pose.y, pose.z, pose.a );
	return 0; // run again
}

int GripperUpdate( ModelGripper *mod, robot_t* robot)
{
   gcfg = mod->GetConfig();
   
   if ( gripperAction == 1 && gripperIsClosed() ){
         mod->CommandOpen();
   }
   if ( gripperAction == 2 && gripperIsOpen() ){
         mod->CommandClose();
   }
   if ( gripperAction == 3 && gripperIsDown() ){
         mod->CommandUp();
   }
   if ( gripperAction == 4 && gripperIsUp() ){
         mod->CommandDown();
   }   

   return 0;
}
