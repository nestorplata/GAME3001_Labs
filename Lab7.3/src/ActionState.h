#pragma once
#ifndef __ACTION_STATE__
#define __ACTION_STATE__
enum ActionState
{
	NO_ACTION = -1,
	ATTACK,
	MOVE_TO_LOS,
	MOVE_TO_PLAYER,
	MOVE_TO_RANGE,
	MOVE_TO_COVER,
	WAIT_BEHIND_COVER,
	PATROL,
	FLEE
};
#endif /* defined (__ACTION_STATE__)*/