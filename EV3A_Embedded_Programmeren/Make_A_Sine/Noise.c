#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void main()
{
  float analog_Sample, random_Number;
  srand(time(NULL));
  while(1)
  {
    random_Number = rand()%4096;
    analog_Sample=random_Number;
    printf("%f\n", analog_Sample);
  }
}
