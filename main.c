#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include "helper_windows.h"
#include "colorize.h"
#define clear() system("cls")
enum Difficulity {EASY, MEDIUM, HARD};
typedef enum Difficulity Difficulity;

struct Node{
    char word[21];
    struct Node* next;
};
typedef struct Node* NodePtr;
typedef struct Node Node;

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

enum situation {MAIN_MENU, SIGN_IN_MENU, REGISTER_MENU, START_MENU, SELECT_LEVEL, GAME, IN_GAME};
typedef enum situation situation;

User user;
situation flag = MAIN_MENU;
FILE* users;
NodePtr head = NULL;
int y = 0;   //even empty lines
int width = 44;   //global width of in_game
int height = 26;  //global height of in_game
int Current_Score = 0;
double Time_Period = 0;
int Wave = 0;
int Word_Counter = 0;    //for counting numbers to determine when we should create a new wave
int Char_Situation = 0;
int SizeOfCurrentWord = 0;
int colors[20];
Difficulity curLevel;
int IN_PROGRESS = 0;
int flag_sit = 0;

void my_callback_on_key_arrival(char c);

void MainMenu();
void SignInMenu();
void Register();
void selectLevelMenu();
void createFilesOfWords();
void startButton();
void createDisplay();
void updateDisplay();
void resetColor();
void clearDisplay();
void updateHead();
void clearHead();
void showDetails();
void Winner();
void Loser();
void newWave();
//void isMobham();
NodePtr createNode();
void addWord(char word[]);
void deleteWord();

int main()
{
   createFilesOfWords();

    for(int i =0;i<10;i++)
        addWord("ali");
   MainMenu();
    HANDLE thread_id = start_listening(my_callback_on_key_arrival);

    while(1){
        if(flag!=IN_GAME)
            continue;

        while(IN_PROGRESS){}
        IN_PROGRESS = 1;
        clearDisplay();
        y++;
        if(y>25)
            {
                Loser();
                break;
            }
        updateDisplay();
        IN_PROGRESS = 0;
        Word_Counter++;

        if(Word_Counter == 10){
            newWave();

            while(IN_PROGRESS){}
            IN_PROGRESS = 1;
            showDetails();
            IN_PROGRESS = 0;

            if(curLevel == HARD)
                Time_Period *= 0.6;
            else if(curLevel == MEDIUM)
                Time_Period *= 0.7;
            else if(curLevel == EASY)
                Time_Period *= 0.8;
            Word_Counter = 0;
        }

        if(head == NULL)
        {
            Winner();
            break;
        }
        usleep((Time_Period/10)*10*10*10*10*10*10);   //   /10 because words of waves are 10 and * 10^6 because it should be micro sec
    }

  WaitForSingleObject(thread_id, INFINITE);
  return 0;
}

void my_callback_on_key_arrival(char c)
{
    if(flag_sit)
        return;
    if(flag == MAIN_MENU){
        if(c == '1'){
            clear();
            flag = SIGN_IN_MENU;
            SignInMenu();
            flag = SELECT_LEVEL;
            selectLevelMenu();
        } else if(c == '2'){
            clear();
            flag = REGISTER_MENU;
            Register();
            flag = SELECT_LEVEL;
            selectLevelMenu();
        }
    }else if(flag == SELECT_LEVEL){
        char tempNickNmae[20] = "";
        strcat(tempNickNmae, user.nickName);
        FILE* temp = fopen(strcat(tempNickNmae,".bin"),"rb");
        switch (c){
            case '1':
                Wave = 1;
                Current_Score = 0;
                Time_Period = 10;
                curLevel = EASY;
                flag = START_MENU;
                clear();
                startButton();
                break;
            case '2':
                Wave = 1;
                Current_Score = 0;
                Time_Period = 8;
                curLevel = MEDIUM;
                flag = START_MENU;
                clear();
                startButton();
                break;
            case '3':
                Wave = 1;
                Current_Score = 0;
                Time_Period = 5;
                curLevel = HARD;
                flag = START_MENU;
                clear();
                startButton();
                break;
            case '4':
                if(!feof(temp) && temp != NULL)
                {
                    Data tempData;
                    fseek(temp, (-1)*sizeof(Data), SEEK_END);
                    fread(&tempData, sizeof(Data), 1, temp);
                    Wave = 1;
                    Current_Score = tempData.score;
                    curLevel = tempData.level;
                    if(tempData.level == EASY)
                        Time_Period = 10;
                    else if(tempData.level == MEDIUM)
                        Time_Period = 8;
                    else if(tempData.level == HARD)
                        Time_Period = 5;

                    flag = START_MENU;
                    clear();
                    startButton();
                }
                break;
            case '5':
                if(!feof(temp) && temp != NULL)
                {
                    Data tempData;
                    fseek(temp, (-2)*sizeof(Data), SEEK_END);
                    fread(&tempData, sizeof(Data), 1, temp);
                    Wave = 1;
                    Current_Score = tempData.score;
                    curLevel = tempData.level;
                    if(tempData.level == EASY)
                        Time_Period = 10;
                    else if(tempData.level == MEDIUM)
                        Time_Period = 8;
                    else if(tempData.level == HARD)
                        Time_Period = 5;

                    flag = START_MENU;
                    clear();
                    startButton();
                }
                break;
            case '6':
                if(!feof(temp) && temp != NULL)
                {
                    Data tempData;
                    fseek(temp, (-3)*sizeof(Data), SEEK_END);
                    fread(&tempData, sizeof(Data), 1, temp);
                    Wave = 1;
                    Current_Score = tempData.score;
                    curLevel = tempData.level;
                    if(tempData.level == EASY)
                        Time_Period = 10;
                    else if(tempData.level == MEDIUM)
                        Time_Period = 8;
                    else if(tempData.level == HARD)
                        Time_Period = 5;

                    flag = START_MENU;
                    clear();
                    startButton();
                }
                break;
            default:
                break;
        }

    }else if(flag == GAME){
        createDisplay();
        showDetails();
        flag = IN_GAME;
    } else if((flag == IN_GAME)&&(head!=NULL)&&(y>1)){
        SizeOfCurrentWord = strlen(head->word);
        char word[20];
        strcpy(word, head->word);
        if(c == word[Char_Situation]){
            colors[Char_Situation] = 1;
            Current_Score++;

            while(IN_PROGRESS){}
            IN_PROGRESS = 1;
            showDetails();
            IN_PROGRESS = 0;
        }
        else
            colors[Char_Situation] = -1;
        Char_Situation++;

        if(Char_Situation == SizeOfCurrentWord){
            while(IN_PROGRESS){}
            IN_PROGRESS = 1;
            clearHead();
            Char_Situation = 0;
            resetColor();
            clearDisplay();
            deleteWord();
            updateDisplay();
            IN_PROGRESS = 0;
        } else{
            while(IN_PROGRESS){}
            IN_PROGRESS = 1;
            updateHead();
            IN_PROGRESS = 0;
        }
    }

    //usleep(200000);     //to prevent program from some bugs and disallow user to cheat

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


    user = temp;
    gotoxy(0,(HEIGHT)+2);
    FILE* file = fopen("users.bin","ab");
    fwrite(&temp, sizeof(User), 1, file);

    clear();
    fclose(file);
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

    char tempNickName[20];
    strcpy(tempNickName, user.nickName);
    FILE* file = fopen(strcat(tempNickName, ".bin"),"rb");
    if((file == NULL) || (file == feof(file))){
        setcolor(7);
        char message[12] = "No History!";
        gotoxy((WIDTH-strlen(message))/2 , HEIGHT/2 );
        printf("%s", message);
        gotoxy(0,HEIGHT+3);
    } else {
        setcolor(4);
        Data data;
        gotoxy(3, HEIGHT/2 + 2);
        printf("Score(s)");
        gotoxy(15, HEIGHT/2 + 2);
        printf("date(s)");
        gotoxy(46, HEIGHT/2 + 2);
        printf("Difficulity");
        setcolor(7);

        if(!feof(file))
            fseek(file,(-1)*sizeof(Data),SEEK_END);
        for(int i=1;i<=3;i++){
            if(feof(file))
                break;
            fread(&data ,sizeof(Data), 1, file);
            gotoxy(3, HEIGHT/2 + i*2 + 2);
            printf("%d. %d",i+3,data.score);
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

            fseek(file,(-2)*sizeof(Data),SEEK_CUR);
        }
    }

    gotoxy(0,HEIGHT+3);
    fclose(file);
    return;
}
void startButton(){
    int WIDTH = 40;
    int HEIGHT = 26;

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

    setcolor(7);
    char str[24] = "Press any key to Start!";
    gotoxy((WIDTH-strlen(str))/2, HEIGHT/2);
    printf("%s",str);
    flag = GAME;
    return;
}
void clearHead(){
    int len = strlen(head->word);
    gotoxy((width-len)/2 , y);
    setcolor(0);
    printf("%s",head->word);
}
void resetColor(){
    for(int i = 0;i<20;i++)
        colors[i] = 0;
}
void clearDisplay(){
    int temp = y;
    NodePtr tempPtr = head;
    setcolor(0);
    while(temp>0 && tempPtr != NULL){
        gotoxy((width-strlen(tempPtr->word))/2 , temp);
        printf("%s",tempPtr->word);
        temp--;
        tempPtr = tempPtr->next;
    }
}
void updateDisplay(){
    if(head==NULL)
        return;
    updateHead();
    int temp = y-1;
    NodePtr tempPtr = head->next;
    setcolor(7);
    while(temp>0 && tempPtr != NULL){
        gotoxy((width-strlen(tempPtr->word))/2 , temp);
        printf("%s",tempPtr->word);
        temp--;
        tempPtr = tempPtr->next;
    }
}
void  updateHead(){
    if(head == NULL)
        return;
    int len = strlen(head->word);
    gotoxy((width-len)/2 , y);
    for(int i=0;i<len;i++){
        if(colors[i] == 1)
            setcolor(2);
        if(colors[i] == 0)
            setcolor(7);
        if(colors[i] == -1)
            setcolor(1);
        printf("%c",head->word[i]);
    }
}
void createDisplay(){
    clear();
    setcolor(7);
    for(int i=0;i<=width;i++)
    {
        if(i%2 == 0)
            setcolor(4);
        else
            setcolor(1);
        gotoxy(i, 0);
        printf("%c",(char)4);
        gotoxy(i, height);
        printf("%c",(char)3);
    }
    for(int i=0;i<=height;i++)
    {
        if(i%2 == 0)
            setcolor(4);
        else
            setcolor(1);
        gotoxy(0, i);
        printf("%c",(char)5);
        gotoxy(width, i);
        printf("%c",(char)6);
    }
}
void showDetails(){
    gotoxy(2, height+2);
    setcolor(6);
    printf("Score : %d",Current_Score);
    gotoxy(2, height+3);
    setcolor(6);
    printf("Wave : %d",Wave);
    gotoxy(20, height+2);
    setcolor(6);
    printf("User : %s",user.nickName);
}
void Winner(){
    clear();
    gotoxy(width/2, height/2);
    setcolor(2);
    IN_PROGRESS = 1;

    char tempNickName[20];
    strcpy(tempNickName, user.nickName);
    FILE* file = fopen(strcat(tempNickName,".bin") , "a+b");
    Data data;
    data.score = Current_Score;
    data.date = time(NULL);
    data.level = curLevel;
    strcpy(data.nickName , user.nickName);

    fwrite(&data,sizeof(Data) , 1 , file);
    fclose(file);
    printf("You Won!");
}
void Loser(){
    clear();
    gotoxy(width/2, height/2);
    setcolor(4);
    IN_PROGRESS = 1;

    char tempNickName[20];
    strcpy(tempNickName, user.nickName);
    FILE* file = fopen(strcat(tempNickName,".bin") , "a+b");
    Data data;
    data.score = Current_Score;
    data.date = time(NULL);
    data.level = curLevel;
    strcpy(data.nickName , user.nickName);

    fwrite(&data,sizeof(Data) , 1 , file);
    fclose(file);
    printf("You Failed!");
}
void newWave(){
    if(Wave == 5)
        return;
    Wave++;
    FILE* file ;
    srand(time(NULL));
    int x;
    char word[20];
    switch (Wave){
        case 1:
            file = fopen("normal.txt","r");
            x = rand()%100;
            for(int i=0;i<10;i++){
                for(int j=0;j<x;j++){
                    fscanf(file,"%s", word);
                }
                addWord(word);
                x = rand()%100;
                rewind(file);
            }
            fclose(file);
            break;
        case 2:
            file = fopen("hard.txt","r");
            x = rand()%100;
            for(int i=0;i<10;i++){
                for(int j=0;j<x;j++){
                    fscanf(file,"%s", word);
                }
                addWord(word);
                x = rand()%100;
                rewind(file);
            }
            fclose(file);
            break;
        case 3:
            file = fopen("long.txt","r");
            x = rand()%100;
            for(int i=0;i<10;i++){
                for(int j=0;j<x;j++){
                    fscanf(file,"%s", word);
                }
                addWord(word);
                x = rand()%100;
                rewind(file);
            }
            fclose(file);
            break;
        case 4:
            file = fopen("long.txt","r");
            x = rand()%100;
            for(int i=0;i<10;i++){
                for(int j=0;j<x;j++){
                    fscanf(file,"%s", word);
                }
                addWord(word);
                x = rand()%100;
                rewind(file);
            }
            fclose(file);
            break;
        case 5:
            file = fopen("long.txt","r");
            x = rand()%100;
            for(int i=0;i<10;i++){
                for(int j=0;j<x;j++){
                    fscanf(file,"%s", word);
                }
                addWord(word);
                x = rand()%100;
                rewind(file);
            }
            fclose(file);
            break;
        default:
            return;
            break;
    }
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
        for( ;j<x;j++){
            if((j%3 == 2) && (j+1 != x)){
                HardWord[j] = arr[rand()%7 + 36];
                continue;
            }
            HardWord[j] = arr[rand()%43];
        }

        HardWord[j] = '\0';
        fputs(HardWord, file);
        fprintf(file,"\n");

    }
    fclose(file);
}
NodePtr createNode(){
    NodePtr temp = (NodePtr)malloc(sizeof(Node));
    temp->next = NULL;
    strcpy(temp->word, "");
    return temp;
}
void addWord(char word[]){
    if(head==NULL){
        head = createNode();
        strcpy(head->word, word);
        return;
    }
    NodePtr temp = head;
    while(head->next != NULL)
        head = head->next;
    head->next = createNode();
    strcpy(head->next->word, word);
    head = temp;
    return;
}
void deleteWord(){
    head = head->next;
    y--;
}
