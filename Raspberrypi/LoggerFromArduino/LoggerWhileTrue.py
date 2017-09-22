import serial
import sys
import time

sys.path.insert(0, '../')
import Recibidor as recibidor

def recibirYSepararLineas():
    while arduino.inWaiting() > 0:
        mientr = arduino.read()
        global entradaRaw

        if mientr == "\n":
            recibidor.handleNewLog(entradaRaw)
            entradaRaw = ""
        else:
            entradaRaw += mientr


try:
    arduino = serial.Serial('/dev/ttyACM0', 9600, timeout = 1) # ls /dev/tty*
    time.sleep(3)
except serial.SerialException:
    print("bad port, check labeling");
    while(True):
        pass;

entradaRaw = ""

recibidor.initReceiverLogger()

while(True):
    if arduino.inWaiting() > 0:
        recibirYSepararLineas()
