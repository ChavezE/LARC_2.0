/**
 * Pendiente:
 * -Funcion adelanteHastaNoDer()
 * -Funcion adelanteHastaNoIzq()
 * 
 * 
 * Revisar los giros, porque no sirve muy bien... Talves usar p exponencial negativa...
 * 
 * Revisar para avanzar tanto cm quitar la correccion los ultimos cms para mas facil, revisar como se 
 * hace en avanzarATantoCms de la pared que funciona muy bien disque.
 * 
 * Ver si es bueno agregar un frenado en cada funcion.
 * 
 * Ver si sirve el metodo de veces++ en avanzarTantosCm con encoders
 */

#include <Adafruit_BNO055.h>


/**
 * PINS
 */
//Motors
const byte adelanteDerecha=0, adelanteIzquierda=1, atrasDerecha=2, atrasIzquierda=3;
int pinA[4] = {29,  27,  35,   37}; //adelante inApin
int pinB[4] = {31,  33,  41,  39}; //atras  inBpin
int pwmpin[4] = {2,  3, 5,   4}; //PWM's input   Salida mega 4->shield atras 5

const byte PINENCODER= 18;

//Sharps
const byte sharpAde=A4, sharpAtras=A1, sharpDerAde=A3, sharpDerCentro, sharpDerAtras=A2, sharpIzqAde=A5, sharpIzqCentro, sharpIzqAtras=A0, sharpGarra;

//LimitSwitch
const byte limitGarraAde, limitGarraAtras;

//Servos
const byte servoGarra, servoPlata;

/*
 * CONSTANTES
 */
const unsigned long constEncoder= 5500UL;//La constante se divide (steps*10000L)/4635L;
const long constPGirar= 30L;//La constante se divide
const long constPCorrect= 18L;//La const se divide
const long constPDist= 110L;//La const se divide

const long velAvanzar= 110L;
const long velGirar= 90L;


/*
 * VARIABLES
 */
Adafruit_BNO055 bno;

//Encoder
volatile unsigned long steps=0;
volatile byte estadoEncoder= 0;//0-parado, 1-adelante, 2-atras

//MIENTRAS
long DIRECCION;

/*
 * SENSORES
 */
long getCompass(){
  return bno.getVector(Adafruit_BNO055::VECTOR_EULER).x()*100;//Para tratarlo como int, pero tomar en cuenta los decimales 180.00=18000
}

void pasoEncoder(){
  if(estadoEncoder==1) steps++;
  else if(estadoEncoder==2) steps--;
}

int getDistance(byte sharp){
  int valores[7];
  for(int x=0; x<7; x++){
    float volts = analogRead(sharp)*0.0048828125;
    if(volts==0) valores[x]= 30000;//Esta muy lejos, da 0 y ponemos valor muy grande
    else valores[x]= round( 13*pow(volts, -1) );
    
    delay(26);
  }

  for(int x=6; x>=1; x--){
    for(int y=1; y<=x; y++){
      if(valores[y-1]>valores[y]){
        int mientr= valores[y];
        valores[y]= valores[y-1];
        valores[y-1]= mientr;
      } 
    }
  }

  return valores[3];
}


/*
 * MOVIMIENTOS
 */ 
//Adelante Basico
void adelante(int izq, int der){
  digitalWrite(pinA[adelanteDerecha], HIGH);
  digitalWrite(pinB[adelanteDerecha], LOW);
  digitalWrite(pinA[adelanteIzquierda], HIGH);
  digitalWrite(pinB[adelanteIzquierda], LOW);
  digitalWrite(pinA[atrasDerecha], HIGH);
  digitalWrite(pinB[atrasDerecha], LOW);
  digitalWrite(pinA[atrasIzquierda], HIGH);
  digitalWrite(pinB[atrasIzquierda], LOW);

  if(der>255) der= 255;
  else if(der<0) der=0;
  if(izq>255) izq= 255;
  else if(izq<0) izq=0;
  analogWrite(pwmpin[adelanteDerecha], der);
  analogWrite(pwmpin[adelanteIzquierda], izq);
  analogWrite(pwmpin[atrasDerecha], der);
  analogWrite(pwmpin[atrasIzquierda], izq);
}

//Atras Basico
void atras(int izq, int der){
  digitalWrite(pinA[adelanteDerecha], LOW);
  digitalWrite(pinB[adelanteDerecha], HIGH);
  digitalWrite(pinA[adelanteIzquierda], LOW);
  digitalWrite(pinB[adelanteIzquierda], HIGH);
  digitalWrite(pinA[atrasDerecha], LOW);
  digitalWrite(pinB[atrasDerecha], HIGH);
  digitalWrite(pinA[atrasIzquierda], LOW);
  digitalWrite(pinB[atrasIzquierda], HIGH);

  if(der>255) der= 255;
  else if(der<0) der=0;
  if(izq>255) izq= 255;
  else if(izq<0) izq=0;
  analogWrite(pwmpin[adelanteDerecha], der);
  analogWrite(pwmpin[adelanteIzquierda], izq);
  analogWrite(pwmpin[atrasDerecha], der);
  analogWrite(pwmpin[atrasIzquierda], izq);
}

//Girar Basico
void girar(int vel){
  //Negativo es a la derecha
  if(vel<0){//- ->derecha     
    if(vel<-255) vel= 255;
    else vel= -vel;

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
  }else{//+ -> izquierda
    if(vel>255) vel= 255;
    
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
void adelanteTantosCms(long cant){//cant en cm_2dec
  steps=0UL;
  
  do{  
    long grados= getCompass();  
    
    grados= grados- DIRECCION;
  
    if(grados > 18000L){
      grados= -( 18000L-(grados-18000L) );
    }else if(grados < -18000L){
      grados= ( 18000L-((-grados)-18000L) );
    }
    
    
    if(grados>600L) grados= 600L;
    else if(grados<-600L) grados= -600L;
    

    //if(steps>50) steps-= abs(grados)/200;
    long e= cant - (steps*10000L)/constEncoder;//Error en cm_2dec
    long avanzar;
    if(e < -50) {
      estadoEncoder= 2;
      avanzar= velAvanzar-e/constPDist;
      if(avanzar>110L) avanzar= 110L;
      
      atras(avanzar+grados/constPCorrect, avanzar-grados/constPCorrect);
    }else if(e>50){
      estadoEncoder= 1;
      avanzar= velAvanzar+e/constPDist;
      if(avanzar>110L) avanzar= 110L;
      
      adelante(avanzar-grados/constPCorrect, avanzar+grados/constPCorrect);
    }else{
      estadoEncoder= 1;//Para que se vaya con el primer if
      adelante(0,0);
      break;
    }
    
    
    delay(10);
  }while(1);
  
}


//Atras m cms
void atrasTantosCms(long cant){//cant en cm_2dec
  //Simplemente se pone cambian el atras() y el adelante()
  steps=0UL;
  
  do{  
    long grados= getCompass();  
    
    grados= grados- DIRECCION;
  
    if(grados > 18000L){
      grados= -( 18000L-(grados-18000L) );
    }else if(grados < -18000L){
      grados= ( 18000L-((-grados)-18000L) );
    }
    
    
    if(grados>600L) grados= 600L;
    else if(grados<-600L) grados= -600L;
    

    //if(steps>50) steps-= abs(grados)/200;
    long e= cant - (steps*10000L)/constEncoder;//Error en cm_2dec
    long avanzar;
    if(e < -50) {
      estadoEncoder= 2;
      avanzar= velAvanzar-e/constPDist;
      if(avanzar>110L) avanzar= 110L;
      
      adelante(avanzar-grados/constPCorrect, avanzar+grados/constPCorrect);
    }else if(e>50){
      estadoEncoder= 1;
      avanzar= velAvanzar+e/constPDist;
      if(avanzar>110L) avanzar= 110L;
      
      atras(avanzar+grados/constPCorrect, avanzar-grados/constPCorrect);
    }else{
      estadoEncoder= 1;//Para que se vaya con el primer if
      adelante(0,0);
      break;
    }
    
    
    delay(10);
  }while(1);
  
}


//Adelante hasta que este a la pared cant cms
void adelanteHastaPared(int separacion){//Argumento en cm, 0<=separacion<=26
  //Nota: Sharps estan metidos 4cm
  int dist;

  //Primero avanzamos hasta que ya estemos cerca 30cm de la pared
  dist= getDistance(sharpAde);
  while(dist>30){
    long grados= getCompass();  
    
    grados= grados- DIRECCION;
  
    if(grados > 18000L){
      grados= -( 18000L-(grados-18000L) );
    }else if(grados < -18000L){
      grados= ( 18000L-((-grados)-18000L) );
    }
    
    if(grados>600L) grados= 600L;
    else if(grados<-600L) grados= -600L;
     
    adelante(velAvanzar +1000/constPDist -grados/constPCorrect, velAvanzar +1000/constPDist +grados/constPCorrect);
    delay(10);

    dist= getDistance(sharpAde);
  }


  //Avanzamos revisando
  int veces=0;
  do{  
    long grados= getCompass();  
    
    grados= grados- DIRECCION;
  
    if(grados > 18000L){
      grados= -( 18000L-(grados-18000L) );
    }else if(grados < -18000L){
      grados= ( 18000L-((-grados)-18000L) );
    }
    
    
    if(grados>600L) grados= 600L;
    else if(grados<-600L) grados= -600L;
    
    dist= getDistance(sharpAde)-4;
    
    long e= separacion - dist;
    long avanzar;
    if(e < -1) {
      avanzar= velAvanzar + 180/e;
      if(avanzar<50) avanzar= 50L;
      
      adelante(avanzar, avanzar);//No se corrigue en esta parte pues.. por facilidad. TODO: adelante(avanzar-grados/constPCorrect, avanzar+grados/constPCorrect);
      veces=0;
    }else if(e>1){
      avanzar= velAvanzar - 180/e;
      if(avanzar<50) avanzar= 50L;

      atras(avanzar, avanzar);//atras(avanzar+grados/constPCorrect, avanzar-grados/constPCorrect);
      veces=0;
    }else{
      adelante(0,0);
      veces++;
      if(veces==3) break;
    }
    
    delay(10);

  }while(1);
  
}

//Atras hasta que este a la pared cant cms
void atrasHastaPared(int separacion){//Argumento en cm, 0<=separacion<=26
  //Nota: Sharps estan metidos 4cm
  int dist;

  //Primero avanzamos hasta que ya estemos cerca 30cm de la pared
  dist= getDistance(sharpAtras);
  while(dist>30){
    long grados= getCompass();  
    
    grados= grados- DIRECCION;
  
    if(grados > 18000L){
      grados= -( 18000L-(grados-18000L) );
    }else if(grados < -18000L){
      grados= ( 18000L-((-grados)-18000L) );
    }
    
    if(grados>600L) grados= 600L;
    else if(grados<-600L) grados= -600L;
     
    atras(velAvanzar +1000/constPDist +grados/constPCorrect, velAvanzar +1000/constPDist -grados/constPCorrect);
    delay(10);

    dist= getDistance(sharpAtras);
  }


  //Avanzamos revisando
  int veces=0;
  do{  
    long grados= getCompass();  
    
    grados= grados- DIRECCION;
  
    if(grados > 18000L){
      grados= -( 18000L-(grados-18000L) );
    }else if(grados < -18000L){
      grados= ( 18000L-((-grados)-18000L) );
    }
    
    
    if(grados>600L) grados= 600L;
    else if(grados<-600L) grados= -600L;
    
    dist= getDistance(sharpAtras)-4;
    
    long e= separacion - dist;
    long avanzar;
    if(e < -1) {
      avanzar= velAvanzar + 180/e;
      if(avanzar<50) avanzar= 50L;
      
      atras(avanzar, avanzar);//No se corrigue en esta parte pues.. por facilidad. TODO: adelante(avanzar-grados/constPCorrect, avanzar+grados/constPCorrect);
      veces=0;
    }else if(e>1){
      avanzar= velAvanzar - 180/e;
      if(avanzar<50) avanzar= 50L;

      adelante(avanzar, avanzar);//atras(avanzar+grados/constPCorrect, avanzar-grados/constPCorrect);
      veces=0;
    }else{
      atras(0,0);
      veces++;
      if(veces==3) break;
    }
    
    delay(10);

  }while(1);
  
}


//Girar a los g grados
void girarAObjetivo(long obj){//obj en grados_2dec
  long grados;
  int veces=0;
  
  do{  
    grados= getCompass(); 
    grados= grados -obj;
  
    if(grados > 18000L){
      grados= -( 18000L-(grados-18000L) );
    }else if(grados < -18000L){
      grados= ( 18000L-((-grados)-18000L) );
    }

    
    if(grados>600L) grados= 600L;
    else if(grados<-600L) grados= -600L;
    
    if(grados<-50){
      grados= grados= grados/constPGirar- velGirar;
      veces=0;
    }else if(grados>50){ 
      grados= grados= grados/constPGirar + velGirar; 
      veces=0;
    }else{
      grados= 0L;
      veces++;
    }
    
    
    /*if(grados>1100L) grados= 1100L;
    else if(grados<-1100L) grados= -1100L;
    
    if(grados<-50){
      grados= ((grados/100)*(grados/100))/-5 - velGirar;
      veces=0;
    }else if(grados>50){
      grados= ((grados/100)*(grados/100))/5+ velGirar;
      veces=0;
    }else{
      grados= 0L;
      veces++;
    }*/
  
    girar(grados);
    
    delay(10);
  }while(veces!=3);
  
}

//Girar g grados derecha (+)
void girarDerTantosGrados(long cant){//No negativa!
  long obj= (getCompass()+cant)%36000L;
  girarAObjetivo(obj);
}

//Girar g grados Izquierda (-)
void girarIzqTantosGrados(long cant){//No negativa!
  long obj= (getCompass()-cant);
  if(obj<0L) obj+= 36000;
  girarAObjetivo(obj);
}


/**
 * SETUP
 */
void setup() {
  Serial.begin(9600);
  delay(1000);


  //Init BNO
  if(!bno.begin(Adafruit_BNO055::OPERATION_MODE_NDOF)){
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    //while(1);
  }
  bno.setExtCrystalUse(true);

  //Init pines
  for (int i=0; i<4; i++)
  {
      pinMode(pinA[i], OUTPUT);
      pinMode(pinB[i], OUTPUT);
      pinMode(pwmpin[i], OUTPUT);
      analogWrite(pwmpin[i], 0);
  }

  pinMode(PINENCODER, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PINENCODER), pasoEncoder, CHANGE);
  
  delay(1000);
  DIRECCION= getCompass();
  estadoEncoder= 1; 
  pinMode(13, OUTPUT);
}


/**
 * LOOP
 */
void loop() {
    atrasHastaPared(10);
    while(1);// Serial.println(getCompass());
    
}


