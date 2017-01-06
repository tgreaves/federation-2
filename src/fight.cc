/*-----------------------------------------------------------------------
		Copyright (c) Alan Lenton 1985-2017
	All Rights Reserved. No part of this software may be reproduced,
	transmitted, transcribed, stored in a retrieval system, or translated
	into any human or computer language, in any form or by any means,
	electronic, mechanical, magnetic, optical, manual or otherwise,
	without the express written permission of the copyright holder.
-----------------------------------------------------------------------*/

#include "fight.h"

#include <cstdlib>

#include "output_filter.h"
#include "player.h"

Fight::Fight(const LocRec& loc, Player *att, Player *def)
{
	loc_rec.star_name = loc.star_name;
	loc_rec.map_name = loc.map_name;
	loc_rec.loc_no = loc.loc_no;
	loc_rec.fed_map = loc.fed_map;

	aggressor = att;
	victim = def;
	aggressor_name = att->Name();
	victim_name = def->Name();

	spacing = MISSILE_DIST;
	aggressor_telemetry = 0;
	victim_telemetry = 0;
}


void	Fight::CalculateDamage()
{
	ClearFightInfoOut(defender_out);
	int damage = (std::rand() % 100);

	defender_out.has_damage = true;
	defender_out.hull_damage = damage/10;

	if(damage < 5)
	{
		defender_out.computer_damage = 1;
		if((std::rand() %2) == 1)
			return;
	}
	if(damage < 10)
	{
		defender_out.sensor_damage = 1;
		if((std::rand() %2) == 1)
			return;
	}
	if(damage < 15)
	{
		defender_out.jammer_damage = 1;
		if((std::rand() %2) == 1)
			return;
	}
	if(damage < 20)
	{
		defender_out.laser_damage = damage;
		if((std::rand() %2) == 1)
			return;
	}
	if(damage < 30)
	{
		defender_out.twin_laser_damage = damage;
		if((std::rand() %2) == 1)
			return;
	}
	if(damage < 50)
	{
		defender_out.quad_laser_damage = damage/2 + 5;
		if((std::rand() %2) == 1)
			return;
	}
	if(damage < 60)
	{
		defender_out.missile_rack_damage = damage/5 + 3;
		if((std::rand() %2) == 1)
			return;
	}
	if(damage < 70)
	{
		defender_out.shield_damage = 2;
		if((std::rand() %2) == 1)
			return;
	}
	defender_out.engine_damage = damage;
}

void	Fight::ClearFightInfoIn(FightInfoIn& info)
{
	info.engines = 0;
	info.sensors = 0;
	info.jammers = 0;
	info.lasers = 0;
	info.twin_lasers = 0;
	info.quad_lasers = 0;
	info.defence_laser = false;
}

void	Fight::ClearFightInfoOut(FightInfoOut& info)
{
	info.has_damage = false;
	info.computer_damage = 0;
	info.sensor_damage = 0;
	info.jammer_damage = 0;
	info.missile_rack_damage = 0;
	info.laser_damage = 0;
	info.twin_laser_damage = 0;
	info.quad_laser_damage = 0;
	info.shield_damage = 0;
	info.hull_damage = 0;
	info.engine_damage = 0;
}

void Fight::CloseRange(Player *player)
{
	Player	*attacker = player;
	Player	*defender = 0;
	if(attacker == aggressor)
		defender = victim;
	else
		defender = aggressor;

	switch(spacing)
	{
		case LASER_DIST:
			player->Send("You are already in laser range!\n",OutputFilter::DEFAULT);
			return;

		case INTERMED_DIST_1:
			attacker->Send("You move into laser range.\n",OutputFilter::DEFAULT);
			defender->Send("Your opponent has moved into laser range!\n",OutputFilter::DEFAULT);
			spacing = LASER_DIST;
			return;

		case INTERMED_DIST_2:
			attacker->Send("You move closer to your opponent...\n",OutputFilter::DEFAULT);
			defender->Send("Your opponent has moved closer!\n",OutputFilter::DEFAULT);
			spacing = INTERMED_DIST_1;
			return;

		case MISSILE_DIST:
			attacker->Send("You start to move closer to your opponent...\n",OutputFilter::DEFAULT);
			defender->Send("Your opponent has started to move closer!\n",OutputFilter::DEFAULT);
			spacing = INTERMED_DIST_2;
			return;
	}
}

Player *Fight::GetOtherPlayer(Player *player)
{
	if (aggressor == player)
		return victim;
	else
		return aggressor;
}

// NOTE: Launch() uses integer arithmetic. DO NOT mess with the brackets
bool Fight::Launch(Player *att)
{
	att->Send("Launching missile...\n",OutputFilter::DEFAULT);
	// Figure out who is the attacker and who is the defender for this round
	Player	*attacker = att;
	Player	*defender = 0;
	int		telemetry;
	if(att == aggressor)
	{
		defender = victim;
		telemetry =  aggressor_telemetry;
		if(Game::player_index->FindCurrent(victim_name) == 0)
		{
			attacker->Send("Your opponent is no longer in the game!\n",OutputFilter::DEFAULT);
			return false;
		}
	}
	else
	{
		defender = aggressor;
		telemetry =  victim_telemetry;
		if(Game::player_index->FindCurrent(aggressor_name) == 0)
		{
			attacker->Send("Your opponent is no longer in the game!\n",OutputFilter::DEFAULT);
			return false;
		}
	}

	if(spacing < INTERMED_DIST_2)	// Are we too close to launch missiles safely?
	{
		attacker->Send("You are too close to use missiles!\n",OutputFilter::DEFAULT);
		return false;
	}

	// OK - it's safe to fire a missile
	ClearFightInfoIn(attacker_in);
	attacker->GetFightInfoIn(attacker_in);
	ClearFightInfoIn(defender_in);
	defender->GetFightInfoIn(defender_in);

	defender->Send("Incoming missile detected!\n",OutputFilter::DEFAULT);
	int	to_hit = (MISSILE_BASE_HIT * attacker_in.missile_rack)/100;
	to_hit += (attacker_in.sensors - defender_in.jammers) * 3;
	to_hit += telemetry;

	int hit_rand = (std::rand() % 100) + 1;
	if(to_hit >= hit_rand)
	{
		// Missile is on target, but defender has a defence laser
		if(defender_in.defence_laser)
		{
			if((std::rand() % 100 + 1) <= (DEFENCE_LASER_HIT * defender_in.lasers)/100)
			{
				attacker->Send("Missile destroyed by defence laser!\n",OutputFilter::DEFAULT);
				defender->Send("Incoming missile destroyed!\n",OutputFilter::DEFAULT);
				return true;
			}
		}

		CalculateDamage();
		attacker->Send("Your missile explodes on target!\n",OutputFilter::DEFAULT);
		defender->Send("Missile hit - checking for damage.\n",OutputFilter::DEFAULT);
		defender->MissileHit(defender_out);
	}
	else
	{
		// Missed! But we might be able to provide some useful info for next time...
		if(attacker == aggressor)
		{
			if(aggressor_telemetry < 5)
				++aggressor_telemetry;
		}
		else
		{
			if(victim_telemetry < 5)
				++victim_telemetry;
		}
		defender->Send("Missile lost lock and missed...\n",OutputFilter::DEFAULT);
		attacker->Send("Your missile lost its target and missed...\n",OutputFilter::DEFAULT);
		return true;
	}
	return true;
}

void Fight::OpenRange(Player *player)
{
	Player	*attacker = player;
	Player	*defender = 0;
	if(attacker == aggressor)
		defender = victim;
	else
		defender = aggressor;

	switch(spacing)
	{
		case LASER_DIST:
			attacker->Send("You start to move away from your opponent...\n",OutputFilter::DEFAULT);
			defender->Send("Your opponent has started to move away!\n",OutputFilter::DEFAULT);
			spacing = INTERMED_DIST_1;
			return;

		case INTERMED_DIST_1:
			attacker->Send("You move further away from your opponent...\n",OutputFilter::DEFAULT);
			defender->Send("Your opponent has moved further away!\n",OutputFilter::DEFAULT);
			spacing = INTERMED_DIST_2;
			return;

		case INTERMED_DIST_2:
			attacker->Send("You move into missile range.\n",OutputFilter::DEFAULT);
			defender->Send("Your opponent has moved into missile range!\n",OutputFilter::DEFAULT);
			spacing = MISSILE_DIST;
			return;

		case MISSILE_DIST:
			player->Send("You are already at missile range!\n",OutputFilter::DEFAULT);
			return;
	}
}

bool Fight::Participant(Player *att, Player *def)
{
	if((att == aggressor) || (att == victim))
		return true;
	if((def == aggressor) || (def == victim))
		return true;
	return false;
}


/* ---------------------- Work in progress ---------------------- */
