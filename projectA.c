#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

//ID: 208734715
//Assuming that the user enter only natural numbers

/*In Minesweeper, mines are scattered throughout a board which is divided into cells.
Cells have two states: uncovered and covered.
A covered cell is blank and clickable, while an uncovered cell is exposed.
A player left - clicks a cell to uncover it.
If a player uncovers a mined cell, the game ends.
Otherwise, the uncovered cells displays either a number,
indicating the quantity of mines adjacent to it, or a blank tile.
To win the game, players must uncover all non - mine cells.*/

#define MaxValue 8

//Square :: BombsNearby as integer ; Isbomb as boolean ; VisibleState as integer
typedef struct Square {
	// 1-8; 0 - " "
	int BombsNearby;
	//true - bomb; false - not bomb
	bool Isbomb;
	// 0 - hide (X); 1 - visible;
	int VisibleState;
} Square;

//MatrixSize :: Row as integer ; Col as integer
typedef struct MatrixSize {
	int Row;
	//column
	int Col;
} MatrixSize;

//Functions Declaration

Square SetNewSquare(int BombsNearby, bool Isbomb, int VisibleState);
Square SetVisibleState(Square square, int NewVisibleState);
bool IsInArray(int arr[], int arrlen, int value);
void SetBombsValueToFalse(Square SquaresMatrix[][MaxValue], MatrixSize Size);
void SetBombsToGameBoardRandomly(Square SquaresMatrix[][MaxValue], MatrixSize Size);
void PrintGameBoard(Square GameBoard[][MaxValue], MatrixSize Size);
void PrintSquare(Square square);
void PrintSquareByVisibleState(Square square);
void SetGameBoardToNewVisibleState(Square GameBoard[][MaxValue], MatrixSize Size, int NewVisibleState);
void Move(Square GameBoard[][MaxValue], MatrixSize matrix_size, int xChoose, int yChoose);
int CheckNearbyValue(Square GameBoard[][MaxValue], MatrixSize Size, int xValue, int yValue);
void SetGameBoardNearbyValue(Square GameBoard[][MaxValue], MatrixSize Size);
bool IsWin(Square GameBoard[][MaxValue], MatrixSize Size);
void SquareAmountExposure(Square GameBoard[][MaxValue], MatrixSize Size, int amount);
void FinishMessage(Square GameBoard[][MaxValue], MatrixSize Size);
void GameBoardSettings(Square GameBoard[][MaxValue], MatrixSize Size);
MatrixSize _SetMatrixSize(int Rows, int Cols);
bool IsMatrixSizeInRange(MatrixSize Size, int RowLowerValue, int RowUpperValue, int ColLowerValue, int ColUpperValue);
MatrixSize CustomMatrixSize();
MatrixSize Menu();

void main()
{
	//Choose game size if game size -1 then exit
	int xChoose, yChoose;
	MatrixSize Matrix_Size = Menu();
	if (Matrix_Size.Row == -1)
		return;

	Square GameBoard[MaxValue][MaxValue];

	//Settings
	GameBoardSettings(GameBoard, Matrix_Size);
	do {
		PrintGameBoard(GameBoard, Matrix_Size);

		//Read Chosen point to uncover
		printf("Please enter your move, row and column :");
		scanf("%d %d", &xChoose, &yChoose);

		//Do the move
		Move(GameBoard, Matrix_Size, xChoose, yChoose);

		//if win or bomb uncoverd is false then keep playing
	} while (!(GameBoard[xChoose][yChoose].Isbomb == true || IsWin(GameBoard, Matrix_Size)));

	//Proper finish massage 
	FinishMessage(GameBoard, Matrix_Size);
}

//Start menu function returns chosen MatrixSize
MatrixSize Menu()
{
	int choose;
	printf("Welcome to Minesweeper!\n\nPlease choose one of the following options and enter it's number:\n\n1 - for size 8X8\n\n2 - for size 12X12\n\n3 - for size 15X15\n\n4 - for custom size\n\n0 - Exit\n\n");
	scanf("%d", &choose);

	switch (choose)
	{
	case 1:
		return _SetMatrixSize(8, 8);
		break;
  /*case 2:
		return _SetMatrixSize(12, 12);
		break;
	case 3:
		return _SetMatrixSize(15, 15);
		break;
	case 4:
		return CustomMatrixSize();
		break;*/
	case 0:
		return _SetMatrixSize(-1, -1);
		break;
	default:
		return Menu();
	}
}

//return user's custom matrix size
MatrixSize CustomMatrixSize()
{
	MatrixSize Size;
	do
	{
		printf("Please enter the size of the board(Lines <= 22 and Cols <= 40) : ");
		scanf("%d %d", &Size.Row, &Size.Col);
		if (!IsMatrixSizeInRange(Size, 2, 22, 2, 40))
			printf("Out Of Range , Set to maximum Size : 25): ");
	} while (!IsMatrixSizeInRange(Size, 2, 22, 2, 40));
	return Size;
}

/* Inputs: current matrix size, row lower value, row upper value, column lower value, column upper value
Output: Is given matrix size is in the range (T/F)*/
bool IsMatrixSizeInRange(MatrixSize Size, int RowLowerValue, int RowUpperValue, int ColLowerValue, int ColUpperValue)
{
	return (Size.Row >= RowLowerValue && Size.Row <= RowUpperValue && Size.Col >= ColLowerValue && Size.Col <= ColUpperValue);
}

//assigning two Integers into MatrixSize variable (operation)
MatrixSize _SetMatrixSize(int Row, int Col)
{
	MatrixSize Size;
	Size.Row = Row;
	Size.Col = Col;
	return Size;
}

//Mines Weeper board settings
void GameBoardSettings(Square GameBoard[][MaxValue],MatrixSize Size)
{
	SetBombsToGameBoardRandomly(GameBoard, Size);
	SetGameBoardNearbyValue(GameBoard, Size);
	SetGameBoardToNewVisibleState(GameBoard, Size, 0);
}

//finish message (GameOver or Win)
void FinishMessage(Square GameBoard[][MaxValue], MatrixSize Size)
{
	PrintGameBoard(GameBoard, Size);
	if (IsWin(GameBoard, Size))
		printf("\nYou Won!! Congratulations!!");
	else
		printf("\nGame Over!");
}

//Same square new visible state
Square SetVisibleState(Square square, int NewVisibleState)
{
	return SetNewSquare(square.BombsNearby, square.Isbomb, NewVisibleState);
}

//assigning of a Square variable (operation)
Square SetNewSquare(int BombsNearby, bool Isbomb, int VisibleState)
{
	Square square;
	square.BombsNearby = BombsNearby;
	square.Isbomb = Isbomb;
	square.VisibleState = VisibleState;
	return square;
}

//Set bombs to the game board randomly
void SetBombsToGameBoardRandomly(Square SquaresMatrix[][MaxValue], MatrixSize Size)
{
	SetBombsValueToFalse(SquaresMatrix, Size);

	int Memory[MaxValue], X, Y, i;
	srand(time(NULL));
	for (i = 0; i < Size.Row; i++) {

		do {
			X = rand() % Size.Row;
			Y = rand() % Size.Col;
		} while (IsInArray(Memory, MaxValue, X * 10 + Y));
		SquaresMatrix[X][Y].Isbomb = true;
		Memory[i] = X * 10 + Y;
	}
}

//assign all the bombs value state to false
void SetBombsValueToFalse(Square SquaresMatrix[][MaxValue], MatrixSize Size)
{
	int x, y;
	for (x = 0; x < Size.Row; x++)
		for (y = 0; y < Size.Col; y++)
			SquaresMatrix[x][y].Isbomb = false;
}

/*Check if value in the array
Input: integer array, array length, value to search
Output: Is value contains in the given array (T/F) */
bool IsInArray(int arr[], int arrlen, int value)
{
	int i;
	for (i = 0; i < arrlen - 1; i++)
	{
		if (arr[i] == value)
			return true;
	}
	return false;
}

//Print Mines Weeper's game board 
void PrintGameBoard(Square GameBoard[][MaxValue], MatrixSize Size)
{
	int x, y;

	printf("\n   ");
	for (y = 0; y < Size.Col; y++) { printf("  %2d",y); }
	printf("\n  ");
	for (y = 0; y < Size.Col; y++) { printf("____"); }
	printf("___\n");

	for (x = 0; x < Size.Row; x++) {
		printf("  %d", x);
		for (y = 0; y < Size.Col; y++) {
			printf(" | ");
			PrintSquareByVisibleState(GameBoard[x][y]);
		}
		printf(" |\n");
	}
	printf("  ");
	for (y = 0; y < Size.Col; y++) { printf("____"); }
	printf("___\n");
}

//Print square by visible state
void PrintSquareByVisibleState(Square square)
{
	switch (square.VisibleState)
	{
	case 0:
		printf("X");
		break;
	case 1:
		PrintSquare(square);
		break;
	}
}

//Print square
void PrintSquare(Square square)
{
	if (square.Isbomb)
		printf("*");
	else
		if (square.BombsNearby == 0)
			printf(" ");
		else
			printf("%d", square.BombsNearby);
}

//Assign all the game board squares to the given visible state 
void SetGameBoardToNewVisibleState(Square GameBoard[][MaxValue], MatrixSize Size, int NewVisibleState)
{
	int x, y;
	for (x = 0; x < Size.Row; x++)
		for (y = 0; y < Size.Col; y++)
			GameBoard[x][y].VisibleState = NewVisibleState;
}

/* Proper move by game rules:
-1: massive exposure
check if input valid
check if bomb is chosen
else square uncover */
void Move(Square GameBoard[][MaxValue], MatrixSize matrix_size, int xChoose, int yChoose)
{
	if (xChoose == -1)
		SquareAmountExposure(GameBoard, matrix_size, yChoose);
	else if (!IsMatrixSizeInRange(_SetMatrixSize(xChoose, yChoose), 0, matrix_size.Row - 1, 0, matrix_size.Col - 1))
		printf("Invalid Choice, Please enter valid choice!\n");
	else if (GameBoard[xChoose][yChoose].VisibleState)
		printf("Invalid Choice\n");
	else if (GameBoard[xChoose][yChoose].Isbomb) {
		SetGameBoardToNewVisibleState(GameBoard,matrix_size, 1);
		printf("Bomb!!! You died!!!\n");
	}
	else
		GameBoard[xChoose][yChoose].VisibleState = 1;
}

//Assign nearby value to all the squares on the game board
void SetGameBoardNearbyValue(Square GameBoard[][MaxValue], MatrixSize Size)
{
	int x, y;
	for (x = 0; x < Size.Row; x++)
		for (y = 0; y < Size.Col; y++)
			GameBoard[x][y].BombsNearby = CheckNearbyValue(GameBoard, Size, x, y);
}

/*Check nearby value of a square
Inputs: game board as square matrix, x value of the square on the game board, y value of the square on the game board
Output: nearby value as integer
*/
int CheckNearbyValue(Square GameBoard[][MaxValue],MatrixSize Size, int xValue, int yValue)
{
	int count = 0, x, y;
	for (x = xValue - 1; x <= xValue + 1; x++)
		for (y = yValue - 1; y <= yValue + 1; y++)
			if (x >= 0 && y >= 0 && y < Size.Col && x < Size.Row && GameBoard[x][y].Isbomb)
			{
				count++;
			}
	return count;
}

/*Check if the player wins by the Mines Weeper rules
Input: gameboard as square matrix
Output: if the player wins as boolean */
bool IsWin(Square GameBoard[][MaxValue], MatrixSize Size)
{
	int x, y, count = 0;
	for (x = 0; x < Size.Row; x++)
		for (y = 0; y < Size.Col; y++)
			if (GameBoard[x][y].VisibleState == 0)
				count++;
	return (count == Size.Row);
}

/*Square amount exposure by the given value
Input: game board as square matrix , the amount of squares to expose as integer */
void SquareAmountExposure(Square GameBoard[][MaxValue], MatrixSize Size, int amount)
{
	int x, y, count = 0;
	if (amount < 1)
		return;
	for (x = 0; x < Size.Row; x++)
		for (y = 0; y < Size.Col; y++)
			if (GameBoard[x][y].VisibleState == 0 && !GameBoard[x][y].Isbomb)
			{
				GameBoard[x][y].VisibleState = 1;
				count++;
				if (count == amount)
					return;
			}
}