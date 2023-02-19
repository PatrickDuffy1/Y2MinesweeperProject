// Patrick Duffy
// G00408635

#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>

#define MAX_X                       8
#define MAX_Y                       8
#define AMOUNT_OF_MINES             10
#define MAX_PLAYERS                 4	// Can be increased or decreased. Can not be less than 1.
#define NUM_OF_SAVE_SLOTS           3

// The next four definitions are used in the tileStatus array.
#define TILE_STATUS_HIDDEN          0	// Will hide value of tile when displayed to screen.
#define TILE_STATUS_FLAG            1	// Places flag on tile when displayed to screen.
#define TILE_STATUS_UNCOVERED       2	// Shows value of tile when displayed to screen.
#define TILE_STATUS_TO_BE_UNCOVERED 3	// Marks tile to be uncovered (set to 2). Mostly used in the clearEmptyTiles function.

// The next four definitions are used to show the current game status.
#define GAME_STATUS_ACTIVE         0	// If game is active.
#define GAME_STATUS_WIN            1	// If all tiles without mines have been cleared.
#define GAME_STATUS_LOSE           2	// If a mine has been hit
#define GAME_STATUS_EXIT           3	// If the users chooses to exit the game


// The next four definitions are used to show the main menu user choise
#define USER_CHOICE_START_NEW_GAME       1    // Start New Game
#define USER_CHOICE_LOAD_SAVED_GAME      2    // Load Saved Game
#define USER_CHOICE_DELETE_SAVED_GAME    3    // Delete Saved Game
#define USER_CHOICE_EXIT                 4    // Exit

// The next two definitions are used to show the save slot status
#define SAVE_SLOT_EMPTY                  0    // Empty
#define SAVE_SLOT_SAVED                  1    // Saved

void displayGrid(char[MAX_Y][MAX_X], int tileStatus[MAX_Y][MAX_X], int cursorX, int cursorY);
void randomMinePosition(char gameGrid[MAX_Y][MAX_X]);
char setTileValue(int x, int y, char gameGrid[MAX_Y][MAX_X]);
void moveCursor(char inputMovement, int* cursorX, int* cursorY);
int clearEmptyTiles(char gameGrid[MAX_Y][MAX_X], int tileStatus[MAX_Y][MAX_X], int playerScores[MAX_PLAYERS], int currentPlayer);
void displayScores(int playerScores[MAX_PLAYERS], int numOfPlayers);
int getHighestScore(int playerScores[MAX_PLAYERS], int numOfPlayers, int loser);
void checkIfEmpty(char saveSlotFiles[NUM_OF_SAVE_SLOTS][15], int saveSlotStatus[NUM_OF_SAVE_SLOTS]);
void deleteSave(char saveSlotFiles[NUM_OF_SAVE_SLOTS][15], int userChoice);
int displaySaveSlotStatus(int saveSlotStatus[NUM_OF_SAVE_SLOTS]);
void showControls();

void saveGame(char saveSlotFiles[NUM_OF_SAVE_SLOTS][15], int saveSlotStatus[NUM_OF_SAVE_SLOTS],
	char gameGrid[MAX_Y][MAX_X], int tileStatus[MAX_Y][MAX_X], int numOfPlayers,
	int currentPlayer, int playerScores[MAX_PLAYERS], int amountNonMinesHidden, int amountOfActions, int saveSlot);

void loadGame(char saveSlotFiles[NUM_OF_SAVE_SLOTS][15], int saveSlotStatus[NUM_OF_SAVE_SLOTS],
	char gameGrid[MAX_Y][MAX_X], int tileStatus[MAX_Y][MAX_X], int* numOfPlayers,
	int* currentPlayer, int playerScores[MAX_PLAYERS], int* amountNonMinesHidden, int* amountOfActions, int saveSlot);

int main()
{
	FILE* file1;
	srand(time(NULL));

	char gameGrid[MAX_Y][MAX_X];			// grid that stores characters representing what is on the tiles (Numbers/Mines).
	char saveSlotFiles[NUM_OF_SAVE_SLOTS][15] = { {"SaveSlot1.txt"}, {"SaveSlot2.txt"}, {"SaveSlot3.txt"} };	// Names of save slot files.
	int saveSlotStatus[NUM_OF_SAVE_SLOTS];	// Shows if save slot is currently in use.
	int tileStatus[MAX_Y][MAX_X];			// Used to decide if a specific tile is hidden, uncovered, flag etc.
	int playerScores[MAX_PLAYERS];			// Stores scores of all players in game.

	char userInput;								// Used for when user enters a char.
	int cursorX = 0;							// X position of the cursor 'X'.
	int cursorY = 0;							// Y position of the cursor 'X'.
	int gameStatus = 0;							// Shows current game status e.g. active, mine hit, etc. 
	int emptyTile = 0;							// Incremented if tile has 0 mines around it.
	int amountNonMinesHidden = MAX_X * MAX_Y;	// Amount of hidden tile not including mines.
	int numOfPlayers;							// Number of players in current game. Cannot be less than 1 or greater than MAX_PLAYERS.
	int currentPlayer = 0;						// Shows wiche players turn it is.
	int amountOfActions = 0;					// Amount of actions that resulted in a tile being uncovered.
	int loser = -1;								// Used if a player hits a mine.
	int winner = -1;							// Used for player with highest score.
	int userChoice;								// Used for when user enters an int.
	int repeatLoop;								// Used if a loop needs to be repeated.
	int validSelection = 0;						// Used to ensure the user made 

	printf("*****************\n");
	printf("** MINESWEEPER **\n");
	printf("*****************\n");

	checkIfEmpty(saveSlotFiles, saveSlotStatus);	// Checks all save slots to see if they have a game saved and uses this to set their status.

	do
	{
		do	//	Repeats until user enters a valid choice.
		{
			printf("\n\n1. Start New Game\n");
			printf("2. Load Saved Game\n");
			printf("3. Delete Saved Game\n");
			printf("4. Exit\n");
			printf("Enter number: ");
			(void)scanf("%d", &userChoice);		// Cast to void to avoid "Return value ignored" warning.
			printf("\n");
		
		}while(userChoice < USER_CHOICE_START_NEW_GAME || userChoice > USER_CHOICE_EXIT);

		if (userChoice == USER_CHOICE_LOAD_SAVED_GAME)
		{
			// Loads previously saved game

			repeatLoop = 0;

			do
			{
				// Asks user to enter the number of the save slot they wish to load.
				printf("\nSave Slots:\n");
				printf("Enter the number of the save slot you wish to load.\n");
				userChoice = displaySaveSlotStatus(saveSlotStatus);		// Shows user save slot names and statuses and asks them to enter choice.

				if (userChoice <= 0 || userChoice >= NUM_OF_SAVE_SLOTS + 1)
				{
					// Returns to main menu if user enters invalid number.

					repeatLoop = 1;
					validSelection = 1;
				}
				else
				{
					// Loads selected save slot if save slot has a game saved.
					if (saveSlotStatus[userChoice - 1] == 1)
					{
						loadGame(saveSlotFiles, saveSlotStatus, gameGrid, tileStatus,
							&numOfPlayers, &currentPlayer, playerScores,
							&amountNonMinesHidden, &amountOfActions, userChoice);	// Loads a previously saved game.

						validSelection = 1;
					}
					else // Asks previous question if save slot is empty.
					{
						validSelection = 0;
					}
				}

			} while (validSelection == 0);

		}
		else if (userChoice == USER_CHOICE_DELETE_SAVED_GAME)
		{
			// Gives user option to delete a save slot.

			repeatLoop = 1;

			printf("Enter the number of the save slot you wish to delete.\n");
			userChoice = displaySaveSlotStatus(saveSlotStatus);	// Shows user save slot names and statuses and asks them to enter choice.

			if (userChoice > 0 && userChoice <= 3)	// Deletes save slot if user enters a valid save slot.
			{
				deleteSave(saveSlotFiles, userChoice);	//Deletes save slot.
				checkIfEmpty(saveSlotFiles, saveSlotStatus);	// Checks all save slots to see if they have a game saved and uses this to set their status.
			}
		}
		else if (userChoice == USER_CHOICE_EXIT)
		{
			// Exits game.
			repeatLoop = 0;
			gameStatus = GAME_STATUS_EXIT;
		}
		else
		{
			// Starts new game.

			repeatLoop = 0;

			do	// Keeps asking user to enter amount of players until user enters a valid number.
			{
				printf("Enter number of players between 1 - %d: ", MAX_PLAYERS);
				(void)scanf("%d", &numOfPlayers);

			} while (numOfPlayers < 1 || numOfPlayers > MAX_PLAYERS);

			randomMinePosition(gameGrid);

			// Calculates amount of mines around a tile if tile is not a mine.
			for (int y = 0; y < MAX_Y; y++)
			{
				for (int x = 0; x < MAX_X; x++)
				{
					if (gameGrid[y][x] != 'M')
					{
						// Count the number of mines around each tile.
						gameGrid[y][x] = setTileValue(x, y, gameGrid);	//Calculates amount of mines around a tile.
					}

					// Set status of all tiles to hidden.
					tileStatus[y][x] = TILE_STATUS_HIDDEN;
				}
			}

			// Sets all players scores to 0.
			for (int i = 0; i < numOfPlayers; i++)
			{
				playerScores[i] = 0;
			}
		}

	} while (repeatLoop != 0);

	// Runs if user has not chosen to exit the game
	if (gameStatus != GAME_STATUS_EXIT)
	{
		// Displays scores, grid and controls.
		printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");	// Ensures user cannot see previous grid unless they scroll up.
		displayScores(playerScores, numOfPlayers);
		printf("Current Player: %d/%d\n", (currentPlayer + 1), numOfPlayers);
		displayGrid(gameGrid, tileStatus, cursorX, cursorY);
		showControls();

		// Repeats until a game end condition has been met.
		while (gameStatus == GAME_STATUS_ACTIVE)
		{
			(void)scanf(" %c", &userInput);	// gets an input from user.

			if (userInput == 'q')	// Uncovers tile if selected
			{
				if (tileStatus[cursorY][cursorX] == TILE_STATUS_HIDDEN)	// Uncovers selected tile if not already uncovered.
				{
					tileStatus[cursorY][cursorX] = TILE_STATUS_UNCOVERED;
					amountOfActions++;

					if (gameGrid[cursorY][cursorX] != 'M')
					{
						if (gameGrid[cursorY][cursorX] == '0')	// Marks tile as TILE_STATUS_TO_BE_UNCOVERED if tile has 0 mines around it.
						{
							emptyTile = 1;
							tileStatus[cursorY][cursorX] = TILE_STATUS_TO_BE_UNCOVERED;
						}
						else	// Shows value of tile if it has mines around it.
						{
							// Casts tile from char to int to get ASCII value of tile, then subtracts 48 (ASCII '0') to get real value.
							// This would not work if the real tile was greater than 9, but is ok in this case because the highest real value a tile can have is 8.
							playerScores[currentPlayer] += ((int)gameGrid[cursorY][cursorX] - 48);
							amountNonMinesHidden--;
						}
					}
					else
					{
						// If mine is hit sets game status to GAME_STATUS_LOSE and stores number of player who hit the mine.

						gameStatus = GAME_STATUS_LOSE;
						loser = currentPlayer;
					}
				}
			}
			else if (userInput == 'f')	// Places/Removes flag from tile.
			{
				if (tileStatus[cursorY][cursorX] == TILE_STATUS_FLAG)
				{
					tileStatus[cursorY][cursorX] = TILE_STATUS_HIDDEN;
				}
				else if (tileStatus[cursorY][cursorX] == TILE_STATUS_HIDDEN)
				{
					tileStatus[cursorY][cursorX] = TILE_STATUS_FLAG;
				}
			}
			else if (userInput == 'x')	// Gives user option to save the current game.
			{
				printf("\n\nEnter the number of the save slot you wish to overwrite: \n");
				userChoice = displaySaveSlotStatus(saveSlotStatus);		// Shows user save slot names and statuses and asks them to enter choice.

				if (userChoice > 0 && userChoice <= 3)	// Saves game if user enters a valid save slot number.
				{
					saveGame(saveSlotFiles, saveSlotStatus, gameGrid, tileStatus,
						numOfPlayers, currentPlayer, playerScores,
						amountNonMinesHidden, amountOfActions, userChoice);	// Saves current game.

					// Asks user if they wish to exit and exits game if user chooses yes.
					printf("\n\nDo you wish to exit game? (y/n): ");
					(void)scanf(" %c", &userInput);

					if (userInput == 'y')
					{
						gameStatus = GAME_STATUS_EXIT;
					}
				}
			}
			else if (userInput == 'e')	// Asks user if they wish to exit and exits game if user chooses yes.
			{
				printf("\n\nDo you wish to exit game? (y/n): ");
				(void)scanf(" %c", &userInput);

				if (userInput == 'y')
				{
					gameStatus = GAME_STATUS_EXIT;
				}
			}
			else	// Move player on grid
			{
				moveCursor(userInput, &cursorX, &cursorY);	// Moves player based on user input.
			}

			if (gameStatus == GAME_STATUS_ACTIVE)
			{
				if (emptyTile > 0)	// If there is an empty tile, clear adjacent tiles.
				{
					amountNonMinesHidden -= clearEmptyTiles(gameGrid, tileStatus, playerScores, currentPlayer);	// Clears all empty tiles adjacent to each other.
					emptyTile = 0;
				}

				currentPlayer = amountOfActions % numOfPlayers;

				// Displays scores, grid and controls.
				printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");	// Ensures user cannot see previous grid unless they scroll up.
				displayScores(playerScores, numOfPlayers);
				printf("Current Player: %d/%d\n", (currentPlayer + 1), numOfPlayers);
				displayGrid(gameGrid, tileStatus, cursorX, cursorY);
				showControls();
			}

			if (amountNonMinesHidden - AMOUNT_OF_MINES <= 0) // Sets gameStatus to GAME_STATUS_WIN if all the tiles that are not mines are uncovered.
			{
				gameStatus = GAME_STATUS_WIN;
			}

			if (gameStatus == GAME_STATUS_LOSE)	// Displays "Mine Hit" message if gameStatus is GAME_STATUS_LOSE.
			{
				printf("\n\nMINE HIT!\n");

				if (numOfPlayers > 1)	// Displays which player hit the mine if there are more than 1 players.
				{
					printf("Player %d loses\n", (loser + 1));
					gameStatus = GAME_STATUS_WIN;	// Sets gameStatus to GAME_STATUS_WIN.
				}
				else	// Displays "You lose" message if there is only 1 player.
				{
					printf("\n\nYOU LOSE!\n");
				}
			}
			
			if (gameStatus == GAME_STATUS_WIN && numOfPlayers == 1)	// Displays "You win" message if gameStatus = GAME_STATUS_WIN and there is only 1 player.
			{
				printf("\n\nYOU WIN!\n");
			}
			else if (gameStatus == GAME_STATUS_WIN)	// If gameStatus = GAME_STATUS_WIN.
			{
				winner = getHighestScore(playerScores, numOfPlayers, loser);	// Calculates which player has the most points (not including the player who hit the mine) and sets winner to the number of that player.

				if (winner == -1)	// Displays "Draw" message if there is no winner (draw) and all tiles were cleared.
				{
					printf("\n\nAll tiles cleared!\n");
					printf("Result: Draw\n");
				}
				else if (winner == -1 && loser != -1)	// // Displays "Draw" message if there is no winner (draw) and a mine was hit.
				{
					printf("Result: Draw\n");
				}
				else if (loser != -1)	// Displays which player had the most points if a mine was hit.
				{
					printf("Player %d wins! Score was: %d\n", (winner + 1), playerScores[winner]);
				}
				else // // Displays which player had the most points all tiles without mines were cleared.
				{
					printf("\n\nAll tiles cleared!\n");
					printf("Player %d wins! Score was: %d\n", (winner + 1), playerScores[winner]);
				}
			}
		}
	}

	if (gameStatus != GAME_STATUS_EXIT)	// Displays uncovered grid if user did not exit the game.
	{
		// Sets cursor to -2,-2 so that it can not be seen on the grid.
		cursorX = -2;
		cursorY = -2;

		// Sets all tiles to uncovered.
		for (int y = 0; y < MAX_Y; y++)
		{
			for (int x = 0; x < MAX_X; x++)
			{
				tileStatus[y][x] = TILE_STATUS_UNCOVERED;
			}
		}

		// Displays the final uncovered grid.
		displayGrid(gameGrid, tileStatus, cursorX, cursorY);
	}
}

// Randomly sets positions of the mines and ensures that no mines are on the same tile. 
void randomMinePosition(char gameGrid[MAX_Y][MAX_X])
{
	int randomX, randomY;

	for (int i = 0; i < AMOUNT_OF_MINES; i++)
	{
		do
		{
			randomX = (rand() % MAX_X);
			randomY = (rand() % MAX_Y);

		} while (gameGrid[randomY][randomX] == 'M');

		gameGrid[randomY][randomX] = 'M';
	}
}

// Displays the grid to the screen.
void displayGrid(char gameGrid[MAX_Y][MAX_X], int tileStatus[MAX_Y][MAX_X], int cursorX, int cursorY)
{
	for (int y = -1; y <= MAX_Y; y++)
	{
		printf("\t");

		for (int x = -1; x <= MAX_X; x++)
		{
			if (y == -1 || y == MAX_Y)	// Prints top/bottom "wall" to screen if at top/bottom of the grid.
			{
				printf("=");
			}
			else if (x == -1 || x == MAX_X)
			{
				printf("|");	// Prints side "wall" to screen if at side of the grid.
			}
			else
			{
				if (x == cursorX && y == cursorY)	// Prints 'X' to screen if equal to co-ordinates of cursor.
				{
					printf("X");
				}
				else if (tileStatus[y][x] == TILE_STATUS_HIDDEN)	// Prints '-' to screen if tileStatus at co-ordinates is TILE_STATUS_HIDDEN.
				{
					printf("-");
				}
				else if (tileStatus[y][x] == TILE_STATUS_FLAG)	// Prints 'F' to screen if tileStatus at co-ordinates is TILE_STATUS_FLAG.
				{
					printf("F");
				}
				else if(gameGrid[y][x] == '0')	// Prints ' ' to screen if tile in gameGrid at co-ordinates is '0'.
				{
					printf(" ");
				}
				else	// Prints the char in gameGrid at co-ordinates.
				{
					printf("%c", gameGrid[y][x]);
				}
			}
		}

		printf("\n");
	}
}

// Displays scores of all players in the game.
void displayScores(int playerScores[MAX_PLAYERS], int numOfPlayers)
{
	printf("Scores: ");

	for (int i = 0; i < numOfPlayers; i++)
	{
		printf("P%d=%d", (i + 1), playerScores[i]);

		if (i < numOfPlayers - 1)
		{
			printf(", ");
		}
	}

	printf("\n");
}

//Calculates amount of mines around a tile.
char setTileValue(int x, int y, char gameGrid[MAX_Y][MAX_X])
{
	int amountOfMines = 0;
	char result;

	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			if (i + y >= 0 && i + y < MAX_Y && j + x >= 0 && j + x < MAX_X)
			{
				if (gameGrid[y + i][x + j] == 'M')
				{
					amountOfMines++;
				}
			}
		}
	}

	result = amountOfMines + '0';	// Changes int amountOfMines into char result.

	return result;
}

// Moves cursor based on users input.
void moveCursor(char inputMovement, int* cursorX, int* cursorY)
{
	// Sets newX and newY to cursorX and cursorY.
	int newY = *cursorY;
	int newX = *cursorX;

	// Changes newX or newY based on users input.
	switch (inputMovement)
	{
	case 'w':
		newY--;
		break;

	case 's':
		newY++;
		break;

	case 'a':
		newX--;
		break;

	case 'd':
		newX++;
		break;
	}

	if (newX >= 0 && newX < MAX_X && newY >= 0 && newY < MAX_Y) // Sets cursorX and cursorY to newX and newY if new co-ordinates are not out of the bounds of the grid.
	{
		*cursorX = newX;
		*cursorY = newY;
	}
}

// Uncovers all adjacent empty tiles and tiles adjacent to empty tiles.
int clearEmptyTiles(char gameGrid[MAX_Y][MAX_X], int tileStatus[MAX_Y][MAX_X], int playerScores[MAX_PLAYERS], int currentPlayer)
{
	int offsets[4][4] = { {-1, 1, 0, 0}, {0, 0, -1, 1} };	// 2d array of offsets relative to the co-ordinates of the current tile. Row 0 stores Y offsets and Row 1 stores X offsets.
	int tilesToChange;	// Stores how many tiles have a tileStatus value of TILE_STATUS_TO_BE_UNCOVERED.
	int amountUncovered = 0;	// Stores amount of tiles that have been uncovered.
	int offsetY, offsetX;	// Current offsets relative to the co-ordinates of the current tile.

	do	// Repeats until no tiles have a tileStatus value of TILE_STATUS_TO_BE_UNCOVERED.
	{
		tilesToChange = 0;

		for (int y = 0; y < MAX_Y; y++)
		{
			for (int x = 0; x < MAX_X; x++)
			{
				if (tileStatus[y][x] == TILE_STATUS_TO_BE_UNCOVERED)
				{
					for (int i = 0; i <= 3; i++)
					{
						// Sets offsetX and offsetY to the value of the element at the index in the array offsets.
						offsetY = y + offsets[0][i];
						offsetX = x + offsets[1][i];

						if (offsetX >= 0 && offsetX < MAX_X && offsetY >= 0 && offsetY < MAX_Y)	// If the tile being checked is not outside of the bounds of the grid.
						{
							if (gameGrid[offsetY][offsetX] == '0' && tileStatus[offsetY][offsetX] == 0)	// Sets tileStatus to TILE_STATUS_TO_BE_UNCOVERED if the tile being checked is empty and was not already uncovered.
							{
								tileStatus[offsetY][offsetX] = TILE_STATUS_TO_BE_UNCOVERED;
							}
							else if (gameGrid[offsetY][offsetX] != '0' && tileStatus[offsetY][offsetX] == 0)	// Sets tileStatus to TILE_STATUS_UNCOVERED if the tile being checked is not empty and was not already uncovered.
							{
								tileStatus[offsetY][offsetX] = TILE_STATUS_UNCOVERED;
								amountUncovered++;

								// Casts tile from char to int to get ASCII value of tile, then subtracts 48 (ASCII '0') to get real value.
								// This would not work if the real tile was greater than 9, but is ok in this case because the highest real value a tile can have is 8.
								playerScores[currentPlayer] += ((int)gameGrid[offsetY][offsetX] - 48);
							}
						}
					}

					tileStatus[y][x] = TILE_STATUS_UNCOVERED;	// Sets current tile which has a tileStatus value of TILE_STATUS_TO_BE_UNCOVERED to a tileStatus value of TILE_STATUS_UNCOVERED.
					amountUncovered++;
					tilesToChange++;
				}
			}
		}

	} while (tilesToChange > 0);

	return amountUncovered;
}

// Calculates which player has the highest score and returns their index number. Returns -1 (draw) if 2 or more players have the same score.
// If the game has loser (player who hit a mine) they are not inclued.
int getHighestScore(int playerScores[MAX_PLAYERS], int numOfPlayers, int loser)
{
	int winner = -1;

	for (int i = 0; i < numOfPlayers; i++)
	{
		if (i != loser)
		{
			if (playerScores[i] > playerScores[winner])
			{
				winner = i;
			}
			else if (playerScores[i] == playerScores[winner])
			{
				winner = -1;
				break;
			}
		}
	}

	return winner;
}

// Checks if save slot is empty (file is empty or first line has char '0') or save slot has a game saved (first line has char '1').
void checkIfEmpty(char saveSlotFiles[NUM_OF_SAVE_SLOTS][15], int saveSlotStatus[NUM_OF_SAVE_SLOTS])
{
	FILE* file1;

	char temp;

	for (int i = 0; i < NUM_OF_SAVE_SLOTS; i++)
	{
		file1 = fopen(saveSlotFiles[i], "r");	// Opens file at index i of saveSlotFiles array.

		if (file1 != NULL)
		{
			(void)fscanf(file1, "%c", &temp);

			if (temp == '1')
			{
				saveSlotStatus[i] = SAVE_SLOT_SAVED;
			}
			else
			{
				saveSlotStatus[i] = SAVE_SLOT_EMPTY;
			}
		}
		else
		{
			saveSlotStatus[i] = SAVE_SLOT_EMPTY;
		}
	}
}

// Saves current game into a selected save slot.
void saveGame(char saveSlotFiles[NUM_OF_SAVE_SLOTS][15], int saveSlotStatus[NUM_OF_SAVE_SLOTS],
	char gameGrid[MAX_Y][MAX_X], int tileStatus[MAX_Y][MAX_X], int numOfPlayers,
	int currentPlayer, int playerScores[MAX_PLAYERS], int amountNonMinesHidden, int amountOfActions, int userChoice)
{
	// Line 1 of file stores the saveSlotStatus.
	// Line 2 of file stores the gameGrid array.
	// Line 3 of file stores the tileStatus array.
	// Line 4 of file stores numOfPlayers.
	// Line 5 of file stores currentPlayer.
	// Line 6 of file stores the playerScores array.
	// Line 7 of file stores amountNonMinesHidden.
	// Line 8 of file stores amountOfActions.

	FILE* file1;

	file1 = fopen(saveSlotFiles[userChoice - 1], "w"); // Opens file at index [userChoice - 1] of saveSlotFiles array.

	// Writes save data to file.
	if (file1 != NULL)
	{
		fprintf(file1, "1\n");

		for (int i = 0; i < MAX_Y; i++)
		{
			for (int j = 0; j < MAX_X; j++)
			{
				fprintf(file1, "%c ", gameGrid[i][j]);
			}
		}

		fprintf(file1, "\n");

		for (int i = 0; i < MAX_Y; i++)
		{
			for (int j = 0; j < MAX_X; j++)
			{
				fprintf(file1, "%d ", tileStatus[i][j]);
			}
		}

		fprintf(file1, "\n%d", numOfPlayers);
		fprintf(file1, "\n%d\n", currentPlayer);

		for (int i = 0; i < numOfPlayers; i++)
		{
			fprintf(file1, "%d ", playerScores[i]);
		}

		fprintf(file1, "\n%d", amountNonMinesHidden);
		fprintf(file1, "\n%d", amountOfActions);
	}

	if (file1 != NULL)
	{
		fclose(file1);
		printf("Game Saved");
	}
}

// Loads selected save slot.
void loadGame(char saveSlotFiles[NUM_OF_SAVE_SLOTS][15], int saveSlotStatus[NUM_OF_SAVE_SLOTS],
	char gameGrid[MAX_Y][MAX_X], int tileStatus[MAX_Y][MAX_X], int* numOfPlayers,
	int* currentPlayer, int playerScores[MAX_PLAYERS], int* amountNonMinesHidden, int* amountOfActions, int saveSlot)
{
	// Line 1 of file stores the saveSlotStatus.
	// Line 2 of file stores the gameGrid array.
	// Line 3 of file stores the tileStatus array.
	// Line 4 of file stores numOfPlayers.
	// Line 5 of file stores currentPlayer.
	// Line 6 of file stores the playerScores array.
	// Line 7 of file stores amountNonMinesHidden.
	// Line 8 of file stores amountOfActions.

	FILE* file1;

	// Temporary variables which after being set will set the pointer variables.
	int temp = 0;
	char tempChar;
	int tempNumOfPlayers = 0;
	int tempCurrentPlayer = 0;
	int tempAmountNonMinesHidden = 0;
	int tempAmountOfActions = 0;

	file1 = fopen(saveSlotFiles[saveSlot - 1], "r");	// Opens file at index [userChoice - 1] of saveSlotFiles array.

	// Reads save data from file and sets variables.
	if (file1 != NULL)
	{
		(void)fscanf(file1, "%d ", &temp);

		for (int i = 0; i < MAX_Y; i++)
		{
			for (int j = 0; j < MAX_X; j++)
			{
				(void)fscanf(file1, "%c ", &gameGrid[i][j]);
			}
		}

		for (int i = 0; i < MAX_Y; i++)
		{
			for (int j = 0; j < MAX_X; j++)
			{
				(void)fscanf(file1, "%d ", &tileStatus[i][j]);
			}
		}

		(void)fscanf(file1, "%d", &tempNumOfPlayers);
		*numOfPlayers = tempNumOfPlayers;

		(void)fscanf(file1, "%d", &tempCurrentPlayer);
		*currentPlayer = tempCurrentPlayer;

		(void)fscanf(file1, "%c", &tempChar);

		for (int i = 0; i < *numOfPlayers; i++)
		{
			(void)fscanf(file1, "%d ", &playerScores[i]);
		}

		(void)fscanf(file1, "%d", &tempAmountNonMinesHidden);
		*amountNonMinesHidden = tempAmountNonMinesHidden;

		(void)fscanf(file1, "%d", &tempAmountOfActions);
		*amountOfActions = tempAmountOfActions;
	}

	if (file1 != NULL)
	{
		fclose(file1);
		printf("Game Successfully Loaded\n\n");
	}
}

// Deletes selected save slot by setting first line of file to char '0'.
void deleteSave(char saveSlotFiles[NUM_OF_SAVE_SLOTS][15], int userChoice)
{
	FILE* file1;

	file1 = fopen(saveSlotFiles[userChoice - 1], "w");	// Opens file at index [userChoice - 1] of saveSlotFiles array.

	if (file1 != NULL)
	{
		fprintf(file1, "0");
	}

	if (file1 != NULL)
	{
		fclose(file1);
	}
}

// Shows user save slot names and statuses and asks them to enter choice.
int displaySaveSlotStatus(int saveSlotStatus[NUM_OF_SAVE_SLOTS])
{
	char saveSlotStatusDisplay[2][10] = { {"Empty"}, {"Saved"} };
	int userChoice;

	printf("0. Back\n");

	for (int i = 0; i < 3; i++)
	{
		printf("%d. Save Slot %d: %s\n", (i + 1), (i + 1), saveSlotStatusDisplay[saveSlotStatus[i]]); // Displays save slot name and its status (Empty/Saved).
	}

	printf("Enter Number of save slot: ");
	(void)scanf("%d", &userChoice);

	return userChoice;
}

// Prints controls to the screen.
void showControls()
{
	printf("Use the w, a, s, d keys to move the cursor 'X'.\n");
	printf("Press 'q' to uncover tile. Press 'f' to place a flag. Press 'x' to save game. Press 'e' to exit the game.\n");
	printf("Enter choice: ");
}