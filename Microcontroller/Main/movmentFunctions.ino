////////////////////////////////////////
//  Falta  hacer funcinoar los giros  //
////////////////////////////////////////

//Go forward the cm given in the parameter
void forwardNCm(long cm)
{
  //Display function in LCD
  lcd.clear();
  writeLCD("FORWARD N CM", 0, 0);
  writeLCD(String(cm), 0, 1);
  //Change precision
  cm *= 100;
  
  //set steps to zero to start counting
  steps = 0UL;
  //get direction we faced at start
  int direction = getCompass();


  do {
    //Check the faced direction
    int degrees = getCompass();
    //Difference of where it is and where it should be
    degrees = degrees - direction;

    degreesCorrections(degrees);

    //cm left to get where it want
    long e = cm - (steps * 10000L) / constEncoder;
    //Vel of motors
    long go;
    
    //If it pass
    if (e < -50)
    {
      //set encoder to substract
      encoderState = 2;
      //Velocity
      go = velForward - e / constPDist;
      //Velocity can't go faster than 110
      if (go > 110L)
      {
        go = 110L;
      }
      //P correction
      backward(go + degrees / constPCorrect, go - degrees / constPCorrect);
    }
    //If it still have cm to go
    else if (e > 50)
    {
      //set encoder to add
      encoderState = 1;
      //Velocity
      go = velForward + e / constPDist;
      //Velocity can't go faster than 110
      if (go > 110L)
      {
        go = 110L;
      }
      //P correction
      forward(go - degrees / constPCorrect, go + degrees / constPCorrect);
    }
    //Completed
    else
    {
      encoderState = 1;
      brake();
      //Get out of do while
      break;
    }

    delay(10);
  } while (true);
}

//Go backward the cm given in the parameter
void backwardNCm(long cm)
{
  //Display function in LCD
  lcd.clear();
  writeLCD("BACKWARD N CM", 0, 0);
  writeLCD(String(cm), 0, 1);
  //Change precision
  cm *= 100;
  //Reset steps counts
  steps = 0L;
  //Get direction at start
  int direction = getCompass();

  do {
    //Check faced directions
    int degrees = getCompass();

    //Difference of where it is and where it should be
    degrees = degrees - direction;

    degreesCorrections(degrees);

    //cm left to get where it want
    long e = cm - (steps * 10000L) / constEncoder;
    //Vel of motors
    long go;
    //If it pass
    if (e < -50)
    {
      encoderState = 2;
      go = velForward - e / constPDist;
      if (go > 110L)
      {
        go = 110L;
      }
      //P correction
      forward(go - degrees / constPCorrect, go + degrees / constPCorrect);
    }
    //If there are cm left
    else if (e > 50)
    {
      encoderState = 1;
      go = velForward + e / constPDist;
      if (go > 110L)
      {
        go = 110L;
      }
      //P correction
      backward(go + degrees / constPCorrect, go - degrees / constPCorrect);
    }
    else
    {
      encoderState = 1;
      forward(0, 0);
      break;
    }

    delay(10);
  } while (true);
}

//Go forward until it detect a wall at a given cm
void forwardUntilWall(int cmFromWall)
{
  //Display function in LCD
  lcd.clear();
  writeLCD("FORWARD UNTIL WALL", 0, 0);
  writeLCD(String(cmFromWall), 0, 1);
  //Distance detected
  int dist;
  //Faced direction at start
  int direction = getCompass();

  dist = getDistance(pinSF);
  //move forward until it detect a wall
  while (dist > 30)
  {
    //Check actual diection
    int degrees = getCompass();
    //Difference of angles
    degrees = degrees - direction;

    degreesCorrections(degrees);
    //P correction
    forward(velForward + 1000 / constPDist - degrees / constPCorrect, velForward + 1000 / constPDist + degrees / constPCorrect);
    delay(10);
    //Check distance
    dist = getDistance(pinSF);
  }

  int times = 0;
  //forward checking distance
  do {
    //Direction
    int degrees = getCompass();
    //Differences of angles
    degrees = degrees - direction;

    degreesCorrections(degrees);
    //Actual distane
    dist = getDistance(pinSF) - 4;

    //Distance left
    long e = cmFromWall - dist;
    //velocity
    long go;
    //Still distance to go
    if (e < -1)
    {
      //Velocity
      go = velForward + 180 / e;
      //doesn't go slower than 50
      if (go < 50)
      {
        go = 50L;
      }

      forward(go, go);
      //Check again from scratch
      times = 0;
    }
    //We get too close
    else if (e > 1)
    {
      //Velocity
      go = velForward - 180 / e;
      //Doesn't fo slower than 50
      if (go < 50)
      {
        go = 50L;
      }

      backward(go, go);
      //Check again from scratch
      times = 0;
    }
    //Everything perfect
    else
    {
      forward(0, 0);
      times++;
      //Three times perfect
      if (times == 2)
      {
        break;
      }

      delay(10);
    }
  } while (true);
}

//Go backs until wall the given parameters
void backwardUntilWall(int cmFromWall)
{
  //Display function in LCD
  lcd.clear();
  writeLCD("BACKWARD UNTIL WALL", 0, 0);
  writeLCD(String(cmFromWall), 0, 1);
  //Distance
  int dist;
  //Direction at start
  int direction = getCompass();

  dist = getDistance(pinSB);
  //Get close to the wall
  while (dist > 30)
  {
    //angles
    int degrees = getCompass();
    //Diff of angles
    degrees = degrees - direction;

    degreesCorrections(degrees);
    //P correction
    backward(velForward + 1000 / constPDist + degrees / constPCorrect, velForward + 1000 / constPDist - degrees / constPCorrect);
    delay(10);

    dist = getDistance(pinSB);
  }

  int times = 0;
  do {
    //Angle
    int degrees = getCompass();
    //Diff of direction
    degrees = degrees - direction;

    degreesCorrections(degrees);

    dist = getDistance(pinSB) - 4;

    //Distance left
    long e = cmFromWall - dist;
    //Velocity
    long go;
    //Get closer to the wall
    if (e < -1)
    {
      go = velForward + 180 / e;
      if (go < 50)
      {
        go = 50L;
      }

      backward(go, go); //forward(go - degrees / constPCorrect, go + degrees / constPCorrect);
      times = 0;
    }
    //Get farther from the wall
    else if (e > 1)
    {
      go = velForward - 180 / e;
      if (go < 50)
      {
        go = 50L;
      }

      forward(go, go); //backward(go + degrees / constPCorrect, go - degrees / constPCorrect);
      times = 0;
    }
    //Perfect distance
    else
    {
      brake();
      times++;
      if (times == 1)
      {
        break;
      }
    }

    delay(10);
  } while (true);
}

//Turn to a given degree
void turnToDegree(int obj)
{
  //Display function in LCD
  lcd.clear();
  writeLCD("TURN TO DEGREE", 0, 0);
  writeLCD(String(obj), 0, 1);
  //Actual angle
  int degrees;
  int times = 0;

  do {
    //Actual angle
    degrees = getCompass();
    
    degreesCorrections(degrees);
    Serial.print("Degrees: ");
    Serial.print(degrees);
    Serial.print("\t");
    Serial.print("obj: ");
    Serial.println(obj);
    
    //Diff of where we are and where we want to be
    degrees = degrees - obj;

    //Set vel of turn
    //Left
    if (degrees < - 50)
    {
      Serial.println("Izquierda");
      degrees =  degrees / constPTurn - velTurn;
      times = 0;
    }
    //Right
    else if (degrees > 50)
    {
      Serial.println("DERECHA");
      degrees = degrees / constPTurn + velTurn;
      times = 0;
    }
    else
    {
      Serial.println("EEY");
      degrees = 0L;
      times++;
    }

    turn(degrees);

    delay(90);
  } while (times < 1);
  brake();
}

//turn n andegrees to the right
void turnRightNDegrees(long degrees)
{
  //Display function in LCD
  lcd.clear();
  writeLCD("TURN RIGHT N DEGREES", 0, 0);
  writeLCD(String(degrees), 0, 1);
  int obj = (getCompass() + degrees) % 36000L;
  turnToDegree(obj);
}

//Turn n degrees to the left
void turnLeftNDegrees(long degrees)
{
  //Display function in LCD
  lcd.clear();
  writeLCD("TURN LEFT N DEGREES", 0, 0);
  writeLCD(String(degrees), 0, 1);
  int obj = getCompass() - degrees;
  if (obj < 0L)
  {
    obj += 36000;
  }

  turnToDegree(obj);
}