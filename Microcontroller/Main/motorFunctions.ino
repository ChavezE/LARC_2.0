///////////////////
// Todo Funciona //
///////////////////

////////////////
//  Movments  //
////////////////

//Set the motors in the configuration given in the parameters
void setMotor(bool b1, bool b2, bool b3, bool b4, bool b5, bool b6, bool b7, bool b8)
{
  digitalWrite(pinMFRA, b1);
  digitalWrite(pinMFRB, b2);

  digitalWrite(pinMFLA, b3);
  digitalWrite(pinMFLB, b4);

  digitalWrite(pinMBRA, b5);
  digitalWrite(pinMBRB, b6);

  digitalWrite(pinMBLA, b7);
  digitalWrite(pinMBLB, b8);
}

//Check if the velocity given in the parameters doesn't pass 255 or be less than 0
void checkVel(int& left, int& right)
{
  if (right > 255)
  {
    right = 255;
  }
  else if (right < 0)
  {
    right = 0;
  }

  if (left > 255)
  {
    left = 255;
  }
  else if (left < 0)
  {
    left = 0;
  }
}

void setVelocity(int left, int right)
{
  analogWrite(pinPWMFR, right);
  analogWrite(pinPWMBR, right);

  analogWrite(pinPWMFL, left);
  analogWrite(pinPWMBL, left);
}

//Stop motors
void brake()
{
  setVelocity(0, 0);
}

//Go Forward, with speed set in parameters
void forward(int left, int right)
{
  //Set enables of motors to go forward
  setMotor(1, 0, 1, 0, 1, 0, 1, 0);
  //Check the velocity
  checkVel(left, right);
  //Set velocity to the motors
  setVelocity(left, right);
}

//Go Backward, with speed set in parameters
void backward(int left, int right)
{
  //Set enables of motors to go forward
  setMotor(0, 1, 0, 1, 0, 1, 0, 1);
  //Check the velocity
  checkVel(left, right);
  //Set velocity to the motors
  setVelocity(left, right);
}

//Turn right, with speed set in parameter
void turnRight(int vel)
{
  //Set enables of motors to go forward
  setMotor(0, 1, 1, 0, 0, 1, 1, 0);
  //Check the velocity
  checkVel(vel, vel);
  //Set velocity to the motors
  setVelocity(vel, vel);
}

//Turn left, with speed set in parameter
void turnLeft(int vel)
{
  //Set enables of motors to go forward
  setMotor(1, 0, 0, 1, 1, 0, 0, 1);
  //Check the velocity
  checkVel(vel, vel);
  //Set velocity to the motors
  setVelocity(vel, vel);
}

//Turn left or right depending on the symbol of the velocity
void turn(int vel)
{
  if (vel > 0)
  {
    turnLeft(vel);
  }
  else if (vel < 0)
  {
    turnRight(vel * -1);
  }
}
