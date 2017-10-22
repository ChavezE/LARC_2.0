
import RPi.GPIO as GPIO
import time
import sys

GPIO.setmode(GPIO.BCM)
GPIO.setup(3, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(4, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(21, GPIO.OUT)

# setting up interrupt on pin 3 to exit the abort the program
def my_callback(channel):
    print "exiting the program..."
    GPIO.cleanup()
    sys.exit()
# this line waits for the interrupt and calles the function above
GPIO.add_event_detect(3, GPIO.FALLING, callback=my_callback, bouncetime=1000)

# loop in this function until begin button is pressed
def waitToBegin():
    print "waiting for input"
    GPIO.output(21,1)
    GPIO.wait_for_edge(24, GPIO.FALLING)
    GPIO.output(21,0)
    print "begining program"


if __name__ == '__main__':

    # call funtion to start polling button to begin the code
    waitToBegin()
    while True:
        print "dummy line"
        time.sleep(0.05)
