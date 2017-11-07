#include <Wire.h>

int const SLAVE_ID = 5;

void setup(){
	Wire.begin();

}


void sendMilkDirective(){
	char milkInstruction  = 'M';
	Wire.beginTransmission(SLAVE_ID);
	Wire.write(milkInstruction);
	Wire.endTransmission();
}

void sendStopDirective(){
	char milkInstruction  = 'S';
	Wire.beginTransmission(SLAVE_ID);
	Wire.write(milkInstruction);
	Wire.endTransmission();
}

void sendOpenDirective(){
	char milkInstruction  = 'O';
	Wire.beginTransmission(SLAVE_ID);
	Wire.write(milkInstruction);
	Wire.endTransmission();
}

void sendCloseDirective(){
	char milkInstruction  = 'C';
	Wire.beginTransmission(SLAVE_ID);
	Wire.write(milkInstruction);
	Wire.endTransmission();
}

void loop(){
	sendMilkDirective();
	delay(500);
	sendStopDirective();
	delay(500);
}