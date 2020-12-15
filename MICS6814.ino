double mics6814_readOx(){
  double oxIn = analogRead(OX_SENSE);
  oxIn = 56000/ ((65535 /oxIn) - 1);
  return oxIn;
}

double mics6814_readRed(){
  double redIn = analogRead(RED_SENSE);
  redIn = 56000/ ((65535 /redIn) - 1);
  return redIn;
}

double mics6814_readNH3(){
  double nh3In = analogRead(NH3_SENSE);
  nh3In = 56000/ ((65535 /nh3In) - 1);
  return nh3In;
}
