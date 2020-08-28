#include <iostream>

char* Board = nullptr;
char* BoardToRender = nullptr;

uint16_t BoardHeight = 20, BoardWidth = 20;
uint16_t ScreenHeight = 20, ScreenWidth = 120;
uint16_t* mines;
uint16_t* flags;

using namespace std;

int main()
{
	Board = new char[BoardHeight * BoardWidth];
	BoardToRender = new char[BoardHeight * BoardWidth];
	flags = new uint16_t[BoardHeight * BoardWidth];
	const uint32_t minesCount = 15;
	mines = new uint16_t[minesCount];

	bool isRunning = true;

	FillMines(minesCount);


	while (isRunning)
	{

	}

	return 0;
}


void FillMines(uint16_t minesCount)
{
	uint32_t totalBlocks = BoardHeight * BoardWidth;

	for (size_t i = 0; i < minesCount; i++)
	{
		uint32_t mineLocation = rand() % totalBlocks;
		Board[mineLocation] = 'X';
	}
}

void FillBoard()
{
	uint32_t totalBlocks = BoardHeight * BoardWidth;

	for (size_t i = 0; i < totalBlocks; i++)
	{
		if (Board[i] != 'X')
			Board[i] = 0x176;
	}
}