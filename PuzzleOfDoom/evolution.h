#pragma once
#ifndef __EVOLUTION_H__
#define __EVOLUTION_H__

#include <iostream>  
#include <cstdlib>
#include <time.h>

using namespace std;

#include "dataOperation.h"

#define		MUTATE_RATE				0.01
#define		MAX_GENERATION			100000		// MAX value of INT is 18446744073709551615
#define		GLOBAL_POP_SIZE			200
#define		IDIVADUAL_MOV_SIZE		5000
#define		MAX_MOV_CROSS_LENGTH	IDIVADUAL_MOV_SIZE / 5


class individual
{
public:
	int individual_mov[IDIVADUAL_MOV_SIZE][3];
	int fitness;
	int lifelength;

	void init();	// init random movement
	void getIndividualFitness();	// init board, then getFitness
	void crossOver(individual* f, individual* m);
	void mutate();
};

int getFitness();

int getFitSides(int x, int y);
int getBestToast(int x, int y);

void evolveForGlobal();
void getBetterPieceFromGlobal(int x, int y);

// movment format: id, para1, para2
void movSwapByID(int idSrc, int idDes);	// ID 0
void movChangeToast(int x, int y);		// ID 1

#endif
