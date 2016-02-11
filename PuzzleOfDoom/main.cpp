#include <iostream>  

#include "dataOperation.h"
#include "evolution.h"

using namespace std;

int main()
{
	// get the data in file: e2pieces.txt
	// saved in arrays of pieces
	// values of COLOR format by this order: UP DOWN LEFT RIGHT
	loadData();
	initBoard();
	cout << "Fitness after init is " << getFitness() << endl;
	saveData(0, getFitness());

	evolveForGlobal();

	// wait to show result
	cout << "PRESS ENTER TO QUIT" << endl;
	getchar();
	return 0;
}
