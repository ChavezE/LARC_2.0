//Go back till there is no wall at the left
void backwardTillNoLeft()
{
  //Display function in LCD
  lcd.clear();
  // writeLCD("BACKWARD TILL NO LEFT", 0, 0);
  //Actual angle to stay in
  long direction = getCompassX100();

  do {
    //Actual angle
    int degrees = getCompassX100();
    degrees = degrees - direction;

    degreesCorrections(degrees);
    //Check wall
    int dist = getDistance(pinSLF);
    //If no wall at left
    if (dist > 30)
    {
      break;
    }
    //P correction
    backward(velForward + degrees / constPCorrect, velForward - degrees / constPCorrect);
    delay(10);
  } while (true);
  brake();
}

//Go back till there is no wall at the right
void backwardTillNoRight()
{
  //Display function in LCD
  lcd.clear();
  // writeLCD("BACKWARD TILL NO RIGHT", 0, 0);
  //Actual angle to stay in
  long direction = getCompassX100();

  do {
    //Actual angle
    int degrees = getCompassX100();
    degrees = degrees - direction;

    degreesCorrections(degrees);
    //Check wall
    int dist = getDistance(pinSRF);
    //If no wall at left
    if (dist > 30)
    {
      break;
    }
    //P correction
    backward(velForward + degrees / constPCorrect, velForward - degrees / constPCorrect);
    delay(10);
  } while (true);
  brake();
}

//Go forward till there is no wall at right
void forwardTillNoRight()
{
  //Display function in LCD
  lcd.clear();
  // writeLCD("FORWARD TILL NO RIGHT", 0, 0);
  //Actual angle to stay in
  long direction = getCompassX100();

  do {
    //Actual angle
    int degrees = getCompassX100();
    degrees = degrees - direction;

    degreesCorrections(degrees);
    //Check wall
    int dist = getDistance(pinSRB);
    //If no wall at left
    if (dist > 30)
    {
      break;
    }
    //P correction
    forward(velForward - degrees / constPCorrect, velForward + degrees / constPCorrect);
    delay(10);
  } while (true);
  brake();
}

//Go forward till there is no wall at left
void forwardTillNoLeft()
{
  //Display function in LCD
  lcd.clear();
  // writeLCD("FORWARD TILL NO LEFT", 0, 0);
  //Actual angle to stay in
  long direction = getCompassX100();

  do {
    //Actual angle
    int degrees = getCompassX100();
    degrees = degrees - direction;

    degreesCorrections(degrees);
    //Check wall
    int dist = getDistance(pinSLB);
    //If no wall at left
    if (dist > 30)
    {
      break;
    }
    //P correction
    forward(velForward - degrees / constPCorrect, velForward + degrees / constPCorrect);
    delay(10);
  } while (true);
  brake();
}
