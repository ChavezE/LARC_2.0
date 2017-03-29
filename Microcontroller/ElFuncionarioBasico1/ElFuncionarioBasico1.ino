#include <Adafruit_BNO055.h>


/**
   PINS
*/
//Motors
const byte adelanteDerecha = 0, adelanteIzquierda = 1, atrasDerecha = 2, atrasIzquierda = 3;
int pinA[4] = {29,  27,  35,   37}; //adelante inApin
int pinB[4] = {31,  33,  41,  39}; //atras  inBpin
int pwmpin[4] = {2,  3, 5,   4}; //PWM's input

const byte PINENCODER = 18;

//Sharps
const byte sharpAde, sharpAtras, sharpDerAde, sharpDerCentro, sharpDerAtras, sharpIzqAde, sharpIzqCentro, sharpIzqAtras;


/*
   CONSTANTES
*/
const unsigned long constEncoder = 5500UL; //La constante se divide (steps*10000L)/4635L;
const long constPGirar = 10L; //La constante se divide
const long constPCorrect = 18L; //La const se divide
const long constPDist = 110L; //La const se divide

const long velAvanzar = 60L;
const long velGirar = 60L;


/*
   VARIABLES
*/
Adafruit_BNO055 bno;

//Encoder
volatile unsigned long steps = 0;
volatile byte estadoEncoder = 0; //0-parado, 1-adelante, 2-atras

//MIENTRAS
long DIRECCION;

/*
   SENSORES
*/
long getCompass() {
  return bno.getVector(Adafruit_BNO055::VECTOR_EULER).x() * 100; //Para tratarlo como int, pero tomar en cuenta los decimales 180.00=18000
}

void pasoEncoder() {
  if (estadoEncoder == 1) steps++;
  else if (estadoEncoder == 2) steps--;
}

int getDistance(byte sharp) {
  return 0;
}


/*
   MOVIMIENTOS
*/
//Adelante Basico
void adelante(int izq, int der) {
  digitalWrite(pinA[adelanteDerecha], HIGH);
  digitalWrite(pinB[adelanteDerecha], LOW);
  digitalWrite(pinA[adelanteIzquierda], HIGH);
  digitalWrite(pinB[adelanteIzquierda], LOW);
  digitalWrite(pinA[atrasDerecha], HIGH);
  digitalWrite(pinB[atrasDerecha], LOW);
  digitalWrite(pinA[atrasIzquierda], HIGH);
  digitalWrite(pinB[atrasIzquierda], LOW);

  if (der > 255) der = 255;
  else if (der < 0) der = 0;
  if (izq > 255) izq = 255;
  else if (izq < 0) izq = 0;
  analogWrite(pwmpin[adelanteDerecha], der);
  analogWrite(pwmpin[adelanteIzquierda], izq);
  analogWrite(pwmpin[atrasDerecha], der);
  analogWrite(pwmpin[atrasIzquierda], izq);
}

//Atras Basico
void atras(int izq, int der) {
  digitalWrite(pinA[adelanteDerecha], LOW);
  digitalWrite(pinB[adelanteDerecha], HIGH);
  digitalWrite(pinA[adelanteIzquierda], LOW);
  digitalWrite(pinB[adelanteIzquierda], HIGH);
  digitalWrite(pinA[atrasDerecha], LOW);
  digitalWrite(pinB[atrasDerecha], HIGH);
  digitalWrite(pinA[atrasIzquierda], LOW);
  digitalWrite(pinB[atrasIzquierda], HIGH);

  if (der > 255) der = 255;
  else if (der < 0) der = 0;
  if (izq > 255) izq = 255;
  else if (izq < 0) izq = 0;
  analogWrite(pwmpin[adelanteDerecha], der);
  analogWrite(pwmpin[adelanteIzquierda], izq);
  analogWrite(pwmpin[atrasDerecha], der);
  analogWrite(pwmpin[atrasIzquierda], izq);
}

//Girar Basico
void girar(int vel) {
  //Negativo es a la derecha
  if (vel < 0) { //- ->derecha
    if (vel < -255) vel = 255;
    else vel = -vel;

    digitalWrite(pinA[adelanteDerecha], LOW);
    digitalWrite(pinB[adelanteDerecha], HIGH);
    digitalWrite(pinA[adelanteIzquierda], HIGH);
    digitalWrite(pinB[adelanteIzquierda], LOW);
    digitalWrite(pinA[atrasDerecha], LOW);
    digitalWrite(pinB[atrasDerecha], HIGH);
    digitalWrite(pinA[atrasIzquierda], HIGH);
    digitalWrite(pinB[atrasIzquierda], LOW);

    analogWrite(pwmpin[adelanteDerecha], vel);
    analogWrite(pwmpin[adelanteIzquierda], vel);
    analogWrite(pwmpin[atrasDerecha], vel);
    analogWrite(pwmpin[atrasIzquierda], vel);
  } else { //+ -> izquierda
    if (vel > 255) vel = 255;

    digitalWrite(pinA[adelanteDerecha], HIGH);
    digitalWrite(pinB[adelanteDerecha], LOW);
    digitalWrite(pinA[adelanteIzquierda], LOW);
    digitalWrite(pinB[adelanteIzquierda], HIGH);
    digitalWrite(pinA[atrasDerecha], HIGH);
    digitalWrite(pinB[atrasDerecha], LOW);
    digitalWrite(pinA[atrasIzquierda], LOW);
    digitalWrite(pinB[atrasIzquierda], HIGH);

    analogWrite(pwmpin[adelanteDerecha], vel);
    analogWrite(pwmpin[adelanteIzquierda], vel);
    analogWrite(pwmpin[atrasDerecha], vel);
    analogWrite(pwmpin[atrasIzquierda], vel);
  }

}


//Adelante m cms
void adelanteTantosCms(long cant) { //cant en cm_2dec
  steps = 0UL;

  do {
    long grados = getCompass();

    grados = grados - DIRECCION;

    if (grados > 18000L) {
      grados = -( 18000L - (grados - 18000L) );
    } else if (grados < -18000L) {
      grados = ( 18000L - ((-grados) - 18000L) );
    }


    if (grados > 600L) grados = 600L;
    else if (grados < -600L) grados = -600L;


    //if(steps>50) steps-= abs(grados)/200;
    long e = cant - (steps * 10000L) / constEncoder; //Error en cm_2dec
    long avanzar;
    if (e < -50) {
      estadoEncoder = 2;
      avanzar = velAvanzar - e / constPDist;
      if (avanzar > 110L) avanzar = 110L;

      atras(avanzar + grados / constPCorrect, avanzar - grados / constPCorrect);
    } else if (e > 50) {
      estadoEncoder = 1;
      avanzar = velAvanzar + e / constPDist;
      if (avanzar > 110L) avanzar = 110L;

      adelante(avanzar - grados / constPCorrect, avanzar + grados / constPCorrect);
    } else {
      estadoEncoder = 1; //Para que se vaya con el primer if
      adelante(0, 0);
      break;
    }


    delay(10);
  } while (1);

}


//Atras m cms
void atrasTantosCms(long cant) { //cant en cm_2dec
  //Simplemente se pone cambian el atras() y el adelante()
  steps = 0UL;

  do {
    long grados = getCompass();

    grados = grados - DIRECCION;

    if (grados > 18000L) {
      grados = -( 18000L - (grados - 18000L) );
    } else if (grados < -18000L) {
      grados = ( 18000L - ((-grados) - 18000L) );
    }


    if (grados > 600L) grados = 600L;
    else if (grados < -600L) grados = -600L;


    //if(steps>50) steps-= abs(grados)/200;
    long e = cant - (steps * 10000L) / constEncoder; //Error en cm_2dec
    long avanzar;
    if (e < -50) {
      estadoEncoder = 2;
      avanzar = velAvanzar - e / constPDist;
      if (avanzar > 110L) avanzar = 110L;

      adelante(avanzar - grados / constPCorrect, avanzar + grados / constPCorrect);
    } else if (e > 50) {
      estadoEncoder = 1;
      avanzar = velAvanzar + e / constPDist;
      if (avanzar > 110L) avanzar = 110L;

      atras(avanzar + grados / constPCorrect, avanzar - grados / constPCorrect);
    } else {
      estadoEncoder = 1; //Para que se vaya con el primer if
      adelante(0, 0);
      break;
    }


    delay(10);
  } while (1);

}


//Adelante hasta que este a la pared cant cms
void adelanteHastaPared(int separacion) { //Argumento en cm, 0<=separacion<=26
  //Nota: Sharps estan metidos 4cm
  int dist;

  //Primero avanzamos hasta que ya estemos cerca 30cm de la pared
  dist = getDistance(sharpAde);
  while (dist > 30) {
    long grados = getCompass();

    grados = grados - DIRECCION;

    if (grados > 18000L) {
      grados = -( 18000L - (grados - 18000L) );
    } else if (grados < -18000L) {
      grados = ( 18000L - ((-grados) - 18000L) );
    }

    if (grados > 600L) grados = 600L;
    else if (grados < -600L) grados = -600L;

    adelante(velAvanzar + 1000 / constPDist - grados / constPCorrect, velAvanzar + 1000 / constPDist + grados / constPCorrect);
    delay(10);

    dist = getDistance(sharpAde);
  }


  //Avanzamos revisando
  do {
    long grados = getCompass();

    grados = grados - DIRECCION;

    if (grados > 18000L) {
      grados = -( 18000L - (grados - 18000L) );
    } else if (grados < -18000L) {
      grados = ( 18000L - ((-grados) - 18000L) );
    }


    if (grados > 600L) grados = 600L;
    else if (grados < -600L) grados = -600L;

    dist = getDistance(sharpAde) - 4;

    long e = separacion - dist;
    long avanzar;
    if (e < -1) {
      avanzar = velAvanzar + e / 2;
      if (avanzar < 40) avanzar = 40L;

      adelante(avanzar - grados / constPCorrect, avanzar + grados / constPCorrect);
    } else if (e > 1) {
      avanzar = velAvanzar - e / 2;
      if (avanzar < 40) avanzar = 40L;

      atras(avanzar + grados / constPCorrect, avanzar - grados / constPCorrect);
    } else {
      adelante(0, 0);
      break;
    }

    delay(10);

  } while (1);

}


//Girar a los g grados
void girarAObjetivo(long obj) { //obj en grados_2dec

  long grados;
  do {
    delay(10);

    //NOTA: Cuidado al redondear, porque puedes llegar al 360 y vale todo
    long grados = getCompass();
    grados = grados - obj;

    if (grados > 18000L) {
      grados = -( 18000L - (grados - 18000L) );
    } else if (grados < -18000L) {
      grados = ( 18000L - ((-grados) - 18000L) );
    }

    if (grados > 600L) grados = 600L;
    else if (grados < -600L) grados = -600L;

    if (grados < -100) grados = grados / constPGirar - velGirar;
    else if (grados > 100) grados = grados / constPGirar + velGirar;
    else grados = 0L;

    girar(grados);

  } while (grados != 0L);

}

//Girar g grados derecha (+)
void girarDerTantosGrados(long cant) { //No negativa!
  long obj = (getCompass() + cant) % 36000L;
  girarAObjetivo(obj);
}

//Girar g grados Izquierda (-)
void girarIzqTantosGrados(long cant) { //No negativa!
  long obj = (getCompass() - cant) % 36000L;
  girarAObjetivo(abs(obj));
}


/**
   SETUP
*/
void setup() {
  delay(1000);
  Serial.begin(9600);
  delay(1000);


  //Init BNO
  if (!bno.begin(Adafruit_BNO055::OPERATION_MODE_NDOF)) {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }
  bno.setExtCrystalUse(true);

  //Init pines
  for (int i = 0; i < 4; i++)
  {
    pinMode(pinA[i], OUTPUT);
    pinMode(pinB[i], OUTPUT);
    pinMode(pwmpin[i], OUTPUT);
    analogWrite(pwmpin[i], 0);
  }

  pinMode(PINENCODER, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PINENCODER), pasoEncoder, CHANGE);

  delay(1000);
  DIRECCION = getCompass();
  estadoEncoder = 1;
}


/*
   LOOP
*/
void loop() {

  girarAObjetivo(9000);
  while (1);
}


