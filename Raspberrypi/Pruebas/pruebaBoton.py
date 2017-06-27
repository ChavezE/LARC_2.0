
import RPi.GPIO as GPIO
import time
import sys

GPIO.setmode(GPIO.BCM)
GPIO.setup(23, GPIO.IN, pull_up_down=GPIO.PUD_UP)

def my_callback(channel):
    print "Boton de reinicio, saliendo del programa ahora"
    sys.exit()

GPIO.add_event_detect(23, GPIO.FALLING, callback=my_callback, bouncetime=500)

if __name__ == '__main__':
    while True:
        print "RESTO DEL CODIGO"
        time.sleep(0.5)

    GPIO.cleanup()
