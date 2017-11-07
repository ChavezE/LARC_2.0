#include <Wire.h>

int const SLAVE_ID = 5;

void setup(){
	Wire.begin(SLAVE_ID);
	Wire.onReceive(receiveEvent);
	pinMode(13, 1);
	digitalWrite(13, 0);
}



void loop(){
	
}




void milkTheFucker(){

}

void stopMilking(){

}

void openMilker(){

}

void closeMilker(){

}

// I2C EVENT HANDLER
void receiveEvent(int n){
	while(Wire.available()){
		char instruction = Wire.read();

		switch (instruction) {
		    case 'M':
		    	// MILK THE SHIT
		    	digitalWrite(13,1);
		    	milkTheFucker();
	      		break;

		    case 'S':
		    	digitalWrite(13, 0);
		    	stopMilking();
		    	break;

		    case 'O':
		    	digitalWrite(13, 0);
		    	openMilker();
		    	break;

		    case 'C':
		    	digitalWrite(13, 0);
		    	closeMilker();
		    	break;

		}
	}
}