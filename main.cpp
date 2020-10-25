#include<Windows.h>
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
void DrawField(wchar_t* screen);

int main()
{
	Board = new char[BoardHeight * BoardWidth];
	BoardToRender = new char[BoardHeight * BoardWidth];
	flags = new uint16_t[BoardHeight * BoardWidth];
	const uint32_t minesCount = 15;
	mines = new uint16_t[minesCount];

	HANDLE consoleHandle = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, NULL, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	SetConsoleActiveScreenBuffer(consoleHandle);

	bool isRunning = true;

	int screenBufferSize = ScreenWidth * ScreenHeight;

	wchar_t* screen = new wchar_t[screenBufferSize];

	for (int i = 0; i < screenBufferSize; i++)
		screen[i] = L' ';

	DWORD bytesWritten = 0;

	FillMines(minesCount);

	FillBoardNumbers();

	while (isRunning)
	{
		DrawField(screen);

		WriteConsoleOutputCharacter(consoleHandle, screen, screenBufferSize, { 0, 0 }, &bytesWritten);
	}

	CloseHandle(consoleHandle);

	cout << "Game over!! :(, your score: " << endl;

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
		if (Board[index] != 'X')
			Board[index] = '0';
}

void FillBoardNumbers()
{
	uint32_t totalBlocks = BoardHeight * BoardWidth;

	for (size_t y = 0; y < BoardHeight; y++)
	{
		for (size_t x = 0; x < BoardWidth; x++)
		{
			if (Board[y * BoardWidth + x] != 'X')
			{
				int nearMinesCount = 0;

				// Same row
				// Next Column
				nearMinesCount += (x + 1) < BoardWidth && Board[y * BoardWidth + (x + 1)] == 'X';

				// Prev Column
				nearMinesCount += x - 1 > 0 && Board[y * BoardWidth + (x - 1)] == 'X';

				// Next row is available
				if (y + 1 < BoardHeight)
				{
					// Same Column
					nearMinesCount += Board[(y + 1) * BoardWidth + x] == 'X';

					// Next Column
					nearMinesCount += x + 1 < BoardWidth && Board[(y + 1) * BoardWidth + (x + 1)] == 'X';

					// Prev Column
					nearMinesCount += x - 1 > 0 && Board[(y + 1) * BoardWidth + (x - 1)] == 'X';
				}

				// Prev row is available
				if (y - 1 > 0)
				{
					// Same Column
					nearMinesCount += Board[(y - 1) * BoardWidth + x] == 'X';

					// Next Column
					nearMinesCount += x + 1 < BoardWidth && Board[(y - 1) * BoardWidth + (x + 1)] == 'X';

					// Prev Column
					nearMinesCount += x - 1 > 0 && Board[(y - 1) * BoardWidth + (x - 1)] == 'X';
				}

				Board[y * BoardWidth + x] = nearMinesCount + '0';
			}
		}
	}
}

void DrawField(wchar_t* screen)
{
	for (int x = 0; x < BoardWidth; x++)
		for (int y = 0; y < BoardHeight; y++)
			screen[y * ScreenWidth + x] = Board[y * BoardWidth + x];
}