/* ========================================================================== */
/*                                                                            */
/*   calcul.c                                                                 */
/*   (c) 2009 Michiel                                                         */
/*                                                                            */
/*   Usage of argc and argv[][]                                               */
/*   (argument counter and argument vector)                                   */
/*   note: run this program from the command prompt (cmd.exe)                 */
/* ========================================================================== */
#include <stdio.h>  // printf, puts
#include <stdlib.h> // atof

//#define DEBUG

float To_The_Power_Of(float, float);

/*****************************************************************************
 main: simple command line calculator
 arguments (inputs):
    int argc:       number of arguments entered by user, given by OS
    array of char*: pointer to every argument entered, given by OS
 return value (output):
    int - 0 if failure, 1 if successful
*****************************************************************************/
int main (int argc, char *argv[])
{
	char usage[] =
	"\nCalcul v2.0 [author: msc]\n"\
	"=========================\n"\
	"Usage: calcul number operator number\n"\
	"operators: + - x / (add, subtract, multiply, divide)\n"\
	"e.g. calcul 12.3 / 45.6";

	if (argc != 4) // er moeten 4 argumenten (inc. programma) ingegeven zijn.
	{
		puts(usage); // geef aan hoe het werkt
		return 0;
	}

	#ifdef DEBUG // debug output if wanted the argc and argv-strings
		printf("debug-output: \n\t argc = %d\n", argc);
		int i;
		for (i=0; i<argc; i++)
			printf("\t %d = %s\n", i, argv[i]);
	#endif

	// the number of inputs is correct, now we continue by converting
	// the inputs from chars to values (f.i. string '1','0','\0' to value 10)
	// note: argv[0] contains 'calcul' the name of the application

	float one = atof(argv[1]);    // convert first value-string to float
	char operator  = argv[2][0];  // set the operator (+, -, /, x)
	float two = atof(argv[3]);    // convert second value-string to float
	float result;                 // will contain the result value

	switch(operator)              // the operator determines the 'result'
	{
	    case '+': result = one + two; break;
	    case '-': result = one - two; break;
	    case 'x': result = one * two; break;
	    case '/': result = one / two; break;
			case 'm': result = To_The_Power_Of(one, two); break;
	    default : puts("invalid operand!"); return 0; // oops
	}

	// done, now generate output and quit...
	printf("result: \n\t %f %c %f = %f\n", one, operator, two, result);

	return 1;
}

// function takes two floats and does does the first float to the power of
// the second float
float To_The_Power_Of(float base_Float, float power_Float)
{
  float result = base_Float;
  int i;
  if(power_Float < 0)
  {
    for(i = 0; i > power_Float+1; i--)
      result = result * base_Float;
    result = 1 / result;
  }
  else if(power_Float > 0)
    for(i = 0; i < power_Float-1; i++)
      result = result * base_Float;
  else if(power_Float == 0)
    result = 1;
  return result;
};
