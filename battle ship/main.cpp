/* 
Rebecca van der Lee
C++ fall 2020
Due: 12/2/2020
Lab 7 Battleship
Description: This program functions as a battleship game where the user  places 5 ships and the computer
			 randomly places its 5 ships. The user and the enemy will take turns firing at eachothers ships
			 until the player wins, loses, or surrenders. They then have the option to play again.
			 */






#include<iostream>  // standard library for c++
#include<iomanip>   // needed to use formatting fucntions such as setw
#include<stdlib.h>  // Stdlib.h allows for clear screen and random numbers.
#include<time.h>    // needed for generating a random number.
#include<cassert> // allows for the use of assert function so that the program gracefully exits when there is a bad input

using namespace std;

struct boardMarkers          // "boardMarkers" is a struct that is used as a container that holds the 
{                             // that contains value for each of the characters wich is used on  the game board
	const char water = '~';
	const char ship = '#';
	const char hit = 'H';
	const char miss = 'M';
};



struct ship                    //  This contains the components of which make up the data
{       
	                             // of a ship in the program. The ship has a starting location, size, direction and a hit tracker.  
	                             
	unsigned int xlocation;    
	unsigned int ylocation;
	unsigned int size;        
	int direction;
	int hits;
};


void initialize(boardMarkers type, ship playership[5], ship enemyship[5], char pgrid1[][10],
	char pgrid2[][10], char egrid1[][10], char egrid2[][10]);
// Preconditions : Arrays and structs are declared and flow of control is valid
// Postconditions: The arrays and structs are initailized
void display(char grid1[][10], char grid2[][10]);
// Preconditions : The flow of control is valid and values are initialized to the arrays
// Postconditions: The player grid is outputted to the screen
void placeShips(boardMarkers type, ship ship[5], char grid1[][10], char grid2[][10], bool& playerInput);
// Preconditions : The flow of control is valid
// Postconditions:  Values are assigned to the player and enemy arrays that signify the placement of their
// ships
void directionCheck(boardMarkers type, ship ship[5], char grid1[][10], bool direction[4], int i, char X,
	char Y);
// Preconditions : flow of control is valid annd enetered the placeships function, the players input
//                 should be between  0 and 9
// Postconditions: bool values are set to false if certain directions are not valid
void assignShip(boardMarkers type, ship ship[5], char grid1[][10], bool direction[4], bool& valid, int i,
	int X, int Y, int dInput);
// Preconditions : flow of control is valid and has moved to the placeships function, the bool values in the
//                 directioncheck function should have a mininum of one value that is true
// Postconditions: an error bool value becomes false or a ship will be assigned replacing water in the grids
void battlePhase(boardMarkers type, ship playership[5], ship enemyship[5],
	char pgrid1[][10], char pgrid2[][10], char egrid1[][10], char egrid2[][10],
	int& winScore, int& loseScore, bool& playerInput);
// Preconditions :  Values have been assigned to all ships for each fleet and the flow of control is valid  
// Postconditions: the game will finish when the enemy or the player won. This happens when the enemy or the player hit each other 17 times 
void turn(boardMarkers type, ship ship[5], char grid1[][10], char grid2[][10],
	char dgrid1[][10], char dgrid2[][10], int& playerScore, bool& playerInput);
// Preconditions : the flow of control is within the battlePhase function, and the surrender bool is false
// Postconditions: Either 1 is added to a ships hit counter and the enemy ship is changed to the type hit.   
//                 or a f, the water characters changes to a miss character
void checkHit(boardMarkers type, char grid1[][10], char grid2[][10], bool& error, bool playerInput,
	int& Score, int X, int Y);
// Preconditions : the flow of control is within the turn function and the user or cpu has inputed x cooridnates and y coordinates
//               
// Postconditions: First the program checks then  a new character is assigned to the grid. The This is charachter a hit or a miss character or bool varibale becomes false and the loop continues.
//                 a miss character or a bool becomes false and a loop continues
void checkSank(ship ship[5], bool playerInput, int X, int Y);
// Preconditions : the flow of control is within the turn function and the user or cpu has inputed 
//                  x and y coordinates, the x and y coordinates must be between 0 and 9
// Postconditions: if the conditions are met then the program will output that the boat sunk
void choice(bool& surrender);
// Preconditions : the flow of control is within the battlephase function and before the function call for turn.
// Postconditions: the flow of control continues if input or the bool surrender becomes true
void inputCheck();
// Preconditions : There is an input made and the flow of control stays valid
// Postconditions: the flow of control continues or the program gracefully closes depenfing on what the input is

int main()                 
{
	boardMarkers type;    // Deeclararion of boardmakers .
	ship playership[5];  // All the ships are declared for both the enemy and the player.
	ship enemyship[5];
	char pgrid1[10][10];  // Four  2d arrays are declared. The arrays acts as the grids in the
	char pgrid2[10][10];  // battleship game.
	char egrid1[10][10];
	char egrid2[10][10];
	char againInput;      // declaration of input character and two integers.
	int winScore = 0;
	int loseScore = 0;
	srand(unsigned int(time(NULL)));  // Initialized of the time seed.

	cout << "                     Welcome to Battleship\n";

	do                              // The game is contained within a loop that repeats. This will make sure that the player can play again if he inputs a 1
	{                               // inputs a '1' to playagain. The game is seperated in a few subfunctions
		bool playerInput = true;    //  each function has its own purpose.

		initialize(type, playership, enemyship, pgrid1, pgrid2, egrid1, egrid2);
		display(pgrid1, pgrid2);
		cout << "To place your ships, you must first choose an x coordinate and a y coordinate. Then "
			<< "choose a direction\n\n";
		placeShips(type, playership, pgrid1, pgrid2, playerInput);
		placeShips(type, enemyship, egrid1, egrid2, playerInput);
		display(egrid1, egrid2);
		battlePhase(type, playership, enemyship, pgrid1, pgrid2, egrid1, egrid2, winScore, loseScore, playerInput);

		cout << "\nThere are still plenty of enemy fleets to battle\n\nIf you want to play again"
			<< " input 1, if not input 2, then press enter: ";
		cin >> againInput;                                      // A message is outputted that asked the user if he wants to play again.
		system("CLS");                                          

	} while (againInput == '1'); // Checks if it should run again.

	cout << "\nWins: " << winScore << endl;                                         //Score is outputted here
	cout << "Losses: " << loseScore << " (A surrender counts as a loss)" << endl;
}

// This function assigns values to both the components of each array and all the structs.
//For this function, the parameters are mostly arrays. Only  boardMarkers is not an array, so
// They are automatically passed by reference. for "boardmakes" it is not needed to be passed by reference".
void initialize(boardMarkers type, ship playership[5], ship enemyship[5], char pgrid1[][10],
	char pgrid2[][10], char egrid1[][10], char egrid2[][10])
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			pgrid1[i][j] = type.water;   //Each grid is initialized to "~" which is water in the game.
			pgrid2[i][j] = type.water;
			egrid1[i][j] = type.water;
			egrid2[i][j] = type.water;
		}
	}
	for (int i = 0; i < 5; i++)
	{
		playership[i].xlocation = 0;   enemyship[i].xlocation = 0;      // Initializtion of the individual 
		playership[i].ylocation = 0;   enemyship[i].ylocation = 0;      // componenents of each struct. These are initialized in a for loop.
		playership[i].direction = 0;   enemyship[i].direction = 0;      
		playership[i].hits = 0;         enemyship[i].hits = 0;

		switch (i)
		{
		case 0: playership[i].size = 5; enemyship[i].size = 5; break; // carrier
		case 1: playership[i].size = 4; enemyship[i].size = 4; break; // battleship
		case 2: playership[i].size = 3; enemyship[i].size = 3; break; // cruiser
		case 3: playership[i].size = 3; enemyship[i].size = 3; break; // submarine
		case 4: playership[i].size = 2; enemyship[i].size = 2; break; // destroyer
		}
	}
}

// This function will display appropriate grids to the screen. This depends on the parameterst that are called. Either playerscreen or enemyscreen will be outputted to the screen.
 //This function passes arrays as the arguments.
//The function is called when the game board is outputtedto the screen.
void display(char grid1[][10], char grid2[][10])
{

	cout << "\n          Player Fleet                   Enemy Fleet" << endl << "     ";

	for (int i = 1; i <= 2; i++)
	{
		for (int i = 1; i <= 10; i++)
			cout << " " << i;
		cout << setw(11);
	}

	cout << endl;

	for (int i = 0; i < 10; i++)           //Imbedded for loops are used to output the board to the screen.
	{
		cout << setw(5) << (i + 1) << " ";

		for (int j = 0; j < 10; j++)
		{
			cout << grid1[i][j] << " ";
		}

		cout << setw(10) << (i + 1) << " ";

		for (int j = 0; j < 10; j++)
		{
			cout << grid2[i][j] << " ";
		}
		cout << endl;
	}

	cout << endl;
}

// This funciton will interpret the user's input and assign the value of a ship to either
// the players or the enemy's grid. The player is asked to enter x and y coordiantes, is asked to enter the size of the ship and the direction of the ship. 
// Thereby, the user or the computer selects a direction and the value of the grids are changed to the ship marker.
// This function uses reference parameters the only exception is "boardMarkes".
void placeShips(boardMarkers type, ship ship[5], char grid1[][10], char grid2[][10], bool& playerInput)
{
	int X;                            //declaration of  variables
	int Y;
	int dInput;
	bool valid;
	bool direction[4];

	for (int i = 0; i < 5; i++)
	{
		int n1 = 0;
		do
		{
			for (int i = 0; i < 4; i++)
			{
				direction[i] = true;
			}

			if (playerInput)
			{
				do
				{
					if (n1 > 0)
					{
						cout << endl << "Invalid coordinates. Try again\n" << endl;
						display(grid1, grid2);                                      //display is called
					}
					switch (i)
					{
					case 0: cout << "Place the Carrier\n" << endl;    break;
					case 1: cout << "Place the Battleship\n" << endl; break;
					case 2: cout << "Place the Cruiser\n" << endl;    break;
					case 3: cout << "Place the Submarine\n" << endl;  break;
					case 4: cout << "Place the Destroyer\n" << endl;  break;
					}

					cout << "Enter the X coordinate: ";
					cin >> X;
					inputCheck();

					cout << "Enter the Y coordinate: ";
					cin >> Y;
					inputCheck();    // the function input check is called

					X = X - 1;
					Y = Y - 1;

					n1++;
				} while ((X < 0 || X > 9) || (Y < 0 || Y > 9));
			}
			else
			{
				X = rand() % 10 + 0;   // The enemy (the computer) randomly inputs coordinates
				Y = rand() % 10 + 0;
			}

			ship[i].xlocation = X;
			ship[i].ylocation = Y;

			directionCheck(type, ship, grid1, direction, i, X, Y);  // call of the fucntion "directionCheck"

		} while ((grid1[Y][X] == type.ship) || (!direction[0] && !direction[1] && !direction[2] && !direction[3]));

		do
		{
			valid = true;

			if (playerInput)
			{
				cout << endl << "Choose a direction\n" << endl;

				if (direction[0])
					cout << "For right input 1" << endl;
				if (direction[1])
					cout << "For left input 2" << endl;
				if (direction[2])
					cout << "For down input 3" << endl;
				if (direction[3])
					cout << "For up input 4" << endl;

				cout << "\nDirection: ";
				cin >> dInput;            // The player inputs a direction
				inputCheck();
			}
			else
			{
				dInput = rand() % 5 + 1;  // input of direction by enemgy. This occurs randomly
			}

			ship[i].direction = dInput;

			assignShip(type, ship, grid1, direction, valid, i, X, Y, dInput);  // the function assignShip is called

			if (valid == false && playerInput)
			{
				cout << endl << "Invalid direction. Try again" << endl;
				display(grid1, grid2);
			}

		} while (!valid);

		if (playerInput)
		{
			system("CLS");
			display(grid1, grid2);
		}
	}

	if (playerInput)
		playerInput = false;
	else
		playerInput = true;
}

// "directionCheck" is a function within the function "placeShips" this lines of code will specifically determines
// which direction will be still water after selecting the starting location of the ship. this fucntion uses arrays. The only exception is the arrays. With the exception
// This function uses value parameters to get the data from "placeships'
void directionCheck(boardMarkers type, ship ship[5], char grid1[][10], bool direction[4], int i, char X,
	char Y)
{

	int S = ship[i].size - 1;

	for (S; S >= 0; S--)
	{
		if (grid1[Y][X + S] != type.water || X + S > 9)
			direction[0] = false; // right

		if (grid1[Y][X + (S / -1)] != type.water || X - S < 0)
			direction[1] = false; // left

		if (grid1[Y + S][X] != type.water || Y + S > 9)
			direction[2] = false; // down

		if (grid1[Y - S][X] != type.water || Y - S < 0)
			direction[3] = false; // up

	}
}

// "assignShip" is a function within "placeShips" this block of codes the direction that has been inputted in a switch statement to assign ships to the grid.
//This function uses value parameters to obtain data from "placeships. Value parameters are only not used for arrays. Arrays are use to get information from placeships

void assignShip(boardMarkers type, ship ship[5], char grid1[][10], bool direction[4], bool& valid, int i,
	int X, int Y, int dInput)
{
	int S = ship[i].size - 1;

	for (S; S >= 0; S--)  //  A for loop using ship size is implemented
	{
		switch (dInput)
		{
		case 1:                                   // A switch statement is used for directions
			if (direction[0])
				grid1[Y][X + S] = type.ship;
			else
				valid = false;
			break;
		case 2:
			if (direction[1])
				grid1[Y][X + (S / -1)] = type.ship;
			else
				valid = false;
			break;
		case 3:
			if (direction[2])
				grid1[Y + S][X] = type.ship;
			else
				valid = false;
			break;
		case 4:
			if (direction[3])
				grid1[Y - S][X] = type.ship;
			else
				valid = false;
			break;
		default: valid = false;
		}
	}
}

// In the function "battlePhase", the player and  the enmy attack each other. This section is  within a loop that exits when the game ends.
// In the loop, the flow of control switches between the player's turn and the enemy's turn. This function uses
// reference parameters. The only exception is the struct for boardMarkers
void battlePhase(boardMarkers type, ship playership[5], ship enemyship[5],
	char pgrid1[][10], char pgrid2[][10], char egrid1[][10], char egrid2[][10],
	int& winScore, int& loseScore, bool& playerInput)
{
	int playerScore = 0;
	int enemyScore = 0;
	bool surrender = false;

	cout << "The game will now commence. Your goal is to sink all  of the enemy ships"
		<< "\n\nTo target an enemy enter a set of coordinates.The cannons will do the rest\n\n";
	do
	{
		choice(surrender);   // call of the fucntion "Surrender"
		if (!surrender)
		{   // call of the function "turn" 
			turn(type, enemyship, egrid1, pgrid2, pgrid1, pgrid2, playerScore, playerInput);
			if (playerScore < 17)
			{
				choice(surrender);
				if (!surrender)
					turn(type, playership, pgrid1, egrid2, pgrid1, pgrid2, enemyScore, playerInput);
			}
		}
	} while (enemyScore < 17 && playerScore < 17 && surrender == false);

	if (surrender)
	{
		cout << "\nYou have surrendered\n";
		loseScore++;
	}
	else if (playerScore == 17)
	{
		cout << "Congratulations captain, you have taken out the enemy fleet.\n" << endl;
		winScore++;
	}
	else
	{
		cout << "\nThe enemy has sunken your last ship it is time to retreat.\n" << endl;
		loseScore++;
	}
}

// This fucntion adds seperation to the code by asking the player for an input. The user can either
// choose to continue to play or surrender. If continue is chosen the program continues. If surrender is chosen the flow
// of control changes "battlePhaes" and the loop exits. This will end he program.
void choice(bool& surrender)
{
	char input;
	do
	{
		cout << "Input c to continue or s to surrender then press enter: ";
		cin >> input;
		system("CLS");

	} while (input != 'c' && input != 's');

	if (input == 's')
		surrender = true;
}

// "turn" is a function that asks for the user's where the ships of the enemy is. The function asked to put in an x and a y coordinate.The function checks if there is an enemy on those coordinates.
// if there is not ship on those coordinates 'M' will be outputted on the grid. Turn switches between player input to enemy input by using the 
// bool "playerInput". The function makes use of reference parameters.
void turn(boardMarkers type, ship ship[5], char grid1[][10], char grid2[][10],
	char dgrid1[][10], char dgrid2[][10], int& Score, bool& playerInput)
{
	int X;
	int Y;
	bool error;

	if (!playerInput)
		cout << "\nThe enemy is attacking" << endl;

	do
	{
		error = true;

		if (playerInput)
		{
			cout << "\nAttack the enemy\n";
			display(dgrid1, dgrid2);
			cout << "\nEnter the X coordinate: ";
			cin >> X;
			inputCheck();                             // the enemy chooses to attack on a set of coordinates

			cout << "Enter the Y coordinate: ";
			cin >> Y;
			inputCheck();

			X = X - 1;
			Y = Y - 1;
		}
		else
		{
			X = rand() % 10 + 0;
			Y = rand() % 10 + 0;             // the enemy attacks random coordinates
		}

		checkHit(type, grid1, grid2, error, playerInput, Score, X, Y); //the function "checkHit" is called

	} while (!error);

	if (playerInput)
		cout << "\nBombs away\n";

	display(dgrid1, dgrid2);

	if (playerInput)
	{
		if (grid1[Y][X] == type.hit)
			cout << "You hit an enemy ship at (" << X + 1 << "," << Y + 1 << ")\n\n";
		if (grid1[Y][X] == type.miss)
			cout << "Sploosh, you missed at (" << X + 1 << "," << Y + 1 << ")\n\n";
	}
	else
	{
		if (grid1[Y][X] == type.hit)
			cout << "Uh oh, the enemy hit your ship at (" << X + 1 << "," << Y + 1 << ")\n\n";
		if (grid1[Y][X] == type.miss)
			cout << "The enemy missed at (" << X + 1 << "," << Y + 1 << ")\n\n";
	}

	checkSank(ship, playerInput, X, Y);

	if (playerInput)
		playerInput = false;
	else
		playerInput = true;
}

// checkHit is a subfunction of Turn.  The purpose of checkHit is to check the coordinates inputed in "turn"
// It chetcks if there is a character that represents a ship. If there is a ship the function reacts; This function
//uses both value and reference parameters.
void checkHit(boardMarkers type, char grid1[][10], char grid2[][10], bool& error, bool playerInput,
	int& Score, int X, int Y)
{
	if (X < 0 || X > 9 || Y < 0 || Y > 9)
	{
		error = false;
		if (playerInput)
			cout << "\nInvalid Coordinates. Try Again" << endl;
	}
	else if (grid1[Y][X] == type.hit || grid1[Y][X] == type.miss)
	{
		error = false;
		if (playerInput)
			cout << "\nYou've already attacked that space. Try Again" << endl;
	}
	else if (grid1[Y][X] == type.ship)
	{
		grid1[Y][X] = type.hit;
		grid2[Y][X] = type.hit;
		Score++;
	}
	else if (grid1[Y][X] == type.water)
	{
		grid1[Y][X] = type.miss;
		grid2[Y][X] = type.miss;
	}
}

// checSank is a subfunction of Turn. The purpose of checSank is to check the coordinates inputed in the function "turn"
// It checks to see if the attaack that player inputs, will let sank a ship of the enemy by chcking each location; The function uses both
// value and reference parameters;
void checkSank(ship ship[5], bool playerInput, int X, int Y)
{
	for (int i = 0; i < 5; i++)
	{
		int S = ship[i].size - 1;

		for (S; S >= 0; S--)
		{
			switch (ship[i].direction)
			{
			case 1:
				if (ship[i].ylocation == Y && ship[i].xlocation + S == X)
					ship[i].hits++; // right
				break;
			case 2:
				if (ship[i].ylocation == Y && ship[i].xlocation - S == X)
					ship[i].hits++; // left
				break;
			case 3:
				if (ship[i].ylocation + S == Y && ship[i].xlocation == X)
					ship[i].hits++; // down
				break;
			case 4:
				if (ship[i].ylocation - S == Y && ship[i].xlocation == X)
					ship[i].hits++; // up
				break;
			}
		}
		if (playerInput)
		{
			if (ship[i].hits == ship[i].size)
			{
				cout << "You sank the enemy ship\n\n";
				ship[i].hits++;
			}
		}
		else
		{
			if (ship[i].hits == ship[i].size)
			{
				switch (i)
				{
				case 0: cout << "The enemy sank your Carrier\n\n";    break;
				case 1: cout << "The enemy sank your Battleship\n\n"; break;
				case 2: cout << "The enemy sank your Cruiser\n\n";    break;
				case 3: cout << "The enemy sank your Submarine\n\n";  break;
				case 4: cout << "The enemy sank your Destroyer\n\n";  break;
				}
				ship[i].hits++;
			}
		}
	}
}

// "inputCheck" is a function that let the programe exit if there is a bad input
void inputCheck()
{
	if (!cin)
		cout << "\nPlease do not use character inputs. Close the program and restart to try again\n\n";
	assert(cin);
}