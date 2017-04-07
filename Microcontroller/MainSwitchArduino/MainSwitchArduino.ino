/**
 * 
 * Ver si es bueno agregar un frenado en cada funcion.
 * 
 * Ver si sirve el metodo de veces++ en avanzarTantosCm con encoders
 */

#include <Adafruit_BNO055.h>
#include <Servo.h>


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
const byte sharpAde=A4, sharpAtras=A1, sharpDerAde=A3, sharpDerCentro, sharpDerAtras=A2, 
  sharpIzqAde=A5, sharpIzqCentro, sharpIzqAtras=A6, sharpGarra=A0;

//LimitSwitch
byte pinLimIn = 45;
byte pinLimOut = 43;

//Servos
byte pinServoGarra = 6;
byte pinServoPlat = 7;
Servo servoGarra;
Servo servoPlat;


/*
 * CONSTANTES
 */
const unsigned long constEncoder= 5500UL;//La constante se divide (steps*10000L)/4635L;
const long constPGirar= 40L;//30L;//La constante se divide
const long constPCorrect= 30L;//18L;//La const se divide
const long constPDist= 110L;//La const se divide

const long velAvanzar= 70L;//110L;
const long velGirar= 60L;//90L;


/*
 * VARIABLES
 */
Adafruit_BNO055 bno;

//Encoder
volatile unsigned long steps=0;
volatile byte estadoEncoder= 0;//0-parado, 1-adelante, 2-atras


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
  long DIRECCION= getCompass();
  
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
  long DIRECCION= getCompass();
  
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
  long DIRECCION= getCompass();

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
  long DIRECCION= getCompass();

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

void adelanteHastaNoIzq(){
  long DIRECCION= getCompass();
  
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
    
    int dist= getDistance(sharpIzqAde);//Sharp no funciona: getDistance(sharpIzqAtras);
    if(dist>30){
      adelanteTantosCms(3000);//Avanzamos porque usamos el sharp de adelante
      break;
    }
    /*
     * int dist= getDistance(sharpIzqAtras);
     * if(dist>30) break;
     */
    
    adelante(velAvanzar -grados/constPCorrect, velAvanzar+grados/constPCorrect);
      
    delay(10);

  }while(1);
  adelante(0,0); 
}

void atrasHastaNoIzq(){
  long DIRECCION= getCompass();
  
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
    
    int dist= getDistance(sharpIzqAde);
    if(dist>30) break;
    
    atras(velAvanzar+grados/constPCorrect, velAvanzar-grados/constPCorrect);
      
    delay(10);

  }while(1);
  atras(0,0);
  
}

void adelanteHastaNoDer(){
  long DIRECCION= getCompass();
  
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
    
    int dist= getDistance(sharpDerAtras);
    if(dist>30) break;
    
    adelante(velAvanzar -grados/constPCorrect, velAvanzar+grados/constPCorrect);
      
    delay(10);

  }while(1);
  adelante(0,0);
  
}

void atrasHastaNoDer(){
  long DIRECCION= getCompass();
  
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
    
    int dist= getDistance(sharpDerAde);
    if(dist>30) break;
    
    atras(velAvanzar+grados/constPCorrect, velAvanzar-grados/constPCorrect);
      
    delay(10);

  }while(1);
  atras(0,0);
  
}



char agarrarVasoDer(){
  atrasTantosCms(6500);

  long DIRECCION= getCompass();

  //Avanzamos hasta pared buscando vasos
  int separacion= 1;
  int veces=0;
  bool agarramos= false;
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
    
    int distAAtras= getDistance(sharpAtras)-4;
    
    long e= separacion - distAAtras;
    long avanzar;
    if(e < -2) {
      avanzar= velAvanzar; //+ 150/e;
      if(avanzar<20) avanzar= 20L;
      
      atras(avanzar, avanzar);//No se corrigue en esta parte pues.. por facilidad. TODO: adelante(avanzar-grados/constPCorrect, avanzar+grados/constPCorrect);
      veces=0;
    }else{
      atras(0,0);
      veces++;
      if(veces==3) break;
    }

    int distAVaso= getDistance(sharpGarra);
    if(distAVaso<30){
      atras(0,0);
      
      //Agarramos el vaso
      servoGarra.write(30);//Abrimos garra

      servoPlat.write(80);//Empezamos a mover la plataforma
      do{
        distAVaso= getDistance(sharpGarra);
      }while(distAVaso>5&&digitalRead(pinLimOut)!=1);
      servoPlat.write(90);//Paramos plataforma

      bool topamos= digitalRead(pinLimOut);
      if(topamos){//Esta muy lejos el vaso, no alcazamos
        servoPlat.write(95);//Regresamos la plataforma
        while(digitalRead(pinLimIn)!=1);
        servoPlat.write(90);//Paramos la plataforma

        servoGarra.write(60);//cerramos garra
        if(distAAtras>5){//No alcazamos... pues intentaremos avanzar un poco a ver que tal
          atrasTantosCms(700);
        }else break;
      }else{
        servoPlat.write(80);//Movemos la plataforma unos ultimos seg de gracia
        unsigned long fin= millis()+500;
        while(millis()<fin&&digitalRead(pinLimOut)!=1);
        servoPlat.write(90);//Paramos la plataforma
        topamos= digitalRead(pinLimOut);
        
        servoGarra.write(60);//Cerramos garra

        servoPlat.write(95);//Regresamos la plataforma
        while(digitalRead(pinLimIn)!=1);
        servoPlat.write(90);//Paramos la plataforma

        if(!topamos){
          agarramos= true;
          break;
        }else if(distAAtras>5){//No alcazamos... pues intentaremos avanzar un poco a ver que tal
          atrasTantosCms(700);
        }
           
      }

      if(agarramos) break;
      
    }
    
    delay(10);

  }while(1);


  //Avisamos si agarramos o no el vaso
  return agarramos? '1':'0';
}


char agarrarVasoIzq(){
  adelanteTantosCms(6500);

  long DIRECCION= getCompass();

  //Avanzamos hasta pared buscando vasos
  int separacion= 1;
  int veces=0;
  bool agarramos= false;
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
    
    int distAAdela= getDistance(sharpAde)-4;
    
    long e= separacion - distAAdela;
    long avanzar;
    if(e < -2) {
      avanzar= velAvanzar; //+ 150/e;
      if(avanzar<30) avanzar= 30L;
      
      adelante(avanzar, avanzar);//No se corrigue en esta parte pues.. por facilidad. TODO: adelante(avanzar-grados/constPCorrect, avanzar+grados/constPCorrect);
      veces=0;
    }else{
      adelante(0,0);
      veces++;
      if(veces==3) break;
    }

    int distAVaso= getDistance(sharpGarra);
    if(distAVaso<30){
      adelante(0,0);
      
      //Agarramos el vaso
      servoGarra.write(30);//Abrimos garra

      servoPlat.write(80);//Empezamos a mover la plataforma
      do{
        distAVaso= getDistance(sharpGarra);
      }while(distAVaso>5&&digitalRead(pinLimOut)!=1);
      servoPlat.write(90);//Paramos plataforma
      
      bool topamos= digitalRead(pinLimOut);
      if(topamos){//Esta muy lejos el vaso, no alcazamos
        servoPlat.write(95);//Regresamos la plataforma
        while(digitalRead(pinLimIn)!=1);
        servoPlat.write(90);//Paramos la plataforma

        servoGarra.write(60);//cerramos garra

        if(distAAdela>5){//No alcazamos... pues intentaremos avanzar un poco a ver que tal
          adelanteTantosCms(600);
        }else break;
        
      }else{
        servoPlat.write(80);//Movemos la plataforma unos ultimos seg de gracia
        unsigned long fin= millis()+700;
        while(millis()<fin&&digitalRead(pinLimOut)!=1);
        servoPlat.write(90);//Paramos la plataforma
        topamos= digitalRead(pinLimOut);
        
        servoGarra.write(60);//Cerramos garra

        servoPlat.write(95);//Regresamos la plataforma
        while(digitalRead(pinLimIn)!=1);
        servoPlat.write(90);//Paramos la plataforma

        if(!topamos){
          agarramos= true;
          break;
        }else if(distAAdela>5){//No alcazamos... pues intentaremos avanzar un poco a ver que tal
          adelanteTantosCms(600);
        }else break;
           
      }

      if(agarramos) break;
      
    }
    
    delay(10);

  }while(1);


  //Avisamos si agarramos o no el vaso
  return agarramos? '1':'0';
}

//El de nestor de solo estirar la mano y listo
void agarrarVaso()
{
  servoPlat.write(90);
  servoGarra.write(30);
  servoPlat.write(80);
  int dist;
  while (true)
  {
    dist = getDistance(sharpGarra);
    if (digitalRead(pinLimOut) == 1 || dist < 5)
    {
      break;
    }
  }
  servoPlat.write(90);
  if (dist < 5 && digitalRead(pinLimOut) == 0)
  {
    Serial.println("Entra delay");
    unsigned long tim1 = millis();
    unsigned long tim2 = millis();
    servoPlat.write(80);
    while (true)
    {
      tim2 = millis();
      if ((tim2 - tim1) > 800 || digitalRead(pinLimOut) == 1)
      {
        break;
      }
    }
    servoPlat.write(90);
  }
  servoGarra.write(60);
  delay(50);
  servoPlat.write(95);
  while(digitalRead(pinLimIn)==0);
  servoPlat.write(90);
}


void entrarVaca(){
  
}


/**
 * SETUP
 */
void setup() {
  delay(5000);
  Serial.begin(9600);


  //Init BNO
  if(!bno.begin(Adafruit_BNO055::OPERATION_MODE_NDOF)){
    /* There was a problem detecting the BNO055 ... check your connections */
    //Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
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
  servoGarra.attach(pinServoGarra);
  servoPlat.attach(pinServoPlat);
  pinMode(pinLimIn, INPUT);
  pinMode(pinLimOut, INPUT);
  

  pinMode(PINENCODER, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PINENCODER), pasoEncoder, CHANGE);
  
  servoPlat.write(95);//Movemos la plataforma hasta adentro
  while(digitalRead(pinLimIn)==0);
  servoPlat.write(90);//Detenemos la plataforma
  servoGarra.write(30);
  estadoEncoder= 1; 
}



/**
 * LOOP
 */
void loop() {
    unsigned long dato;
    unsigned long dato1, dato2;
    if(Serial.available()>0){
      char orden= Serial.read();
      
      switch(orden){
        case 'a':
          while(Serial.available()<1);
          dato= Serial.read();
          delay(1000);
          adelanteTantosCms(dato*100UL);
          Serial.write('1');
          break;

        case 'b':
          while(Serial.available()<1);
          dato= Serial.read();
          delay(1000);
          atrasTantosCms(dato*100UL);
          Serial.write('1');
          break;

        case 'u':
          while(Serial.available()<1);
          dato= Serial.read();
          delay(1000);
          atrasHastaPared(dato);
          Serial.write('1');
          break;

        case 'w':
          while(Serial.available()<1);
          dato= Serial.read();
          delay(1000);
          adelanteHastaPared(dato);
          Serial.write('1');
          break;

        case 'r':
          while(Serial.available()<1);
          dato1= Serial.read();
          delay(1000);
          while(Serial.available()<1);
          dato2= Serial.read();
          delay(1000);
          girarDerTantosGrados(((dato1<<8)|dato2)*100UL);
          Serial.write('1');
          break;

        case 'i':
          while(Serial.available()<1);
          dato1= Serial.read();
          delay(1000);
          while(Serial.available()<1);
          dato2= Serial.read();
          delay(1000);
          girarIzqTantosGrados(((dato1<<8)|dato2)*100UL);
          Serial.write('1');
          break;  

        case 'g':
          while(Serial.available()<1);
          dato1= Serial.read();
          delay(1000);
          while(Serial.available()<1);
          dato2= Serial.read();
          delay(1000);
          girarAObjetivo(((dato1<<8)|dato2)*100UL);
          Serial.write('1');
          break;    

        case 'v':
          Serial.write(agarrarVasoDer());
          break;

        case 'k':
          Serial.write(agarrarVasoIzq());
          break;

        case 'z':
          atrasHastaNoDer();
          Serial.write('1');
          break;

        case 'x':
          adelanteHastaNoDer();
          Serial.write('1');
          break;

        case 'y':
          atrasHastaNoIzq();
          Serial.write('1');
          break;

        case 'h':
          adelanteHastaNoIzq();
          Serial.write('1');
          break;

        case '-':
          agarrarVaso();
          Serial.write('1');
          break;
        
      }

      
    }
    
}


