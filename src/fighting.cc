/*-----------------------------------------------------------------------
		Copyright (c) Alan Lenton 1985-2015
	All Rights Reserved. No part of this software may be reproduced,
	transmitted, transcribed, stored in a retrieval system, or translated
	into any human or computer language, in any form or by any means,
	electronic, mechanical, magnetic, optical, manual or otherwise,
	without the express written permission of the copyright holder.
-----------------------------------------------------------------------*/

#include "fighting.h"

#include "player.h"


Fighting::~Fighting()
{
	for(FightList::iterator iter = fight_list.begin();iter != fight_list.end();++iter)
		delete *iter;
	fight_list.clear();
}


bool	Fighting::Attack(Player *attacker,Player *defender)
{
	if((attacker == 0) || (defender == 0))
		return false;

	if((attacker->TempFlagIsSet(Player::FIGHTING) || defender->TempFlagIsSet(Player::FIGHTING)))
		return false;	// One of the players is already in a fight

	FightRec	*rec = new FightRec;

	rec->adverseries.first = attacker;
	rec->adverseries.second = defender;
	rec->status = STANDOFF;
	rec->last_tick = std::time(0);
	rec->command = CMD_NONE;
	rec->moving = 0;
	rec->mvt_rounds_left = 0;

	fight_list.push_back(rec);

	return true;
}
