void forwardCalibration(int& velLF, int& velLB, int& velRF, int& velRB)
{
  char vel = '0';
  forward(velLF, velLB, velRF, velRB);
  if(Serial.available())
  {
    vel = Serial.read();
    if(vel == 'q')
    {
      while(Serial.available() < 1);
      velLF = Serial.readString().toInt();
      Serial.println(velLF);
    }
    else if(vel == 'p')
    {
      while(Serial.available() < 1);
      velRF = Serial.readString().toInt();
      Serial.println(velRF);
    }
    else if(vel == 'z')
    {
      while(Serial.available() < 1);
      velLB = Serial.readString().toInt();
      Serial.println(velLB);
    }
    else if(vel == 'm')
    {
      while(Serial.available() < 1);
      velRB = Serial.readString().toInt();
      Serial.println(velRB);
    }
  }
}

void sharpCalibration()
{
  int sharpF = round(13 * pow((analogRead(pinSF) * 0.0048828125), -1));
  int sharpRF = round(13 * pow((analogRead(pinSRF) * 0.0048828125), -1));
  int sharpLF = round(13 * pow((analogRead(pinSLF) * 0.0048828125), -1));
  int sharpB = round(13 * pow((analogRead(pinSB) * 0.0048828125), -1));
  int sharpRB = round(13 * pow((analogRead(pinSRB) * 0.0048828125), -1));
  int sharpLB = round(13 * pow((analogRead(pinSLB) * 0.0048828125), -1));

  Serial.print("F: ");
  Serial.print(sharpF);
  Serial.print("\t");
  Serial.print("RF: ");
  Serial.print(sharpRF);
  Serial.print("\t");
  Serial.print("LF: ");
  Serial.print(sharpLF);
  Serial.print("\t");
  Serial.print("B: ");
  Serial.print(sharpB);
  Serial.print("\t");
  Serial.print("LB: ");
  Serial.print(sharpLB);
  Serial.print("\t");
  Serial.print("RB: ");
  Serial.println(sharpRB);
}

void tryForwardP()
{
  //Stay in zero
  int angle = getCompass();

  //Set speed to default
  int leftFront = velLF;
  int leftBack = velLB;
  int rightFront = velRF;
  int rightBack = velRB;

  //Start moving
  forward(leftFront, leftBack, rightFront, rightBack);
  
  //Stay in forwardP forever
  while(true)
  {
    //P correction
    forwardP(angle, leftFront, leftBack, rightFront, rightBack, false);
    //delay(100);
  }
}

void tryBackwardP()
{
  //Stay in zero
  int angle = getCompass();

  //Set speed to default
  int leftFront = velLF;
  int leftBack = velLB;
  int rightFront = velRF;
  int rightBack = velRB;

  //Start moving
  backward(leftFront, leftBack, rightFront, rightBack);
  
  //Stay in forwardP forever
  while(true)
  {
    //P correction
    backwardP(angle, leftFront, leftBack, rightFront, rightBack, false);
    //delay(100);
  }
}

void limitSwitchesCalibration()
{
  Serial.print("LI: ");
  Serial.print(digitalRead(pinLI));
  Serial.print("\t");
  Serial.print("LO: ");
  Serial.print(digitalRead(pinLO));
  Serial.print("\t");
  Serial.print("LR: ");
  Serial.print(digitalRead(pinLR));
  Serial.print("\t");
  Serial.print("LL: ");
  Serial.print(digitalRead(pinLL));
  Serial.print("\t");
  Serial.println();
}
