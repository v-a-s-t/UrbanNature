const int sampleWindow = 50;
unsigned int sample;

void sampleMicPP()
{
  unsigned long startMillis = millis();
  unsigned int peakToPeak = 0;
  unsigned int signalMax = 0;
  unsigned int signalMin = 4096;
  int sampleLength = 300; //30 seconds
  double averageSample = 0;
  
  for (int i = 0; i < sampleLength; i++) {
    peakToPeak = 0;
    signalMax = 0;
    signalMin = 4096;
    startMillis = millis();
    // collect data for 50 mS
    while (millis() - startMillis < sampleWindow)
    {
      sample = analogRead(MIC_SENSE);
      if (sample < 4096)  // toss out spurious readings
      {
        if (sample > signalMax)
        {
          signalMax = sample;  // save just the max levels
        }
        else if (sample < signalMin)
        {
          signalMin = sample;  // save just the min levels
        }
      }
    }
    peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
    double volts = peakToPeak;  // convert to volts
    delay(100);
    if(volts > averageSample){
      averageSample = volts;
    }
  }
  micPPSample = averageSample;
}
