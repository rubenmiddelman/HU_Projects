#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#define DOUBLE_PI 6.28318530717959
#define SAMPLERATE 44100

int main()
{
  //amplitude * sin(freq * 2PI * tijd) is the main formula to use, we need to generate this for every tick in the sine.
  //we need to offset the sine to get the right
  //also need to turn the values from ticks(-1, 1) to voltages(0.0, 3.3)

  float frequency = 440;
  double sine_Sample_Digital;
  int sine_Sample_Analog;
  float phase = 0;

  for(int i = 0; i< 44100; i++)
  {
    phase += frequency / SAMPLERATE;
    sine_Sample_Digital = sin(DOUBLE_PI * phase);
    //I do this math to go from a sine that is between  -1, 1 to 0.0, 3.2
    //this is so that we have a sine that can be output to a DAC
    printf("%f\n", sine_Sample_Digital);
  }
  return 1;
}
