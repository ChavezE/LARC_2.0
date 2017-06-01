void degreesCorrections(int &degrees)
{
  //Corrections
  if (degrees > 18000L)
  {
    degrees = -(18000L - (degrees - 18000L));
  }
  else if (degrees < -18000L)
  {
    degrees = (18000L - ((-degrees) - 18000L));
  }

  //Corrections
  if (degrees > 600L)
  {
    degrees = 600L;
  }
  else if (degrees < -600L)
  {
    degrees = -600L;
  }
}

//Go back till there is no wall at the left
void backwardTillNoLeft()
{
  //Actual angle to stay in
  long direction = getCompass();

  do {
    //Actual angle
    int degrees = getCompass();
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
  //Actual angle to stay in
  long direction = getCompass();

  do {
    //Actual angle
    int degrees = getCompass();
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
  //Actual angle to stay in
  long direction = getCompass();

  do {
    //Actual angle
    int degrees = getCompass();
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
  //Actual angle to stay in
  long direction = getCompass();

  do {
    //Actual angle
    int degrees = getCompass();
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
