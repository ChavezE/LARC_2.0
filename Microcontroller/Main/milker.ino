

void milkerUp()
{
  digitalWrite(pinMotA, 1);
  digitalWrite(pinMotB, 0);
}

void milkerDown()
{
  digitalWrite(pinMotA, 0);
  digitalWrite(pinMotB, 1);
}

void milkerStop()
{
  digitalWrite(pinMotA, 0);
  digitalWrite(pinMotB, 0);
}

void extract()
{
  digitalWrite(pinExtractorA, 1);
  digitalWrite(pinExtractorB, 0);
}

void stopExtract()
{
  digitalWrite(pinExtractorA, 0);
  digitalWrite(pinExtractorB, 0);
}

void moveMilkerDown()
{
  milkerDown();
  while(digitalRead(pinLMD) == normalState);
  milkerStop();
}

void moveMilkerUp()
{
  milkerUp();
  delay(15000);
}

void milk()
{
  moveMilkerUp();
  delay(1000);
  //Put milker functions here
  moveMilkerDown();
}
