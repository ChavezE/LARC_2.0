//Get the distance of the sharp given in the parameters
int getDistance(byte sharp)
{
  int i, j, key, numLength = 7;
  int num[numLength];
  
  float volts = analogRead(sharp) * 0.0048828125;
  //If the raw data gives zero is really far
  if (volts == 0)
  {
    num[0] = 30000;
  }
  //The distance is acceptable save the formula
  else
  {
    num[0] = round(13 * pow(volts, -1));
  }
  delay(26);
  
  for (j = 1; j < numLength; j++)
  {
    //Raw data
    volts = analogRead(sharp) * 0.0048828125;
    //If the raw data gives zero is really far
    if (volts == 0)
    {
      num[j] = 30000;
    }
    //The distance is acceptable save the formula
    else
    {
      num[j] = round(13 * pow(volts, -1));
    }
    delay(26);

    key = num[j];
    for (i = j - 1; (i >= 0) && (num[i] < key); i--)
    {
      num[i + 1] = num[i];
    }
    num[i + 1] = key;
  }
  return num[numLength / 2];
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
  int angle = bno.getVector(Adafruit_BNO055::VECTOR_EULER).x();
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
