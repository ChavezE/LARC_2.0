void getInCow(){
  //Clear LCD and write that we are getting under the cow
  ////lcd.clear();
  ////writeLCD("Entrando a vaca", 0, 0);

  //Angle at start
  int iAm = getCompass();

  //Velocity of motors
  int LF = velSlowLF;
  int LB = velSlowLB;
  int RF = velSlowRF;
  int RB = velSlowRB;

  //Degrees the robot will turn to try avoiding the legs when encountering one
  int iTurn = 6;

  //Distances of sided front and back sharps
  int distRF = getDistance(pinSRF);
  int distLF = getDistance(pinSLF);
  int distRB = getDistance(pinSRB);
  int distLB = getDistance(pinSLB);

  //the leg that touch was the right one
  bool bRight = digitalRead(pinLR);
  //The leg that touch was the left one
  bool bLeft = digitalRead(pinLL);

  forward(LF, LB, RF, RB);
  while(digitalRead(pinLR) == false && digitalRead(pinLL) == false && distLF > 30 && distRF > 30)
  {
    //P Correction
    forwardP(iAm, LF, LB, RF, RB, true);
    //Sharps
    distRF = getDistance(pinSRF);
    distLF = getDistance(pinSLF);
  }

  //the leg that touch was the right one
  bRight = digitalRead(pinLR);
  //The leg that touch was the left one
  bLeft = digitalRead(pinLL);
  brake();

  //Get away form the leg
  if (bRight == true || bLeft == true)
  {
    backwardNCm(5, true);
  }

  //Reset vel
  LF = velSlowLF;
  LB = velSlowLB;
  RF = velSlowRF;
  RB = velSlowRB;

  //Check sharps
  distRB = getDistance(pinSRB);
  distLB = getDistance(pinSLB);

  //If right leg was touched
  if(bRight){
    //Turn 90 degrees to the left
    turnNDegrees(-90);
    //Go backward to make sure the sharp of the back havent detect the leg
    backwardNCm(10, false);
    //Forward till back sharp detect the leg
    forward(LF, LB, RF, RB);
    do{
      distRB = getDistance(pinSRB);
    }while(distRB > 30);
    //Keep moving until the back sharp lose the leg
    while(distRB < 30){
      distRB = getDistance(pinSRB);
    }
    //Stop
    brake();
    //Move forward a little bit to get away from the leg
    forwardNCm(6, true);
    //Turn 90 degrees to the right
    turnNDegrees(90);
    //Restart the function
    getInCow();
    //forwardNCm(10, false);
  }
  //If left leg was touched
  else if(bLeft){
    //Turn 90 degrees to the right
    turnNDegrees(90);
    //Go backward to make sure the sharp of the back havent detect the leg
    backwardNCm(10, false);
    //Go forward until the back sharp detect the leg
    forward(LF, LB, RF, RB);
    do{
      distLB = getDistance(pinSLB);
    }while(distLB > 30);
    //Keep moving until the back sharp lose the leg
    while(distLB < 30){
      distLB = getDistance(pinSLB);
    }
    //Stop
    brake();
    //Move forward a little to get away from the leg
    forwardNCm(4, false);
    //Turn 90 degrees to th eleft
    turnNDegrees(-90);
    //Start funciton again
    getInCow();
    //forwardNCm(10, false);
  }
  //If no leg was touched
  else{
    //If right sharp detect the leg
    if(distRF < 30){
      //Check actual angle
      int angle = getCompass();
      //Turn right until left sharp detect the leg
      while(distLF > 30){
        turnNDegrees(3);
        distLF = getDistance(pinSLF);
      }
      //Check new angle
      int angle2 = getCompass();
      //Get the biggest from the two angles
      int biggest = (angle > angle2 ? angle : angle2);
      //Get the smallest from the two angles
      int smallest = (angle < angle2 ? angle : angle2);
      //Diff between the smallest and the biggest angle
      int counter = 0;
      //Save the data of the biggest angle
      int plus = biggest;
      int sub = biggest;
      while(true){
        //If one of the data is equal to the smallest angle break loop
        if(plus == smallest || sub == smallest){
          break;
        }
        //Add one to variable and check if is not bigger than 360
        plus = (plus + 1) % 360;
        //Substract one to variable and check if is not bigget than 360
        sub = (sub - 1) % 360;
        //Add one angle of difference
        counter++;
      }
      //Get half of the counter
      counter /= 2;
      //Turn to the left the half of degrees of difference between the two angles
      turnNDegrees(counter * -1);
    }
    else if(distLF < 30){
      //Check actual angle
      int angle = getCompass();
      //Turn right until right sharp detect the leg
      while(distRF > 30){
        turnNDegrees(-3);
        distRF = getDistance(pinSRF);
      }
      //Check new angle
      int angle2 = getCompass();
      //Get the biggest from the two angles
      int biggest = (angle > angle2 ? angle : angle2);
      //Get the smallest from the two angles
      int smallest = (angle < angle2 ? angle : angle2);
      //Diff between the smallest and the biggest angle
      int counter = 0;
      //Save the data of the biggest angle
      int plus = biggest;
      int sub = biggest;
      while(true){
        //If one of the data is equal to the smallest angle break loop
        if(plus == smallest || sub == smallest){
          break;
        }
        //Add one to variable and check if is not bigger than 360
        plus = (plus + 1) % 360;
        //Substract one to variable and check if is not bigget than 360
        sub = (sub - 1) % 360;
        //Add one angle of difference
        counter++;
      }
      //Get half of the counter
      counter /= 2;
      //Turn to the right the half of degrees of difference between the two angles
      turnNDegrees(counter);
    }
    forwardNCm(10, false);
  }
  //To be sure
  brake();
}
