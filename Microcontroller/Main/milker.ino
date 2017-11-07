

void milkerUp()
{
  digitalWrite(pinMotA, 1);
  digitalWrite(pinMotB, 0);
}

void milkerDown()
{
  digitalWrite(pinMotA, 0);
  digitalWrite(pinMotB, 1);
}

void milkerStop()
{
  digitalWrite(pinMotA, 0);
  digitalWrite(pinMotB, 0);
}

void extract()
{
  digitalWrite(pinExtractorA, 1);
  digitalWrite(pinExtractorB, 0);
}

void stopExtract()
{
  digitalWrite(pinExtractorA, 0);
  digitalWrite(pinExtractorB, 0);
}

void moveMilkerDown()
{
  milkerDown();
  while(digitalRead(pinLMD) == normalState);
  milkerStop();
}

void moveMilkerUp()
{
  milkerUp();
  delay(23000);
  milkerStop();
}

void milk()
{
  moveMilkerUp();
  delay(1000);
  //Put milker functions here
  moveMilkerDown();
}

//Move repetitive times the lower part of the milker
void sendMilkDirective(){
	char milkInstruction  = 'M';
	Wire.beginTransmission(SLAVE_ID);
	Wire.write(milkInstruction);
	Wire.endTransmission();
}

//Stop both servos of milker
void sendStopDirective(){
	char milkInstruction  = 'S';
	Wire.beginTransmission(SLAVE_ID);
	Wire.write(milkInstruction);
	Wire.endTransmission();
}

//Open upper part of milker
void sendOpenDirective(){
	char milkInstruction  = 'O';
	Wire.beginTransmission(SLAVE_ID);
	Wire.write(milkInstruction);
	Wire.endTransmission();
}

//Close upper part of milker
void sendCloseDirective(){
	char milkInstruction  = 'C';
	Wire.beginTransmission(SLAVE_ID);
	Wire.write(milkInstruction);
	Wire.endTransmission();
}

void sendTurnOnLedDirective()
{
  char milkInstruction  = 'L';
	Wire.beginTransmission(SLAVE_ID);
	Wire.write(milkInstruction);
	Wire.endTransmission();
}

void sendTurnOffLedDirective()
{
  char milkInstruction  = 'K';
	Wire.beginTransmission(SLAVE_ID);
	Wire.write(milkInstruction);
	Wire.endTransmission();
}

void simpleMilker()
{
  lcd.clear();
  writeLCD("Abrete", 0, 0);
  sendOpenDirective();
  delay(6000);
  lcd.clear();
  writeLCD("Cierrate", 0, 0);
  sendCloseDirective();
  delay(2000);
  lcd.clear();
  writeLCD("Dale", 0, 0);
  sendMilkDirective();
  delay(15000);
}
