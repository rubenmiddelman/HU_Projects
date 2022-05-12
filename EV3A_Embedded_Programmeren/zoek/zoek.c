/////////////////////////////
//  code for excercise 4.  //
//   code can search for   //
//     a word in a file.   //
//        writen by        //
//     ruben middelman     //
/////////////////////////////



#include <stdio.h>
#include <string.h>

//#define USER_INPUT_TEST
//#define PRINT_EVERYTHING_FROM_FILE

//main does all the work
//first it makes all the variables
//then it opens a file
//then it checks in a while loop
//to see if the word is found in a sentence
int main(int argc, char *argv[])
{
  FILE *P_File; // pointer that is used to open the file

  //tells people how to use the program
  if(argc != 3){
    puts(
  	"=========================\n"\
    "\n Zoek V1.0 [author : Ruben Middelman]"
  	"\n Usage: ./zoek <word to be searched> <filepath>\n"
  	"\n e.g. ./zoek pointer zoek.c");
    return 0;
  }
  //makes al the needed variable
  int row_Number;
  int column_Number;

  char str_Read_From_File[150];
  char *P_Str_Found_By_Strstr;

  //makes a string that is the size of the arguments given as argument to the program
  size_t size_Of_Filename_Str = strlen(argv[2]);
  char filename_Str[size_Of_Filename_Str];
  size_t size_Of_Word_Str = strlen(argv[1]);
  char word_To_Be_Searched_Str[size_Of_Filename_Str];

  //copies the string from the argv to a usable string
  memccpy(filename_Str, argv[2], '\0', size_Of_Filename_Str);
  memccpy(word_To_Be_Searched_Str, argv[1], '\0', size_Of_Word_Str);

  #ifdef USER_INPUT_TEST
    printf("DEBUG MODE\n");
    printf("the file that will be searched = %s\n", filename_Str);
    printf("the word that will be searched for = %s\n", word_To_Be_Searched_Str);
  #endif

  P_File = fopen(filename_Str, "r"); //opens the file with te given filename of the program

  //checks the file to see if the file opend well
  if(P_File == NULL)
  {
    printf("%s\n", "file doesn't exist");
    return 0;
  }

  //this is the main while loop that does all the work
  //first it opens every line in the file by itself
  //then it checks for every line if there is a matching word with strstr
  //then it checks if the whole line where the words overlap, it does this with 2 for loops
  //last it prints the line if all the characters match
  while(1)
  {
    if(fgets(str_Read_From_File, 150, P_File) == NULL)
      break;
    else
    {
      #ifdef PRINT_EVERYTHING_FROM_FILE
        printf("DEBUG MODE\n");
        printf("%s\n", str_Read_From_File);
      #endif
      row_Number =  row_Number + 1;
      P_Str_Found_By_Strstr = strstr(str_Read_From_File, word_To_Be_Searched_Str);
      if(P_Str_Found_By_Strstr)
      {
        int i;
        int j;
        for(i = 0; i < 150; i++) //loops between the max amount of characters from the string from the file
        {
          int letters_Correct_Counter = 0;
          for(j = 0; j < size_Of_Word_Str; j++)// loops for all the letters of the main word
            if(word_To_Be_Searched_Str[j] == str_Read_From_File[i+j])
              letters_Correct_Counter = letters_Correct_Counter + 1;
          if(letters_Correct_Counter == size_Of_Word_Str)
            //if this is TRUE te word is found on this position of i
            printf("[%d] [%d]  %s\n", row_Number, i, str_Read_From_File);
        }
      }
    }
  }
  fclose(P_File); //closes the file
  return 1;
}
