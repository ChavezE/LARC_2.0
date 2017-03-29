
void setup() {
  delay(5000);
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(A0, OUTPUT);


  //Enviar
  //Serial.write(96u);//Envia byte de preferencia agregando u
  //digitalWrite(13, 1);
  //while(1);
}

void loop() {
  if(Serial.available()>0){
    /*
     * Con char si hasta 255 y se comparan con literales char '\x__'
     * Con unsigned char si hasta 255 y se comparan con literales unsigned (_u)
     */
    
    char c= Serial.read();
    if(c=='\xff'){
      digitalWrite(13, 1);
      delay(1000);
      digitalWrite(13, 0);
    }else{
      digitalWrite(A0, 1);
      delay(1000);
      digitalWrite(A0, 0);
    }
  }

}
