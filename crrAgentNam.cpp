// YOUR NAME: Nam Bui
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

namespace
{
   // If you need to define any new types or functions, put them here in
   // this unnamed namespace.  But no variables allowed!
   
    int barValue(const ChocolateBar &bar, int depth)
    {
		ChocolateBar newBar;
		ChocolateSquareLocation bestBite, bite;
		int bestOutcome, biteColumn, biteRow, outcome;
		
		if (!bar.isAnyLeft())
		{
			return depth % 2 == 0 ? 100 : -100;
		}
		
		// disregard everything above depth 5 for faster running time
		if (depth > 5)
		{
			return 0;
		}

		bestOutcome = depth % 2 == 0 ? INT_MIN : INT_MAX;
		
		for (biteColumn = bar.getWidth() - 1; biteColumn >= 0; biteColumn -= 1)
		{
			for (biteRow = bar.getHeight() - 1; biteRow >= 0; biteRow -= 1)
			{
				bite = ChocolateSquareLocation(biteColumn, biteRow);
				if (bar.isSquareThere(bite))
				{
					newBar = bar;
					newBar.takeBite(bite);
					outcome = barValue(newBar, depth++);
					if (outcome > bestOutcome)
					{
						bestOutcome = outcome;
					}
				}
			}
		}
		
		return bestOutcome;
	}
}

// Rename and complete this agent function.
ChocolateSquareLocation crrAgentNam(const ChocolateBar &bar)
{
	
	// some hardcode to reduce running time
	int remainWidth0 = 0;
	int remainHeight0 = 0;
	
	int remainWidth1 = 0;
	int remainHeight1 = 0;


	// count remaining width0
	for (int x = bar.getWidth() - 1; x > 0; x -= 1)
	{
		if (bar.isSquareThere(x, 0))
		{
			remainWidth0 = x;
			break;
		}
	}
		
	// count remaining height0
	for (int x = bar.getHeight() - 1; x > 0; x -= 1)
	{
		if (bar.isSquareThere(0, x))
		{
			remainHeight0 = x;
			break;
		}
	}
	
	// count remaining width1
	for (int x = bar.getWidth() - 1; x > 0; x -= 1)
	{
		if (bar.isSquareThere(x, 1))
		{
			remainWidth1 = x;
			break;
		}
	}
		
	// count remaining height1
	for (int x = bar.getHeight() - 1; x > 0; x -= 1)
	{
		if (bar.isSquareThere(1, x))
		{
			remainHeight1 = x;
			break;
		}
	}
	
	int differences0 = abs(remainWidth0 - remainHeight0);
		
	// win cases

	// only row 0 is left take it
	if (bar.isSquareThere(0, 1) == false && bar.isSquareThere(1, 0) == true)
	{
		return ChocolateSquareLocation(1, 0);
	}
	
	// only column 0 is left take it
	if (bar.isSquareThere(1, 0) == false && bar.isSquareThere(0, 1) == true)
	{
		return ChocolateSquareLocation(0, 1);
	}
	
	// L shape keep making the L has both the same width and height
	if (bar.isSquareThere(1, 1) == false)
	{		
		if (remainHeight0 > remainWidth0)
		{
			return ChocolateSquareLocation(0, remainHeight0 - differences0 + 1);
		}
		
		if (remainHeight0 < remainWidth0)
		{
			return ChocolateSquareLocation(remainWidth0 - differences0 + 1, 0);
		}
	}
	
	// if can make L shape do it
	if (bar.isSquareThere(1, 1) == true && differences0 == 0)
	{
		return ChocolateSquareLocation(1, 1);
	}
	
	// steps shape with 2 columns keeping them 1 size apart
	if (remainHeight0 > remainHeight1 && bar.isSquareThere(2, 0) == false && remainHeight1 > 0 && !(remainHeight1 + 2 > remainHeight0))
	{
		return ChocolateSquareLocation(0, remainHeight1 + 2);
	}
	
	// if can create the steps shape with 2 columns 1 size apart do it
	if (bar.isSquareThere(2, 0) == true && remainHeight0 - remainHeight1 == 1 && remainHeight1 > 0)
	{
		return ChocolateSquareLocation(2, 0);
	}
	
	// steps shape with 2 rows keeping them 1 size apart
	if (remainWidth0 > remainWidth1 && bar.isSquareThere(0, 2) == false && remainWidth1 > 0 && !(remainWidth1 + 2 > remainWidth0))
	{
		return ChocolateSquareLocation(remainWidth1 + 2, 0); 
	}
	
	// if can create the steps shape with 2 rows 1 size apart do it
	if (bar.isSquareThere(0, 2) == true && remainWidth0 - remainWidth1 == 1 && remainWidth1 > 0)
	{
		return ChocolateSquareLocation(0, 2);
	}
	
	// start the game by taking 2,2 if it's still there
	if (bar.isSquareThere(3, 3) == true)
    {
		return ChocolateSquareLocation(3, 3);
    }
	
	// if someone else took 2,2 then take 1, remain height in column 1 - 1 so to not lose
	if (bar.isSquareThere(2, 2) == false && remainHeight0 == remainHeight1 && remainHeight1 > 1
										 && remainWidth0 == remainWidth1 && remainWidth1 > 1)
    {
		return ChocolateSquareLocation(1, remainHeight1 - 1);
    }
	
	// The minimaxagent
	ChocolateBar newBar;
    ChocolateSquareLocation bestBite, bite;
    int bestOutcome, outcome, biteColumn, biteRow;
	
	bestOutcome = INT_MIN;
	
	for (biteColumn = bar.getWidth() - 1; biteColumn >= 0; biteColumn -= 1)
	{
		for (biteRow = bar.getHeight() - 1; biteRow >= 0; biteRow -= 1)
		{
			bite = ChocolateSquareLocation(biteColumn, biteRow);
			if (bar.isSquareThere(bite))
			{
				newBar = bar;
				newBar.takeBite(bite);
				outcome = barValue(newBar, 1);
				if (outcome > bestOutcome)
				{
					bestBite = bite;
					bestOutcome = outcome;
				}
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

	//itr1
	I was not able to finish the agent using minimax alg, so i'm just throwing a bunch of position together that would lead to my win while defending against myself.
	This agent also make use of the fact that a lot of other people probably do not have an optimal agent and using part of the agent like nibbletop and biteright and counter it

	//itr2
	upgraded from itr1, using the minimaxagent from the pseudocode that is provided
	this beat the itr1 agent 100% of the time while running extremely fast 
	however, it is expected to struggle against real minimax agent that is using alpha-beta pruning
	as it is not a true minimax due to it disregarding everything with a depth of 6 or greater for faster running time.
*/

