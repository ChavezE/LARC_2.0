
'''
THIS FILE CONTAINS SERIAL PROTOCOLS TO INTERCONECT ARDUINO-RASPBERRY
IN THE LATIN AMERICAN ROBOTICS COMPETITION BY THE TEAM ROBORREGOS
AUTHORS:
	ALEX
	NESTOR
	EMILIO 
'''

#Stop the robot
def brake():
	#Tell the arduino to run brake()
	arduino.write("a")
	time.sleep(1);
	#Wait for something to happen
	while(arduino.inWaiting() <= 0):
		pass;

	if(arduino.read() == "1"):
		return True

	print("No Brake")