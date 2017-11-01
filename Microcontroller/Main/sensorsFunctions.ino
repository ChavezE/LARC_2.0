/**
 * Get the distance of the sharp given in the parameters.
 * It uses the mediana. Max reads is 7.
 * 
 * @param sharp {byte} pinSharp
 * @param cantReads {cantReads=7}
 * 
 * @return {int} the mediana distance 
 */
int getDistance(byte sharp, byte cantReads)
{
  int reads[7];
  
  for (int j = 0; j < cantReads; j++)
  {
    //Raw data
    float volts = analogRead(sharp) * 0.0048828125;
    //If the raw data gives zero is really far
    if (volts == 0)
    {
      reads[j] = 30000;
    }
    //The distance is acceptable save the formula
    else
    {
      reads[j] = round(13.0 / volts);
    }
    delay(26);

  }

  return Estadisticas<int>::createNewMediana(reads, cantReads).getMediana();
  
}

//Return the lecture of the BNO in eulers
long getCompassX100()
{
  long angle = bno.getVector(Adafruit_BNO055::VECTOR_EULER).x() * 100;
  //Display function in LCD
  ////lcd.clear();
  // //writeLCD("GETTING ANGLE", 0, 0);
  // //writeLCD(String(angle), 0, 1);
  return angle;
}

//Return the lecture of the BNO in eulers
int getCompass()
{
  int angle = round(bno.getVector(Adafruit_BNO055::VECTOR_EULER).x());
  //Display function in LCD
  ////lcd.clear();
  // //writeLCD("GETTING ANGLE", 0, 0);
  // //writeLCD(String(angle), 0, 1);
  return angle;
}

//Interrup funcitons, add or substract to steps
void encoderStep()
{
  //If we are moving forward
  if (encoderState == 1)
  {
    steps++;
  }
  //else if we are moving backwards
  else if (encoderState == 2)
  {
    steps--;
  }
}

void writeLCD(String word, int iCol, int iRow)
{
  bool firstLine = true;
  for (int iI = 0; iI < word.length(); iI++)
  {
    if (iI > 15 && firstLine == true)
    {
      iRow = 1;
      iCol = 0;
      firstLine = false;
    }
    lcd.setCursor(iCol, iRow);
    lcd.print(word[iI]);
    iCol++;
  }
}
