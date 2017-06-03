///////////////////
// Todo Funciona //
///////////////////

//Get the distance of the sharp given in the parameters
int getDistance(byte sharp)
{
  //Display function in LCD
  lcd.clear();
  writeLCD("GETTING DISTANCE", 0, 0);
  //Number of reads
  byte numReads = 7;

  //Index of the value to return
  byte numReturn = numReads;
  if (numReturn % 2 != 0)
  {
    numReturn = (numReturn / 2) + 0.5;
  }
  else
  {
    numReturn /= 2;
  }

  //Values
  int values[numReads];

  //Get all the values from the sharp
  for (int iI = 0; iI < numReads; iI++)
  {
    //Raw data
    float volts = analogRead(sharp) * 0.0048828125;
    //If the raw data gives zero is really far
    if (volts == 0)
    {
      values[iI] = 30000;
    }
    //The distance is acceptable save the formula
    else
    {
      values[iI] = round(13 * pow(volts, -1));
    }
    delay(26);
  }

  //Sort the data
  for (int iI = numReads - 1; iI >= 1; iI--)
  {
    for (int iJ = 1; iJ <= iI; iJ++)
    {
      if (values[iJ - 1] > values[iJ])
      {
        int aux = values[iJ];
        values[iJ] = values[iJ - 1];
        values[iJ - 1] = aux;
      }
    }
  }

  //Return the median of the distances
  writeLCD(String(values[numReturn]), 0, 1);
  return values[numReturn];
}

//Return the lecture of the BNO in eulers
int getCompass()
{
  //Display function in LCD
  lcd.clear();
  writeLCD("GETTING ANGLE", 0, 0);
  return bno.getVector(Adafruit_BNO055::VECTOR_EULER).x() * 100;
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
  for(int iI = 0; iI < word.length(); iI++)
  {
    if(iI > 15 && firstLine == true)
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