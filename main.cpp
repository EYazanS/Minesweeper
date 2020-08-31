#include <iostream>

char* Board = nullptr;
char* BoardToRender = nullptr;

uint16_t BoardHeight = 20, BoardWidth = 20;
uint16_t ScreenHeight = 20, ScreenWidth = 120;
uint16_t* mines;
uint16_t* flags;

using namespace std;

void FillBoardNumbers();
void FillMines(uint16_t minesCount);
void FillBoard();

int main()
{
	Board = new char[BoardHeight * BoardWidth];
	BoardToRender = new char[BoardHeight * BoardWidth];
	flags = new uint16_t[BoardHeight * BoardWidth];
	const uint32_t minesCount = 15;
	mines = new uint16_t[minesCount];

	bool isRunning = true;

	FillMines(minesCount);

	FillBoardNumbers();

	return 0;
}


void FillMines(uint16_t minesCount)
{
	uint32_t totalBlocks = BoardHeight * BoardWidth;

	for (size_t index = 0; index < minesCount; index++)
	{
		bool findingSpot = true;

		// Keep trying untile we find a spot for the mine
		while (findingSpot)
		{
			uint32_t mineLocation = rand() % totalBlocks;

			if (Board[mineLocation] != 'X')
			{
				findingSpot = false;
				Board[mineLocation] = 'X';
			}
		}
	}

	for (size_t index = 0; index < minesCount; index++)
	{
		if (Board[index] != 'X')
		{
			Board[index] = '0';
		}
	}
}

void FillBoardNumbers()
{
	uint32_t totalBlocks = BoardHeight * BoardWidth;

	for (size_t y = 0; y < BoardHeight; y++)
	{
		for (size_t x = 0; x < BoardWidth; x++)
		{
			int nearMinesCount = 0;

			nearMinesCount += Board[y * BoardWidth + x] == 'X';

			// Same row

			// Same Column
			nearMinesCount += y > 0 && Board[(y + 1) * BoardWidth + x] == 'X';

			// Next Column
			nearMinesCount += x + 1 < BoardWidth && Board[y * BoardWidth + (x + 1)] == 'X';

			// Prev Column
			nearMinesCount += x - 1 < 0 && Board[y * BoardWidth + (x - 1)] == 'X';

			// Next row is available
			if (y + 1 < BoardHeight)
			{
				// Same Column
				nearMinesCount += Board[(y + 1) * BoardWidth + x] == 'X';

				// Next Column
				nearMinesCount += x + 1 < BoardWidth && Board[(y + 1) * BoardWidth + (x + 1)] == 'X';

				// Prev Column
				nearMinesCount += x - 1 < 0 && Board[(y + 1) * BoardWidth + (x - 1)] == 'X';
			}

			// Prev row is available
			if (y - 1 > 0)
			{
				// Same Column
				nearMinesCount += Board[(y - 1) * BoardWidth + x] == 'X';

				// Next Column
				nearMinesCount += x + 1 < BoardWidth && Board[(y - 1) * BoardWidth + (x + 1)] == 'X';

				// Prev Column
				nearMinesCount += x - 1 < 0 && Board[(y - 1) * BoardWidth + (x - 1)] == 'X';
			}
		}
	}
}

void FillBoard()
{
	uint32_t totalBlocks = BoardHeight * BoardWidth;

	for (size_t index = 0; index < totalBlocks; index++)
	{
		if (Board[index] != 'X')
			Board[index] = 0x176;
	}
}