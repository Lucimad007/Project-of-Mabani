#include <stdio.h>
#include <stdlib.h>
#include "helper_windows.h"
#include "colorize.h"
#define clear() system("cls")
enum situation {MAIN_MENU, SIGN_IN_MENU, SIGN_UP_MENU, SELECT_LEVEL, GAME};
typedef enum situation situation;

situation flag = MAIN_MENU;

void my_callback_on_key_arrival(char c);

void MainMenu(){
    int WIDTH = 32;
    int HEIGHT = 20;

    setcolor(9);
    for(int i=1;i<=HEIGHT;i++)
    {
        for(int j=1;j<=WIDTH;j++)
        {
            if((j == 1) || (j == WIDTH) || (i == HEIGHT) || (i == 1))
            {
                 printf("%c",(char)4);
            } else
                printf(" ");
        }
        printf("\n");
    }
    setcolor(5);
    char str1[11] = "1. sign in";
    char str2[11] = "2. sign up";

    gotoxy((WIDTH-strlen(str1))/2,(HEIGHT)/2 - 1);
    printf("%s",str1);
    gotoxy((WIDTH-strlen(str2))/2,(HEIGHT)/2 + 1);
    printf("%s",str2);
    gotoxy(0, HEIGHT + 3);

}
int main()
{

  //Example codes.
  MainMenu();
  //clear();
  HANDLE thread_id = start_listening(my_callback_on_key_arrival);



  WaitForSingleObject(thread_id, INFINITE);
  return 0;
}
void my_callback_on_key_arrival(char c)
{
    if(flag == MAIN_MENU){
        if(c == '1'){
            clear();
            printf("DONE 1!");  //THIS IS A TEST!
            flag = SIGN_IN_MENU;
        } else if(c == '2'){
            clear();
            printf("DONE 2!");  //THIS IS A TEST!
            flag = SIGN_UP_MENU;
        }
    }
}
