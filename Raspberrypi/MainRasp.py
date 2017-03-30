import serial
import time

'''
VARIABLES GLOBALES
'''
#Inicializar el Serial
time.sleep(5);
try:
    arduino= serial.Serial('/dev/ttyACM0', 9600, timeout=1);
except serial.SerialException:
    print "Desconecta y reconecta el arduino";
    
time.sleep(5);

terrinesZone = "c" #Terrines Position; c = unknown, r = right, l = left
cowPos = 0  #Position where we find the Cow; 0 = unknown, 1 = right, 2 = center, 3 = left
cowAngle = 360 #Angle where we find the Cow; 360 = unknown, any other number is the angle
tankPos = 0  #Position where we find the Tank; 0 = unknown, 1 = right, 2 = center, 3 = left
tankAngle = 360 #Angle where we find the Tank; 360 = unknown, any other number is the angle


'''
    METODOS
'''
###Movimientos

#Turn right the angles in the parameters
def turnRight(angle):
    arduino.write("r") #Send Turn Right angle
    parte1= (angle>>8)&255;#Upper
    parte2= angle&255;#down
    arduino.write(chr(parte1))
    arduino.write(chr(parte2))
    while(True):
        if(arduino.inWaiting()>0 and arduino.read() == "1"):
            break

#Turn left the angles in the parameters
def turnLeft(angle):
    arduino.write("i") #Send Turn Right angle
    parte1= (angle>>8)&255;#Upper
    parte2= angle&255;#down
    arduino.write(chr(parte1))
    arduino.write(chr(parte2))
    while(True):
        if(arduino.inWaiting()>0 and arduino.read() == "1"):
            break

#Move Forward till it detect a wall in front
def ForwardTillWall(separation):
    arduino.write("w") #Send Move Forward until Separation
    arduino.write(chr(separation))
    while(True):
        if(arduino.inWaiting()>0 and arduino.read() == "1"):
            break

#Move Forward till it detect a wall in front
def BackwardTillWall(separation):
    arduino.write("s") #Send Move Forward until Separation
    arduino.write(chr(separation))
    while(True):
        if(arduino.inWaiting()>0 and arduino.read() == "1"):
            break



#####Rutinas

#Go to terrines zone
def goToTerrines():
    turnLeft(90)
    result="n"
    if(terrinesZone == "c" or terrinesZone == "r"):
        arduino.write("v") 
        while(True):
            if(arduino.inWaiting()>0):
                result= arduino.read()
                break
    if(result=="1"):
        terrinesZone= "r";
    else:
        arduino.write("t") 
        while(True):
            if(arduino.inWaiting()>0):
                result= arduino.read()
                break
        result= "l"
    
    return True;


'''
    MAIN
'''
if __name__ == "__main__":
    print "hola"
    while(True):
        completed= goToTerrines()
        if(not completed):
            pass
            break
        '''
        completed = lookForCow()
        if(!completed):
            pass
            break
        completed = goToCow()
        if(!completed):
            pass
            break
        completed = getInCow()
        if(!completed):
            pass
            break
        completed = milk()
        if(!completed):
            pass
            break
        completed = goToGravel()
        if(!completed):
            pass
            break
        completed = lookForTank()
        if(!completed):
            pass
            break
        completed = goToTank()
        if(!completed):
            pass
            break
        completed = throwTerrine()
        if(!completed):
            pass
            break
        completed = getToInit()
        if(!completed):
            pass
            break
    '''
    
    
