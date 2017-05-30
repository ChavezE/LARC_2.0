'''
THIS FILE CONTAINS SERIAL PROTOCOLS TO INTERCONECT ARDUINO-RASPBERRY
IN THE LATIN AMERICAN ROBOTICS COMPETITION BY THE TEAM ROBORREGOS
AUTHORS:
	ALEX
	NESTOR
	EMILIO 
'''

###Movimientos

#Turn right the angles in the parameters
def turnRight(angle):
    arduino.write("r") #Send Turn Right angle
    time.sleep(1);
    parte1= (angle>>8)&255;#Upper
    parte2= angle&255;#down
    arduino.write(chr(parte1))
    time.sleep(1);
    arduino.write(chr(parte2))
    time.sleep(1);
    while(arduino.inWaiting() <= 0):
        pass;
    if(arduino.read() == "1"):
        return True;

#Turn left the angles in the parameters
def turnLeft(angle):
    arduino.write("i") #Send Turn Right angle
    time.sleep(1);
    parte1= (angle>>8)&255;#Upper
    parte2= angle&255;#down
    arduino.write(chr(parte1))
    time.sleep(1);
    arduino.write(chr(parte2))
    time.sleep(1);
    while(arduino.inWaiting() <= 0):
        pass;
    if(arduino.read() == "1"):
        return True;

#Turn to an angle
def turnTo(angle):
    arduino.write("g") #Send Turn Right angle
    time.sleep(1);
    parte1= (angle>>8)&255;#Upper
    parte2= angle&255;#down
    arduino.write(chr(parte1))
    time.sleep(1);
    arduino.write(chr(parte2))
    time.sleep(1);
    while(arduino.inWaiting() <= 0):
        pass;
    if(arduino.read() == "1"):
        return True;


#Move Forward till it detect a wall in front
def ForwardTillWall(separation):
    arduino.write("w") #Send Move Forward until Separation
    time.sleep(1);
    arduino.write(chr(separation))
    time.sleep(1);
    while(arduino.inWaiting() <= 0):
        pass;
    if(arduino.read() == "1"):
        return True;

#Move Forward till it detect a wall in front
def BackwardTillWall(separation):
    arduino.write("s") #Send Move Forward until Separation
    time.sleep(1);
    arduino.write(chr(separation))
    time.sleep(1);
    while(arduino.inWaiting() <= 0):
        pass;
    if(arduino.read() == "1"):
        return True;
    

#Move Forward the cms said
def ForwardCms(cant):
    arduino.write("a") #Send Move Forward until Separation
    time.sleep(1);
    arduino.write(chr(cant))
    time.sleep(1);
    while(arduino.inWaiting() <= 0):
        pass;
    if(arduino.read() == "1"):
        return True;
    

#Move Backward the cms said
def BackwardCms(cant):
    arduino.write("b") #Send Move Backward until Separation
    time.sleep(1);
    arduino.write(chr(cant))
    time.sleep(1);
    while(arduino.inWaiting() <= 0):
        pass;
    if(arduino.read() == "1"):
        return True;

#Move foward until ther is not left wall
def fowardUntilNotLeft():
    arduino.write("h");
    time.sleep(1);
    while(arduino.inWaiting()<=0):
        pass;

#Move Backward untile there is no left wall
def BackwardUntilNotLeft():
    arduino.write("y");
    time.sleep(1);
    while(arduino.inWaiting()<=0):
        pass;


#Move foward until ther is not right wall
def fowardUntilNotRight():
    arduino.write("x");
    time.sleep(1);
    while(arduino.inWaiting()<=0):
        pass;

#Move Backward untile there is no right wall
def BackwardUntilNoRight():
    arduino.write("z");
    time.sleep(1);
    while(arduino.inWaiting()<=0):
        pass;

def grabTerrine():
    arduino.write("-");
    time.sleep(1);
    while(arduino.inWaiting() <= 0):
        pass;
    if(arduino.read() == "1"):
        return True;
