/*-----------------------------------------------------------------------
		Copyright (c) Alan Lenton & Interactive Broadcasting 2003-7
	All Rights Reserved. No part of this software may be reproduced,
	transmitted, transcribed, stored in a retrieval system, or translated
	into any human or computer language, in any form or by any means,
	electronic, mechanical, magnetic, optical, manual or otherwise,
	without the express written permission of the copyright holder.
-----------------------------------------------------------------------*/

#include "equip_parser.h"

#include <cstring>

#include "equipment.h"
#include "ship.h"

const char	*EquipParser::el_names[] =
	{ "equipment", "computer", "hull", "weapon", ""	};

EquipParser::~EquipParser()
{
}


void	EquipParser::ComputerStart(const char **attrib)
{
	Computer	*computer = new Computer;
	computer->level = FindNumAttrib(attrib,"level",-1);
	if((computer->weight = FindNumAttrib(attrib,"weight",-1)) < 0)
		computer->level = -1;
	if((computer->power = FindNumAttrib(attrib,"power",-1)) < 0)
		computer->level = -1;
	if((computer->capacity = FindNumAttrib(attrib,"capacity",-1)) < 0)
		computer->level = -1;
	if((computer->cost = FindNumAttrib(attrib,"cost",-1)) < 0)
		computer->level = -1;
	else
		computer->cost *= 1000;
	if((computer->repair = FindNumAttrib(attrib,"repair",-1)) < 0)
		computer->level = -1;
	else
		computer->repair *= 1000;

	if((computer->level < 0) || (computer->level > Ship::MAX_COMP))
		delete computer;
	else
		Ship::comp_types[computer->level] = computer;
}

void	EquipParser::HullStart(const char **attrib)
{
	Hull	*hull = new Hull;
	const std::string	*val = FindAttrib(attrib,"name");
	if(val == 0)
		hull->capacity = -1;
	else
		hull->name = *val;
	
	hull->capacity = FindNumAttrib(attrib,"capacity",-1);
	if((hull->cost = FindNumAttrib(attrib,"cost",-1)) < 0)
		hull->capacity = -1;
	else
		hull->cost *= 1000;
	if((hull->mount_pts = FindNumAttrib(attrib,"mount-pts",-1)) < 0)
		hull->capacity = -1;

	if((hull->fuel = FindNumAttrib(attrib,"fuel",-1)) < 0)
		hull->capacity = -1;

	switch(hull->capacity)
	{
		case  200:	Ship::hull_types[0] = hull;	break;
		case  400:	Ship::hull_types[1] = hull;	break;
		case  600:	Ship::hull_types[2] = hull;	break;
		case  800:	Ship::hull_types[3] = hull;	break;
		case 1000:	Ship::hull_types[4] = hull;	break;
		case 1400:	Ship::hull_types[5] = hull;	break;
		default  :	delete hull;						break;
	}
}

void	EquipParser::StartElement(const char *element,const char **attrib)
{
	int	equip;
	for(equip = 0;el_names[equip][0] != '\0';equip++)
	{
		if(std::strcmp(el_names[equip],element) == 0)
			break;
	}

	switch(equip)
	{
		case 0:	break;							// 'equipment'
		case 1:	ComputerStart(attrib);		// 'computer'
		case 2:	HullStart(attrib);			// 'hull'
		case 3:	WeaponStart(attrib);			// 'weapon'
	}
}

void	EquipParser::WeaponStart(const char **attrib)
{
	Weapon	*weapon = new Weapon;
	const std::string	*val = FindAttrib(attrib,"name");
	if(val == 0)
		weapon->damage = -1;
	else
		weapon->name = *val;

	if((weapon->weight = FindNumAttrib(attrib,"weight",-1)) < 0)
		weapon->damage = -1;
	if((weapon->cost = FindNumAttrib(attrib,"cost",-1)) < 0)
		weapon->damage = -1;
	else
		weapon->cost *= 1000;
	if((weapon->repair = FindNumAttrib(attrib,"repair",-1)) < 0)
		weapon->damage = -1;
	else
		weapon->repair *= 1000;
	if((weapon->power = FindNumAttrib(attrib,"power",-1)) < 0)
		weapon->damage = -1;
	weapon->damage = FindNumAttrib(attrib,"damage",-1);

	switch(weapon->damage)
	{
		case  0:	Ship::weapon_types[0] = weapon;	break;
		case  5:	Ship::weapon_types[1] = weapon;	break;
		case  8:	Ship::weapon_types[2] = weapon;	break;
		case 12:	Ship::weapon_types[3] = weapon;	break;
		default:	delete weapon;
	}
}

