# Vehicle.inc
# model for the robot "Vehicle"

#Vehicle's linefollowers
define vehicle_linefollowers linefollower
(
	# number of sonars
	scount 3

	# define the pose of each transducer [xpos ypos zpos]
	spose[0] [ 0.6 0.25 -.25 ]	# left sensor
	spose[1] [ 0.6 0 -.25 ]	# middle sensor
	spose[2] [ 0.6 -0.25 -.25 ]		# right sensor

	# define width (in meters) which is to be scanned by each sensor
	# along the two axes
	scanWidth .2
	
	# specify if the colour detection algorithm should inverted
	# i.e. whether it is detecting a white(1) / black(0) line.
	# invert 0

	# define the size of each transducer
	# [xsize ysize] in meters
	ssize [0.01 0.05 0.01]
)

#Vehicle's sonars
define vehicle_sonars ranger
(
      # number of sonars
      scount 4

      # define the pose of each transducer [xpos ypos heading]
      spose[0] [ 0.75 0.1875 0]	#fr left tooth
      spose[1] [ 0.75 -0.1875 0]	#fr right tooth
      spose[2] [ 0.25 0.5 30]		# left corner
      spose[3] [ 0.25 -0.5 -30]	# right corner
		
      # define the field of view of each transducer 
      #[range_min range_max view_angle]
      sview [0.3 2.0 10]

      # define the size of each transducer
      # [xsize ysize] in meters
      ssize [0.01 0.05]
)


# vehicle's body
define vehicle position
(
      # actual size
      size [1.25 1 1]

      # Vehicle's centre of rotation is offset from its centre of area
      #origin [0.125 0 0 0]

      # the shape of Vehicle
 
	block
	(
		points 6
		point[5] [0 0]
		point[4] [0 1]
		point[3] [0.75 1]
		point[2] [1 0.75]
		point[1] [1 0.25]
		point[0] [0.75 0]
		z [0 1]
	)

	block
	(
		points 4
		point[3] [1 0.75]
		point[2] [1.25 0.75]
		point[1] [1.25 0.625]
		point[0] [1 0.625]
		z [0 0.5]
	)

	block
	(
		points 4
		point[3] [1 0.375]
		point[2] [1.25 0.375]
		point[1] [1.25 0.25]
		point[0] [1 0.25]
		z [0 0.5]
	)
  
   gripper
   (
      pose [1 0 0 0] 
      size [1 1 1]
   
   )
  
    # differential steering model
	drive "diff"
	localization "gps"         	  
	localization_origin [0 0 0 0] 	# Start odometry at (0, 0, 0).

	# sensors attached to bigbob
	vehicle_linefollowers()  
	
	# sensors attached to vehicle
	vehicle_sonars()  
	
	obstacle_return 1           	# Can hit things.
	ranger_return 1             	# reflects sonar beams 
	linefollower_return 1             	# reflects sonar beams
)
