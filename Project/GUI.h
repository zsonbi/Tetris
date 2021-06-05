#pragma once
#ifndef GUI_H_INCLUDED
#define GUI_H_INDLUDED
/// <summary>
/// Number of rows
/// </summary>
static const int rows = 20;
/// <summary>
/// Number of columns
/// </summary>
static const int cols = 10;
/// <summary>
/// The size of the entire matrix on the screen
/// </summary>
static const int matrixSize = 20;
/// <summary>
/// The number of different colors which are stored
/// </summary>
static const int numberOfColors = 7;
/// <summary>
/// Initializes the window and the Tetris game
/// </summary>
void Init(int, char**);

#endif // !GUI_H_INCLUDED
