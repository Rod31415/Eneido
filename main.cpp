
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <winuser.h>
#include "dirent.h"
#include <string>
#include <fstream>
#include <iostream>
#include <sys/stat.h>
//#include <direct.h>

#define up 72
#define down 80
#define left 75
#define right 77
#define enter 13
#define esc 27
#define del 8

std::string LastFile;

int mouseX = 0, mouseY = 0;
HANDLE h;
bool ButtonPressed = true;
POINT p;
int colorBackground = 48;



void replace(std::string& s,
    char c1, char c2)
{

    for (int i = 0; i < s.length(); i++)
    {
        if (s[i] == c1)
            s[i] = c2;

    }
    //return s;
}


void mouseCoord() {
    GetCursorPos(&p);
    mouseX = p.x / 8.0;
    mouseY = (p.y - 23) / 16.0;
}

void gotoxy(unsigned int x, unsigned int y) {

	COORD Pos; Pos.Y = y; Pos.X = x;
	SetConsoleCursorPosition(h, Pos);
}

void background(unsigned int color)
{
    DWORD n,size;                   
    COORD coord = { 0 };              
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    GetConsoleScreenBufferInfo(h, &csbi);

    size = csbi.dwSize.X * csbi.dwSize.Y;
    SetConsoleTextAttribute(h, color);
    FillConsoleOutputCharacter(h, TEXT(' '), size, coord, &n);
    GetConsoleScreenBufferInfo(h, &csbi);
    FillConsoleOutputAttribute(h, csbi.wAttributes, size, coord, &n);
    
}

void rect( int x,int y,  int width,  int height,unsigned int color) {
    std::string str( width-1, ' ' );
    SetConsoleTextAttribute(h, color);
    for (int i = 0; i < height; i++) { 
        gotoxy(x, y +i); printf("%s", str.c_str());
    }
}

class Buttons {

private:
    int x=0, y=0, width=0, height=0;
    std::string name;
    
public:
    int Selected=false ;
    int  color = 115;


    void init(unsigned int x, unsigned int y, unsigned int width, unsigned int height,std::string name) {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
        this->Selected = false;
        this->name = name;
        this->color = 115;
    }
    
    void drawButton() {
        SetConsoleTextAttribute(h, color);
        gotoxy(this->x, this->y);
        printf("%s", name.c_str());
    }

    void collision(int mx,  int my) {
        if (mx >= this->x && mx < this->x + this->width && my >= this->y && my < this->y + this->height) { this->color = 15; this->Selected = true; }
        else { this->color = 115; this->Selected = false; }
    }

};

bool collisionBtn(int x,int y,int width,int height) {
    if (mouseX >= x && mouseX < x + width && mouseY >= y && mouseY < y + height) { return true; }
    else { return false; }
}

std::string convert(char* a)
{
    std::string s(a);
    return s;
}

//void compile()



//std::string files[100];
class File {
private:
    std::fstream file;
    std::ifstream fileIn;
    std::ofstream fileOut;
    std::string *filestr;
    std::string files[100];
    std::string fileSelected;
    std::string fileName;
    int totalHeight = 0;
public:
    int heightString;
    void init() {
        this->filestr = new std::string[100];
    this->fileSelected = "a.txt";
    this->totalHeight = 0;
    this->fileName;
    }
    void WriteFile(int x,int y,int width,int height) {
        SetConsoleTextAttribute(h, 15);
        
        if (fileIn.is_open()) {
            gotoxy( x+width, y ); printf("%s", this->fileName.c_str());
            
            
            for (int i = 0; i <height; i++) {
                gotoxy(1 + x, y + i); printf("%s", this->filestr[i].c_str());

            }
            SetConsoleTextAttribute(h, 118); gotoxy(x + width, y+2);
            printf("Guardar");
            SetConsoleTextAttribute(h, 15);
            bool e = true;
            int row = 0;
            int col = 0;
            bool clicked = false;
            bool press = false;
            while (e) {
                if (GetAsyncKeyState(VK_LBUTTON) < 0) {
                    if (!press) { press = true; clicked = true; }
                    else { clicked = false; }
                }
                else { press = false; }
                if(clicked){
                    mouseCoord();
                    if (collisionBtn(x + width, y + 2, 7, 1)) {
                        e = false;
                        ButtonPressed = false;
                        SetConsoleTextAttribute(h, 128);
                        gotoxy(x+width, y+2);
                        printf("Guardar");
                        SetConsoleTextAttribute(h, 15);
                        break;
                    }
                    else if (collisionBtn(x, y, x + width, y + height)) {
                        row = mouseY - y;
                        if (this->filestr[row].length() < mouseX - x) { col = this->filestr[row].length()+1; }
                        else { col = mouseX-x; }
                        gotoxy(col + x, row + y);
                    }
                }
                if (_kbhit()) {

                        char letter = _getch();

                        if (letter == left && col > 0) { col--; }
                        else if (letter == right && col < this->filestr[row].length() + 1) { col++; }
                        else if (letter == down && row < height) { row++; if (col > this->filestr[row].length())col = this->filestr[row].length() + 1; }
                        else if (letter == up && row > 0) { row--; if (col > this->filestr[row].length())col = this->filestr[row].length() + 1; }
                        else if (letter == 8 && col > 1) {
                            this->filestr[row].erase(col - 2, 1);  col--;
                            SetConsoleTextAttribute(h, 15);
                            gotoxy(1 + x, y + row); printf("%s       ", this->filestr[row].c_str());
                        }
                        else if (letter != -32 && letter != 77 && letter != 8) {

                            gotoxy(x + width, y + 4); printf("%i   ", letter);
                            std::string correctletter = "";
                            correctletter = (char)letter;
                            this->filestr[row].insert(col - 1, correctletter);
                            col++;
                            SetConsoleTextAttribute(h, 15);
                            gotoxy(1 + x, y + row); printf("%s", this->filestr[row].c_str());
                        }
                        gotoxy(col + x, row + y);
                    
                    
                }
                
            }
            fileIn.close();
            fileOut.open(this->fileSelected);
            for (int d = 0; d < height; d++) {
                fileOut << this->filestr[d] << "\n";
            }
            fileOut.close();
            fileIn.open(this->fileSelected);
            //Sleep(100);
            gotoxy(x+width/2, y + height); printf("Se ha guardado");








        }
        else { gotoxy(x, y + 1); printf("No hay ningun archivo abierto"); }
        

    }
    

    void openFile(int x, int y, int heightext,std::string dir) {

        int maxFiles = 100;
        int maxFilesPerScroll = 40;
        int scrollIndex = 0;
        int widthWindow = 120;

        int l = 0;
        char* ptr1, * ptr2;
        int  searched1,searched2;
        DIR* dr;
        struct stat info;
        struct dirent* en;
        
        
         replace(dir,92,47);
        dir = dir + "/";
        dr = opendir(dir.c_str());
        char t[100] = "txt";
        char f[100] = "";
        

        rect(x, y, widthWindow, maxFilesPerScroll+2, 31);
        if (dr) {

            while ((en = readdir(dr)) != NULL&&l < maxFiles) {

                ptr1 = strtok(en->d_name, ".");
                ptr2 = strtok(NULL, ".");
                if (ptr2 != NULL)
                {
                    searched1 = strcmp(ptr2, t);
                    if (searched1 == 0)
                    {

                        this->files[l] = (convert(en->d_name) + ".txt");
                        l++;
                    }
                }
                else if(convert(en->d_name)!=".."&& convert(en->d_name) != ".") {
                    this->files[l] = convert(en->d_name);
                    l++;
                }
            }
            closedir(dr);


        }

        

        std::string foulder=dir;
        bool ClickedLButton = false,ButtonDown=false;


        std::string fould =foulder;
        gotoxy(x + 1, y+1);
        SetConsoleTextAttribute(h, 115);
        printf("%s", dir.c_str());
        SetConsoleTextAttribute(h, 31);
        printf(" %c", 188);

        if (l > 0) {
            int n = maxFilesPerScroll;
            if (maxFilesPerScroll > l)n = l;
            for (int i = 0; i < n - 1; i++) {
                gotoxy(x + 2, y + i + 2);
                printf("%c%c> %s", 204, 205, this->files[i + scrollIndex].c_str());

            }
            gotoxy(x + 2, y + n + 1);
            printf("%c%c> %s", 200, 205, this->files[n - 1 + scrollIndex].c_str());
        }
        Sleep(200);
        bool key = false;
        bool ren = false;
        while (true) {


            if (GetAsyncKeyState(VK_UP)&& scrollIndex > 0)
            {
                scrollIndex--;
                Sleep(10);
                key = true;

            }
            else if (GetAsyncKeyState(VK_DOWN)&& (maxFilesPerScroll + scrollIndex < l))
            {
                
                scrollIndex++;
                Sleep(10);
                key = true;

            }



            if (key || ren) {

                float scrollBarInit = ((float)scrollIndex / l) * maxFilesPerScroll;
                float scrollBarLength = ((float)maxFilesPerScroll / l) * maxFilesPerScroll;


               if (l > 0) {
                   rect(x, y+2, widthWindow, maxFilesPerScroll , 31);
                   if (scrollBarLength > maxFilesPerScroll) {
                       scrollBarLength = maxFilesPerScroll;
                   }
                       SetConsoleTextAttribute(h, 240);
                       for (int i = 0; i < scrollBarLength; i++) {
                           gotoxy(x, y + i + 2 + scrollBarInit); printf(" ");
                       }
                       SetConsoleTextAttribute(h, 31);
                   
                   int n = maxFilesPerScroll;
                   if (maxFilesPerScroll > l)n = l;
                   for (int i = 0; i < n - 1; i++) {
                       gotoxy(x + 2, y + i + 2);
                       printf("%c%c> %s", 204, 205, this->files[i + scrollIndex].c_str());

                   }
                   gotoxy(x + 2, y + n + 1);
                   printf("%c%c> %s", 200, 205, this->files[n - 1 + scrollIndex].c_str());
               }
               ren = false;
               key = false;

           }

            mouseCoord();
            if (GetKeyState(VK_LBUTTON)>0) {
                if (!ButtonDown) { ClickedLButton = true; }
                if (ButtonDown) { ClickedLButton = false; }
                ButtonDown = true;
            }
            else { ButtonDown = false; }
            
            if(ClickedLButton){

                if (mouseY - y - 2 >-1&& mouseY - y - 2 < l) {
                    

                    this->fileSelected = foulder + this->files[mouseY - y - 2 + scrollIndex];

                    if (this->fileSelected.find(".txt") != std::string::npos) {



                        this->fileName = this->files[mouseY - y - 2 + scrollIndex];
                        gotoxy(x + 1, y);
                        printf("%s", this->fileName.c_str());

                        break;
                    }
                    else {
                        scrollIndex = 0;
                        ren = true;
                        foulder = this->fileSelected+"/";

                        dr = opendir(foulder.c_str());
                        char s[100] = "txt";
                        int searched;

                        rect(x, y, widthWindow, maxFilesPerScroll+2, 31);

                        l = 0;
                        if (dr) {

                            while ((en = readdir(dr)) != NULL&&l < maxFiles) {

                                ptr1 = strtok(en->d_name, ".");
                                ptr2 = strtok(NULL, ".");
                                if (ptr2 != NULL)
                                {
                                    searched = strcmp(ptr2, s);
                                    if (searched == 0)
                                    {

                                        this->files[l] = (convert(en->d_name) + ".txt");
                                        l++;
                                    }
                                }
                                else if (convert(en->d_name) != ".." && convert(en->d_name) != ".") {
                                    this->files[l] = convert(en->d_name);
                                    l++;
                                }
                            }
                            closedir(dr);


                        }
                        std::string fould = foulder;
                        gotoxy(x + 1, y + 1);
                        SetConsoleTextAttribute(h, 115);
                        printf("%s", foulder.c_str());
                        SetConsoleTextAttribute(h, 31);
                        printf(" %c", 188);

                        if (l > 0) {
                            int n = maxFilesPerScroll;
                            if (maxFilesPerScroll > l)n = l;
                            for (int i = 0; i < n - 1; i++) {
                                gotoxy(x + 2, y + i + 2);
                                printf("%c%c> %s", 204, 205, this->files[i+scrollIndex].c_str());

                            }

                            gotoxy(x + 2, y + n + 1);
                            printf("%c%c> %s", 200, 205, this->files[n - 1 + scrollIndex].c_str());

                        }
                    }



                }
                else if(collisionBtn(x+2+foulder.length(),y+1,1,1)){
                    scrollIndex = 0;
                    ren = true;
                    int n = 0;
                    for (int i = foulder.length() - 1; i > 0; i--) {
                        if (foulder[i] == 92 || foulder[i] == 47) {
                            if (n == 0) {
                            foulder = foulder.substr(0, i);
                            }
                            else {
                                foulder = foulder.substr(0, i+1);
                            }
                   n++;
                        }
                        if (n == 2) { break; }
                    }
                    



                    dr = opendir(foulder.c_str());
                    char s[100] = "txt";
                    int searched;

                    rect(x, y, widthWindow, maxFilesPerScroll+2, 31);

                    l = 0;
                    if (dr) {

                        while ((en = readdir(dr)) != NULL&&l< maxFiles) {

                            ptr1 = strtok(en->d_name, ".");
                            ptr2 = strtok(NULL, ".");
                            if (ptr2 != NULL)
                            {
                                searched = strcmp(ptr2, s);
                                if (searched == 0)
                                {

                                    this->files[l] = (convert(en->d_name) + ".txt");
                                    l++;
                                }
                            }
                            else if (convert(en->d_name) != ".." && convert(en->d_name) != ".") {
                                this->files[l] = convert(en->d_name);
                                l++;
                            }
                        }
                        closedir(dr);


                    }
                    std::string fould = foulder;
                    gotoxy(x + 1, y + 1);
                    SetConsoleTextAttribute(h, 115);
                    printf("%s", foulder.c_str());
                    SetConsoleTextAttribute(h, 31);
                    printf(" %c", 188);

                    if (l > 0) {
                        int n = maxFilesPerScroll;
                        if (maxFilesPerScroll > l){n = l;}
                        for (int i = 0; i < n - 1; i++) {
                            gotoxy(x + 2, y + i + 2);
                            printf("%c%c> %s", 204, 205, this->files[i + scrollIndex].c_str());

                        }

                        gotoxy(x + 2, y + n + 1);
                        printf("%c%c> %s", 200, 205, this->files[n - 1 + scrollIndex].c_str());

                    }
                }

               
                
            }


        }
       
        this->totalHeight = 0;

        this->filestr = new std::string[100];
        fileIn.open(this->fileSelected);

        while (!fileIn.eof() && this->totalHeight < heightext) {
            std::getline(fileIn, this->filestr[this->totalHeight]);
            ++this->totalHeight;
        }
            
        
    }

    void newFile(int x,int y) {
        std::string name="";
        gotoxy(x, y);
        SetConsoleTextAttribute(h,115);
        printf("Ingresa el nombre del archivo");
        gotoxy(x+1, y + 1);
        std::getline(std::cin,name);
        Sleep(50);
        std::ofstream o(name);
        fileIn.open(name);
        rect(x, y, 50, 10,115);

    }
};


const int TotalButton = 2;
Buttons b[TotalButton];
Buttons subbFile[3];
Buttons subbProgram[2];
File actualfile;

void FileAction(int n) {
    mouseCoord();
    
    int t = 0;
    for (int i = 0; i < n; i++) {
        subbFile[i].collision(mouseX, mouseY);
        
        if (subbFile[i].Selected) {
            t++;
            switch (i) {
            case 0: actualfile.newFile(10, 10); break;
            case 1:  char cwd[256]; LastFile = "C:" + (char)47;/*_getcwd(cwd, sizeof(cwd));*/ if (LastFile == "") { LastFile = std::string(cwd); } actualfile.openFile(15, 5, 100, "C:");
                break;
            }
            break;
        }
    }
   if(t==0){
        ButtonPressed = true;  background(colorBackground); for (int i = 0; i < TotalButton; i++) { b[i].Selected = false; b[i].drawButton(); }
    }
}

void ProgramAction(int n) {
    mouseCoord();
    int t = 0;
    for (int i = 0; i < n; i++) {
        subbFile[i].collision(mouseX, mouseY);

        if (subbFile[i].Selected) {
            t++;
            switch (i) {
            //case 0: actualfile.newFile(10, 10); break;
            //case 1: actualfile.openFile(15, 5, 100); break;
            }
        }
    }

    if (t == 0) {
        ButtonPressed = true;  background(colorBackground); for (int i = 0; i < TotalButton; i++) { b[i].Selected = false; b[i].drawButton(); }
    }

}

void functions(int selected) {
    
    bool ButtonDown = true;
    bool clicked = false;

    switch (selected)
    {

    case 1:
            rect(0, 1, 10, 10, 14);
            for (int i = 0; i < 3; i++) {
                subbFile[i].color = 15;
                subbFile[i].drawButton();
            }
            for (int j = 0; j < TotalButton; j++) { b[j].drawButton(); }
            
            while (ButtonPressed==false) {
               
                if (GetAsyncKeyState(VK_LBUTTON) < 0) {
                    if (!ButtonDown) { ButtonDown = true; clicked = true; }
                    else { clicked = false; }
                }
                else {
                    ButtonDown = false;
                }if (clicked) {
                    FileAction(3);
                }

            }
            
            
        break;
    case 2:
            rect(0, 1, 14, 10, 14);
            for (int i = 0; i < 2; i++) {
                subbProgram[i].color = 15;
                subbProgram[i].drawButton();
            }
            for (int j = 0; j < TotalButton; j++) { b[j].drawButton(); }
            while (!ButtonPressed) {
                if (GetAsyncKeyState(VK_LBUTTON) < 0) {

                    //ProgramAction(2);
                }
            }
                break;
        
    }
    
    
}





int zoneX=15, zoneY=5, zoneWidth=60, zoneHeight = 40;

int main(int argc, char argv[]) {

    h = GetStdHandle(STD_OUTPUT_HANDLE);

    HANDLE hInput = CreateFileW(L"CONIN$", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    DWORD prev_mode;
    GetConsoleMode(hInput, &prev_mode);
    SetConsoleMode(hInput, ENABLE_EXTENDED_FLAGS |    (prev_mode & ~ENABLE_QUICK_EDIT_MODE));

    ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);


    CONSOLE_SCREEN_BUFFER_INFO scrBufferInfo;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &scrBufferInfo);
    short winWidth = scrBufferInfo.srWindow.Right - scrBufferInfo.srWindow.Left + 1;
    short winHeight = scrBufferInfo.srWindow.Bottom - scrBufferInfo.srWindow.Top + 1;
    short scrBufferWidth = scrBufferInfo.dwSize.X;
    short scrBufferHeight = scrBufferInfo.dwSize.Y;
    COORD newSize;
    newSize.X = scrBufferWidth;
    newSize.Y = winHeight;
    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), newSize);


    actualfile.init();
    int espacio = 0;
        b[0].init(espacio, 0, 8, 1, "Archivos");
        espacio += 9;
        b[1].init(espacio, 0, 8, 1, "Opciones");



        subbFile[0].init(1, 1, 5, 1, "Nuevo");
        subbFile[1].init(1, 2, 5, 1, "Abrir");
        subbFile[2].init(1, 3, 8, 1, "Imprimir");

        subbProgram[0].init(1, 1, 12, 1, "Personalizar");
        subbProgram[1].init(1, 2, 5, 1, "Abrir");


        bool ButtonDown = false;
        bool clicked = false;
    background(colorBackground);
    actualfile.WriteFile(zoneX, zoneY, zoneWidth, zoneHeight);
    for (int i = 0; i < TotalButton; i++) {
        b[i].drawButton();
    }
    while (true) {
        //Sleep(10);
        if (GetAsyncKeyState(VK_LBUTTON) < 0 && ButtonPressed) {
            if (ButtonDown == false) { ButtonDown = true; clicked = true; }
            else { clicked = false; }


        }
        else {
            ButtonDown = false;
        }
        if(clicked){
            mouseCoord();
            background(colorBackground);
            
            for (int i = 0; i < TotalButton; i++) {
                
                
                b[i].collision(mouseX, mouseY);
                if (b[i].Selected) {
                    ButtonPressed = false; 
                    gotoxy(50, 0);
                    printf("%i", i);
                    functions(i + 1);
                   
                }
                
                
            }
            
            for (int j = 0; j < TotalButton; j++) { b[j].drawButton(); }
            rect(zoneX, zoneY, zoneWidth, zoneHeight,15);
            actualfile.WriteFile(zoneX, zoneY, zoneWidth, zoneHeight);
            gotoxy(50, 1);
            printf("%i", clicked);
            clicked = false;
            ButtonPressed = true;
            
        }

    }
	
}
