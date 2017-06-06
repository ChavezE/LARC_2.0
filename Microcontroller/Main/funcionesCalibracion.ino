void forwardCalibration(int& velLF, int& velLB, int& velRF, int& velRB)
{
  char vel = '0';
  forward(velLF, velLB, velRF, velRB);
  if(Serial.available())
  {
    vel = Serial.read();
    if(vel == 'q')
    {
      while(Serial.available() < 1);
      velLF = Serial.readString().toInt();
      Serial.println(velLF);
    }
    else if(vel == 'p')
    {
      while(Serial.available() < 1);
      velRF = Serial.readString().toInt();
      Serial.println(velRF);
    }
    else if(vel == 'z')
    {
      while(Serial.available() < 1);
      velLB = Serial.readString().toInt();
      Serial.println(velLB);
    }
    else if(vel == 'm')
    {
      while(Serial.available() < 1);
      velRB = Serial.readString().toInt();
      Serial.println(velRB);
    }
  }
}
