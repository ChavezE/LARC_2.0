import serial
import time
try:
        arduino = serial.Serial('/dev/ttyACM0', 9600, timeout = 1)
	time.sleep(3)
except serial.SerialException:
        print("Desconecta y reconecta el arduino");
        while(True):
                pass;

def forwardNCm(cm):
        print("forwardNCm")
        arduino.write("a")
        #Send the speed to turn
        arduino.write(chr(cm))
        #Wait for arduino response
        while(arduino.inWaiting() <= 0):
                pass;
        if(arduino.read() == "1"):
                print("True")
                return True
        else:
                print("False")
                return False

def backwardNCm(cm):
        print("backwardNCm")
        arduino.write("b")
        #Send the speed to turn
        arduino.write(chr(cm))
        #Wait for arduino response
        while(arduino.inWaiting() <= 0):
                pass;
        if(arduino.read() == "1"):
                print("True")
                return True
        else:
                print("False")
                return False

def forwardUntilWallN(cm):
        print("forwardUntilWallN")
        arduino.write("c")
        #Send the speed to turn
        arduino.write(chr(cm))
        #Wait for arduino response
        while(arduino.inWaiting() <= 0):
                pass;
        if(arduino.read() == "1"):
                print("True")
                return True
        else:
                print("False")
                return False

def backwardUntilWallN(cm):
        print("backwardUntilWallN")

        arduino.write("d")

        #Send the speed to turn
        arduino.write(chr(cm))

        #Wait for arduino response
        while(arduino.inWaiting() <= 0):
                pass;

        if(arduino.read() == "1"):
                print("True")
                return True
        else:
                print("False")
                return False

def turnToObjective(angle):
        print("turnToObjective")

        #Tell arduino to run turn()
        arduino.write("e")

        part1 = (angle>>8)&255
        part2 = angle&255

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

#direction = 0 right, 1 = left
def turnNDegrees(angle, direction):
        print("turnNDegrees")

        #Send the case for forward
        arduino.write("f")

        #Send velocity for left motor
        arduino.write(chr(angle))
        #send velocity for right motor
        arduino.write(chr(direction))

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

def forwardUntilNoRight():
        print("forwardUntilNoRight")
        #Tell the arduino to run brake()
        arduino.write("g")

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

def forwardUntilNoLeft():
        print("forwardUntilNoLeft")
        #Tell the arduino to run brake()
        arduino.write("h")

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

def backwardUntilNoRight():
        print("backwardUntilNoRight")
        #Tell the arduino to run brake()
        arduino.write("i")

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

def backwardUntilNoLeft():
        print("backwardUntilNoLeft")
        #Tell the arduino to run brake()
        arduino.write("j")

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

def getSharpDistance(num):
    print("getSharpDistance")
    arduino.write("k")
    arduino.write(chr(num))
    # wait for arduino response
    while (arduino.inWaiting() <= 0):
        pass
    cm = arduino.read()
    print cm
