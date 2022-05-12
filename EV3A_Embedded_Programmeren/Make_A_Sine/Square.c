#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

int main()
{
  #define DOUBLE_PI 6.28318530717959
  float frequency = 44000;
  float samplerate = 44800;
  double square_Sample_Digital;
  double square_Sample_Analog;

  float phase = 0;

  for(int i = 0; i< 100; i++)
  {
    //make a sinewave the same way we do for a sine
    phase += frequency / samplerate;
    square_Sample_Digital = sin(DOUBLE_PI * phase);
    //then check if the sample is above or below 0 to set it to minimum or maximum
    //if we add amp this is the point to do it
    if(square_Sample_Digital < 0)
    {
      square_Sample_Analog = 0.0;
    }else
    {
      square_Sample_Analog = 4000;
    }
    printf("%f\n", square_Sample_Analog);
  }
  return 1;
}
