//Communication between the Raspberry and the arduino
void communication() {
  unsigned long data;
  unsigned long data1, data2;
  char order = '0';
  char sharp = 'A';
  int iDistance = 0;
  int num = 0;
  int iAm = 0;

  if (Serial.available() > 0)
  {
    order = Serial.read();
    switch (order)
    {
      case 'a':
        while (Serial.available() < 1);

        data = Serial.read();
        forwardNCm(data, false);
        Serial.write('1');
        break;

      case 'b':
        while (Serial.available() < 1);

        data = Serial.read();
        backwardNCm(data, false);
        Serial.write('1');
        break;

      case 'c':
        while (Serial.available() < 1);

        data = Serial.read();
        forwardUntilWallN(data);
        Serial.write('1');
        break;

      case 'd':
        while (Serial.available() < 1);

        data = Serial.read();
        backwardUntilWallN(data);
        Serial.write('1');
        break;

      case 'e':
        while (Serial.available() < 2);
        data1 = Serial.read();
        data2 = Serial.read();
        num = (((data1 << 8) | data2) * 100UL) / 100;
        turnToObjectiveN(num);
        Serial.write('1');
        break;

      case 'f':
        while (Serial.available() < 2);

        data1 = Serial.read();
        data2 = Serial.read();
        if (data2 == 1)
        {
          data1 *= - 1;
        }
        turnNDegrees(data1);
        Serial.write('1');
        break;

      case 'g':
        forwardUntilNoRight();
        Serial.write('1');
        break;

      case 'h':
        forwardUntilNoLeft(getCompass());
        Serial.write('1');
        break;

      case 'i':
        backwardUntilNoRight();
        Serial.write('1');
        break;

      case 'j':
        backwardUntilNoLeft(getCompass());
        Serial.write('1');
        break;

      case 'k':
        while (Serial.available() < 1);
        num  = Serial.read() - 48;
        iDistance = getDistance(pinSharp[num]);
        Serial.println(iDistance);
        break;

      case 'l':
        iAm = getCompass();
        goGrabTerrineMainEntrance();
        Serial.write('1');
        break;

      case 'm':
        getInCow(false);
        Serial.write('1');
        break;

      case 'n':
        while(Serial.available() < 1);
        sharp = Serial.read();
        if(sharp == 'n'){
          turnToObjectiveN(iNorth);
        }
        else if(sharp == 'e'){
          turnToObjectiveN(iEast);
        }
        else if(sharp == 's'){
          turnToObjectiveN(iSouth);
        }
        else if(sharp == 'w'){
          turnToObjectiveN(iWest);
        }
        Serial.write('1');
        break;

      case 'o':
        forward(velLF, velLB, velRF, velRB);
        Serial.write('1');
        break;

      case 'p':
        backward(velLF, velLB, velRF, velRB);
        Serial.write('1');
        break;

      case 'q':
        brake();
        Serial.write('1');
        break;

      case 'r':
        turnRight(velLF, velLB, velRF, velRB);
        Serial.write('1');
        break;

      case 's':
        turnLeft(velLF, velLB, velRF, velRB);
        Serial.write('1');
        break;

      case 't':
        goToStartNew();
        Serial.write('1');
        break;

      case 'u':
        milk();
        Serial.write('1');
        break;

      case 'v':
        inARestart = true;
        terrineNextIsUltraBasic = false;
        goToStartRestar(false);
        Serial.write('1');
        break;

      case 'w':
        goToTank();
        Serial.write('1');
        break;

      case 'x':
        clawToStartPoint();
        Serial.write('1');
        break;

      case 'y':
        goToStartFromTank();
        Serial.write('1');
        break;
    }
    ////writeLCD("C", 15, 0);
  }
}
