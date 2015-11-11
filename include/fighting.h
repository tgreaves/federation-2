/*-----------------------------------------------------------------------
		Copyright (c) Alan Lenton 1985-2015
	All Rights Reserved. No part of this software may be reproduced,
	transmitted, transcribed, stored in a retrieval system, or translated
	into any human or computer language, in any form or by any means,
	electronic, mechanical, magnetic, optical, manual or otherwise,
	without the express written permission of the copyright holder.
-----------------------------------------------------------------------*/

#ifndef FIGHTING_H
#define FIGHTING_H

#include <ctime>
#include <list>
#include <utility>

class Player;
struct FightRec;

// In Adverseries, first is the attecker, second is the defender
typedef std::pair<Player *,Player *>	Adverseries;
typedef std::list<FightRec *>				FightList;

struct FightRec
{
	Adverseries	adverseries;
	int 			status;
	std::time_t	last_tick;
	int 			command;

	Player		*moving;				// player is closing or withrawing
	int 			mvt_rounds_left;
};

class Fighting
{
public:

	enum
	{
		CMD_NONE, LAUNCH, FIRE_SL, FIRE_TL, FIRE_QL, RIPPLE,
		MV_CLOSE, MV_STANDOFF, FLEE, MOVED, QUITTING
	};	// Commands

	enum	{ STANDOFF, CLOSING, CLOSE, WITHDRAWING, FLEEING, HAS_QUIT };	// Status

private:
	static const int NO_DEBRIS = -1;

	FightList	fight_list;
	int 			debris;

public:
	Fighting() : debris(NO_DEBRIS)	{	}
	~Fighting();

	bool	Attack(Player *attacker,Player *defender);

};

#endif
