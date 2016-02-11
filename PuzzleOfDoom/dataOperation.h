#pragma once
#ifndef __DATA_OPERATION_H__
#define __DATA_OPERATION_H__

#include <iostream>  
#include <fstream>  
#include <cstdlib>
#include <time.h>

using namespace std;

#define		INT		unsigned __int64

#define		UP		0
#define		DOWN	1
#define		LEFT	2
#define		RIGHT	3

// Debug mode switch
//#define		LWR		4
#define		LWR		16				// length and width range, 4 for debug, 16 for real situation

extern int pieces[LWR * LWR][4];	// 0 - 3 is the pieces in the corner
									// 4 - 59 is the pieces in the sides
									// 60 - 255 is the pieces in the middle part
									// values of COLOR format by this order: UP DOWN LEFT RIGHT

extern int puzzle_board[LWR][LWR];	// store the ID of pieces in every POSITION(x and y) of the board
extern int toast[LWR][LWR];			// store the toast for every piece by POSITION(x and y)
									// means which side is the upside 0-UP, 1-DOWN, 2-LEFT, 3-RIGHT

extern int getFitSides(int x, int y);

void loadData();
void saveData(INT generation, int fitness);
void saveIslandData(int x, int y, int r, int fitness);

void findNextPiece(int x, int y);
bool getFromRestPiece(int x, int y);
void initBoard();
void initSideToast();
void initRestBoard();

int getUpSide(int i, int t);		// get color value of upside by i(the id of pieces) and t(the toast of that piece)
int getDownSide(int i, int t);		// get color value of downside by i(the id of pieces) and t(the toast of that piece)
int getLeftSide(int i, int t);		// get color value of leftside by i(the id of pieces) and t(the toast of that piece)
int getRightSide(int i, int t);		// get color value of rightside by i(the id of pieces) and t(the toast of that piece)

int getPosXbyID(int id);
int getPosYbyID(int id);


#endif
