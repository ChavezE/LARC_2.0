#include <Adafruit_BNO055.h>


/**
 * PINS
 */
//Motors
const byte adelanteDerecha=0, adelanteIzquierda=1, atrasDerecha=2, atrasIzquierda=3;
int pinA[4] = {29,  27,  35,   37}; //adelante inApin
int pinB[4] = {31,  33,  41,  39}; //atras  inBpin
int pwmpin[4] = {2,  3, 5,   4}; //PWM's input


//BNO
Adafruit_BNO055 bno;



/*
 * METODOS
 */
void adelante(byte izq, byte der){
  digitalWrite(pinA[adelanteDerecha], HIGH);
  digitalWrite(pinB[adelanteDerecha], LOW);
  digitalWrite(pinA[adelanteIzquierda], HIGH);
  digitalWrite(pinB[adelanteIzquierda], LOW);
  digitalWrite(pinA[atrasDerecha], HIGH);
  digitalWrite(pinB[atrasDerecha], LOW);
  digitalWrite(pinA[atrasIzquierda], HIGH);
  digitalWrite(pinB[atrasIzquierda], LOW);

  analogWrite(pwmpin[adelanteDerecha], der);
  analogWrite(pwmpin[adelanteIzquierda], izq);
  analogWrite(pwmpin[atrasDerecha], der);
  analogWrite(pwmpin[atrasIzquierda], izq);
}

long getCompass(){
  return bno.getVector(Adafruit_BNO055::VECTOR_EULER).x()*100;//Para tratarlo como int, pero tomar en cuenta los decimales 180.00=18000
}



/**
 * SETUP
 */
long DIRECCION;
void setup() {
  delay(1000);
  Serial.begin(9600);
  delay(1000);


  //Init BNO
  if(!bno.begin(Adafruit_BNO055::OPERATION_MODE_NDOF)){
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  bno.setExtCrystalUse(true);

  //Init pines
  for (int i=0; i<4; i++)
  {
      pinMode(pinA[i], OUTPUT);
      pinMode(pinB[i], OUTPUT);
      pinMode(pwmpin[i], OUTPUT);
  }

  
  delay(1000);
  DIRECCION= getCompass();
}

/**
 * LOOP
 */
void loop() {
    delay(10);
  
    long grados= getCompass();  
    
    grados= grados- DIRECCION;
  
    if(grados > 18000L){
      grados= -( 18000L-(grados-18000L) );
    }else if(grados < -18000L){
      grados= ( 18000L-((-grados)-18000L) );
    }
    
    
    if(grados>600L) grados= 600L;
    else if(grados<-600L) grados= -600L;
    
    adelante(70L-grados/15L, 70L+grados/15L);
    
  
}


