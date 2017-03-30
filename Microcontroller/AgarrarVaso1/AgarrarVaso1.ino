

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
      servoGarra.write();//Abrimos garra

      servoPlata.write();//Empezamos a mover la plataforma
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
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
