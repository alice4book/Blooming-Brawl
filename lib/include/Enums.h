#pragma once

enum EPlayerID 
{
	None,
	Player1,
	Player2,
	RobotDestroyer
};

enum EState
{
	Empty,
	Growing,
	Grown,
	Impassable,
	Overgrown,
	Burned
};