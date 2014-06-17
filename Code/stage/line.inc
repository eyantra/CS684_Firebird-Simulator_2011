# line shape
define line position
(
	# actual size
	size [8 0.2 .01]
	#size [0.625 0.5 0.5]

	# Bigbob's centre of rotation is offset from its centre of area
	origin [0.125 0 0 0]

	# estimated mass in KG
	mass 1.0 

	block
	(
		points 4
		point[3] [0 0]
		point[2] [0 0.1]
		point[1] [5 0.1]
		point[0] [5 0]
		z [0 0.01]
	)

	block
	(
		points 4
		point[3] [0 0.1]
		point[2] [0 0.2]
		point[1] [5 0.2]
		point[0] [5 0.1]
		z [0 0.01]
	)


	obstacle_return 0           	# Can hit things.
	laser_return 0                 # reflects laser beams
	ranger_return 0             	# reflects sonar beams
	blob_return 0               	# Seen by blobfinders  
	linefollower_return 1
)
