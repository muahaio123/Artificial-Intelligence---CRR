// YOUR NAME: Thanh Long Le
//
// CS 4318, spring 2021
// Agent Challenge 5: Chocolate Russian roulette
//
// Rename this file and the function below.  For example, if your agent name
// is Jones, rename this crrAgentSmith.cpp file to crrAgentJones.cpp and the
// crrAgentSmith function below to crrAgentJones.  Complete your agent
// function and turn it in on Blackboard.  Random-number generation is not
// allowed; your agent must be entirely deterministic.  Feel free to create
// other agents--each in a separate .cpp file--for yours to play against,
// but turn in only one.  Test your agent(s) with
//
//    nice bash crrBuild.bash
//
// and then
//
//    nice ./crrRunSim
//
// Each submitted agent will play each other using chocolate bar sizes
// varying from 3x4 to 6x9, once moving first and once moving second, to
// determine the standings, which will be posted soon after the agents are
// due.

#include "crr.h"
#include <vector>

namespace
{
	// If you need to define any new types or functions, put them here in
	// this unnamed namespace.  But no variables allowed!
	
	int countRow(const ChocolateBar &bar, int row)	// count all the chocolate left in any row (even the poison)
	{
		int chocolate = 0;
		for (int c = 0; c < bar.getWidth(); ++c)
		{
			if (bar.isSquareThere(ChocolateSquareLocation(c, row)))
				chocolate++;
			else
				return chocolate;
		}
		return chocolate;
	}
	
	int countColumn(const ChocolateBar &bar, int column)	// count all chocolate left in any column (even the poison)
	{
		int chocolate = 0;
		for (int r = 0; r < bar.getHeight(); ++r)
		{
			if (bar.isSquareThere(ChocolateSquareLocation(column, r)))
				chocolate++;
			else
				return chocolate;
		}
		return chocolate;
	}
	
	bool isTwoVertical(ChocolateBar &bar)	// if we are in a 2 finger vertical situation
	{
		for (int x = 2; x < bar.getWidth(); ++x)
			if (countColumn(bar, x) != 0)
				return false;
		
		return true;
	}
	
	bool isTwoHorizontal(ChocolateBar &bar)	// if we are in a 2 finger horizontal situation
	{
		for (int y = 2; y < bar.getHeight(); ++y)
			if (countRow(bar, y) != 0)
				return false;
				
		return true;
	}
	
	int eval(ChocolateBar &bar, bool isMax)
	{
		int countColumn0 = countColumn(bar, 0), countRow0 = countRow(bar, 0);
		
		// if we get into a 2 arm situations (where 1-1 is eaten)
		if (!bar.isSquareThere(ChocolateSquareLocation(1, 1)))
		{
			if (countColumn0 == countRow0)	// 2 arms is equal
				return isMax == true ? -100 : 100;	// we lose
			else
				return isMax == true ? 100 : -100;	// we win
		}
		
		if (isTwoVertical(bar) == true)
		{
			int countColumn1 = countColumn(bar, 1);
			
			// if it is 2 fingers vertically and they have the same amount of schocolate (even the poison)
			if (countColumn0 == countColumn1)	// if they equal
				return isMax == true ? -100 : 100;	// we lose
			else
				return isMax == true ? 100 : -100;	// we win
		}
		
		if (isTwoHorizontal(bar) == true)
		{
			int countRow1 = countRow(bar, 1);
			
			// if we are in a situation of 2 fingers horizontally and they have the same amount of chocolate
			if (countRow0 == countRow1)	// if they equal
				return isMax == true ? -100 : 100;	// we lose
			else
				return isMax == true ? 100 : -100;	// we win
		}
		
		// a special situation where: 2 arms, 2-1 and 1-2 is eaten, and only 1-1 is still there
		if (bar.isSquareThere(ChocolateSquareLocation(1, 1)))
		{
			if (countRow0 == countColumn0)	// if they equal
				return isMax == true ? 100 : -100;	// we win
		}
		
		if (!bar.isAnyLeft())	// if the poison is eaten
			return isMax == true ? 100 : -100;	// we win
		
		return 50;
	}
	
	int minimax(ChocolateBar &bar, int depth, bool isMax)
	{
		ChocolateSquareLocation bite;
		ChocolateBar newBar;
		
		int bestOutcome, outcome;
		int depthLimit = bar.getHeight() * bar.getWidth() / (bar.getHeight() + bar.getWidth());	// depth limit is half of the size of the bar
		
		int score = eval(bar, isMax);	// evaluate the current board
		
		//-------return the value points---------
		if (score == 100 || score == -100)	// only return if they reach a win or lose situation
			return score;
		
		if (depth > depthLimit)	// end if run over the depth limit
			return 0;
		//-------return the value points---------
		
		// recuresively to call min and max
		if (isMax == true)	// we are the maximizing player
		{
			bestOutcome = INT_MIN;
			
			// for each square in the board
			for (int x = bar.getWidth() - 1; x >= 0; --x)
			{
				for (int y = bar.getHeight() - 1; y >= 0; --y)
				{
					bite = ChocolateSquareLocation(x, y);
					if (bar.isSquareThere(bite))	// if it is a valid move
					{
						newBar = bar;
						newBar.takeBite(bite);
						
						outcome = minimax(newBar, depth + 1, !isMax);	// minimize it for the opponent
						bestOutcome = max(outcome, bestOutcome);
					}
					
					if (bestOutcome == 100)
						break;
				}
				if (bestOutcome == 100)
					break;
			}
		}
		else	// opponent is minimizing
		{
			bestOutcome = INT_MAX;
			
			for (int x = bar.getWidth() - 1; x >= 0; --x)
			{
				for (int y = bar.getHeight() - 1; y >= 0; --y)
				{
					bite = ChocolateSquareLocation(x, y);
					if (bar.isSquareThere(bite))	// if it is a valid move
					{
						newBar = bar;
						newBar.takeBite(bite);
						
						outcome = minimax(newBar, depth + 1, !isMax);	// maxing it out for us
						bestOutcome = min(outcome, bestOutcome);
					}
					
					if (bestOutcome == -100)
						break;
				}
				if (bestOutcome == -100)
					break;
			}
		}
		
		return bestOutcome;
	}
}

// Rename and complete this agent function.
ChocolateSquareLocation crrAgentSmith(const ChocolateBar &bar)
{
	// Your function must end up returning a valid ChocolateSquareLocation.
	// No random-number generation allowed!
	
	// Replace this return statement with your agent code.
	// (The (0, 0) bite is not a good one, but it's guaranteed to be legal.)
	
	ChocolateBar newBar;
	ChocolateSquareLocation bestBite, bite;
	int bestOutcome, outcome;
	
	bool isMax = true;	// we are maximizing player
	bestOutcome = INT_MIN;
	
	for (int x = bar.getWidth() - 1; x >= 0; --x)
		for (int y = bar.getHeight() - 1; y >= 0; --y)
		{
			bite = ChocolateSquareLocation(x, y);
			
			if (bar.isSquareThere(bite))
			{
				newBar = bar;
				newBar.takeBite(bite);
				outcome = minimax(newBar, 1, !isMax);	// go to the  next move performed by the opponent (minimizing)
				
				if (outcome > bestOutcome)	// we are maxing so we only take the largest outcome
				{
					bestBite = bite;
					bestOutcome = outcome;
				}
			}
		}
	
	return bestBite;
}

/*

 - First, carefully comment your code above to clarify how it works.
 - Here, describe your approach and analyze it.  How exactly did you develop
   and test it?  What are its strengths and weaknesses?  Why do you expect
   it to do well against the other submitted agents?
 - Also make a note here if you talked about the assignment with anyone or
   gave or received any kind of help.
	- this is a version of minimax created in the combination of Professor LeGrand's code, with the online version of minimax
	URL: https://www.geeksforgeeks.org/minimax-algorithm-in-game-theory-set-3-tic-tac-toe-ai-finding-optimal-move/
  
	- i look for pattern like 2 arms, 2 finger vertically and horizontally
*/
