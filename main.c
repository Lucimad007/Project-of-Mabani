#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helper_windows.h"
#include "colorize.h"
#define clear() system("cls")
struct User{
    char name[20];
    char family[20];
    int age;
    char nickName[20];
    char password[20];
};
typedef struct User User;

enum situation {MAIN_MENU, SIGN_IN_MENU, REGISTER_MENU, SELECT_LEVEL, GAME};
typedef enum situation situation;

situation flag = MAIN_MENU;
FILE* users;

void my_callback_on_key_arrival(char c);

void MainMenu();
void SignInMenu();

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
            flag = SIGN_IN_MENU;
            SignInMenu();
        } else if(c == '2'){
            clear();
            flag = REGISTER_MENU;
            Register();
        }
    } else if(flag == SIGN_IN_MENU){
        //SignInMenu();
    }
}
////////////////////////////////////////////////////////////
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
    char str2[12] = "2. register";
    char str3[10] = "Main Menu";

    gotoxy((WIDTH-strlen(str1))/2,(HEIGHT)/2 - 1);
    printf("%s",str1);
    gotoxy((WIDTH-strlen(str2))/2,(HEIGHT)/2 + 1);
    printf("%s",str2);

    setcolor(6);
    gotoxy((WIDTH-strlen(str3))/2,(HEIGHT)/2 - 3);
    printf("%s",str3);

    gotoxy(0, HEIGHT + 3);

}
////////////////////////////////////////////////////////////
void SignInMenu(){
    char Name[20];
    char Password[20];

    int WIDTH = 32;
    int HEIGHT = 20;

    setcolor(2);
    for(int i=1;i<=HEIGHT;i++)
    {
        for(int j=1;j<=WIDTH;j++)
        {
            if((j == 1) || (j == WIDTH) || (i == HEIGHT) || (i == 1))
            {
                 printf("%c",(char)3);
            } else
                printf(" ");
        }
        printf("\n");
    }
        setcolor(5);
    char str1[25] = "Pls enter your nickName:";
    char str2[25] = "Pls enter your password:";
    char str3[15] = "<Sign In Menu>";
    //char str4[5] = "back";

    gotoxy((WIDTH-strlen(str1))/2,(HEIGHT)/2 - 2);
    printf("%s",str1);
    gotoxy((WIDTH-strlen(str2))/2,(HEIGHT)/2 + 1);
    printf("%s",str2);

    setcolor(6);
    gotoxy((WIDTH-strlen(str3))/2,(HEIGHT)/2 - 4);
    printf("%s",str3);

//    setcolor(1);
//    gotoxy(4,HEIGHT-3);
//    printf("%s",str4);

    gotoxy((WIDTH-strlen(str1))/2,(HEIGHT)/2 - 1);
    gets(Name);

    gotoxy((WIDTH-strlen(str2))/2,(HEIGHT)/2 + 2);
    gets(Password);

    gotoxy(0,(HEIGHT)+2);
    FILE* file = fopen("users.bin","rb");
    if(file != NULL){
        User user;
        while(!feof(file)){
            fread(&user, sizeof(User), 1, file);
            if(strcmp(user.nickName , Name) == 0)
                if(strcmp(user.password , Password) == 0)
                    {
                        flag = GAME;
                        clear();
                        fclose(file);
                        return;
                    }
        }
    }

    clear();
    fclose(file);
    SignInMenu();
    return;
}
////////////////////////////////////////////////////////////
void Register(){
    User user;
    int WIDTH = 32;
    int HEIGHT = 28;

    setcolor(2);
    for(int i=1;i<=HEIGHT;i++)
    {
        for(int j=1;j<=WIDTH;j++)
        {
            if((j == 1) || (j == WIDTH) || (i == HEIGHT) || (i == 1))
            {
                 printf("%c",(char)5);
            } else
                printf(" ");
        }
        printf("\n");
    }
        setcolor(5);
    char str1[21] = "Pls enter your Name:";
    char str2[23] = "Pls enter your Family:";
    char str3[20] = "Pls enter your Age:";
    char str4[25] = "Pls enter your NickName:";
    char str5[25] = "Pls enter your Password:";
    char str6[16] = "<Register Menu>";
    //char str7[5] = "back";

    gotoxy((WIDTH-strlen(str1))/2,(HEIGHT)/2 - 4);
    printf("%s",str1);
    gotoxy((WIDTH-strlen(str2))/2,(HEIGHT)/2 - 2);
    printf("%s",str2);
    gotoxy((WIDTH-strlen(str3))/2,(HEIGHT)/2);
    printf("%s",str3);
    gotoxy((WIDTH-strlen(str4))/2,(HEIGHT)/2 + 2);
    printf("%s",str4);
    gotoxy((WIDTH-strlen(str5))/2,(HEIGHT)/2 + 4);
    printf("%s",str5);

    setcolor(8);
    gotoxy((WIDTH-strlen(str6))/2,(HEIGHT)/2 - 7);
    printf("%s",str6);

//    setcolor(1);
//    gotoxy(4,HEIGHT-3);
//    printf("%s",str4);

    gotoxy((WIDTH-strlen(str1))/2,(HEIGHT)/2 - 3);
    gets(user.name);
    gotoxy((WIDTH-strlen(str2))/2,(HEIGHT)/2 - 1);
    gets(user.family);
    gotoxy((WIDTH-strlen(str3))/2,(HEIGHT)/2 + 1);
    scanf("%d",&user.age);
    getchar();             // to get rid of \n
    gotoxy((WIDTH-strlen(str4))/2,(HEIGHT)/2 + 3);
    gets(user.nickName);
    gotoxy((WIDTH-strlen(str5))/2,(HEIGHT)/2 + 5);
    gets(user.password);



    gotoxy(0,(HEIGHT)+2);
    FILE* file = fopen("users.bin","ab");

    fwrite(&user, sizeof(User), 1, file);

    clear();
    fclose(file);
    return;
}
