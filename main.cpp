#include<Windows.h>
#include <iostream>
#include <thread>

char* Board = nullptr;
char* BoardToRender = nullptr;

uint16_t BoardHeight = 20, BoardWidth = 20;
uint16_t ScreenHeight = 20, ScreenWidth = 120;
uint16_t* mines;
uint16_t* flags;

int score;
int playerX, playerY;
bool Keys[5];

using namespace std;

void FillBoardNumbers();
void FillMines(uint16_t minesCount);
void GetInput();
void ChangeCursorPosition(const HANDLE& consoleHandle);
void ProcessPlayerMovement(bool& buttonHeld, int& currentBlocks);
void DrawField(wchar_t* screen);
void InitBoardToRender();
void UpdateBoardToRender(int x, int y);
void DidPlayerWin(int currentBlocks, bool& isRunning, bool& hasWon);

HANDLE GetConsoleHandle();

void DidThePlayerFindAMine(bool& isRunning);

int main()
{
	Board = new char[BoardHeight * BoardWidth];
	BoardToRender = new char[BoardHeight * BoardWidth];
	flags = new uint16_t[BoardHeight * BoardWidth];
	const uint32_t minesCount = 15;
	mines = new uint16_t[minesCount];
	playerX = 0;
	playerY = 0;
	score = 0;

	HANDLE consoleHandle = GetConsoleHandle();

	int currentBlocks = 0;
	bool hasWon = false;
	bool isRunning = true;
	bool buttonHeld = false;

	int screenBufferSize = ScreenWidth * ScreenHeight;

	wchar_t* screen = new wchar_t[screenBufferSize];

	for (int i = 0; i < screenBufferSize; i++)
		screen[i] = L' ';

	DWORD bytesWritten = 0;

	FillMines(minesCount);

	FillBoardNumbers();

	InitBoardToRender();

	while (isRunning)
	{
		GetInput();

		ProcessPlayerMovement(buttonHeld, currentBlocks);

		ChangeCursorPosition(consoleHandle);

		DrawField(screen);

		WriteConsoleOutputCharacter(consoleHandle, screen, screenBufferSize, { 0, 0 }, &bytesWritten);

		DidThePlayerFindAMine(isRunning);

		DidPlayerWin(currentBlocks, isRunning, hasWon);
	}

	CloseHandle(consoleHandle);

	cout << "Game over!";

	if (hasWon)
		cout << " You Win!!";

	cout << ", your score: " << score << endl;

	return 0;
}

void DidThePlayerFindAMine(bool& isRunning)
{
	if (Keys[4] && BoardToRender[playerY * BoardWidth + playerX] == 'X')
	{
		this_thread::sleep_for(1000ms);
		isRunning = false;
	}
}

void DidPlayerWin(int currentBlocks, bool& isRunning, bool& hasWon)
{
	if (currentBlocks == (BoardWidth * BoardHeight) - 20)
	{
		this_thread::sleep_for(1000ms);
		isRunning = false;
		hasWon = true;
	}
}

HANDLE GetConsoleHandle()
{
	HANDLE consoleHandle = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, NULL, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(consoleHandle);
	return consoleHandle;
}

void ProcessPlayerMovement(bool& buttonHeld, int& currentBlocks)
{
	// Did the player press the left arrow, and can we move left?
	playerX -= !buttonHeld && Keys[0] && (playerX - 1) >= 0;

	// Did the player press the up arrow, and can we move up?
	playerY -= !buttonHeld && Keys[1] && (playerY - 1) >= 0;

	// Did the player press the right arrow, and can we move right?
	playerX += !buttonHeld && Keys[2] && (playerX + 1) < BoardWidth;

	// Did the !buttonHeld press the down arrow, and can we move down?
	playerY += !buttonHeld && Keys[3] && (playerY + 1) < BoardHeight;

	// Did the playe press space
	if (!buttonHeld && Keys[4])
	{
		UpdateBoardToRender(playerX, playerY);
		score += 10;
		currentBlocks++;
	}

	// Because its fast, we have to prevent constent holding
	bool anyHeld = false;

	for (size_t keyIndex = 0; keyIndex < 5; keyIndex++)
		if (Keys[keyIndex])
			anyHeld = Keys[keyIndex];

	buttonHeld = anyHeld;
}

void ChangeCursorPosition(const HANDLE& consoleHandle)
{
	COORD pos = { playerX, playerY };

	SetConsoleCursorPosition(consoleHandle, pos);
}

void GetInput()
{
	// Input
	for (size_t keyIndex = 0; keyIndex < 5; keyIndex++)
	{
		// Left arrow virtual key code  // Top arrow virtual key code // Right arrow virtual key code// Down arrow virtual key code // Z virtual key code key
		Keys[keyIndex] = (0x8000 & GetAsyncKeyState((unsigned char)("\x25\x26\x27\x28\x20"[keyIndex]))) != 0;
	}
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

void InitBoardToRender()
{
	for (int x = 0; x < BoardWidth; x++)
		for (int y = 0; y < BoardHeight; y++)
			BoardToRender[y * BoardWidth + x] = '?';
}

void UpdateBoardToRender(int x, int y)
{
	BoardToRender[y * BoardWidth + x] = Board[y * BoardWidth + x];
}

void DrawField(wchar_t* screen)
{
	for (int x = 0; x < BoardWidth; x++)
		for (int y = 0; y < BoardHeight; y++)
			screen[y * ScreenWidth + x] = BoardToRender[y * BoardWidth + x];
}