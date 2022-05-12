/////////////////////////////////////
//     Comminucation Module for    //
//      Project 4D for prof 4D     //
//         code writen by          //
//         Ruben Middelman         //
//           Used by the           //
//HU University of Applied Sciences//
/////////////////////////////////////

#include <Arduino.h>

//all the function prototypes
//normally not needed in arduino ide but ruben uses packetIO so is needed now
int LRC_Checker();

int Send_FC01(byte destination_ID);
int Send_FC05(byte destination_ID);
int Send_FC06(byte destination_ID);
int Send_FC11(byte destination_ID);

int Make_Message_To_Be_Sent(int destination_ID_Module_Number, int amount_Of_Bytes_To_Be_Transmitted, int function_Code_To_Be_Sent, String &message_That_Is_Fully_Assembled);

int Make_Start_Comminuction_Byte(String &start_Comminucation_Byte);
int Make_Packet_Lenght_Byte(int amount_Of_Bytes_To_Be_Transmitted, String &packet_Length_Byte);
int Make_Function_Code_Byte(int function_Code_Number_To_Be_Sent, String &function_Code_Byte);
int Make_Source_ID_Byte(String &source_ID_Byte);
int Make_Destination_ID_Byte(int what_Module_To_Send_To, String &destination_ID_String);
int Get_Data_Bytes();
int Make_End_Comminucation_Byte(String &end_Comminucation_Byte);
int Make_LRC_Byte(String &LRC_Comminucation_Byte, String start_Comminucation_Byte, String packet_Length_Byte, String function_Code_Byte, String source_ID_Byte, String destination_ID_Byte, String end_Comminucation_Byte);

int Hex_To_Decimal(char hex_Number_As_An_Array_To_Convert[]);
void Decimal_To_Hex(int decimal_Number_To_Be_Converted_To_Hex, String &hex_Number_Array_As_String);

void Read_Incomming_Message(String &message_That_Was_Sent);
void Disect_Message(String message_That_Was_Recieved);
void Get_Data_Bytes(String message_That_Was_Recieved);

void Figure_Out_Responce();

void Arduino_Reset_Function_With_Input();

#define start_Comminucation 1
#define destination_ID_As_Define 25
#define end_Comminucation 4

#define LED_Red 7
#define LED_Yellow 8
#define LED_Green 12

//struct holds all info from recieved message so that we can seperate it where needed
struct Disected_Message{
  byte Packet_Lenght;
  byte Function_Code;
  byte Source_ID;
  byte Start_Comminucation = start_Comminucation;
  byte Destination_ID = destination_ID_As_Define;
  byte End_Comminucation = end_Comminucation;
  byte LRC_For_Data;
  byte LRC;
  byte Was_LRC_Correct;
  char Data_Bytes[100];
};

//quite nice that these are the only global variables in the whole code.

//make a string that can be edited so i don't have to give a lot of functions an extra pointer
String message_To_Be_Sent_Data;
String last_Sent_Message;

//make 2 structs so that the data from the message with all the important data can be saved for later
Disected_Message Message_As_Struct;
//it is slightly a shit way to save the extra 37 bytes this does take up extra storage but thats oke because we use so little
Disected_Message Message_As_Struct_With_37_Bytes_Of_Data;

int has_Data_Been_Recieved_Yet = 0;


void setup() {
  Serial.begin(9600);
  Serial.println("Welcome to the program");
  Serial.println("to continue copy in youre message");
  pinMode(LED_Red, OUTPUT);
  pinMode(LED_Yellow, OUTPUT);
  pinMode(LED_Green, OUTPUT);
}

void loop(){
  //turn RED LED on because program is running
  digitalWrite(LED_Red, HIGH);
  String incomming_Message;
  Read_Incomming_Message(incomming_Message);

}


//functions that send individual bytes with function codes
int Send_FC01(byte destination_ID){
  //this is the ask for retransmit function
  String FC01_Message;
  message_To_Be_Sent_Data ="";
  Make_Message_To_Be_Sent(destination_ID, 4, 1, FC01_Message);
  Serial.println(FC01_Message);
  //last sent message is used for if retransmit is recieved so that we can send back
  //the last sent message
  last_Sent_Message = FC01_Message;
  return 1;
}
int Send_FC05(byte destination_ID){
  //this function code is used for comminucation with the MM
  String FC05_Message;
  message_To_Be_Sent_Data ="";
  Make_Message_To_Be_Sent(destination_ID, 4, 5, FC05_Message);
  Serial.println(FC05_Message);
  last_Sent_Message = FC05_Message;
  return 5;
}
int Send_FC06(byte destination_ID){
   //this function code is used for comminucation with the CO
  String FC06_Message;
  message_To_Be_Sent_Data ="";
  Make_Message_To_Be_Sent(destination_ID, 4, 6, FC06_Message);
  Serial.println(FC06_Message);
  last_Sent_Message = FC06_Message;
  return 6;
}
int Send_FC11(byte destination_ID){
  //send to the NM so that we can continue to the next module
  String FC11_Message;
  message_To_Be_Sent_Data ="";
  Make_Message_To_Be_Sent(destination_ID, 4, 11, FC11_Message);
  Serial.println(FC11_Message);
  last_Sent_Message = FC11_Message;
  return 11;
}

//lets also make a function that pre programs the acknoledge so we don't allways have to make the message
void Make_Acknoledge(int destination_ID, String &acknoledge_Message_To_Be_Sent){
    message_To_Be_Sent_Data = "06";
    Make_Message_To_Be_Sent(destination_ID, 5, 12, acknoledge_Message_To_Be_Sent);
  
}

//this function can reset the arduino just via code
//which is nice so you don't have to hot wire it
void(* Arduino_Reset_Function) (void) = 0;

int Make_Message_To_Be_Sent(int destination_ID_Module_Number, int amount_Of_Bytes_To_Be_Transmitted, int function_Code_To_Be_Sent, String &message_That_Is_Fully_Assembled){
  //This function makes the full message that is to be sent to a different module
  //will probably consist of different functions that will be added togheter.

  //function is tested and works. still an error in the LRC that is very weird but the rest works fine
  String comminucation_Byte;
  String source_ID_Byte;
  String destination_ID_Byte;
  String function_Code_Byte;
  String packet_Length_Byte;
  String data_Bytes;
  String end_Comminucation_Byte;
  String LRC_Byte;
  Make_Start_Comminuction_Byte(comminucation_Byte);
  Make_Packet_Lenght_Byte(amount_Of_Bytes_To_Be_Transmitted, packet_Length_Byte);
  Make_Source_ID_Byte(source_ID_Byte);
  Make_Function_Code_Byte(function_Code_To_Be_Sent, function_Code_Byte);
  Make_Destination_ID_Byte(destination_ID_Module_Number, destination_ID_Byte);
  data_Bytes = message_To_Be_Sent_Data;
  Make_End_Comminucation_Byte(end_Comminucation_Byte);
  Make_LRC_Byte(LRC_Byte, comminucation_Byte, source_ID_Byte, destination_ID_Byte, function_Code_Byte, packet_Length_Byte, end_Comminucation_Byte, data_Bytes);
  message_That_Is_Fully_Assembled = comminucation_Byte + " " + packet_Length_Byte + " " + function_Code_Byte + " " + source_ID_Byte + " " + destination_ID_Byte + " " + data_Bytes + " " + end_Comminucation_Byte + " " + LRC_Byte;
  return 1;
}
//all the functions that are used individualy to make every part of the bit
int Make_Start_Comminuction_Byte(String &start_Comminucation_Byte){
  //function works and is tested
  //also doesn't need to be here but lets keep it for consistancy
  start_Comminucation_Byte= "01";
  return 1;
}
int Make_Packet_Lenght_Byte(int amount_Of_Bytes_To_Be_Transmitted, String &packet_Length_Byte){
  //function works and is tested
  Decimal_To_Hex(amount_Of_Bytes_To_Be_Transmitted, packet_Length_Byte);
  return 1;
}
int Make_Function_Code_Byte(int function_Code_Number_To_Be_Sent, String &function_Code_Byte){
  //function works and is tested
  switch(function_Code_Number_To_Be_Sent){
    case 0x01:
      function_Code_Byte = "C3";
      break;
    case 0x05:
      function_Code_Byte = "D3";
      break;
    case 0x06:
      function_Code_Byte = "D7";
      break;
    case 0x0B:
      function_Code_Byte = "EB";
      break;
    case 0x07:
      function_Code_Byte = "D4";
      break;
    case 0x0C:
      function_Code_Byte = "EC";
      break;
    case 0x0E:
      function_Code_Byte = "DC";
      break;
  }
  return 1;
}
int Make_Source_ID_Byte(String &source_ID_Byte){
  //function works and is tested
  //the source ID is pre defined for this module but we still use a function to
  //keep consistancy
  source_ID_Byte = "19";
  return 1;
}
int Make_Destination_ID_Byte(int what_Module_To_Send_To, String &destination_ID_String){
  //function works and is tested
  //this produces a string from 8 characters that is the binary value of what we will send to
  Decimal_To_Hex(what_Module_To_Send_To, destination_ID_String);

  return 1;
}
int Make_End_Comminucation_Byte(String &end_Comminucation_Byte){
  //function works and is tested
  end_Comminucation_Byte = "04";
  return 1;
}
int Make_LRC_Byte(String &LRC_Comminucation_Byte, String start_Comminucation_Byte, String packet_Length_Byte, String function_Code_Byte, String source_ID_Byte, String destination_ID_Byte, String end_Comminucation_Byte, String data_Bytes){
  //function is tested but there seems to be a weird bug that needs to be fixed, everything else works.
  //mabye check arduino forums

  //weird bug has been resolved but nod in a great way... made the decimal to hex function les optimal
  //so have to find out what causes it?
  int length_Of_String;
  int start_Comminucation_Int;
  int packet_Length_Int;
  int function_Code_Int;
  int source_ID_Int;
  int destination_ID_Int;
  int end_Comminucation_Int;
  int data_Int;
  int LRC_As_Int;

  length_Of_String = start_Comminucation_Byte.length() + 1;
  char start_Comminucation_Array[length_Of_String];
  start_Comminucation_Byte.toCharArray(start_Comminucation_Array, length_Of_String);
  start_Comminucation_Int = Hex_To_Decimal(start_Comminucation_Array);

  length_Of_String = packet_Length_Byte.length() + 1;
  char packet_Lenght_Array[length_Of_String];
  packet_Length_Byte.toCharArray(packet_Lenght_Array, length_Of_String);
  packet_Length_Int = Hex_To_Decimal(packet_Lenght_Array);

  length_Of_String = source_ID_Byte.length() + 1;
  char source_ID_Array[length_Of_String];
  source_ID_Byte.toCharArray(source_ID_Array, length_Of_String);
  source_ID_Int = Hex_To_Decimal(source_ID_Array);
  
  length_Of_String = data_Bytes.length() + 1;
  char data_Bytes_Array[length_Of_String];
  data_Bytes.toCharArray(data_Bytes_Array, length_Of_String);
  int data_Int_Togheter = 0;
  for(int i = 0; i < length_Of_String; i++){
    char data_Bytes_2_Spots[2];
    data_Bytes_2_Spots[0] =data_Bytes_Array[i];
    data_Bytes_2_Spots[1] = data_Bytes_Array[i+1];
    i++;
    i++;
    data_Int = Hex_To_Decimal(data_Bytes_2_Spots);
    if(i > 3){
      data_Int_Togheter = data_Int ^ data_Int_Togheter;
    }else if(i < 3){
      data_Int_Togheter = data_Int;
    }
  }

  length_Of_String = destination_ID_Byte.length() + 1;
  char destination_ID_Array[length_Of_String];
  destination_ID_Byte.toCharArray(destination_ID_Array, length_Of_String);
  destination_ID_Int = Hex_To_Decimal(source_ID_Array);

  length_Of_String = end_Comminucation_Byte.length() + 1;
  char end_Comminucation_Array[length_Of_String];
  end_Comminucation_Byte.toCharArray(end_Comminucation_Array, length_Of_String);
  end_Comminucation_Int = Hex_To_Decimal(end_Comminucation_Array);
  //this for loop wil convert all the strings to an int so that it can do the LRC check
  //TODO: this function should also be made to work dynamicly that doesn't work right now so something to add
  //very weird bug where if you convert here form dec to hex it end the byte weirdly but everywhere else it works??
  if(data_Bytes.length() > 1){
    LRC_As_Int = start_Comminucation_Int ^ packet_Length_Int ^ function_Code_Int ^ source_ID_Int ^ destination_ID_Int ^ end_Comminucation_Int ^ data_Int_Togheter;
  }else{
    LRC_As_Int = start_Comminucation_Int ^ packet_Length_Int ^ function_Code_Int ^ source_ID_Int ^ destination_ID_Int ^ end_Comminucation_Int;
  }
  Decimal_To_Hex(LRC_As_Int, LRC_Comminucation_Byte);

  return 1;
}
//functions that are helpfull for others

//needs to be given the decimal number and an array where the hex number will be put into.
//function is fully tested and working for at least 8 bits
void Decimal_To_Hex(int decimal_Number_To_Be_Converted_To_Hex, String &hex_Number_Array_As_String){
   int place_Of_Hex_Symbol = 0;
   char hex_Number_Array[100];
   long quotient = decimal_Number_To_Be_Converted_To_Hex;
   long remaining_Number_After_Devision;
   while (quotient != 0)
   {
     //first we do a modulo on the quotient to see what remains
     //then we assign a symbol to it
       remaining_Number_After_Devision = quotient % 16;
       if (remaining_Number_After_Devision < 10){
           hex_Number_Array[place_Of_Hex_Symbol++] = 48 + remaining_Number_After_Devision;
       }
       else{
           hex_Number_Array[place_Of_Hex_Symbol++] = 55 + remaining_Number_After_Devision;
       }
       quotient = quotient / 16;
   }
   hex_Number_Array[place_Of_Hex_Symbol] = 0;
   int length_Of_New_Array = sizeof(hex_Number_Array);
   char hex_Number_Array_Correct_Order[length_Of_New_Array];
   hex_Number_Array_Correct_Order[place_Of_Hex_Symbol] = 0;
   //array is flipped so need to be turned around
   hex_Number_Array_Correct_Order[0] = hex_Number_Array[1];
   hex_Number_Array_Correct_Order[1] = hex_Number_Array[0];
   //when array starts with 0 the arduino blocks it so needs to be replaced with a char 0
   if(hex_Number_Array_Correct_Order[0]== 0){
     hex_Number_Array_Correct_Order[0] = '0';
     hex_Number_Array_Correct_Order[1] = hex_Number_Array[0];
   }
   hex_Number_Array[2] = 0;
   hex_Number_Array_Correct_Order[2] = 0;
   hex_Number_Array_As_String = String(hex_Number_Array_Correct_Order);
}

//needs an array with a hex value for example  {'F', 'F'} returns 255
int Hex_To_Decimal(char hex_Number_As_An_Array_To_Convert[]){
  int length_Of_Hex_array= sizeof(hex_Number_As_An_Array_To_Convert);
  int total_Decimal_Value;
  int ascii_Amount_Of_i_In_Array;
  int amount_Of_Number_Without_Ascii;
  int single_Decimal_Value;
  for(int i = 0; i < length_Of_Hex_array; i++){
    //checks if the char is a character from A to F or if it is a number
    if(int(hex_Number_As_An_Array_To_Convert[i]) >= 65 && int(hex_Number_As_An_Array_To_Convert[i]) <= 70){
      ascii_Amount_Of_i_In_Array = hex_Number_As_An_Array_To_Convert[i];
      amount_Of_Number_Without_Ascii = ascii_Amount_Of_i_In_Array - 55;
      //this is conversion that works the same as with binary,
      //it does 16 to the power of the amount to get the right number
      single_Decimal_Value = (amount_Of_Number_Without_Ascii) *pow(16,(length_Of_Hex_array - i - 1));
    }else{
      single_Decimal_Value = (hex_Number_As_An_Array_To_Convert[i]-48)*pow(16,(length_Of_Hex_array - i - 1));
    }
    total_Decimal_Value = total_Decimal_Value +single_Decimal_Value;
  }
  return total_Decimal_Value;
}

void Read_Incomming_Message(String &message_That_Was_Sent){
  String incomming_Message;
  String acknoledge_Message;
  if(Serial.available()> 0){
    incomming_Message = Serial.readString();
    message_That_Was_Sent = incomming_Message;
    Serial.println(incomming_Message);
    Disect_Message(incomming_Message);
    Get_Data_Bytes_From_Message(incomming_Message);
    Make_Acknoledge(Message_As_Struct.Source_ID, acknoledge_Message);
    LRC_Check_Incomming_Message(incomming_Message);
    Serial.println(acknoledge_Message);
    Figure_Out_Responce(Message_As_Struct.Function_Code);
  }
}

void Disect_Message(String message_That_Was_Recieved){
  int length_Of_String;

  //transform the string into an array so that we can easily split the string into smaller parts
  length_Of_String = message_That_Was_Recieved.length() + 1;
  char message_That_Was_Recieved_Array[length_Of_String];
  message_That_Was_Recieved.toCharArray(message_That_Was_Recieved_Array, length_Of_String);

  // first byte is start comminucation so we dont need to look at that
  // so first we will look at the packet lenght
  char packet_Length_Array[2];
  packet_Length_Array[0] = message_That_Was_Recieved_Array[3];
  packet_Length_Array[1] = message_That_Was_Recieved_Array[4];
  //turn it into an int so we can use it
  Message_As_Struct.Packet_Lenght = Hex_To_Decimal(packet_Length_Array);

  //next we will take a look at the function code, which is probably the most important
  char function_Code_Array[2];
  function_Code_Array[0] = message_That_Was_Recieved_Array[6];
  function_Code_Array[1] = message_That_Was_Recieved_Array[7];
  Message_As_Struct.Function_Code = Hex_To_Decimal(function_Code_Array);

  char source_ID_Array[2];
  source_ID_Array[0] = message_That_Was_Recieved_Array[12];
  source_ID_Array[1] = message_That_Was_Recieved_Array[13];
  Message_As_Struct.Source_ID = Hex_To_Decimal(source_ID_Array);

  //LRC will allways be at the the last 2 characters so we will just get it like this
  char LRC_Array[2];
  LRC_Array[0] = message_That_Was_Recieved_Array[sizeof(message_That_Was_Recieved_Array)-1];
  LRC_Array[1] = message_That_Was_Recieved_Array[sizeof(message_That_Was_Recieved_Array)-2];
  Message_As_Struct.LRC = Hex_To_Decimal(LRC_Array);
  //the rest of the bytes are predefined so we don't have to do anything with those

}

void LRC_Check_Incomming_Message(String message_That_Was_Recieved){
  int LRC_Check;
  //we need to check if there is data inculded with the package so that we can check
  if(Message_As_Struct.Packet_Lenght > 4){
    LRC_Check = Message_As_Struct.Start_Comminucation ^ Message_As_Struct.Packet_Lenght ^ Message_As_Struct.Function_Code ^ Message_As_Struct.Source_ID ^ Message_As_Struct.Destination_ID ^ Message_As_Struct.End_Comminucation ^ Message_As_Struct.LRC_For_Data;
  }else{
    LRC_Check = Message_As_Struct.Start_Comminucation ^ Message_As_Struct.Packet_Lenght ^ Message_As_Struct.Function_Code ^ Message_As_Struct.Source_ID ^ Message_As_Struct.Destination_ID ^ Message_As_Struct.End_Comminucation;
  }
  if(LRC_Check == Message_As_Struct.LRC){
    Message_As_Struct.Was_LRC_Correct = 1;
  }else{
    Message_As_Struct.Was_LRC_Correct = 0;
  }
}

void Get_Data_Bytes_From_Message(String message_That_Was_Recieved){
  int length_Of_String;
  int amount_Of_Data_Bytes;
  //the places that the data takes up in the main string
  int amount_Of_Data_Places;

  //transform the string into an array so that we can easily split the string into smaller parts
  length_Of_String = message_That_Was_Recieved.length() + 1;
  char message_That_Was_Recieved_Array[length_Of_String];
  message_That_Was_Recieved.toCharArray(message_That_Was_Recieved_Array, length_Of_String);

  //the standard message is 7 bytes + the data bytes so lets see how many data there is
  amount_Of_Data_Bytes = Message_As_Struct.Packet_Lenght - 4;
  amount_Of_Data_Places = amount_Of_Data_Bytes * 3;

  //this part gets tricky. there was a bug when were the original message got appended to the data bytes
  //this has a work around where i just turn the rest of the bytes to 0's
  //bit of a shite hack So
  //TODO: need to find a better way to do this

  //first we need to check if there is actually any data to be recoverd
  char data_Array[amount_Of_Data_Places];
  if(amount_Of_Data_Bytes > 0){
    for (int i = 0; i < amount_Of_Data_Places; i++){
      data_Array[i] = message_That_Was_Recieved_Array[15+i];
      if(i >= amount_Of_Data_Places-1){
        data_Array[i] = 0;
      }
    }
    //now put it in the struct this does make the array verry big but thats oke. other way isn't possible
    strcpy(Message_As_Struct.Data_Bytes, data_Array);
  }else{
    //if there isn't any data then just make the string empty
    strcpy(Message_As_Struct.Data_Bytes, "");
  }
    //this part is to check the LRC for the data array
    // i do this here so i don't have to do it later
  int data_Int_Togheter = 0;
  int data_Int;
  if(amount_Of_Data_Bytes > 0){
      for(int i = 0; i < amount_Of_Data_Bytes; i++){
      char data_Bytes_2_Spots[2];
      data_Bytes_2_Spots[0] = data_Array[i];
      data_Bytes_2_Spots[1] = data_Array[i+1];
      i++;
      i++;
      data_Int = Hex_To_Decimal(data_Bytes_2_Spots);
      if(i > 3){
        data_Int_Togheter = data_Int ^ data_Int_Togheter;
      }else if(i < 3){
        data_Int_Togheter = data_Int;
      }
    }
  }
  Message_As_Struct.LRC_For_Data = data_Int_Togheter;
}

void Figure_Out_Responce(int function_Code_From_Message_Recieved){
  switch (function_Code_From_Message_Recieved){
    case 0xC3:
      //retransmit
      Serial.println("Printing last sent message");
      Serial.println(last_Sent_Message);
      break;
    case 0xDB:
      //reset requested
      Arduino_Reset_Function_With_Input();
      break;
    case 0xDF:
      //request status
      String status_Byte_Message;
      String status_Byte;
      if(has_Data_Been_Recieved_Yet == 0){
        Decimal_To_Hex(231, status_Byte);
        message_To_Be_Sent_Data = status_Byte;
      }else{
        Decimal_To_Hex(227, status_Byte);
        message_To_Be_Sent_Data = status_Byte;
      }
      Make_Message_To_Be_Sent(Message_As_Struct.Source_ID, 8, 14, status_Byte_Message);
      Serial.println(status_Byte_Message);
      last_Sent_Message = status_Byte_Message;
      break;
    case 0xE7:
      //RFID tag MM found
      //stuur aanvraag naar MM
      //the data from this message is the destination id so we will need to convert that data to an int       
      int destination_ID_Of_MM = Hex_To_Decimal(Message_As_Struct.Data_Bytes);
      Send_FC05(destination_ID_Of_MM);
      //now we have to tell the NM that we can continue to a different module
      String message_To_Be_Sent_To_NM;
      Make_Message_To_Be_Sent(Message_As_Struct.Source_ID, 4, 11, message_To_Be_Sent_To_NM);
      last_Sent_Message = message_To_Be_Sent_To_NM;
      break;
    case 0xF3:
      //RFID tag CO found
      //need to send request to CO first with FC06
      Send_FC06(27);
      //we can wait for the correct responce or we can just send the data.
      //for now we just send the data
      //also right now we are only able to send in our own group
      //needs fixing so that we can send between all the groups
      //TODO: read the string from the CO and only if that is the correct string then we can send the responce
      String responce_Message;
      message_To_Be_Sent_Data = String(Message_As_Struct_With_37_Bytes_Of_Data.Data_Bytes);
      delay(500);
      //27 is the ID of the CO we will use
      //need to find a better way to find what CO we will send to
      Make_Message_To_Be_Sent(27, 29, 7, responce_Message);
      Serial.println(responce_Message);
      //safe the message for if we need to resend the message
      last_Sent_Message = responce_Message;
      break;
    case 0xD0:
       //save the data from this message because it is important we will need to send the data later to the 
      strcpy(Message_As_Struct_With_37_Bytes_Of_Data.Data_Bytes, Message_As_Struct.Data_Bytes);
      Serial.println("Data from modules is stored");
      has_Data_Been_Recieved_Yet = 1;
      break;
    default:
      //if a function is recieved that is not know we need to send a fc01 retransmit because we cant do anything with that
      Send_FC01(Message_As_Struct.Source_ID);
      break;
  }
}

//I really dont like that people can reset my arduino with the sending of a message so here is an interfacing thing where arduino
//won't reset directly
void Arduino_Reset_Function_With_Input(){
  Serial.println("reset has been requested do you really want to reset the arduino");
  Serial.println("type 1 to reset or 0 to cancel");
  while (Serial.available() < 1){
    delay(100);
  }
  while (Serial.available() > 0){
    byte incomming_Awnser= Serial.read();
    if(incomming_Awnser == 49){
      Serial.println("arduino is resetting in 3 seconds");
      Serial.println("all data will be deleted");
      delay(3000);
      Arduino_Reset_Function();
    }else if(incomming_Awnser == 48){
      Serial.println("Arduino not resetting");
    }
  }
}