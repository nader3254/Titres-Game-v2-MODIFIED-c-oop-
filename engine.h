#pragma once
#ifndef HEADER_H
#define HEADER_H

#include <Windows.h>
#include <math.h>
#include <time.h>
#include <chrono>  
#include <iostream>
#include <map>
#include <sstream>
#include <stdlib.h>
#include <vector>
#include <random>
#define MATH_PI 3.141592653

using namespace std;

//#include "MasterStructures.h"
#include "configscreen.h"


class engine
{
private:
  WorkArea *   work = new WorkArea("initialize graphics area"); 
  void  InitGameStat();
  void	ClearRelicsBuf();
  void	ClearDisplay();
  void	GenerateNewTetromino();  // init the first tetromino
  void	DrawBoundaries();
  void  UpdateTopInfo();
  void	UpdateBottomInfo();
  void  DrawTetromino();
  void RenderDidplay();
  void GameOver();
  void HandleKeyPress();
  void TryLowerTetromino();
  void SaveTetrominoRelics();
  void TryClearFullLineRelics();
  void RelicsRowShift(int row);
  void RelicsRowClearAnimation(int y);
  void GameSpeedAdjuster();
  void ShowMsg(stringstream const& t, int pos );



BOOL NoCollision(int dx, int dy, int dr);
 const int NUM_TETRO_TYPE = 8;
 const int TETRO_W        = 10;
 const int TETRO_H        = 4;
 const int SLEEP_TIME     = 30;      // milisecond pause between frames
 const int SPEED_UP_NUM   = 20;     // Speed increases for every 20 tetronimos
// Game Stats
 BOOL* buf_relics          ;      // holds tetromino relics
 double game_speed         ;
 int speed_count           ;
 int score                 ;
 int num_pieces            ;      // generated pieces so far
 int tetro_x               ;     // init xpos at the middle
 int tetro_y               ;    // init y pos on the top
 int cur_tetro_type        ;
 int cur_tetro_orientation ;
 BOOL vk_up_down           ;    // If VK_UP is pressed 




public:
    engine(/* args */);
    ~engine();
};





#endif