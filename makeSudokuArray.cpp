#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <mem.h>
#include <windows.h>

#define MAX_SIZE 30

int Map[MAX_SIZE][MAX_SIZE];
int MapSize;
int x_size;// size of block x
int y_size;// size of bloxk y
int t1, t2;

void gotoxy(int x, int y); 
void MakeMap(); // Functions for creating arrays
void PrintMap();
void SetBlocksize(); // Calculate the block size depending on the size of the array.
int  GetUniqueNumber(int x, int y); //finds candidate numbers to fit into a particular position in the array.
int  Menu();

int main()
{
    int ch, End = 1;

    srand((unsigned)time(NULL));
    End = Menu();

    while(End)
    {
        system("cls");
        MakeMap();
        PrintMap();
        ch = getch();
        if(ch == 27) End = Menu();
    }

    return 0;
}

void MakeMap()
{
    int i, j, num, cnt = 0, n = 0, progress = -1;
    int Cell[MAX_SIZE];

    memset(Map, 0, sizeof(Map));
    t1 = clock();
    for(i = 0, j = 0; i < MapSize; )
    {
        //Uncommented here shows the change in j value. But the output is getting slower.
        //gotoxy(0, 1);
        //printf("j = ");
        //for(n = 0; n < MapSize; n++) j >= n ? printf("%2d", n + 1) : printf("  ");

        //i shows the change in value. 
		//If you annotate to this part, the output will be faster because there is no output. 
        if(progress != i)
        {
            gotoxy(0, 0);
            printf("i = ");
            for(n = 0; n < MapSize; n++) i >= n ? printf("%2d", n + 1) : printf("  ");
            puts("");
            progress = i;
        }


        if(num = GetUniqueNumber(j, i))
        {
            Map[i][j] = num;
            ++j == MapSize ? i++, j = 0, cnt = 0 : 0;
        }

        // If there is no number to enter into the coordinates (j, i), 
		// initialize Map [i] and search it repeatedly by a fixed number.
        // If you annotate this part, it can take a long time or you may not find it.
        // The number of repetitions is the number found by experience. Too large or too small can take a lot of time.
        else if(++cnt < i * 50)
        {
            memset(Map[i], 0, sizeof(Map[i]));
            j = 0;
        }

        // If it finds it repeatedly, but it does not find it, it initializes to the first line of the block and searches again.
        // If you do not understand it well, keep a look at the change in j value and the change in i value.
        else 
        {
            while(i % y_size)
            {
                memset(Map[i--], 0, sizeof(Map[i]));
            }
            memset(Map[i], 0, sizeof(Map[i]));
            j = cnt = 0;
        }
    }
    //getch();
    t2 = clock();
}

int GetUniqueNumber(int x, int y)
{
    int i, j, col, row, idx = 0;
    int Cell[MAX_SIZE], Temp[MAX_SIZE + 1] = {0,};

    // Finds out which blocks are x, y coordinates.
    //  012 345 678
    //0 000 050 000
    //1 000 010 000
    //2 000 000 000

    //3 000 125 000
    //4 000 478 000
    //5 120 9?0 053

    //6 000 080 000
    //7 000 000 000
    //8 000 000 000
    //
	// If the numbers are placed as above and "?" To find the unique value of the position, 
	// find the start position of the block with the following formula
    // col = (4/3) * 3 = 1 * 3 = 3, row = (5/3) * 3 = 1 * 3 = 3
    col = (x / x_size) * x_size;
    row = (y / y_size) * y_size;

    for(i =  row; i < y_size + row; i++)
    {
        for(j = col; j < x_size + col; j++)
        {
            //Map 배열안의 숫자를 index로 하여 Temp배열을 1로 채운다.
            //The Temp array will look like this. {1,1,1,0,1,1,0,1,1,1}
            //                                                    0 1 2 3 4 5 6 7 8 9
            Temp[Map[i][j]] = 1;//for example, Temp[Map[i][j]] = Temp[Map[3][3]] = Temp[1] = 1
        }                       //Temp[Map[i][j]] = Temp[Map[3][4]] = Temp[2] = 1
    }

    for(i = 0; i < MapSize; i++)
    {
        //Check the rows and columns exactly as above and fill the Temp array with 1.
        //The result will be {1,1,1,1,1,1,0,1,1,1}
        Temp[Map[y][i]] = 1;
        Temp[Map[i][x]] = 1;
    }

    for(i = 1; i <= MapSize; i++)
    {
        // Find candidate numbers in the Temp array.
        // In the Temp array, the number of positions where 0 is present is the candidate number.
        if(!Temp[i]) Cell[idx++] = i;//Fill the candidate value from cell array 0, that is, the first array.
    }

    //If there is no candidate number, it returns 0. Otherwise, it randomly returns one of the candidate numbers.
    if(idx == 0) return idx;
    //Cell array contains candidate number as idx, so we select one randomly and return it.
    else return Cell[rand() % idx]; 
}

void PrintMap()
{
    int i, j;
    const char *str[] = {"  ", "１", "２", "３", "４", "５", "６", "７", "８", "９", "Ａ", "Ｂ", "Ｃ", "Ｄ", "Ｅ", "Ｆ", "Ｇ"};
    //const char *Num = "  ＡＢＣＤＥＦＧＨＩＪＫＬＭＮＯＰＱＲＳＴＵＶＷＸＹＺ";
    //const char *Num = "0ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    //system("cls");
    printf("----<ESC : Select Menu>----\n");
    for(i = 0; i < MapSize; i++)
    {
        for(j = 0; j < MapSize; j++)
        {
            //if(MapSize == 25) printf("%c%c", Num[Map[i][j] * 2],Num[Map[i][j] * 2 + 1]);
            MapSize < 25 ? printf("%s", str[Map[i][j]]) : printf("%2d ", Map[i][j]);
            if((j + 1) % x_size == 0) printf(" ");
            if((i + 1) % y_size == 0 && j == MapSize - 1) printf("\n");
        }
        printf("\n");
    }
    printf("Time = %0.3fsec\n", (t2 - t1)/1000.);
    printf("\nPress any key to continue\n");
}

int Menu()
{
    int num;

    printf("1 : Create  4 X  4 sudoku array\n");
    printf("2 : Create  6 X  6 sudoku array\n");
    printf("3 : Create  9 X  9 sudoku array\n");
    printf("4 : Create 12 X 12 sudoku array\n");
    printf("5 : Create 16 X 16 sudoku array\n");
    printf("6 : Create 25 X 25 sudoku array\n");
    printf("7 : Create 30 X 30 sudoku array\n");
    printf("8 : quit programm\n");

    while(1)
    {
        num = getch();
        if(num >= '1' && num <= '8') break;
        else if(num == 27) return 0;
    }

    switch(num)
    {
        case '1' : MapSize =  4; break;
        case '2' : MapSize =  6; break;
        case '3' : MapSize =  9; break;
        case '4' : MapSize = 12; break;
        case '5' : MapSize = 16; break;
        case '6' : MapSize = 25; break;
        case '7' : MapSize = 30; break;
        case '8' : return 0;
    }
    SetBlocksize();
    return 1;
}

void SetBlocksize()
{
 switch(MapSize)
 {
  case 4 :
   y_size = x_size = 2;
   break;
  case 6 :
   y_size = 2;
   x_size = 3;
   break;
  case 9 :
   y_size = x_size = 3;
   break;
  case 12 :
   y_size = 3;
   x_size = 4;
   break;
  case 16 :
   y_size = x_size = 4;
   break;
  case 25 :
   y_size = x_size = 5;
            system("mode con: lines=36 cols=90");//Resize console
   break;
  case 30 :
   y_size = 5;
   x_size = 6;
   system("mode con: lines=45 cols=100");
   break;
        default : exit(1);
 }
}

void gotoxy(int x, int y) 
{
    COORD XY = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), XY);
}

