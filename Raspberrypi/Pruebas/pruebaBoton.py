
import RPi.GPIO as GPIO
import time
import sys

GPIO.setmode(GPIO.BCM)
GPIO.setup(23, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(24, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(25, GPIO.OUT)

# setting up interrupt on pin 23 to exit the abort the program
def my_callback(channel):
    print "exiting the program..."
    GPIO.cleanup()
    sys.exit()
# this line waits for the interrupt and calles the function above
GPIO.add_event_detect(23, GPIO.FALLING, callback=my_callback, bouncetime=1000)

# loop in this function until begin button is pressed
def waitToBegin():
    print "waiting for input"
    GPIO.output(25,1)
    GPIO.wait_for_edge(24, GPIO.FALLING)
    GPIO.output(25,0)
    print "begining program"


if __name__ == '__main__':

    # call funtion to start polling button to begin the code
    waitToBegin()
    while True:
        print "dummy line"
        time.sleep(0.05)
