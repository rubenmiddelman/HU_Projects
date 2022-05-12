/////////////////////////////
//  code for excercise 6.  //
// code can fill a struct  //
//  and looped through it. //
//        writen by        //
//     ruben middelman     //
//      Used by the        //
//     HU University of    //
//     Applied Sciences    //
/////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define ARGUMENT_PRINT

struct Record
{
  int number;
  char name[255];
};

typedef struct Record RECORD;
typedef RECORD* P_RECORD;

void Display_RECORD_Array(RECORD *ps)
{
    for (; ps->name[0]; ps++)
  	    printf ("naam=%s,\t nr=%d\n", ps->name, ps->number);
}

//no other functions because program is able to be done in under 30 lines
int main(int argc, char *argv[])
{
  //this should be a ? but if you are entering more arguments you need a tutorial on how to use
  #ifdef ARGUMENT_PRINT
    int j;
    printf("DEBUG OUTPUT FOR THE ARGUMENTS GIVEN WITH THE FUNCTION\n");
    printf("amount of arguments %d\n", argc);
    for(j = 0; j < argc; j++)
    {
      printf("%s\n", argv[j]);
    }
  #endif
  if(argc == 2)
  {
    puts("========================="
    "\n record V1.0 [author : Ruben Middelman]"
  	"\n input van record is [nr, naam]");
  }
  printf("geef het gewenste aantal records [nr, naam] aan:");
  int i;

  char amount_Of_Records[10];
  //get the amount of records and turn it into an into
  //also gets() gives a huge warning when compiled
  //need to ask why it needs to be used there
  gets(amount_Of_Records);
  int amount_Of_Records_As_Int = atoi(amount_Of_Records);

  P_RECORD p_Array_Of_Records = malloc(sizeof(RECORD)*amount_Of_Records_As_Int);
  P_RECORD *top = p_Array_Of_Records;

  //for loop fills the array one by one
  for(i = 1; i <= amount_Of_Records_As_Int; i++, p_Array_Of_Records++)
  {
    //TODO: i feel like this should be a function but when it is only 10 lines it's kind of stupid
    //so need to ask if it is okay to put this in the main because it otherwise will just take up more memory
    printf("vulle van record %d van de %d\n", i, amount_Of_Records_As_Int);
    char nr_In_Record[255];
    //char name_In_Record[255];
    printf("nr:     ");
    gets(nr_In_Record);
    printf("naam:   ");
    gets(p_Array_Of_Records->name);
    int int_In_Record = atoi(nr_In_Record);
    p_Array_Of_Records->number = int_In_Record;
    //strcpy(p_Array_Of_Records[i].name, name_In_Record);
  }
  for(i = 1; i <= amount_Of_Records_As_Int; i++, top++)
    printf("[%d] nr:  %d, name:  %s\n", i, top->number, top->name);
  free(p_Array_Of_Records);
}
