#pragma once
#include "GUI.h"
#include <Windows.h>
#include <iostream>
#include <vector>
#include <array>
#include <time.h>
constexpr int RIGHT = 1;;
constexpr int LEFT = -1;;

/// <summary>
/// A square's (it only stores color, but I am too lazy to rewrite)
/// </summary>
class Square {
public:
	int color = -1;

	Square() {
	}
};

/// <summary>
/// The pieces which are falling down -,-
/// </summary>
class FallingPieces {
public: int color;
	  std::vector<std::array<int, 2>> pieces;
	  //The type of the majestic falling pieces
private:
	int type = rand() % 7;
public: FallingPieces() {
	color = rand() % numberOfColors;
	pieces.push_back({ rows - 1,5 });
	pieces.push_back({ rows - 1,4 });
	//I couldn't find a more efficient method so this left
	switch (type)
	{
	case 0:
		pieces.push_back({ rows - 1,6 });
		pieces.push_back({ rows - 1,7 });
		break;
	case 1:
		pieces.push_back({ rows,4 });
		pieces.push_back({ rows - 1,6 });
		break;
	case 2:
		pieces.push_back({ rows ,6 });
		pieces.push_back({ rows - 1,6 });
		break;
	case 3:
		pieces.push_back({ rows ,4 });
		pieces.push_back({ rows ,5 });
		break;
	case 4:
		pieces.push_back({ rows,5 });
		pieces.push_back({ rows,6 });
		break;
	case 5:
		pieces.push_back({ rows - 1,6 });
		pieces.push_back({ rows ,5 });
		break;
	case 6:
		pieces.push_back({ rows ,3 });
		pieces.push_back({ rows ,4 });
		break;

	default:
		break;
	}
}

	  /// <summary>
	  /// Rotates the piece to the right or to the left
	  /// </summary>
	  /// <param name="dir"></param>
	  bool Rotate(int dir) {
		  if (type == 3)
			  return false;
		  //Sets the 0 relative cord to the 1. piece in the pieces vector
		  int row = this->pieces[0][0];
		  int col = this->pieces[0][1];

		  for (size_t i = 1; i < pieces.size(); i++)
		  {
			  int temp = (pieces[i][0] - row) * (dir * -1);
			  pieces[i][0] = (pieces[i][1] - col) * dir + row;
			  pieces[i][1] = temp + col;

			  //Test for if it will go out of the screen, because of the rotation
			  if (pieces[i][0] < 0 || pieces[i][0] >= rows || pieces[i][1] < 0 || pieces[i][1] >= cols)
			  {
				  return false;
			  }
		  }

		  return true;
	  }
};

class Tetris
{
public:
	Tetris() {
		score = 0;
		lines = 0;
		level = 0;
		srand(time(0) * 15);
		fallingPieces = new FallingPieces();
		over = false;
	}

public:
	Square board[rows][cols];
	FallingPieces* fallingPieces;
	bool over;
private:
	int score;
	int lines;
	int level;
	int lineScores[4] = { 40,100,300,1200 };
	bool softDrop = false;

	//**************************************************
	//Methods
public:
	/// <summary>
	/// Makes the fallingPieces fall
	/// </summary>
	bool Fall() {
		if (over)
			return true;
		if (softDrop)
			score++;
		if (HeightAndCollisionDetection()) {
			for (size_t i = 0; i < fallingPieces->pieces.size(); i++)
			{
				board[fallingPieces->pieces[i][0]][fallingPieces->pieces[i][1]].color = fallingPieces->color;
			}
			do
			{
			} while (CheckForFullRows());

			fallingPieces = new FallingPieces();
			if (HeightAndCollisionDetection())
				over = true;
			return true;
		}

		else {
			for (size_t i = 0; i < fallingPieces->pieces.size(); i++)
			{
				fallingPieces->pieces[i][0]--;
			}
		}
		return false;
	}

	/// <summary>
	/// Move the fallingPieces right or left
	/// </summary>
	/// <param name="dir">RIGHT or LEFT</param>
	void Move(int dir) {
		//So that the dear player can't move the piece out of the screen
		for (size_t i = 0; i < fallingPieces->pieces.size(); i++)
		{
			if (fallingPieces->pieces[i][1] + dir < 0 || fallingPieces->pieces[i][1] + dir >= cols)
				return;
		}
		for (size_t i = 0; i < fallingPieces->pieces.size(); i++)
		{
			if (board[fallingPieces->pieces[i][0]][fallingPieces->pieces[i][1] + dir].color != -1) {
				return;
			}
		}
		for (size_t i = 0; i < fallingPieces->pieces.size(); i++)
		{
			fallingPieces->pieces[i][1] += dir;
		}
	}

	/// <summary>
	/// Rotates the fallingPieces
	/// </summary>
	/// <param name="dir">RIGHT or LEFT</param>
	void Rotate(int dir) {
		//Make a save of the current state so we can revert back if the rotation would cause cheating or bugs
		std::vector<std::array<int, 2>> save = fallingPieces->pieces;
		if (!fallingPieces->Rotate(dir)) {
			//Revert to the previous state
			fallingPieces->pieces = save;
		}
		else
		{
			//Check if the rotation would go into already set squares
			bool isCorrect = true;
			for (size_t i = 0; i < fallingPieces->pieces.size(); i++)
			{
				if (board[fallingPieces->pieces[i][0]][fallingPieces->pieces[i][1]].color != -1) {
					isCorrect = false;
					break;
				}
			}
			if (!isCorrect)
				//Revert to the previous state
				fallingPieces->pieces = save;
		}
	}

	/// <summary>
	/// Makes the fallingPieces fall to the bottom instantly
	/// </summary>
	void HardDrop() {
		if (over)
			return;
		int counter = 0;
		do
		{
			counter++;
		} while (!Fall());
		score += counter * 2;
		std::cout << score << std::endl;
	}

	/// <summary>
	/// Turns on softDrop score
	/// </summary>
	void SoftDropOn() {
		softDrop = true;
	}

	/// <summary>
	/// Turns of softDrop score
	/// </summary>
	void SoftDropOff() {
		softDrop = false;
		std::cout << score << std::endl;
	}

	/// <summary>
	/// Returns the score
	/// </summary>
	int GetScore() {
		return score;
	}

private:
	//----------------------------------------------------------------------------
	//Returns true if the fallingPieces hit the bottom or an another piece
	bool HeightAndCollisionDetection() {
		for (size_t i = 0; i < fallingPieces->pieces.size(); i++)
		{
			if (fallingPieces->pieces[i][0] > 0) {
				if (board[fallingPieces->pieces[i][0] - 1][fallingPieces->pieces[i][1]].color != -1) {
					return true;
				}
			}
			else
				return true;
		}

		return false;
	}

	//------------------------------------------------------------------------------
	//Checks for full rows so it can dissapear and the user can get score for it
	//returns true if there was atleast one full row false if there wasn't
	bool CheckForFullRows() {
		int count = 0;

		for (int i = rows - 1; i >= 0; i--)
		{
			for (int j = 0; j < cols; j++)
			{
				if (board[i][j].color == -1) {
					if (count != 0) {
						goto End;
					}
					break;
				}

				if (j == cols - 1) {
					//Deletes this full row
					DeleteRow(i);
					count++;
				}
			}
		}

	End:
		if (count != 0) {
			lines += count;
			level = lines / 10;
			score += lineScores[count - 1] * (level + 1);
			std::cout << score << std::endl;
			return true;
		}

		return false;
	}

	//-----------------------------------------------------------------------
	//Deletes the specified row in the board matrix
	void DeleteRow(int row) {
		for (size_t i = row; i < rows - 1; i++)
		{
			for (size_t j = 0; j < cols; j++)
			{
				board[i][j].color = board[i + 1][j].color;
			}
		}

		for (size_t i = 0; i < cols; i++)
		{
			board[rows - 1][i].color = -1;
		}
	}
};
