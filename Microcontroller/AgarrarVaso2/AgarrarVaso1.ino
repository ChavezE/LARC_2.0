#include <Servo.h>

Servo servoGarra;
Servo servoPlat;
byte sharpAtras = 4;
byte sharpGarra = 5;
byte pinServoGarra = 9;
byte pinServoPlat = 8;
byte pinLimIn = 43;
byte pinLimOut = 45;
int DIRECCION = 1;
void atrasTantosCms(byte iI)
{
  return 0;
}

long getCompass()
{
  return 20;
}

void agarrarVasoDer(){
  atrasTantosCms(6000);

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
    int dist = 0;
    dist= getDistance(sharpAtras)-4;
    
    long e= separacion - dist;
    long avanzar;
    if(e < -2) {
      avanzar= velAvanzar + 100/e;
      if(avanzar<20) avanzar= 20L;
      
      atras(avanzar, avanzar);//No se corrigue en esta parte pues.. por facilidad. TODO: adelante(avanzar-grados/constPCorrect, avanzar+grados/constPCorrect);
      veces=0;
    }else if(e>2){
      avanzar= velAvanzar - 100/e;
      if(avanzar<20) avanzar= 20L;

      adelante(avanzar, avanzar);//atras(avanzar+grados/constPCorrect, avanzar-grados/constPCorrect);
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
      servoGarra.write(40);//Abrimos garra

      servoPlata.write(95);//Empezamos a mover la plataforma
      do{
        dist= getDistance(sharpGarra);
      }while(dist>5&&digitalRead(limitGarraAde)!=1);
      servoPlata.write();//Paramos plataforma
      
      if(digitalWrite(limitGarraAde)==1){//Esta muy lejos el vaso, no alcazamos
        servoPlata.write();//Regresamos la plataforma
        while(digitalRead(limitGarraAtras)!=1);
        servoPlata.write();//Paramos la plataforma

        servoGarra.write();//cerramos garra
      }else{
        servoPlata.write();//Movemos la plataforma unos ultimos seg de gracia
        unsigned long fin= millis()+500;
        while(millis()<inicio&&digitalRead(limitGarraAde)!=1);
        servoPlata.write();//Paramos la plataforma
        
        servoGarra.write();//Cerramos garra

        servoPlata.write();//Regresamos la plataforma
        while(digitalRead(limitGarraAtras)!=1);
        servoPlata.write();//Paramos la plataforma

        if(digitalRead(limitGarraAde)!=1) agarramos= true;//No alcazamos
        else break;
        
      }
      
    }
    
    delay(10);

  }while(1);


  //Avisamos si agarramos o no el vaso
  if(agarramos) Serial.write("1");
  else Serial.write("0");
}


void setup() {
  Serial.begin(9600);
  servoGarra.attach(pinServoGarra);
  servoPlat.attach(pinServoPlat);

}

void loop() {
  servoGarra.write(40);

}
