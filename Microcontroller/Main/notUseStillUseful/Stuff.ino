 /*
      case 'a':
        brake();
        Serial.write('1');
        break;

      case 'b':
        while (Serial.available() < 2);
        data1 = Serial.read();
        data2 = Serial.read();

        forward(data1, data2);
        Serial.write('1');
        break;

      case 'c':
        while (Serial.available() < 2);
        data1 = Serial.read();
        data2 = Serial.read();

        backward(data1, data2);
        Serial.write('1');
        break;

      case 'd':
        while (Serial.available() < 1);
        data = Serial.read();

        turnRight(data);
        Serial.write('1');
        break;

      case 'e':
        while (Serial.available() < 1);
        data = Serial.read();

        turnLeft(data);
        Serial.write('1');
        break;

      case 'f':
        while (Serial.available() < 2);
        data1 = Serial.read();
        data2 = Serial.read();

        turn(((data1 << 8) | data2) * 100UL);
        Serial.write('1');
        break;

      case 'g':
        while (Serial.available() < 2);
        data1 = Serial.read();
        data2 = Serial.read();

        forwardNCm(((data1 << 8) | data2) * 100UL);
        Serial.write('1');
        break;

      case 'h':
        while (Serial.available() < 2);
        data1 = Serial.read();
        data2 = Serial.read();

        backwardNCm(((data1 << 8) | data2) * 100UL);
        Serial.write('1');
        break;

      case 'i':
        while (Serial.available() < 2);
        data1 = Serial.read();
        data2 = Serial.read();

        forwardUntilWall(((data1 << 8) | data2) * 100UL);
        Serial.write('1');
        break;

      case 'j':
        while (Serial.available() < 2);
        data1 = Serial.read();
        data2 = Serial.read();

        backwardUntilWall(((data1 << 8) | data2) * 100UL);
        Serial.write('1');
        break;

      case 'k':
        while (Serial.available() < 2);
        data1 = Serial.read();
        data2 = Serial.read();

        turnToDegree(((data1 << 8) | data2) * 100UL);
        Serial.write('1');
        break;

      case 'l':
        while (Serial.available() < 2);
        data1 = Serial.read();
        data2 = Serial.read();

        turnRightNDegrees(((data1 << 8) | data2) * 100UL);
        Serial.write('1');
        break;

      case 'm':
        while (Serial.available() < 2);
        data1 = Serial.read();
        data2 = Serial.read();

        turnLeftNDegrees(((data1 << 8) | data2) * 100UL);
        Serial.write('1');
        break;

      case 'n':
        platIn();
        Serial.write('1');
        break;

      case 'o':
        platOut();
        Serial.write('1');
        break;

      case 'p':
        openClaw();
        Serial.write('1');
        break;

      case 'q':
        closeClaw();
        Serial.write('1');
        break;

      case 'r':
        while (Serial.available() < 1);
        sharp = Serial.read();

        switch (sharp)
        {
          case 0:
            {
              int dist = getDistance(pinSF);
              Serial.println(dist);
              break;
            }
          case 1:
            {
              int dist = getDistance(pinSB);
              Serial.println(dist);
              break;
            }

          case 2:
            {
              int dist = getDistance(pinSRF);
              Serial.println(dist);
              break;
            }

          case 3:
            {
              int dist = getDistance(pinSRC);
              Serial.println(dist);
              break;
            }

          case 4:
            {
              int dist = getDistance(pinSRB);
              Serial.println(dist);
              break;
            }

          case 5:
            {
              int dist = getDistance(pinSLF);
              Serial.println(dist);
              break;
            }

          case 6:
            {
              int dist = getDistance(pinSLC);
              Serial.println(dist);
              break;
            }

          case 7:
            {
              int dist = getDistance(pinSLB);
              Serial.println(dist);
              break;
            }

          case 8:
            {
              int dist = getDistance(pinSC);
              Serial.println(dist);
              break;
            }
        }

        break;

      case 's':
        int angle = getCompassX100();
        Serial.println(angle);
        break;
    }
  }
}
*/

THIS IS PYTHON, from the comunication.py file

'''
#Stop the robot
def brake():
        print("Brake")
        #Tell the arduino to run brake()
        arduino.write("a")
        #time.sleep(1);
        #Wait for something to happen
        while(arduino.inWaiting() <= 0):
                pass;
        #Completed
        if(arduino.read() == "1"):
                print("True")
                return True
        else:
                print("False")
                return False

#Go Forward, the parameters are the velocity of the motors
def forward(left, right):
        print("forward")
        #Left speed limit
        if left > 255:
                left = 255
        #Right speed limit
        if right > 255:
                right = 255

        #Send the case for forward
        arduino.write("b")

        #Send velocity for left motor
        arduino.write(chr(left))
        #send velocity for right motor
        arduino.write(chr(right))

        #Wait for arduino response
        while(arduino.inWaiting() <= 0):
                pass;
        #Completed
        if(arduino.read() == "1"):
                print("True")
                return True
        else:
                print("False")
                return False

#Go backward, the parameters are the velocity of the motors
def backward(left, right):
        print("backward")
        #Left speed limit
        if left > 255:
                left = 255
        #Right speed limit
        if right > 255:
                right = 255
        #Send the case for forward
        arduino.write("c")

        #Send velocity for left motor
        arduino.write(chr(left))
        #send velocity for right motor
        arduino.write(chr(right))

        #Wait for arduino response
        while(arduino.inWaiting() <= 0):
                pass;
        if(arduino.read() == "1"):
                print("True")
                return True
        else:
                print("False")
                return False


#Turn right at a given specific speed
def turnRight(speed):
        print("turn Right")
        #Set speed limit
        if speed > 255:
                speed = 255
        #Send the case for forward
        arduino.write("d")

        #Send the speed to turn
        arduino.write(chr(speed))

        #Wait for arduino response
        while(arduino.inWaiting() <= 0):
                pass;
        if(arduino.read() == "1"):
                print("True")
                return True
        else:
                print("False")
                return False

#Turn left at a specific speed
def turnLeft(speed):
        print("turnLeft")
        #Set speed limit
        if speed > 255:
                speed = 255
        #Send the case for forward
        arduino.write("e")

        #Send velocity for left motor
        arduino.write(chr(speed))

        #Wait for arduino response
        while(arduino.inWaiting() <= 0):
                pass;
        if(arduino.read() == "1"):
                print("True")
                return True
        else:
                print("False")
                return False

#Turn in a specific speed, negative turn right and positive turn left
def turn(speed):
        print("Turn")

        #Tell arduino to run turn()
        arduino.write("f")

        part1 = (speed>>8)&255
        part2 = speed&255

        arduino.write(chr(part1))
        arduino.write(chr(part2))

        #Wait for arduino response
        while(arduino.inWaiting() <= 0):
                pass;
        if(arduino.read() == "1"):
                print("True")
                return True
        else:
                print("False")
                return False

#Go Forward an x amount of cm
def forwardNCm(cm):
        print("ForwardNCm")
        #Tell arduino to run turn()
        arduino.write("g")

        part1 = (speed>>8)&255
        part2 = speed&255

        arduino.write(chr(part1))
        arduino.write(chr(part2))

        #Wait for arduino response
        while(arduino.inWaiting() <= 0):
                pass;
        if(arduino.read() == "1"):
                print("True")
                return True
        else:
                print("False")
                return False

#Go backward an x amount of cm
def backwardNCm(cm):
        print("BackwardNCm")
        #Tell arduino to run turn()
        arduino.write("h")

        part1 = (speed>>8)&255
        part2 = speed&255

        arduino.write(chr(part1))
        arduino.write(chr(part2))

        #Wait for arduino response
        while(arduino.inWaiting() <= 0):
                pass;
        if(arduino.read() == "1"):
                print("True")
                return True
        else:
                print("False")
                return False

#Go Forward until it found a wall at a certain distance
def forwardUntilWall(cm):
        print("forwardUntilWall")
        #Tell arduino to run turn()
        arduino.write("i")

        part1 = (speed>>8)&255
        part2 = speed&255

        arduino.write(chr(part1))
        arduino.write(chr(part2))

        #Wait for arduino response
        while(arduino.inWaiting() <= 0):
                pass;
        if(arduino.read() == "1"):
                print("True")
                return True
        else:
                print("False")
                return False

#Go backward until it found a wall at a certain distance
def backwardUntilWall(cm):
        print("backward until wall")
        #Tell arduino to run turn()
        arduino.write("j")

        part1 = (speed>>8)&255
        part2 = speed&255

        arduino.write(chr(part1))
        arduino.write(chr(part2))

        #Wait for arduino response
        while(arduino.inWaiting() <= 0):
                pass;
        if(arduino.read() == "1"):
                print("True")
                return True
        else:
                print("False")
                return False


#Turn to an specific angle
def turnToDegree(angle):
        print("turnToDegree")
        #Tell arduino to run turn()
        arduino.write("k")

        part1 = (speed>>8)&255
        part2 = speed&255

        arduino.write(chr(part1))
        arduino.write(chr(part2))

        #Wait for arduino response
        while(arduino.inWaiting() <= 0):
                pass;
        if(arduino.read() == "1"):
                print("True")
                return True
        else:
                print("False")
                return False


#Turn right a certain amount of degrees
def turnRightNDegrees(angle):
        print("turnRightNDegrees")
        #Tell arduino to run turn()
        arduino.write("l")

        part1 = (speed>>8)&255
        part2 = speed&255

        arduino.write(chr(part1))
        arduino.write(chr(part2))

        #Wait for arduino response
        while(arduino.inWaiting() <= 0):
                pass;
        if(arduino.read() == "1"):
                print("True")
                return True
        else:
                print("False")
                return False


#Turn left a certain amount of degrees
def turnLeftNDegrees(angle):
        print("Turn Left N Degrees")
        #Tell arduino to run turn()
        arduino.write("m")

        part1 = (speed>>8)&255
        part2 = speed&255

        arduino.write(chr(part1))
        arduino.write(chr(part2))

        #Wait for arduino response
        while(arduino.inWaiting() <= 0):
                pass;
        if(arduino.read() == "1"):
                print("True")
                return True
        else:
                print("False")
                return False


#Move Platafform In
def platIn():
        print("platIn")
        #Tell the arduino to run brake()
        arduino.write("n")
        #Wait for something to happen
        while(arduino.inWaiting() <= 0):
                pass;

        if(arduino.read() == "1"):
                print("True")
                return True

        else:
                print("False")
                return False

#Move Platafform Out
def platOut():
        print("platOut")
        #Tell the arduino to run brake()
        arduino.write("o")
        #Wait for something to happen
        while(arduino.inWaiting() <= 0):
                pass;

        if(arduino.read() == "1"):
                print("True")
                return True

        else:
                print("False")
                return False


#Open Claw
def openClaw():
        print("openClaw")
        #Tell the arduino to run brake()
        arduino.write("p")
        #Wait for something to happen
        while(arduino.inWaiting() <= 0):
                pass;

        if(arduino.read() == "1"):
                print("True")
                return True

        else:
                print("False")
                return False

#Close Claw
def closeClaw():
        print("closeClaw")
        #Tell the arduino to run brake()
        arduino.write("q")
        #Wait for something to happen
        while(arduino.inWaiting() <= 0):
                pass;

        if(arduino.read() == "1"):
                print("True")
                return True

        else:
                print("False")
                return False

#Get angle with BNO, the string in the parameter set wich sharp to use
def getDistance(sharp):
        sharps = {
        "pinSF" : 0,
        "pinSB" : 1,
        "pinSRF" : 2,
        "pinSRC" : 3,
        "pinSRB" : 4,
        "pinSLF" : 5,
        "pinSLC" : 6,
        "pinSLB" : 7,
        "pinSC" : 8
        }
        print("getDistance")
        #Tell the arduino to run getCompass()
        arduino.write("r")
        #Tell the arduino what sharp to use
        arduino.write(chr(sharps[sharp]))
        #Wait for something to happen
        while(arduino.inWaiting() <= 0):
                pass;

        distance = arduino.readline()

        print(distance)

        return distance

#Get angle with BNO
def getCompass():
        print("Getting angle")
        #Tell the arduino to run getCompass()
        arduino.write("s")
        #Wait for something to happen
        while(arduino.inWaiting() <= 0):
                pass;

        angle = arduino.readline()

        print(angle)

        return angle

'''