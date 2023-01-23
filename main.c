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
    bool Mobham;
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
int pos = 0;             //position for over-writing history
int countOfScores = 0;   //count of scores in user's file

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
void addNormalWord();
void addLongWord();
void addHardWord();
void addMobhamWord();
void overWriteHistory(Data data);  //stands for position
NodePtr createNode();
void addWord(char word[], bool isMobham);
void deleteWord();

int main()
{
   createFilesOfWords();
   MainMenu();
   newWave();   // to add first words and get ready for begining of the game
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

            if(Time_Period<1)
                Time_Period = 1;
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
                if(countOfScores < 1)
                    break;
                pos = 3;

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
                if(countOfScores < 2)
                    break;
                pos = 2;

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
                if(countOfScores < 3)
                    break;
                pos = 1;

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
        fclose(temp);

    }else if(flag == GAME){
        createDisplay();
        while(IN_PROGRESS){}
        IN_PROGRESS = 1;
        showDetails();
        IN_PROGRESS = 0;
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

            countOfScores++;
            //fseek(file,(-2)*sizeof(Data),SEEK_CUR);    // i did't use this because i wanted to fix a bug!
            fseek(file,(-1)*sizeof(Data),SEEK_CUR);
            long x = ftell(file);
            if(x == 0)
                break;
            fseek(file,(-1)*sizeof(Data),SEEK_CUR);
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
        if(tempPtr->Mobham)
        {
            for(int i=1;i<width;i++)
            {
                gotoxy(i,temp);
                printf("*");
            }
        } else{
            gotoxy((width-strlen(tempPtr->word))/2 , temp);
            printf("%s",tempPtr->word);
        }

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
        if(tempPtr->Mobham)
        {
            for(int i=1;i<width;i++)
            {
                gotoxy(i,temp);
                printf("*");
            }
        } else{
            gotoxy((width-strlen(tempPtr->word))/2 , temp);
            printf("%s",tempPtr->word);
        }
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
            setcolor(4);
        if(colors[i] == 0)
            setcolor(7);
        if(colors[i] == -1)
            setcolor(6);
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
    createDisplay();

    char message[36] = "You are really good at this game!";
    char message2[9] = "score : ";

    IN_PROGRESS = 1;

    Data data;
    data.score = Current_Score;
    data.date = time(NULL);
    data.level = curLevel;
    strcpy(data.nickName , user.nickName);
    overWriteHistory(data);

    gotoxy((width-strlen(message))/2, height/2);
    setcolor(2);
    printf("%s", message);

    gotoxy((width-strlen(message2))/2, height/2 + 1);
    setcolor(2);
    printf("%s%d", message2,Current_Score);

    for(int i=5;i>0;i--){
        gotoxy(width/2 , height/2 +3);
        printf("%d",i);
        gotoxy(0 , height + 1);

        usleep(1000000);
    }
    clear();
    exit(0);
}
void Loser(){
    clear();
    createDisplay();
    char message[12] = "You Failed!";
    char message2[8] = "Wave : ";
    char message3[9] = "Score : ";
    char message4[15] = "Difficulity : ";


    IN_PROGRESS = 1;
    Data data;
    data.score = Current_Score;
    data.date = time(NULL);
    data.level = curLevel;
    strcpy(data.nickName , user.nickName);
    overWriteHistory(data);

    setcolor(4);
    gotoxy((width-strlen(message))/2 , height / 2);
    printf("%s",message);
    gotoxy((width-strlen(message2))/2 , height / 2 + 1);
    printf("%s%d",message2,Wave);
    gotoxy((width-strlen(message3))/2 , height / 2 + 2);
    printf("%s%d",message3,Current_Score);

    gotoxy((width-strlen(message4))/2 , height / 2 + 3);
    printf("%s",message4);
    if(curLevel == EASY)
        printf("Easy");
    else if(curLevel == MEDIUM)
        printf("Medium");
    else if(curLevel == HARD)
        printf("Hard");

    for(int i=5;i>0;i--){
        gotoxy(width/2 , height/2 + 5);
        printf("%d",i);

        gotoxy(0 , height + 1);
        usleep(1000000);
    }

    clear();
    exit(0);
}
void newWave(){
    if(Time_Period == 1 && head == NULL)
        Winner();
    if(Time_Period == 1)
        return;

    Wave++;

    switch (Wave){
        case 1:
            for(int i = 0;i<10;i++)
            addNormalWord();
            break;
        case 2:
            for(int i = 0;i<10;i++)
            {
                if(i%2 == 0)
                    addNormalWord();
                else
                    addLongWord();
            }
            break;
        case 3:
            for(int i = 0;i<10;i++)
            {
                if(i%3 == 0)
                    addNormalWord();
                else if(i%3 == 1)
                    addLongWord();
                else
                    addHardWord();
            }
            break;

        default:                 //4th Wave and later
            for(int i = 0;i<10;i++)
            {
                if(i%3 == 0)
                    addLongWord();
                else if(i%3 == 1)
                    addHardWord();
                else
                    addMobhamWord();
            }
            break;
    }
}
void overWriteHistory(Data data){
    char tempNickName[20];
    strcpy(tempNickName, user.nickName);
    FILE* file;
    if(pos==0){
        file = fopen(strcat(tempNickName,".bin") , "ab");
        fwrite(&data,sizeof(Data) , 1 , file);
        fclose(file);
    }
    else{
        file = fopen(strcat(tempNickName,".bin") , "rb");
        Data datas[countOfScores];

        for(int i=0;i<countOfScores;i++){
            fread(&datas[i] , sizeof(Data) , 1 , file);
        }
        fclose(file);


        if(pos == 3)
            datas[countOfScores-1] = data;
        else if(pos == 2)
            datas[countOfScores-2] = data;
        else if(pos == 1)
            datas[countOfScores-3] = data;

        file = fopen(tempNickName, "wb");
        for(int i=0;i<countOfScores;i++){
            fwrite(&datas[i] , sizeof(Data) , 1 , file);
        }
        fclose(file);
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
void addNormalWord(){
    FILE* file ;
    char word[20];
    int x;
    file = fopen("normal.txt","r");
    x = rand()%100;
    for(int j=0;j<x;j++){
        fscanf(file,"%s", word);
    }
    addWord(word,false);

    close(file);
}
void addLongWord(){
    FILE* file ;
    char word[20];
    int x;
    file = fopen("long.txt","r");
    x = rand()%100;
    for(int j=0;j<x;j++){
        fscanf(file,"%s", word);
    }
    addWord(word,false);

    close(file);
}
void addHardWord(){
    FILE* file ;
    char word[20];
    int x;
    file = fopen("hard.txt","r");
    x = rand()%100;
    for(int j=0;j<x;j++){
        fscanf(file,"%s", word);
    }
    addWord(word,false);

    close(file);
}
void addMobhamWord(){
    int rnd = rand()%3;
    if(rnd == 0){
        FILE* file ;
        char word[20];
        int x;
        file = fopen("normal.txt","r");
        x = rand()%100;
        for(int j=0;j<x;j++){
            fscanf(file,"%s", word);
        }

        addWord(word,true);

        close(file);
    } else if(rnd == 1){
        FILE* file ;
        char word[20];
        int x;
        file = fopen("hard.txt","r");
        x = rand()%100;
        for(int j=0;j<x;j++){
            fscanf(file,"%s", word);
        }
        addWord(word,true);

        close(file);
    } else if(rnd == 2){
        FILE* file ;
        char word[20];
        int x;
        file = fopen("hard.txt","r");
        x = rand()%100;
        for(int j=0;j<x;j++){
            fscanf(file,"%s", word);
        }
        addWord(word,true);

        close(file);
    }
}
NodePtr createNode(){
    NodePtr temp = (NodePtr)malloc(sizeof(Node));
    temp->next = NULL;
    temp->Mobham = false;
    strcpy(temp->word, "");
    return temp;
}
void addWord(char word[], bool isMobham){
    if(head==NULL){
        head = createNode();
        strcpy(head->word, word);
        if(isMobham)
            head->Mobham = true;
        else
            head->Mobham = false;
        return;
    }

    NodePtr temp = head;
    while(head->next != NULL)
        head = head->next;
    head->next = createNode();
    strcpy(head->next->word, word);
    if(isMobham)
            head->Mobham = true;
        else
            head->Mobham = false;

    head = temp;
    return;
}
void deleteWord(){
    head = head->next;
    y--;
}
