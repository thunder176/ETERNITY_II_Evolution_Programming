#include "evolution.h"

individual global_population[GLOBAL_POP_SIZE];

// get the number of all fitting inner sides 
int getFitness()
{
	int ret = 0;

	for (int i = 0, x = 0, y = 0; i < LWR * LWR; i++)
	{
		// every piece only compare its right side and down side to avoid duplicate
		// pieces in 4 corner and right side and down side of the board need to compare separately
		if (x == 0 && y == 0)
		{
			// left-up corner of the board
			// compare right and down side
			if (getRightSide(puzzle_board[x][y], toast[x][y]) == getLeftSide(puzzle_board[x][y + 1], toast[x][y + 1]))
				ret++;
			if (getDownSide(puzzle_board[x][y], toast[x][y]) == getUpSide(puzzle_board[x + 1][y], toast[x + 1][y]))
				ret++;
		}
		else if (x == 0 && y == LWR - 1)
		{
			// right-up corner of the board
			// compare down side
			if (getDownSide(puzzle_board[x][y], toast[x][y]) == getUpSide(puzzle_board[x + 1][y], toast[x + 1][y]))
				ret++;
		}
		else if (x == LWR - 1 && y == 0)
		{
			// left-down corner of the board
			// compare right side
			if (getRightSide(puzzle_board[x][y], toast[x][y]) == getLeftSide(puzzle_board[x][y + 1], toast[x][y + 1]))
				ret++;
		}
		else if (x == LWR - 1 && y == LWR - 1)
		{
			// right-down corner of the board
			// no side need to compare
		}
		else if (y == LWR - 1)
		{
			// pieces in right side, only compare down side
			if (getDownSide(puzzle_board[x][y], toast[x][y]) == getUpSide(puzzle_board[x + 1][y], toast[x + 1][y]))
				ret++;
		}
		else if (x == LWR - 1)
		{
			// pieces in down side, only compare right side
			if (getRightSide(puzzle_board[x][y], toast[x][y]) == getLeftSide(puzzle_board[x][y + 1], toast[x][y + 1]))
				ret++;
		}
		else
		{
			// other pieces, compare right side and down side
			if (getRightSide(puzzle_board[x][y], toast[x][y]) == getLeftSide(puzzle_board[x][y + 1], toast[x][y + 1]))
				ret++;
			if (getDownSide(puzzle_board[x][y], toast[x][y]) == getUpSide(puzzle_board[x + 1][y], toast[x + 1][y]))
				ret++;
		}

		y++;
		if (y == LWR)
		{
			y = 0;
			x++;
		}
	}
	return ret;
}

int getFitSides(int x, int y) // t: TOAST of piece
{
	int i = puzzle_board[x][y];	// ID of piece
	int t = toast[x][y];
	int ret = 0;
	int v_clr = 0;

	// NOTICE: if color value == 0 means it's in the corner or some sides
	//         then cannot compare with any exist piece
	v_clr = getUpSide(i, t);
	if (v_clr != 0 && v_clr == getDownSide(puzzle_board[x - 1][y], toast[x - 1][y]))
		ret++;

	v_clr = getDownSide(i, t);
	if (v_clr != 0 && v_clr == getUpSide(puzzle_board[x + 1][y], toast[x + 1][y]))
		ret++;

	v_clr = getLeftSide(i, t);
	if (v_clr != 0 && v_clr == getRightSide(puzzle_board[x][y - 1], toast[x][y - 1]))
		ret++;

	v_clr = getRightSide(i, t);
	if (v_clr != 0 && v_clr == getLeftSide(puzzle_board[x][y + 1], toast[x][y + 1]))
		ret++;

	return ret;
}

// This function will change the TOAST in position(x, y) to the best situation
// and return the fitting sides number
int getBestToast(int x, int y)
{
	int i = puzzle_board[x][y];		// ID of piece
	int tCurrent = toast[x][y];		// backup the TOAST of piece
	int fitCurrent = getFitSides(x, y);

	// The toast of pieces in corner or sides cannot be changed
	if (pieces[i][DOWN] == 0 || pieces[i][LEFT] == 0)
		return fitCurrent;

	// It's already the best toast
	if (fitCurrent == 4)
		return fitCurrent;

	for (int it = 0; it < 4; it++)
	{
		toast[x][y] = it;
		if (fitCurrent < getFitSides(x, y))	// find better one
		{
			tCurrent = it;
			fitCurrent = getFitSides(x, y);
		}
		else							// worse than before
		{
			toast[x][y] = tCurrent;
		}
	}

	return fitCurrent;
}

void getBetterPieceFromGlobal(int x, int y)
{
	int iCurrent;
	int fitCurrent;

	iCurrent = puzzle_board[x][y];
	fitCurrent = getFitSides(x, y);
	if (pieces[iCurrent][LEFT] == 0)
	{
		// This piece is in the corner part. ID: from 0 to 3
		// Toast cannot be changed
		for (int iChange = 0; iChange < 4; iChange++)
		{
			if (fitCurrent == 2)
				return;

			if (iChange == iCurrent)
				continue;

			int xChange, yChange;
			xChange = getPosXbyID(iChange);
			yChange = getPosYbyID(iChange);
			if (xChange == -1 || yChange == -1)
			{
				cout << ">>>SYSTEM ERROR: Pieces missing. ID is " << iChange << endl;
				exit(-2);
			}

			// if getBestToast(yChange, yChange) == 4 means the piece in position(yChange, yChange) 
			// is the best one in that position. Then we won't change.
			if (getFitSides(xChange, yChange) != 2)
			{
				// use iCurrent to backup the piece ID
				iCurrent = puzzle_board[x][y];
				puzzle_board[x][y] = iChange;
				if (getFitSides(x, y) >= fitCurrent)
				{
					// swap the 2 pieces
					fitCurrent = getFitSides(x, y);
					puzzle_board[xChange][yChange] = iCurrent;
				}
				else
				{
					puzzle_board[x][y] = iCurrent;
					fitCurrent = getFitSides(x, y);
				}
			}
		}
	}
	else if (pieces[iCurrent][DOWN] == 0)
	{
		// This piece is in the sides part. ID: from 4 to (LWR - 1) * 4 - 1
		// Toast cannot be changed
		for (int iChange = 4; iChange < (LWR - 1) * 4; iChange++)
		{
			if (fitCurrent == 3)
				return;

			if (iChange == iCurrent)
				continue;

			int xChange, yChange;
			xChange = getPosXbyID(iChange);
			yChange = getPosYbyID(iChange);
			if (xChange == -1 || yChange == -1)
			{
				cout << ">>>SYSTEM ERROR: Pieces missing. ID is " << iChange << endl;
				exit(-2);
			}

			// if getBestToast(yChange, yChange) == 4 means the piece in position(yChange, yChange) 
			// is the best one in that position. Then we won't change.
			if (getFitSides(xChange, yChange) != 3)
			{
				// use iCurrent to backup the piece ID
				iCurrent = puzzle_board[x][y];
				puzzle_board[x][y] = iChange;
				if (getFitSides(x, y) >= fitCurrent)
				{
					// swap the 2 pieces
					fitCurrent = getFitSides(x, y);
					puzzle_board[xChange][yChange] = iCurrent;
				}
				else
				{
					puzzle_board[x][y] = iCurrent;
					fitCurrent = getFitSides(x, y);
				}
			}
		}
	}
	else
	{
		// This piece is in the center part. ID: from (LWR - 1) * 4 to LWR * LWR - 1
		for (int iChange = (LWR - 1) * 4; iChange < LWR * LWR; iChange++)
		{
			if (fitCurrent == 4)
				return;

			if (iChange == iCurrent)
				continue;

			int xChange, yChange;
			xChange = getPosXbyID(iChange);
			yChange = getPosYbyID(iChange);
			if (xChange == -1 || yChange == -1)
			{
				cout << ">>>SYSTEM ERROR: Pieces missing. ID is " << iChange << endl;
				exit(-2);
			}

			// if getBestToast(yChange, yChange) == 4 means the piece in position(yChange, yChange) 
			// is the best one in that position. Then we won't change.
			if (getBestToast(xChange, yChange) != 4)
			{
				// use iCurrent to backup the piece ID
				iCurrent = puzzle_board[x][y];
				puzzle_board[x][y] = iChange;
				if (getBestToast(x, y) >= fitCurrent)
				{
					// swap the 2 pieces
					fitCurrent = getFitSides(x, y);
					puzzle_board[xChange][yChange] = iCurrent;
					getBestToast(xChange, yChange);
				}
				else
				{
					puzzle_board[x][y] = iCurrent;
					fitCurrent = getFitSides(x, y);
				}
			}
		}
		/*for (int ix = x, iy = y; ix != x && iy != y; )
		{
			if (fitCurrent == 4)
				return;

			iy++;
			if (iy == LWR - 1)
			{
				iy = 1;
				ix++;
				if (ix == LWR - 1)
					ix = 1;
			}

			// if getBestToast(ix, iy) == 4 means the piece in position(ix, iy)
			// is the best one in that position. Then we won't change.
			if (getBestToast(ix, iy) != 4)
			{
				// use iCurrent to backup the piece ID
				iCurrent = puzzle_board[x][y];
				int iChange = puzzle_board[ix][iy];
				puzzle_board[x][y] = iChange;
				if (getBestToast(x, y) > fitCurrent)
				{
					// swap the 2 pieces
					fitCurrent = getFitSides(x, y);
					puzzle_board[ix][iy] = iCurrent;
					getBestToast(ix, iy);
				}
				else
				{
					puzzle_board[x][y] = iCurrent;
					fitCurrent = getBestToast(x, y);
				}
			}
		}*/
	}
}

void evolveForGlobal()
{
	INT i = 0;
	time_t seconds;
	time(&seconds);
	srand((unsigned int)seconds);

	for (int ipop = 0; ipop < GLOBAL_POP_SIZE; ipop++)
	{
		global_population[ipop].init();
	}
	// MAX value of INT is 18446744073709551615
	for (int j = 1; j <= MAX_GENERATION; j++)
	{
		int fit_total = 0;
		int fit_avg = 0;
		for (int ipop = 0; ipop < GLOBAL_POP_SIZE; ipop++)
		{
			global_population[ipop].getIndividualFitness();
			fit_total += global_population[ipop].fitness;
		}

		fit_avg = fit_total / GLOBAL_POP_SIZE;
		for (int ipop = 0; ipop < GLOBAL_POP_SIZE; ipop++)
		{
			if (global_population[ipop].fitness >= fit_avg)
				global_population[ipop].lifelength++;
			else
				global_population[ipop].lifelength -= 2;
		}

		for (int ipop = 0; ipop < GLOBAL_POP_SIZE; ipop++)
		{
			// individual dead become children
			if (global_population[ipop].lifelength < 0)
			{
				int m, f;
				do
				{
					m = rand() % GLOBAL_POP_SIZE;
					f = rand() % GLOBAL_POP_SIZE;
				} while (m == f || global_population[m].fitness < fit_avg || global_population[f].fitness < fit_avg);
				global_population[ipop].crossOver(&global_population[f], &global_population[m]);
			}
		}

		int fit_best = 0;
		int i_best = 0;
		for (int ipop = 0; ipop < GLOBAL_POP_SIZE; ipop++)
		{
			if (global_population[ipop].fitness > fit_best)
			{
				fit_best = global_population[ipop].fitness;
				i_best = ipop;
			}
		}
		global_population[i_best].getIndividualFitness();

		cout << "After " << j << " generations, best fitness now is " << getFitness() << endl;
		if (j % 10 == 0)
			saveData(j, getFitness());

		for (int ipop = 0; ipop < GLOBAL_POP_SIZE; ipop++)
		{
			global_population[ipop].mutate();
		}

		// debug mode
		/*if (getFitness() == 2 * LWR * (LWR - 1))
		{
			cout << "Find a solution!" << endl;
			cout << "After " << j << " generations, fitness now is " << getFitness() << endl;
			saveData(j, getFitness());
			return;
		}*/
	}
}

void movSwapByID(int idSrc, int idDes)
{
	// This piece is in the corner part. ID: from 0 to 3
	// This piece is in the sides part. ID: from 4 to (LWR - 1) * 4 - 1
	// This piece is in the center part. ID: from (LWR - 1) * 4 to LWR * LWR - 1

	if ((idSrc < 4 && idDes < 4) || ((idSrc >= 4 && idSrc <= (LWR - 1) * 4 - 1) && (idDes >= 4 && idDes <= (LWR - 1) * 4 - 1)) ||
		((idSrc >= (LWR - 1) * 4 && idSrc <= LWR * LWR - 1) && (idDes >= (LWR - 1) * 4 && idDes <= LWR * LWR - 1)))
	{
		int xs, ys, xd, yd;

		xs = getPosXbyID(idSrc);
		ys = getPosYbyID(idSrc);

		xd = getPosXbyID(idDes);
		yd = getPosYbyID(idDes);

		puzzle_board[xs][ys] = idDes;
		puzzle_board[xd][yd] = idSrc;
	}
	else
	{
		cout << "SYSTEM ERROR: movSwapByID swap different kind of piece" << endl;
	}
}

void movChangeToast(int x, int y)
{
	int i = puzzle_board[x][y];		// ID of piece

	// The toast of pieces in corner or sides cannot be changed
	if (pieces[i][DOWN] == 0 || pieces[i][LEFT] == 0)
		cout << "SYSTEM ERROR: movChangeToast toast piece in side or corner" << endl;

	getBestToast(x, y);
}

void individual::init()
{
	time_t seconds;
	time(&seconds);
	srand((unsigned int)seconds);

	for (int i = 0; i < IDIVADUAL_MOV_SIZE; i++)
	{
		if (rand() % 2)
		{
			// movSwapByID
			this->individual_mov[i][0] = 0;
			int is, id;
			is = rand() % (LWR * LWR);
			if (is < 4)
			{
				do
				{
					id = rand() % 4;
				} while (id == is);
			}
			else if (is < (LWR - 1) * 4)
			{
				do
				{
					id = (rand() % ((LWR - 1) * 4 - 4)) + 4;
				} while (id == is);
			}
			else
			{
				do
				{
					id = (rand() % ((LWR * LWR - 1) - ((LWR - 1) * 4) + 1)) + (LWR - 1) * 4;
				} while (id == is);
			}
			this->individual_mov[i][1] = is;
			this->individual_mov[i][2] = id;
		}
		else
		{
			// movChangeToast
			this->individual_mov[i][0] = 1;
			int x, y;
			x = rand() % LWR;
			y = rand() % LWR;
			if (x == 0)
				x++;
			else if (x == LWR - 1)
				x--;
			if (y == 0)
				y++;
			else if (y == LWR - 1)
				y--;
			this->individual_mov[i][1] = x;
			this->individual_mov[i][2] = y;
		}
	}

	this->lifelength = (rand() % 6) + 5;
}

void individual::getIndividualFitness()
{
	initBoard();
	for (int i = 0; i < IDIVADUAL_MOV_SIZE; i++)
	{
		if (this->individual_mov[i][0] == 0)
		{
			movSwapByID(this->individual_mov[i][1], this->individual_mov[i][2]);
		}
		else if (this->individual_mov[i][0] == 1)
		{
			movChangeToast(this->individual_mov[i][1], this->individual_mov[i][2]);
		}
	}
	this->fitness = getFitness();
}

void individual::crossOver(individual* f, individual* m)
{
	time_t seconds;
	time(&seconds);
	srand((unsigned int)seconds);
	this->lifelength = (rand() % 6) + 5;

	bool MorF = true;
	int length = rand() % MAX_MOV_CROSS_LENGTH;
	for (int i = 0; i < IDIVADUAL_MOV_SIZE; i++)
	{
		if (MorF)
		{
			this->individual_mov[i][0] = f->individual_mov[i][0];
			this->individual_mov[i][1] = f->individual_mov[i][1];
			this->individual_mov[i][2] = f->individual_mov[i][2];
		}
		else
		{
			this->individual_mov[i][0] = m->individual_mov[i][0];
			this->individual_mov[i][1] = m->individual_mov[i][1];
			this->individual_mov[i][2] = m->individual_mov[i][2];
		}
		length--;
		if (length < 0)
		{
			MorF = !MorF;
			length = rand() % MAX_MOV_CROSS_LENGTH;
		}
	}
}

void individual::mutate()
{
	time_t seconds;
	time(&seconds);
	srand((unsigned int)seconds);

	for (int i = 0; i < IDIVADUAL_MOV_SIZE; i++)
	{
		if (((double)rand() / RAND_MAX) < MUTATE_RATE)
		{
			if (this->individual_mov[i][0] == 0)
			{
				// movSwapByID
				int is, id;
				is = rand() % (LWR * LWR);
				if (is < 4)
				{
					do
					{
						id = rand() % 4;
					} while (id == is);
				}
				else if (is < (LWR - 1) * 4)
				{
					do
					{
						id = (rand() % ((LWR - 1) * 4 - 4)) + 4;
					} while (id == is);
				}
				else
				{
					do
					{
						id = (rand() % ((LWR * LWR - 1) - ((LWR - 1) * 4) + 1)) + (LWR - 1) * 4;
					} while (id == is);
				}
				this->individual_mov[i][1] = is;
				this->individual_mov[i][2] = id;
			}
			else
			{
				// movChangeToast
				int x, y;
				x = rand() % LWR;
				y = rand() % LWR;
				if (x == 0)
					x++;
				else if (x == LWR - 1)
					x--;
				if (y == 0)
					y++;
				else if (y == LWR - 1)
					y--;
				this->individual_mov[i][1] = x;
				this->individual_mov[i][2] = y;
			}
		}
	}
}
