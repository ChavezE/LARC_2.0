/**
 * Move the claw to the initial point: in, down, horizontal.
 * It has two modes: fast and slowAndSecure.
 * +fast: horizontalClaw, platIn, downClaw
 * +slowAndSecure: clawUp, clawIn, clawHorizontal, clawDown
 *
 * NOTE: This method doesnt do anything with close/open claw.
 *
 * @param safeAndSlow {bool=false}
 *
 */
 void clawToStartPoint(bool safeAndSlow) {
   if (safeAndSlow) {
     upClaw();

     platIn();

     horizontalClaw();

     downClaw();
   } else {
     horizontalClaw();

     platIn();

     downClaw();
   }

 }

 /**
  * Move up the claw and check if the distance of the claw is
  * less than certain distance.
  *
  * Note: This leaves the claw up
  *
  * @return {bool} if we have the terrine
  */
 bool checkHaveTerrine() {
   const int valorMax = 9;

   upClaw();

   const int cantReads = 6;
   int sum = 0;
   for (int x = 1; x <= cantReads; x++) {
    sum += ultrasonicClaw.ping_cm(); // TODO: Create separate ping function with already average
    delay(50);
   }

   return sum / cantReads < valorMax && sum / cantReads > 0;
 }

 /**
  * Function to move out the claw and try to grab the terrine.
  * It does NOT close the claw in case the limit was reached
  * and maybe if we close we could push a terrine.
  *
  * @return {bool} false if limit was touch
  */
 bool tryToGrabTerrine() {
    const int maxValue = 8;

    openClaw();

    // Claw out until feel the terrine or limit
    int suma = 0, cant = 0;
    const int cantToProm = 5;

    platformStartToOut();
    while (digitalRead(pinLO) == HIGH) {
      int mientr = ultrasonicClaw.ping_cm();
      suma += mientr;

      if (++cant == cantToProm) {
        if (suma / cantToProm < maxValue && suma / cantToProm > 0) {
          delay(1000); // Delay to get nearer to the terrine
          break;
        } else {
          suma = 0;
          cant = 0;
        }
      }

      delay(50);
    }

    platformStop();

    return digitalRead(pinLO) == HIGH;
 }


/**
 * Routine Basic form to go and grab terrine.
 * The robot starts in the center of the field pointing to the north,
 * it turns left 90 and then it starts backwards.
 *
 * @param northAngle {int} Angle where the north is (the angle where the
 *  robot starts pointing to).
 *
 */
void goGrabTerrineBasic(const int northAngle) {
  SerialLog serialLogger;
  //serialLogger.init();
  LCDLogger lcdLogger;
  lcdLogger.init();

  AbstractLoggable *loggerArray[2]{&lcdLogger, &serialLogger};
  Logger logger("Mega", "GrabTerrines", LevelLogger::INFO, loggerArray, 1);

  logger.log("Grab Terrines");
  delay(1000);

  const int gradosObjetivo = northAngle - 90 < 0 ?
    northAngle - 90 + 360 : northAngle - 90;

  clawToStartPoint(false);

  turnToObjectiveN(gradosObjetivo);
  backwardNCm(50, false);

  encoderState = 1;
  int untilSteps = (encoder30Cm / 30) * 30; // 70
  steps = 0;
  int distFront, distBack;
  while (steps < untilSteps) {
    backwardWithLeftWall(gradosObjetivo, 17, false, distFront,
      distBack); // TODO: See if this part can be merge with the other part of look at the blank space
  }
  brake();
  turnToObjectiveN(gradosObjetivo);

  int mientr1, mientr2, mientr3, mientr4;
  bool grabbed = false;
  do {
    logger.log("En inicio");
    delay(2000);


    // Backward until we find a "blank space"
    // This make sense in the correct field
    backward(velSlowLF, velSlowLB, velSlowRF, velSlowRB);
    while (getDistance(pinSLB) < 30) {
      backwardP(gradosObjetivo, mientr1, mientr2, mientr3, mientr4, true);
    }
    brake(); // TODO: Check if we need to implement a harder brake with seconds to the other direction
    logger.log("Out find blank");
    delay(1000);

    do {
      logger.log("Look not blank");
      // Backward until we dont find a "blank space" that is a terrine
      backward(velSlowLF, velSlowLB, velSlowRF, velSlowRB);
      while (getDistance(pinSLB) > 35 && digitalRead(pinLLB) == HIGH && digitalRead(pinLRB) == HIGH) { // MIENTRAS la distancia es 10 por pista de pruebas
        backwardP(gradosObjetivo, mientr1, mientr2, mientr3, mientr4, true); // TODO: Check if it is neccesary to quit only if n times
      }
      brake();
      logger.log("Found not blank");
      delay(1000);

      // If we get to the wall, lets return and restart
      if (digitalRead(pinLLB) == LOW || digitalRead(pinLRB) == LOW) {
        logger.log("Limits tocando");
        forwardNCm(65, true);
        break;
      }

      turnToObjectiveN(gradosObjetivo);
      // Lets move until the terrine, looking at the limits
      encoderState = 1;
      int untilSteps2 = (encoder30Cm / 30) * 6; // TODO: Implement a way to confirm that we arrive 'exactly' in front to the terrine
      steps = 0;
      while (steps < untilSteps2 && digitalRead(pinLLB) == HIGH && digitalRead(pinLRB) == HIGH) {
        backwardP(gradosObjetivo, mientr1, mientr2, mientr3, mientr4, true); 
      }
      brake();

      if (digitalRead(pinLLB) == LOW || digitalRead(pinLRB) == LOW) {
        logger.log("Limits tocando");
        forwardNCm(65, true);
        break;
      }
      

      if (tryToGrabTerrine()) {
        logger.log("No limit");
        delay(2000);
        // TODO: Look for the terrine with the claw in one side and change it to grab it
        closeClaw();

        if (checkHaveTerrine()) {
          logger.log("Lo agarramos");
          clawToStartPoint(true);
          grabbed = true;
        } else {
          logger.log("No grabbed");
          delay(2000);
        }
      } else {
        logger.log("Tocamos limit");
        delay(2000);
      }

      if (!grabbed) {
        clawToStartPoint(false);
        closeClaw();
        // backwardNCm(2, true);
      }

    } while(!grabbed);

  } while (!grabbed);

  parkingFrontRight(false);

}

/**
 * This routine is to grab the terrine in the left side.
 * It starts pointing to the west and in the gate.
 *
 * @param northAngle {int} Angle where the north is (the angle where the
 *  robot starts pointing to).
 *
 * @return {bool} true if grabbed
 * TODO: If we detect something one time, that is the side.
 *
 */
bool goGrabTerrineBasicLeftSide(const int northAngle) {
  SerialLog serialLogger;
  //serialLogger.init();
  LCDLogger lcdLogger;
  lcdLogger.init();

  AbstractLoggable *loggerArray[2]{&lcdLogger, &serialLogger};
  Logger logger("Mega", "GTerrineLeft", LevelLogger::INFO, loggerArray, 1);

  logger.log("GTerrines Left");
  delay(1000);

  const int gradosObjetivo = northAngle;//northAngle - 90 < 0 ?
    //northAngle - 90 + 360 : northAngle - 90;

  clawToStartPoint(false);

  //turnToObjectiveN(gradosObjetivo);
  forwardNCm(50, false); // Pass the gate

  // Pass all the first wall with wall-correction
  encoderState = 1;
  int untilSteps = (encoder30Cm / 30) * 25; // 70
  steps = 0;
  int distFront, distBack;
  while (steps < untilSteps) {
    forwardWithLeftWall(gradosObjetivo, 17, false, distFront,
      distBack); // TODO: See if this part can be merge with the other part of look at the blank space
  }
  brake();
  turnToObjectiveN(gradosObjetivo);

  int mientr1, mientr2, mientr3, mientr4;
  bool grabbed = false;
  do {
    logger.log("En inicio");
    delay(2000);


    // Forward until we find a "blank space"
    // This make sense in the correct field
    forward(velSlowLF, velSlowLB, velSlowRF, velSlowRB);
    while (getDistance(pinSLB) < 30) {
      forwardP(gradosObjetivo, mientr1, mientr2, mientr3, mientr4, true);
    }
    brake(); // TODO: Check if we need to implement a harder brake with seconds to the other direction
    logger.log("Out find blank");
    delay(1000);

    do {
      logger.log("Look not blank");
      // Forward until we dont find a "blank space" that is a terrine
      forward(velSlowLF, velSlowLB, velSlowRF, velSlowRB);
      while (getDistance(pinSLB) > 35 && digitalRead(pinLL) == HIGH && digitalRead(pinLR) == HIGH) { // MIENTRAS la distancia es 10 por pista de pruebas
        forwardP(gradosObjetivo, mientr1, mientr2, mientr3, mientr4, true); // TODO: Check if it is neccesary to quit only if n times
      }
      brake();
      logger.log("Found not blank");
      delay(1000);

      // If we get to the wall, lets return and restart
      if (digitalRead(pinLL) == LOW || digitalRead(pinLR) == LOW) {
        logger.log("Limits tocando1");
        backwardNCm(65, true);
        break;
      }
      turnToObjectiveN(gradosObjetivo);

      // Lets move until the terrine, NOT looking at the limits
      backwardNCm(8, true);
      

      if (tryToGrabTerrine()) {
        logger.log("No limit");
        delay(2000);
        // TODO: Look for the terrine with the claw in one side and change it to grab it
        closeClaw();

        if (checkHaveTerrine()) {
          logger.log("Lo agarramos");
          clawToStartPoint(true);
          grabbed = true;
        } else {
          logger.log("No grabbed");
          delay(2000);
        }
      } else {
        logger.log("Tocamos limit");
        delay(2000);
      }

      if (!grabbed) {
        clawToStartPoint(false);
        closeClaw();
        forwardNCm(10, true);
        // TODO: Here we can return a little to look again in the part we forwarding to arrive to terrine with the claw
      }

    } while(!grabbed);

  } while (!grabbed);

  return true;

}


void parkingFrontRight(const bool& bSlow) {
  turnNDegrees(20);
  forwardNCm(20, bSlow);
  turnNDegrees(-20);
}


/**
 * NO PROBADA DESPUES DE CAMBIARLA A DEVELOPMENT. Antes si funcionaba, esperemos que en esta branch tambien.
 * Function to go to the terrines zone and grab a terrine.
 * This is expected to be called when the robot it already fully in the
 * right wall pointing the front part to the terrine zone.
 * New version with N+D.
 *
 * TODO:
 * -Checar las validaciones con sus letreros aparescan y sean pertinentes
 * -Implementar brake con un poco de reverse para detenernos en el vaso
 * -Timeout para que ya esta en la zona de vasos
 *
 * -Implementar que hacer si salio mucho un "fallo" al topar contra la
 *   pared de enfrente
 *
 * -Utilizar los encoders
 * -Ver timeouts especificos
*/
void goGrabTerrines(const int gradosObjetivo)
{
  SerialLog serialLogger;
  serialLogger.init();
  LCDLogger lcdLogger;
  lcdLogger.init();

  AbstractLoggable *loggerArray[2]{&serialLogger, &lcdLogger};
  Logger logger("Mega", "GrabTerrines", LevelLogger::INFO, loggerArray, 0);
  Logger loggerOnlySerial("Mega", "GrabTerrines", LevelLogger::INFO, loggerArray, 0);

  logger.log("Grab Terrines");
  delay(2000);

  const int separacionParedVasosObj = 5 + 5; // deseado + minSharps


  // Nos acercaremos a la plataforma de vasos derechitos a una cierta dist de la pared.
  // Despues de esta parte, muy probablemete ya estemos en la zona de vasos.
  // No es seguro ni es un hecho. Solo esta "garantizado" que si ya
  // llegamos, esta parte ya termino. Pero, puede que ya termino y todavia no llegamos.
  const int separacionParedCaminoObj = 10 + separacionParedVasosObj;

  int cantMedidas = 0, sumaMedidas = 0;
  bool caminoALosVasos = true, achievedSeparacion = false;

  openClaw();
  platIn();
  int mientr1 = 0, mientr2 = 0, mientr3 = 0, mientr4 = 0;
  int distFront, distBack;
  unsigned long timeMaxLimit = millis() + 20000UL; // TODO: Implementar TIME LIMIT
  do {

    int separacion;
    if (achievedSeparacion) {
      forwardP(gradosObjetivo, mientr1, mientr2, mientr3, mientr4, true);
      separacion = getDistance(pinSRF); // Sharp Right front
    } else {
      separacion = backwardWithLeftWall(gradosObjetivo, separacionParedCaminoObj,
        true, distFront, distBack); // THIS DOESNT WORK ANY MORE
    }

    if (digitalRead(pinLR) == HIGH) {
      brake();
      logger.log("Limit Right");

      int diffObjYCompass = getAngleDifferenceD(gradosObjetivo, getCompass());

      // Comprobemos si es porque ya llegamos y estamos muy cerca o porque nos fuimos chueco
      if (//millis() >= (timeMaxLimit - (timeMaxLimit / 8)) &&// || caminado > 60cm*0.8) &&
        abs(diffObjYCompass) < 15) {
        // Probablemente ya llegamos y chocamos.
        // Hay que regresar y repetir
        // TODO: Hay que aumentar la const separacionParedVasosObj
        logger.log("Choque vasos. DiffGrados= " + diffObjYCompass);
        // TODO: Hacer algo mas avanzado aqui para que el limit no se atore contra la pared
        int angulo = gradosObjetivo + 35;
        turnToObjectiveN(angulo >= 360 ? angulo - 360 : angulo);
      } else {
        // Nos fuimos chueco y chocamos con la pared.
        // Hay que regresar y repetir
        // TODO: hay que cambiar las const de p
        logger.log("Choque chueco");
      }

      backwardNCm(5, true);
      turnToObjectiveN(gradosObjetivo);

      // TODO: Modificar la funcion backwardUntilNoRight para que acepte de parametro el degreesObjetive
      // backwardUntilNoRight(); // Nos vamos al centro
      // forwardNCm(35, false); // Vamos a la parte que si tenemos pared a la derecha
      backwardNCm(50, true);

      cantMedidas = 0;
      sumaMedidas = 0;
      caminoALosVasos = true;
      achievedSeparacion = false;
      timeMaxLimit = millis() + 20000UL;
    } else {

      sumaMedidas += separacion;
      if ((++cantMedidas) == 2) {
        int prom = sumaMedidas / cantMedidas; // TODO: Division entera
        loggerOnlySerial.log("Prom= " + String(prom));
        if (achievedSeparacion) {
          if (prom >= separacionParedVasosObj - 2 && prom <= separacionParedVasosObj + 2) {
            caminoALosVasos = false;
            logger.log("Dist zona vasos");
          }
        } else if (prom >= separacionParedCaminoObj - 2 && prom <= separacionParedCaminoObj + 2) {
          achievedSeparacion = true;
          logger.log("Separacion Achieved");
        }

        sumaMedidas = 0;
        cantMedidas = 0;
      }


      if (caminoALosVasos) {
        int vasoDist = getDistance(pinSC);
        // int frenteDist = getDistance(pinSF); MUERE ESTO, porque ya muchas variables que seguir y se alenta esto

        if (millis() >= timeMaxLimit) {
          caminoALosVasos = false;
          logger.log("Time limit");
          delay(2000);
        } else if (vasoDist < 30) {
          brake();
          backwardNCm(35, true);
          brake();

          caminoALosVasos = false;
          logger.log("Sharp garra encontramos");
          delay(2000);
        }
//         } else if (frenteDist < 30) {
//           brake();
//           backwardNCm(60 - frenteDist, true);
//           brake();

//           caminoALosVasos = false;
//           lcd.clear();
//           writeLCD("Sharp Frente", 0, 0);
//           loggerToRasp.log("Sharp en frente");
// //          delay(2000);
//         }
        // else if(caminado > 60cm) caminoALosVasos = false;
      }

    }
  } while(caminoALosVasos);
  brake();

  logger.log("LLEGADO A VASOS");
  delay(2000);


  // Empezamos a buscar vasos
  bool falloLejosPared = false;
  bool falloLejosVaso = false;
  bool falloChueco = false;
  bool falloParedLimitDer = false;

  //If the claw sharp detect something
  bool bTerrine = false;

  //Distance detected by sharp on claw
  int clawDistance;
  int frontDistance;
  do {
    lcdLogger.setDelayInLog(0);
    logger.log("En while");
    int LF = velSlowLF;
    int LB = velSlowLB;
    int RF = velSlowRF;
    int RB = velSlowRB;

    openClaw();
    platIn();

    //Start moving forward
    forward(LF, LB, RF, RB);
    //Keep moving forward until the claw or front sharp detect something or a limit switch is pushed
    do
    {
      forwardP(gradosObjetivo, LF, LB, RF, RB, true);
      clawDistance = getDistance(pinSC);
    } while (clawDistance > 30 && digitalRead(pinLL) == LOW && digitalRead(pinLR) == LOW);
    //Stop moving
    brake();

    // if TOCAMOS LIMIT AY QUE REPETIR
    int limit1 = digitalRead(pinLL), limit2 = digitalRead(pinLR);
    if (limit1 == HIGH || limit2 == HIGH) {
      logger.log("Tocaron limits: LimitLL=" + String(limit1) + ", LimitLR=" + String(limit2));
      delay(2000);

      // Aprovechamos y nos corregimos chocando al 100
      forward(velSlowLF, velSlowLB, velSlowRF, velSlowRB);
      delay(1000);
      if (digitalRead(pinLL) != digitalRead(pinLR)) {
        logger.log("Diferentes limits: LimitLL=" + String(limit1) + ", LimitLR=" + String(limit2));
        // Chocamos contra pared derecha
        brake();

        falloParedLimitDer = true;

        backwardNCm(5, true);
        turnToObjectiveN(gradosObjetivo);

        continue;
      } else {
        brake();
        logger.log("Iguales limits");
        // TODO: Ver las banderas de "fallo..." y ver cual es la modificacion para el prox intento
        // We go back to start again
        backwardNCm(35, false);
        continue;
      }
    }


    //Nos vamos tantillo pa tras
    backwardNCm(1, true); // TODO: Implementar algo mas avanzado para asegurar que nos paremos en el vaso

    lcdLogger.setDelayInLog(2000);

    const int diffCompassATolerar = 6;
    if (getAngleDifferenceD(gradosObjetivo, getCompass()) > diffCompassATolerar) {
      logger.log("Brujula chuecos. Diff=" + getAngleDifferenceD(gradosObjetivo, getCompass()));

      falloChueco = true;
      turnToObjectiveN(gradosObjetivo);
    }

    if (getDistance(pinSRF) > separacionParedVasosObj + 3) { // No se usa getDistance(pinSRB) porque puede que no haya pasado todo el robot a la zona de vasos
      logger.log("Lejos pared. Dist=" + getDistance(pinSRF));

      falloLejosPared = true;
      forwardNCm(4, true);
      continue;
    }

    const int distPuedeSalirGarra = 30;
    if (clawDistance > distPuedeSalirGarra) {
      logger.log("Max garra exceded, lejos vaso. Dist=" + clawDistance);

      falloLejosVaso = true;
      forwardNCm(4, true);
      continue;
    }


    logger.log("Agarraremos");

    openClaw();

    platformStartToOut();
    while (clawDistance > 5 && digitalRead(pinLO) == 1) {
      clawDistance = getDistance(pinSC);
    }

    platformStop();

    if (digitalRead(pinLO) == 1) {
      logger.log("Garra poco mas afuera");

      // We move it a little more
      platformStartToOut();

      // TODO: Medir velocidad para predecir la cant de tiempo que hay que sumar
      unsigned long timeToStop = millis() + 1100;
      while (digitalRead(pinLO) == 1 && timeToStop > millis());
      platformStop();
    }

    if (digitalRead(pinLO) == 0) {
      logger.log("Muy lejos vaso");

      falloLejosVaso = true;
      platIn();
      forwardNCm(4, true);
      continue;
    }


    // Checaremos si ya tenemos el vaso
    closeClaw();
    logger.log("Cerrar Garra");
    delay(3000);

    platIn();
    logger.log("Revisar agarramos");

    // Check if we grab a terrine
    backwardNCm(30, false);
    //Turn 90 degrees to the right
    //turnNDegrees(90);
    //If there is something in front of the claw it grabbed a terrine
    if (getDistance(pinSC) < 30 )
    {
      //Terrine grabbed
      bTerrine = true;
      logger.log("Lo agarramos!");
      //turnNDegrees(-90);
    }
    //If there is nothing in front of the claw
    else
    {
      logger.log("NO lo agarramos :(");
      forwardNCm(4, true);
      //Go bak to original position
      //turnNDegrees(-90);
    }

  } while (!bTerrine);

  logger.log("Grab FINISHED");

}


/*
/////Alternate versions with sharp////

bool checkHaveTerrine() {
   const int distMin = 25;

   upClaw();

   return getDistance(pinSC) < distMin;
}


bool tryToGrabTerrine() {
   openClaw();

   // Claw out until 5cms to the terrine
   platformStartToOut();
   int clawDistance = getDistance(pinSC);
  while (clawDistance > 5 && digitalRead(pinLO) == HIGH) {
    clawDistance = getDistance(pinSC);
  }

  platformStop();

  if (digitalRead(pinLO) == HIGH) {

    // We move it a little more
    platformStartToOut();

    // TODO: Medir velocidad para predecir la cant de tiempo que hay que sumar
    unsigned long timeToStop = millis() + 700;
    while (digitalRead(pinLO) == HIGH && timeToStop > millis());
    platformStop();
  }

  return digitalRead(pinLO) == HIGH;
}


*/
