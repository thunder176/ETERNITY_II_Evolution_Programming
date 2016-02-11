#include "dataOperation.h"

int pieces[LWR * LWR][4];				// 0 - 3 is the pieces in the corner
										// 4 - 59 is the pieces in the sides
										// 60 - 255 is the pieces in the middle part
										// values of COLOR format by this order: UP DOWN LEFT RIGHT
int isPieceUsed[LWR * LWR] = { 0 };	// 0 - unused, 1 - been used

int puzzle_board[LWR][LWR];				// store which piece in which place of the board
int toast[LWR][LWR];					// toast for every piece

void loadData()
{
	fstream fDataSrc, fBestRecord;

	// Debug mode switch
	//fDataSrc.open("debug_e2pieces.txt", ios::in);
	fDataSrc.open("e2pieces.txt", ios::in);

	if (!fDataSrc.is_open())
	{
		cout << "SYSTEM ERROR: Cannot open the data file." << endl;
		exit(-3);
	}
	for (int i = 0, j = 0; !fDataSrc.eof() && i < 256; )
	{
		fDataSrc >> pieces[i][j];

		//cout << pieces_corner[i][j] << " ";
		j++;
		if (j == 4)
		{
			j = 0;
			i++;
			//cout << endl;
		}
	}
	fDataSrc.close();

	/* test for getting side value
	cout << getUpSide(5, 0) << " ";
	cout << getDownSide(5, 0) << " ";
	cout << getLeftSide(5, 0) << " ";
	cout << getRightSide(5, 0) << endl;

	cout << getUpSide(5, 1) << " ";
	cout << getDownSide(5, 1) << " ";
	cout << getLeftSide(5, 1) << " ";
	cout << getRightSide(5, 1) << endl;

	cout << getUpSide(5, 2) << " ";
	cout << getDownSide(5, 2) << " ";
	cout << getLeftSide(5, 2) << " ";
	cout << getRightSide(5, 2) << endl;

	cout << getUpSide(5, 3) << " ";
	cout << getDownSide(5, 3) << " ";
	cout << getLeftSide(5, 3) << " ";
	cout << getRightSide(5, 3) << endl;*/
}

void saveData(INT generation, int fitness)
{
	fstream fout;
	char filename[1024] = { 0 };
	sprintf_s(filename, "D:/Software Engineering/Thunder_workspace/visual studio 2015/Projects/PuzzleOfDoom/result/result_%lld_%d.txt",
		generation, fitness);

	// Debug mode switch
	//fout.open("D:/Software Engineering/Thunder_workspace/visual studio 2015/Projects/PuzzleOfDoom/result/debug.txt", ios::app);
	fout.open(filename, ios::app);
	if (!fout.is_open())
	{
		cout << "SYSTEM ERROR: Cannot open the reault file." << endl;
		exit(-3);
	}

	fout << "After " << generation << " generations, fitness now is " << fitness << endl;
	fout << "The pieces ID in the board:" << endl;
	for (int i = 0, x = 0, y = 0; i < LWR * LWR; i++)
	{
		fout << puzzle_board[x][y] << "\t";
		y++;
		if (y == LWR)
		{
			y = 0;
			x++;
			fout << endl;
		}
	}

	fout << "The TOAST of pieces in the board:" << endl;
	for (int i = 0, x = 0, y = 0; i < LWR * LWR; i++)
	{
		fout << toast[x][y] << "\t";
		y++;
		if (y == LWR)
		{
			y = 0;
			x++;
			fout << endl;
		}
	}

	fout << "The fit side of every piece in the board:" << endl;
	for (int x = 0, y = 0; x < LWR; )
	{
		fout << getFitSides(x, y) << "\t";

		y++;
		if (y == LWR)
		{
			y = 0;
			x++;
			fout << endl;
		}
	}

	fout << endl << endl;

	fout.close();
}

void saveIslandData(int x, int y, int r, int fitness)
{
	fstream fout;
	char filename[1024] = { 0 };
	sprintf_s(filename, "D:/Software Engineering/Thunder_workspace/visual studio 2015/Projects/PuzzleOfDoom/result/result_island_%d.txt", fitness);

	// Debug mode switch
	//fout.open("D:/Software Engineering/Thunder_workspace/visual studio 2015/Projects/PuzzleOfDoom/result/island_debug.txt", ios::app);
	fout.open(filename, ios::app);
	if (!fout.is_open())
	{
		cout << "SYSTEM ERROR: Cannot open the reault file." << endl;
		exit(-3);
	}

	fout << "The island ( " << x << ", " << y << " )fitness is " << fitness << endl;
	fout << "The pieces ID in the island:" << endl;
	for (int i = 0, ix = x, iy = y; i < r * r; i++)
	{
		fout << puzzle_board[ix][iy] << "\t";
		iy++;
		if ((iy - y) == r)
		{
			iy = y;
			ix++;
			fout << endl;
		}
	}

	fout << "The TOAST of pieces in the island:" << endl;
	for (int i = 0, ix = x, iy = y; i < r * r; i++)
	{
		fout << toast[ix][iy] << "\t";
		iy++;
		if ((iy - y) == r)
		{
			iy = y;
			ix++;
			fout << endl;
		}
	}

	fout << "The fit side of every piece in the island:" << endl;
	for (int i = 0, ix = x, iy = y; i < r * r; i++)
	{
		fout << getFitSides(ix, iy) << "\t";
		iy++;
		if ((iy - y) == r)
		{
			iy = y;
			ix++;
			fout << endl;
		}
	}

	fout << endl << endl;

	fout.close();
}

void findNextPiece(int x, int y)
{
	if (x == 0 && y == 0)
	{
		// left-up corner of the board
		// have returned to start point now
		// search from top to bottom then
		findNextPiece(x + 1, y);
	}
	else if (x == 0 && y == LWR - 1)
	{
		// right-up corner of the board
		if (getFromRestPiece(x, y))
			findNextPiece(x + 1, y);
		else
			return;
	}
	else if (x == LWR - 1 && y == 0)
	{
		// left-down corner of the board
		if (getFromRestPiece(x, y))
			findNextPiece(x, y + 1);
		else
			return;
	}
	else if (x == LWR - 1 && y == LWR - 1)
	{
		// right-down corner of the board
		getFromRestPiece(x, y);
		return;
	}
	else if (y == LWR - 1)
	{
		// pieces in right sides
		if (getFromRestPiece(x, y))
			findNextPiece(x + 1, y);
		else
			return;
	}
	else if (x == LWR - 1)
	{
		// pieces in down sides
		if (getFromRestPiece(x, y))
			findNextPiece(x, y + 1);
		else
			return;
	}
	else if (x == 0)
	{
		// pieces in up sides
		if (getFromRestPiece(x, y))
		{
			findNextPiece(x, y + 1);
			findNextPiece(x + 1, y);
		}
		else
			return;
	}
	else if (y == 0)
	{
		// pieces in left sides
		if (getFromRestPiece(x, y))
			findNextPiece(x + 1, y);
		else
			return;
	}
	else
	{
		// other pieces in the center
		if (getFromRestPiece(x, y))
		{
			findNextPiece(x, y + 1);
			findNextPiece(x + 1, y);
		}
		else
			return;
	}
}

bool getFromRestPiece(int x, int y)
{
	int iCornor = 0;				// id: from 0 to 3
	int iSide = 4;					// id: from 4 to ((LWR - 1) * 4 - 1)
	int iCenter = (LWR - 1) * 4;	// id: from (LWR - 1) * 4  to LWR * LWR

	if (puzzle_board[x][y] != 0)
	{
		// already have a piece in this position
		return false;
	}

	if (x == 0 && y == 0)
	{
		// left-up corner of the board
		// cannot reach this position
		cout << "SYSTEM ERROR: DFS error." << endl;
		exit(-6);
	}
	else if (x == 0 && y == LWR - 1)
	{
		// right-up corner of the board
		toast[x][y] = DOWN;

		for (iCornor = 0; iCornor < 4; iCornor++)
		{
			if (isPieceUsed[iCornor])
				continue;
			if (getLeftSide(iCornor, toast[x][y]) == getRightSide(puzzle_board[x][y - 1], toast[x][y - 1]))
			{
				puzzle_board[x][y] = iCornor;
				isPieceUsed[iCornor] = 1;
				return true;
			}
		}
		return false;
	}
	else if (x == LWR - 1 && y == 0)
	{
		// left-down corner of the board
		toast[x][y] = UP;

		for (iCornor = 0; iCornor < 4; iCornor++)
		{
			if (isPieceUsed[iCornor])
				continue;
			if (getUpSide(iCornor, toast[x][y]) == getDownSide(puzzle_board[x - 1][y], toast[x - 1][y]))
			{
				puzzle_board[x][y] = iCornor;
				isPieceUsed[iCornor] = 1;
				return true;
			}
		}
		return false;
	}
	else if (x == LWR - 1 && y == LWR - 1)
	{
		// right-down corner of the board
		toast[x][y] = RIGHT;

		for (iCornor = 0; iCornor < 4; iCornor++)
		{
			if (isPieceUsed[iCornor])
				continue;
			if (puzzle_board[x - 1][y] != 0 && getUpSide(iCornor, toast[x][y]) == getDownSide(puzzle_board[x - 1][y], toast[x - 1][y]))
			{
				puzzle_board[x][y] = iCornor;
				isPieceUsed[iCornor] = 1;
				return true;
			}
			if (puzzle_board[x][y - 1] != 0 && getLeftSide(iCornor, toast[x][y]) == getRightSide(puzzle_board[x][y - 1], toast[x][y - 1]))
			{
				puzzle_board[x][y] = iCornor;
				isPieceUsed[iCornor] = 1;
				return true;
			}
		}
		return false;
	}
	else if (y == LWR - 1)
	{
		// pieces in right sides
		toast[x][y] = RIGHT;

		for (iSide = 4; iSide < (LWR - 1) * 4; iSide++)
		{
			if (isPieceUsed[iSide])
				continue;
			if (puzzle_board[x - 1][y] != 0 && getUpSide(iSide, toast[x][y]) == getDownSide(puzzle_board[x - 1][y], toast[x - 1][y]))
			{
				puzzle_board[x][y] = iSide;
				isPieceUsed[iSide] = 1;
				return true;
			}
			if (puzzle_board[x][y - 1] != 0 && getLeftSide(iSide, toast[x][y]) == getRightSide(puzzle_board[x][y - 1], toast[x][y - 1]))
			{
				puzzle_board[x][y] = iSide;
				isPieceUsed[iSide] = 1;
				return true;
			}
		}
		return false;
	}
	else if (x == LWR - 1)
	{
		// pieces in down sides
		toast[x][y] = UP;

		for (iSide = 4; iSide < (LWR - 1) * 4; iSide++)
		{
			if (isPieceUsed[iSide])
				continue;
			if (puzzle_board[x - 1][y] != 0 && getUpSide(iSide, toast[x][y]) == getDownSide(puzzle_board[x - 1][y], toast[x - 1][y]))
			{
				puzzle_board[x][y] = iSide;
				isPieceUsed[iSide] = 1;
				return true;
			}
			if (puzzle_board[x][y - 1] != 0 && getLeftSide(iSide, toast[x][y]) == getRightSide(puzzle_board[x][y - 1], toast[x][y - 1]))
			{
				puzzle_board[x][y] = iSide;
				isPieceUsed[iSide] = 1;
				return true;
			}
		}
		return false;
	}
	else if (x == 0)
	{
		// pieces in up sides
		toast[x][y] = DOWN;

		for (iSide = 4; iSide < (LWR - 1) * 4; iSide++)
		{
			if (isPieceUsed[iSide])
				continue;
			if (( y == 1 || puzzle_board[x][y - 1] != 0) && getLeftSide(iSide, toast[x][y]) == getRightSide(puzzle_board[x][y - 1], toast[x][y - 1]))
			{
				puzzle_board[x][y] = iSide;
				isPieceUsed[iSide] = 1;
				return true;
			}
		}
		return false;
	}
	else if (y == 0)
	{
		// pieces in left sides
		toast[x][y] = LEFT;

		for (iSide = 4; iSide < (LWR - 1) * 4; iSide++)
		{
			if (isPieceUsed[iSide])
				continue;
			if (puzzle_board[x - 1][y] != 0 && getUpSide(iSide, toast[x][y]) == getDownSide(puzzle_board[x - 1][y], toast[x - 1][y]))
			{
				puzzle_board[x][y] = iSide;
				isPieceUsed[iSide] = 1;
				return true;
			}
		}
		return false;
	}
	else
	{
		// other pieces in the center
		// toast[x][y] = UP;

		for (iCenter = (LWR - 1) * 4; iCenter < LWR * LWR; iCenter++)
		{
			if (isPieceUsed[iCenter])
				continue;

			for (int it = UP; it < RIGHT; it++)
			{
				/*if (puzzle_board[x][y - 1] != 0 && puzzle_board[x - 1][y] != 0)
				{
					if (getLeftSide(iCenter, it) == getRightSide(puzzle_board[x][y - 1], toast[x][y - 1]) &&
						getUpSide(iCenter, it) == getDownSide(puzzle_board[x - 1][y], toast[x - 1][y]))
					{
						puzzle_board[x][y] = iCenter;
						toast[x][y] = it;
						isPieceUsed[iCenter] = 1;
						return true;
					}
				}
				else if (puzzle_board[x][y - 1] != 0 && getLeftSide(iCenter, it) == getRightSide(puzzle_board[x][y - 1], toast[x][y - 1]))
				{
					puzzle_board[x][y] = iCenter;
					toast[x][y] = it;
					isPieceUsed[iCenter] = 1;
					return true;
				}
				else if (puzzle_board[x - 1][y] != 0 && getUpSide(iCenter, it) == getDownSide(puzzle_board[x - 1][y], toast[x - 1][y]))
				{
					puzzle_board[x][y] = iCenter;
					toast[x][y] = it;
					isPieceUsed[iCenter] = 1;
					return true;
				}*/
				if (puzzle_board[x][y - 1] != 0 && getLeftSide(iCenter, it) == getRightSide(puzzle_board[x][y - 1], toast[x][y - 1]))
				{
					puzzle_board[x][y] = iCenter;
					toast[x][y] = it;
					isPieceUsed[iCenter] = 1;
					return true;
				}
				if (puzzle_board[x - 1][y] != 0 && getUpSide(iCenter, it) == getDownSide(puzzle_board[x - 1][y], toast[x - 1][y]))
				{
					puzzle_board[x][y] = iCenter;
					toast[x][y] = it;
					isPieceUsed[iCenter] = 1;
					return true;
				}
			}
		}
		return false;
	}
}

void initBoard()
{
	/* DFS way */
	initSideToast();
	int pieceID = 0;
	puzzle_board[0][0] = pieceID;
	isPieceUsed[pieceID] = 1;
	toast[0][0] = LEFT;
	findNextPiece(0, 1);

	int ret = 0;
	for (int i = 0; i < LWR * LWR; i++)
	{
		if (isPieceUsed[i] == 0)
			ret++;
	}
	//cout << "There are " << ret << " pieces still unused." << endl;
	initRestBoard();

	ret = 0;
	for (int i = 0; i < LWR * LWR; i++)
	{
		if (isPieceUsed[i] == 0)
			ret++;
	}
	if (ret > 0)
	{
		cout << "SYSTEM ERROR: Pieces missing." << endl;
		exit(-7);
	}

	/* init by order way */
	/*int iCornor, iSide, iCenter;

	iCornor = 0;
	iSide = 4;
	iCenter = (LWR - 1) * 4;
	for (int i = 0, x = 0, y = 0; i < LWR * LWR; i++)
	{
		if (x == 0 && y == 0)
		{
			// left-up corner of the board
			puzzle_board[x][y] = iCornor;
			toast[x][y] = LEFT;
			iCornor++;
		}
		else if (x == 0 && y == LWR - 1)
		{
			// right-up corner of the board
			puzzle_board[x][y] = iCornor;
			toast[x][y] = DOWN;
			iCornor++;
		}
		else if (x == LWR - 1 && y == 0)
		{
			// left-down corner of the board
			puzzle_board[x][y] = iCornor;
			toast[x][y] = UP;
			iCornor++;
		}
		else if (x == LWR - 1 && y == LWR - 1)
		{
			// right-down corner of the board
			puzzle_board[x][y] = iCornor;
			toast[x][y] = RIGHT;
			iCornor++;
		}
		else if (y == LWR - 1)
		{
			// pieces in right sides
			puzzle_board[x][y] = iSide;
			toast[x][y] = RIGHT;
			iSide++;
		}
		else if (x == LWR - 1)
		{
			// pieces in down sides
			puzzle_board[x][y] = iSide;
			toast[x][y] = UP;
			iSide++;
		}
		else if (x == 0)
		{
			// pieces in up sides
			puzzle_board[x][y] = iSide;
			toast[x][y] = DOWN;
			iSide++;
		}
		else if (y == 0)
		{
			// pieces in left sides
			puzzle_board[x][y] = iSide;
			toast[x][y] = LEFT;
			iSide++;
		}
		else
		{
			// other pieces in the center
			puzzle_board[x][y] = iCenter;
			toast[x][y] = UP;
			iCenter++;
		}

		y++;
		if (y == LWR)
		{
			y = 0;
			x++;
		}
	}*/
}

void initSideToast()
{
	for (int i = 0, x = 0, y = 0; i < LWR * LWR; i++)
	{
		if (x == 0 && y == 0)
		{
			// left-up corner of the board
			toast[x][y] = LEFT;
		}
		else if (x == 0 && y == LWR - 1)
		{
			// right-up corner of the board
			toast[x][y] = DOWN;
		}
		else if (x == LWR - 1 && y == 0)
		{
			// left-down corner of the board
			toast[x][y] = UP;
		}
		else if (x == LWR - 1 && y == LWR - 1)
		{
			// right-down corner of the board
			toast[x][y] = RIGHT;
		}
		else if (y == LWR - 1)
		{
			// pieces in right sides
			toast[x][y] = RIGHT;
		}
		else if (x == LWR - 1)
		{
			// pieces in down sides
			toast[x][y] = UP;
		}
		else if (x == 0)
		{
			// pieces in up sides
			toast[x][y] = DOWN;
		}
		else if (y == 0)
		{
			// pieces in left sides
			toast[x][y] = LEFT;
		}

		y++;
		if (y == LWR)
		{
			y = 0;
			x++;
		}
	}
}

void initRestBoard()
{
	// This piece is in the corner part. ID: from 0 to 3
	// This piece is in the sides part. ID: from 4 to (LWR - 1) * 4 - 1
	// This piece is in the center part. ID: from (LWR - 1) * 4 to LWR * LWR - 1
	for (int x = 0, y = 1; x < LWR; )
	{
		if (puzzle_board[x][y] == 0)
		{
			// need to fill with random piece
			if ((x == 0 && y == LWR - 1) || (x == LWR - 1 && y == 0) || (x == LWR - 1 && y == LWR - 1))
			{
				// pieces in the corner. ID: from 0 to 3
				for (int i = 0; i < 4; i++)
				{
					if (isPieceUsed[i])
						continue;
					else
					{
						isPieceUsed[i] = 1;
						puzzle_board[x][y] = i;
						break;
					}
				}
			}
			else if ((y == LWR - 1) || (x == LWR - 1) || (x == 0) || (y == 0))
			{
				// pieces in sides. ID: from 4 to (LWR - 1) * 4 - 1
				for (int i = 4; i < (LWR - 1) * 4; i++)
				{
					if (isPieceUsed[i])
						continue;
					else
					{
						isPieceUsed[i] = 1;
						puzzle_board[x][y] = i;
						break;
					}
				}
			}
			else
			{
				// other pieces in the center. ID: from (LWR - 1) * 4 to LWR * LWR - 1
				for (int i = (LWR - 1) * 4; i < LWR * LWR; i++)
				{
					if (isPieceUsed[i])
						continue;
					else
					{
						isPieceUsed[i] = 1;
						puzzle_board[x][y] = i;
						break;
					}
				}
			}
		}
		y++;
		if (y == LWR)
		{
			y = 0;
			x++;
		}
	}
}

int getUpSide(int i, int t)
{
	if (t == UP)
		return pieces[i][UP];
	else if (t == DOWN)
		return pieces[i][DOWN];
	else if (t == LEFT)
		return pieces[i][LEFT];
	else if (t == RIGHT)
		return pieces[i][RIGHT];
	else
		return -1;
}

int getDownSide(int i, int t)
{
	if (t == UP)
		return pieces[i][DOWN];
	else if (t == DOWN)
		return pieces[i][UP];
	else if (t == LEFT)
		return pieces[i][RIGHT];
	else if (t == RIGHT)
		return pieces[i][LEFT];
	else
		return -1;
}

int getLeftSide(int i, int t)
{
	if (t == UP)
		return pieces[i][LEFT];
	else if (t == DOWN)
		return pieces[i][RIGHT];
	else if (t == LEFT)
		return pieces[i][DOWN];
	else if (t == RIGHT)
		return pieces[i][UP];
	else
		return -1;
}

int getRightSide(int i, int t)
{
	if (t == UP)
		return pieces[i][RIGHT];
	else if (t == DOWN)
		return pieces[i][LEFT];
	else if (t == LEFT)
		return pieces[i][UP];
	else if (t == RIGHT)
		return pieces[i][DOWN];
	else
		return -1;
}

// return -1 if cannot find the piece
int getPosXbyID(int id)
{
	for (int i = 0, x = 0, y = 0; i < LWR * LWR; i++)
	{
		if (puzzle_board[x][y] == id)
			return x;

		y++;
		if (y == LWR)
		{
			y = 0;
			x++;
		}
	}
	return -1;
}

// return -1 if cannot find the piece
int getPosYbyID(int id)
{
	for (int i = 0, x = 0, y = 0; i < LWR * LWR; i++)
	{
		if (puzzle_board[x][y] == id)
			return y;

		y++;
		if (y == LWR)
		{
			y = 0;
			x++;
		}
	}
	return -1;
}
