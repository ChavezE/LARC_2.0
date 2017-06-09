'''
Strategy:

Perfect round:

	The round start at the center of the map on the grass side; the first corner to visit is the one at the right,
	if the terrines aren't there the robot do a 180° turn and go to the other corner. The robot should have founded
	the terrine zone by now, that spot is saved in the memory; the next step is to grab a terrine; after grabing a 
	terrine the robot should position himself in an angle to face the cows and with the vision algorithm find one 
	(Save the angle the robot was facing when the algorithm find a cow, as one possible angle where ther is a cow);
	after detecting a cow the robot should move and position himself inside the cow for start the milking 
	(The possible angle of the cow now is secure and we save this data to use it next time we need to go to a cow);
	after getting the milk move out of the cow and find a wall, follow the wall until it find the entrance to the
	gravel zone, in the gravel zone the vision algorithm should make the robot face the container, move to the 
	container and pour the milk inside it after that move back to the starting point and go to the opposite side of
	the terrine zone and leave the terrine in the surface; without a terrine go to the terrine zone and start over.

After a LoP:
	Find the entrance of the grass zone and the gravel zone, position the robot in the grass zone and start doing the
	algorithm of the perfect round.
'''

#Go to the corner of terrines
def goToTerrines():
	#Find a corner:

	#Check if is the terrines zone

	#If not
		#Go to another corner
	