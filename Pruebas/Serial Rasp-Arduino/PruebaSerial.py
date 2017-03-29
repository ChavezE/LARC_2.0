'''
Libreria usada: pyserial (hay que instalara: pip install pyserial)
https://github.com/pyserial/pyserial
https://pythonhosted.org/pyserial/shortintro.html

char chr(byte n): Regresa un string/char de valor n [0,255] en asci (como los trata el arduino)
byte ord(char c): Regresa el valor de byte en asci del char/string c

'''

import serial
import time


#Inicializar el Serial
try:
    arduino= serial.Serial('/dev/ttyACM0', 9600, timeout=1);
except serial.SerialException:
    print "Desconecta y reconecta el arduino";
    exit();

time.sleep(5);


'''
#Escribir
#arduino.write("b");#Se manda como caracter con ese valor de caracter
arduino.write(chr(255)); #Asi funciona. Muere: arduino.write(98);


time.sleep(5);
'''


#Leer
while(arduino.inWaiting()<=0):
    pass;
time.sleep(2);
#print ord(arduino.read(1));#Regresa como numero
print arduino.read(1);#Regresa como char


'''
#Cerrar y  no seque mas
arduino.flushOutput();
arduino.flushInput();

arduino.close();
'''

