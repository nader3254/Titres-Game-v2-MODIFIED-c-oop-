
#ifndef CONFIG_SCREEN_H
#define CONFIG_SCREEN_H

#include "configscreen.h"
#include <Windows.h>
#include <math.h>
#include <time.h>
#include <chrono>  
#include <iostream>
#include <map>
#include <sstream>
#include <stdlib.h>
#include <vector>
#include <conio.h>
#define MATH_PI 3.141592653

using namespace std;


//#include "MasterStructures.h"

#ifndef __cplusplus	
      extern "c"{
#endif 



 class WorkArea
{
private:

    void CmdSettings();
    void Tetrominoes();
    void CreatCmdBuffer();
    void DisableResize();
    void FixedSize();
    void DisableCursorBlinking();
    void StartScreen();
    void RenderDidplay();
    void ClearDisplayBuf();
    void HandleNormalExit();




public:
    WorkArea(/* args */);
    WorkArea(string s);
    ~WorkArea();



 HANDLE console_screen;
 const int CONSOLE_W   = 44;
 const int CONSOLE_H   = 44;
 DWORD chars_written   ;
 char* buf_display     ;
 map<char, vector<string>> tetromino_repo;
 BOOL* buf_relics     ;  

enum TetroType { I, O, Z, T, L, S, J, Savior };
string     window_title         = "Tetrisconsol - nader";
SMALL_RECT min_window_size   = { 0, 0, (short)1, (short)1 };
SMALL_RECT window_size       = { 0, 0, (short)(CONSOLE_W - 1),(short)(CONSOLE_H - 1) };
    

};

#ifndef __cplusplus	
        }
#endif 


#endif //CONFIG_SCREEN_H