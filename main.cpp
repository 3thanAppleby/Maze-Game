/*
	GPF Week 6 - Maze Start Code
*/

#include <iostream>
#include <vector>
#include <time.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include "Framework.h"

using namespace std;

// Screen dimensions
int gScreenWidth{ 1920 };
int gScreenHeight{ 1080 };

// Delay to slow things down
int gTimeDelayMS{ 100 };

// Maze size as constants
constexpr int kMazeColumnsX{ 20 };
constexpr int kMazeRowsY{ 20 };

//Stores current MazeLevel;
int Level = 0;


void saveMaze();
void loadMaze();

/*
	2 Dimensional Arrays
	You can think of these as being an array of arrays
	
	The maze has kMazeRowsY columns across (20 by default) represented by an array
	We then have kMazeColumnsX (20) of these arrays, one for each row
	
	Columns and rows can be confusing so we tend to prefer working with x and y
	x is the horizontal axis (columns) across and y is the vertical axis (rows) down.

	Each single item is called a cell. 
	
	E.g. to output the value of the cell at 16 X (column 16) and 1 Y (row 1) we would write:
	cout << map[1][16] << endl; // outputs G
*/

char map[kMazeColumnsX][kMazeRowsY];

int PlayerY, PlayerX;

//Draws the Maze.
void DrawMaze() 
{
	int widthofCell{ gScreenWidth / kMazeColumnsX };
	int heightofCell{ gScreenHeight / kMazeRowsY };


	int screenY{ 0 };
	for (int cellx = 0; cellx < kMazeColumnsX; cellx++)
	{
		int screenX{ 0 };
		for (int celly = 0; celly < kMazeRowsY; celly++)
		{
			switch (map[cellx][celly])
			{
			case 'W':
				ChangeColour(0, 0, 0); 
				break;
			case '.':
				ChangeColour(255, 255, 255); 
				break;
			case 'G':
				ChangeColour(255, 255, 10); 
				break;
			case 'P':
				ChangeColour(0, 0, 255); 
				break;

			case 'F':
				ChangeColour(255, 50, 50);
				break;


			}

			//Draws the maze
			DrawRectangle(screenX, screenY, widthofCell, heightofCell); 

			screenX += widthofCell; 

		}

		screenY += heightofCell; 
	}
}

//Checks to see if the player can move into a square, if it can't that is because it is a wall or a fire.
bool canMoveThere(int y, int x) 
{
	if (map[y][x] != 'W')
	{
		return true;
	}

	return false;


	if (map[y][x] != 'F')
	{
		return true;
	}

	return false;

}

//Finds a new random space for the goal to move to
void FindFreeSpace(int &y, int &x) 
{
	do
	{
		x = rand() % kMazeColumnsX;
		y = rand() % kMazeRowsY;

	} while (!canMoveThere(y,x));
}

//Removes the player from the map
void removePlayer() 
{
		map[PlayerY][PlayerX] = '.';
}

//Moves the player depending on what key is pressed
void Playermove() 
{
	switch (GetLastKeyPressed())
	{
	case EKeyPressed::eLeft:
		if (canMoveThere(PlayerY, PlayerX - 1))
		{
			PlayerX--;
		}
		break;

	case EKeyPressed::eRight:
		if (canMoveThere(PlayerY, PlayerX + 1))
		{
			PlayerX++;
		}
		break;

	case EKeyPressed::eUp:
		if (canMoveThere(PlayerY - 1, PlayerX))
		{
			PlayerY--;
		}
		break;

	case EKeyPressed::eDown:
		if (canMoveThere(PlayerY + 1, PlayerX))
		{
			PlayerY++;
		}
		break;


	case EKeyPressed::eSave: 
	{
		saveMaze();
		break;

	}

	case EKeyPressed::eLoad:
	{

		loadMaze();
		break;

	}

	default:
		break;

	}
}


//Adds the player back into the map
void addPlayer() 
{	
		map[PlayerY][PlayerX] = 'P';
}

//Saves the Maze.
void saveMaze()
{

	int widthofCell{ gScreenWidth / kMazeColumnsX }; 
	int heightofCell{ gScreenHeight / kMazeRowsY }; 

	ofstream output;
	output.open("Maze0.txt");

	if (output.fail())
	{
		cerr << "Couldn't write to file!" << endl;
	}

	//Adds the player into the .txt file.
	addPlayer();

	int screenY{ 0 };
	for (int cellx = 0; cellx < kMazeColumnsX; cellx++)
	{
		int screenX{ 0 };
		for (int celly = 0; celly < kMazeRowsY; celly++)
		{

			output << map[cellx][celly] << " ";

		}
		output << endl;

	}

	output << PlayerY << " " << PlayerX << endl;
	output.close();

}

//Loads the Maze.
void loadMaze() 
{
	string filename = "Maze" + std::to_string(Level) + ".txt";

	ifstream input;
	input.open(filename);

	int widthofCell{ gScreenWidth / kMazeColumnsX };
	int heightofCell{ gScreenHeight / kMazeRowsY };

	int screenY{ 0 };
	for (int cellx = 0; cellx < kMazeColumnsX; cellx++)
	{
		int ScrrenX{ 0 };
		for (int celly = 0; celly < kMazeRowsY; celly++)
		{
			input >> map[cellx][celly];
			
		}

	}

	input >> PlayerY >> PlayerX;
	input.close();

	Level++;

	if (Level == 4)
	{
		cout << "Well done you have completed all my Mazes" << endl;
		exit(-1);
	}

}




int main()
{
	//Starts a timer.
	void StartClock();

	srand(time(0)); //Randomises the seed.
	
	//Loads the Maze.
	loadMaze();
	
	//Finds the Player.
	for (int Y = 0; Y < kMazeRowsY; Y++)
	{
		for (int X = 0; X < kMazeColumnsX; X++)
		{
			if (map[Y][X] == 'P')
			{
				PlayerY = Y;
				PlayerX = X;
			}
		}
	}

	while (UpdateFramework()) 
	{
		//Draws the Maze.
		DrawMaze();   

		//Removes the Player.
		removePlayer();   
		 
		//Moves the Player.
		Playermove();   

		//If the player hits the goal it moves to a different location and the console prints the time it took them to get to the goal.
		if (map[PlayerY][PlayerX] == 'G') 
		{

			cout << "Well done, you have reached the goal" << " it took you " << GetElapsedTime() << " Seconds" << endl; 

			loadMaze();

			//Resets the timer.
			StartClock();
			
		}

		if (map[PlayerY][PlayerX] == 'F') //If the player hits a Firepit they lose
		{
			cout << " You lose " << endl;
			break;
		}

		//Adds the player back into the map.
		addPlayer(); 

	}

	return 0;
}
