#include <Wire.h>

int const SLAVE_ID = 5;

void setup(){
	Wire.begin();
	pene
}


void sendMilkDirective(){
	char milkInstruction  = 'M'
	Wire.beginTransmission(SLAVE_ID);
	Wire.write(milkInstruction);
	Wire.endTransmission();
}

void loop(){
	
}