////////////////////////////
//        code writen     //
//            by          //
//      Ruben Middelman   //



#include <stdio.h>
#include <stdbool.h>

#define AMOUNT_OF_ENEMIES 5
#define AMOUNT_OF_PIXELS_HOR 640
#define AMOUNT_OF_PIXELS_VERT 480


void Init_Enemies();
void Init_Player();
void Move_Enemies_Horizontally();
void Move_Enemies_Vertically();
void Make_Grid(int pixels_Horizontal, int pixels_Vertical);
void Switch_Enemy_Directions(bool left_Or_Right);
void Check_If_Enemies_Need_To_Switch();
void Check_If_Enemies_Are_Alive();
void Move_Player();

//struct for the enemies,
//this struct contains the position and if the enemy is alive or not;
struct Enemy
{
  bool is_Alive : true;
  int x_Position;
  int y_Position;
  int direction;
  int enemy_Sprite;
};

typedef struct Enemy ENEMY;

ENEMY enemy_Array[AMOUNT_OF_ENEMIES];

//this is the struct for the player.
//this contains his location on the x axis
//and how many lives the player has
struct Player
{
  int lives : 3;
  int x_Position;
};

typedef struct Player PLAYER;

struct Shot
{
  bool hit;
  int x_Position;
  int y_Position;
};

typedef struct Shot SHOT;
typedef SHOT* PSHOT;

void main()
{
  Init_Enemies();

  for(int i = 0; i < 30; i++)
  {
    Move_Enemies_Horizontally();
    for(int j = 0; j < AMOUNT_OF_ENEMIES; j++)
    {
      printf("%d %d %d\n", enemy_Array[j].x_Position, enemy_Array[j].y_Position, enemy_Array[j].enemy_Sprite);
    }
    Check_If_Enemies_Need_To_Switch();
    printf("\n");
  }
}

//function that builds all the Init_Enemies
//is based on the amount of enemies that are defined
void Init_Enemies()
{
  for(int i = 0; i < AMOUNT_OF_ENEMIES; i++)
  {
    enemy_Array[i].x_Position = i;
    enemy_Array[i].y_Position = 0;
    enemy_Array[i].direction = 1;
    enemy_Array[i].enemy_Sprite = 3;
  }
}

//initializers the player.
//player is set in the middle of the screen
void Init_Player()
{
  PLAYER Player_Ship;
  Player_Ship.x_Position = 0;
}

//function that moves all the enemies in the enemy_Array
//does this by scrolling through the array and moving every one in the right direction
void Move_Enemies_Horizontally()
{
  for(int i =0; i < AMOUNT_OF_ENEMIES; i++)
  {
    enemy_Array[i].x_Position = enemy_Array[i].x_Position + enemy_Array[i].direction;
  }
}

//function that moves all the enemies vertically,
void Move_Enemies_Vertically()
{
  for(int i =0; i < AMOUNT_OF_ENEMIES; i++)
  {
    enemy_Array[i].y_Position++;
  }
}

//function that switches the enemy directions
//does this by getting a
void Switch_Enemy_Directions(bool left_Or_Right)
{
  if(!left_Or_Right)
  {
    for(int i = 0; i < AMOUNT_OF_ENEMIES; i++)
    {
      enemy_Array[i].direction = -1;
    }
  }else
  {
    for(int i = 0; i < AMOUNT_OF_ENEMIES; i++)
    {
      enemy_Array[i].direction = 1;
    }
  }
}

//function to check if all the enemies are alive
//don't know yet wat we should send
void Check_If_Enemies_Are_Alive()
{
  for(int i = 0; i < AMOUNT_OF_ENEMIES; i++)
  {
    if(enemy_Array[i].is_Alive)
    {
      //
    }else if(!enemy_Array[i].is_Alive)
    {
      //
    }
  }
}

void Move_Player()
{
  //tristans part comes here
}

//function switches enemy direction when the enemies hit the boundry of the screen
void Check_If_Enemies_Need_To_Switch()
{
  if(enemy_Array[AMOUNT_OF_ENEMIES-1].x_Position == 20)
  {
    Switch_Enemy_Directions(0);
    Move_Enemies_Vertically();
  }else if(enemy_Array[0].x_Position == 0)
  {
    Switch_Enemy_Directions(1);
    Move_Enemies_Vertically();
  }
}
