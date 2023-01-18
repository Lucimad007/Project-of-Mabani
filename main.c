#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "helper_windows.h"
#include "colorize.h"
#define clear() system("cls")
enum Difficulity {EASY, MEDIUM, HARD};
typedef enum Difficulity Difficulity;
struct User{
    char name[20];
    char family[20];
    int age;
    char nickName[20];
    char password[20];
};
typedef struct User User;
struct Data{
    char nickName[20];
    int score;
    time_t date;
    Difficulity level;
};
typedef struct Data Data;

enum situation {MAIN_MENU, SIGN_IN_MENU, REGISTER_MENU, SELECT_LEVEL, GAME};
typedef enum situation situation;

User user;
situation flag = MAIN_MENU;
FILE* users;

void my_callback_on_key_arrival(char c);

void MainMenu();
void SignInMenu();
void Register();
void selectLevelMenu();
void createFilesOfWords();

int main()
{
   createFilesOfWords();
   //Data data1,data2,data3;
   //data1.score = 300; data2.score = 400; data3.score =350; FILE *file = fopen("ali.bin","wb"); data1.level = HARD;
  // data2.level = MEDIUM; data3.level = EASY;data1.date = time(NULL);data2.date = time(NULL);data3.date = time(NULL);
  // fwrite(&data1, sizeof(Data), 1, file);
  // fwrite(&data2, sizeof(Data), 1, file);
   //fwrite(&data3, sizeof(Data), 1, file);
   //fclose(file);
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
        User temp;
        while(!feof(file)){
            fread(&temp, sizeof(User), 1, file);
            if(strcmp(temp.nickName , Name) == 0)
                if(strcmp(temp.password , Password) == 0)
                    {
                        user = temp;
                        flag = SELECT_LEVEL;
                        clear();
                        fclose(file);
                        selectLevelMenu();
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
    User temp;
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
    gets(temp.name);
    gotoxy((WIDTH-strlen(str2))/2,(HEIGHT)/2 - 1);
    gets(temp.family);
    gotoxy((WIDTH-strlen(str3))/2,(HEIGHT)/2 + 1);
    scanf("%d",&temp.age);
    getchar();             // to get rid of \n
    gotoxy((WIDTH-strlen(str4))/2,(HEIGHT)/2 + 3);
    gets(temp.nickName);
    gotoxy((WIDTH-strlen(str5))/2,(HEIGHT)/2 + 5);
    gets(temp.password);



    gotoxy(0,(HEIGHT)+2);
    FILE* file = fopen("users.bin","ab");
    fwrite(&temp, sizeof(User), 1, file);

    user = temp;
    flag = SELECT_LEVEL;
    clear();
    fclose(file);
    selectLevelMenu();
    return;
}
////////////////////////////////////////////////////////////
void selectLevelMenu(){
    int WIDTH = 66;
    int HEIGHT = 24;
    char str1[8] = "1. Easy";
    char str2[10] = "2. Medium";
    char str3[8] = "3. Hard";
    char str4[21] = "<Select Level Menu>";
    char str5[17] = "previous games :";

    setcolor(5);
    for(int i=1;i<=HEIGHT;i++)
    {
        for(int j=1;j<=WIDTH;j++)
        {
            if((j == 1) || (j == WIDTH) || (i == HEIGHT) || (i == 1))
            {
                 printf("%c",(char)6);
            } else
                printf(" ");
        }
        printf("\n");
    }

    setcolor(4);
    gotoxy((WIDTH-strlen(str4))/2 , HEIGHT/2 -10);
    printf("%s",str4);

    setcolor(7);
    gotoxy((WIDTH-strlen(str1))/2 , HEIGHT/2 - 8);
    printf("%s",str1);
    gotoxy((WIDTH-strlen(str2))/2 , HEIGHT/2 - 6);
    printf("%s",str2);
    gotoxy((WIDTH-strlen(str3))/2 , HEIGHT/2 - 4);
    printf("%s",str3);

    setcolor(4);
    gotoxy((WIDTH-strlen(str5))/2 , HEIGHT/2 - 2);
    printf("%s",str5);

    FILE* file = fopen(strcat(user.nickName,".bin"),"rb");
    if((file == NULL) || (file == feof(file))){
        setcolor(7);
        char message[12] = "No History!";
        gotoxy((WIDTH-strlen(message))/2 , HEIGHT/2 );
        printf("%s", message);
        gotoxy(0,HEIGHT+3);
    } else {
        setcolor(7);
        Data data;
        gotoxy(3, HEIGHT/2 + 2);
        printf("Score(s)");
        gotoxy(15, HEIGHT/2 + 2);
        printf("date(s)");
        gotoxy(46, HEIGHT/2 + 2);
        printf("Difficulity");

        for(int i=1;i<=3;i++){
            if(feof(file))
                break;
            fread(&data ,sizeof(Data), 1, file);
            gotoxy(3, HEIGHT/2 + i*2 + 2);
            printf("%d. %d",i,data.score);
            gotoxy(15, HEIGHT/2 + i*2 + 2);
            char *date = ctime(&data.date);
            printf("%s",date);
            gotoxy(46, HEIGHT/2 + i*2 + 2);
            if(data.level == EASY)
                printf("easy");
            else if(data.level == MEDIUM)
                printf("medium");
            else if(data.level == HARD)
                printf("hard");
        }
    }

    gotoxy(0,HEIGHT+3);
    fclose(file);
}
////////////////////////////////////////////////////////////
void createFilesOfWords(){
    FILE* file = fopen("normal.txt","w");
    char arr[43] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','1','2','3','4','5','6','7','8','9','0','_','@','%','$','^','&','!'};
    char word[11];
    char LongWord[21];
    char HardWord[21];

    srand(time(NULL));

    for(int i=0;i<100;i++)
    {
        int x = rand()%8 + 3;
        int j=0;
        for( ;j<x;j++)
            word[j] = arr[rand()%36];
        word[j] = '\0';
        fputs(word, file);
        fprintf(file,"\n");
    }
    fclose(file);

    file = fopen("long.txt","w");
    for(int i=0;i<100;i++)
    {
        int x = rand()%11 + 10;
        int j=0;
        for( ;j<x;j++)
            LongWord[j] = arr[rand()%36];
        LongWord[j] = '\0';
        fputs(LongWord, file);
        fprintf(file,"\n");
    }
    fclose(file);

    file = fopen("hard.txt","w");
    for(int i=0;i<100;i++)
    {
        int x = rand()%11 + 10;
        int j=0;
        for( ;j<x;j++)
            HardWord[j] = arr[rand()%43];
        HardWord[j] = '\0';
        fputs(HardWord, file);
        fprintf(file,"\n");
    }
    fclose(file);
}
