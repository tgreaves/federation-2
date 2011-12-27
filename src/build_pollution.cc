/*-----------------------------------------------------------------------
		Copyright (c) Alan Lenton & Interactive Broadcasting 2003-8
	All Rights Reserved. No part of this software may be reproduced,
	transmitted, transcribed, stored in a retrieval system, or translated
	into any human or computer language, in any form or by any means,
	electronic, mechanical, magnetic, optical, manual or otherwise,
	without the express written permission of the copyright holder.
-----------------------------------------------------------------------*/

#include "build_pollution.h"

#include <sstream>

#include "commodities.h"
#include "disaffection.h"
#include "fedmap.h"
#include "infra.h"
#include "misc.h"
#include "player.h"
#include "tokens.h"
#include "xml_parser.h"

const std::string	Pollution::success =
"The completion of a new pollution control unit causes much comment about why it should \
be needed. There are also suggestions that the units should be paid for by factory \
owners, rather than taxpayers...\n";

Pollution::Pollution(FedMap *the_map,const std::string& the_name,const char **attribs)
{
	fed_map = the_map;
	name = the_name;
	level_builds = XMLParser::FindNumAttrib(attribs,"level",0);
	unused_builds = XMLParser::FindNumAttrib(attribs,"unused",0);
	total_builds = level_builds + unused_builds;
	ok_status = true;
}

Pollution::Pollution(FedMap *the_map,Player *player,Tokens *tokens)
{
	fed_map = the_map;
	name = tokens->Get(1);
	name[0] = std::toupper(name[0]);
	
	level_builds = unused_builds = total_builds = 0;
	switch(fed_map->Economy())
	{
		case Infrastructure::RESOURCE:	
		case Infrastructure::INDUSTRIAL:	
		case Infrastructure::TECHNICAL:	level_builds++;	break;
		default:									unused_builds++;	break;
	}	
	total_builds++;

	player->Send(success);
	ok_status = true;
}

Pollution::~Pollution()
{

}

bool	Pollution::Add(Player *player,Tokens *tokens)
{
	switch(fed_map->Economy())
	{
		case Infrastructure::RESOURCE:	(level_builds < 2) ? level_builds++ : unused_builds++;	break;
		case Infrastructure::INDUSTRIAL:	(level_builds < 5) ? level_builds++ : unused_builds++;	break;
		case Infrastructure::TECHNICAL:	(level_builds < 9) ? level_builds++ : unused_builds++;	break;
		default:									unused_builds++;
	}	
	total_builds ++;

	player->Send(success);
	return(true);
}

bool	Pollution::Demolish(Player *player)
{
	if(unused_builds > 0)
		--unused_builds;
	else
		--level_builds;
	--total_builds;
	return(true);
}

void	Pollution::Display(Player *player)
{
	std::ostringstream	buffer;
	buffer << "    Pollution Control Units: " << total_builds << " built\n";
	buffer << "      General: " << level_builds << "\n";
	buffer << "      Unallocated: " << unused_builds << "\n";
	player->Send(buffer);
}

void	Pollution::LevelUpdate()
{
	int	level = 0;
	switch(fed_map->Economy())
	{
		case Infrastructure::RESOURCE:	level = 2;	break;
		case Infrastructure::INDUSTRIAL:	level = 5;	break;
		case Infrastructure::TECHNICAL:	level = 9;	break;
		default:	return;
	}
	level_builds += unused_builds;
	unused_builds = 0;
	if(level_builds > level)
	{
		unused_builds = level_builds - level;
		level_builds = level;
	}
}

bool	Pollution::RequestResources(Player *player,const std::string& recipient,int quantity)
{
	if(recipient == "Floating")
	{
		if(total_builds >= quantity)
			return(true);
	}
	return(false);
}	

bool	Pollution::Riot()
{
	if(unused_builds > 0)
		unused_builds--;
	else
	{
		if(level_builds > 0)
			level_builds--;
	}

	if(--total_builds <= 0)
		return(true);
	else
		return(false);
}
	
void	Pollution::UpdateDisaffection(Disaffection *discontent)
{
	discontent->TotalPollutionPoints(total_builds);
}

void	Pollution::Write(std::ofstream& file)
{
	file << "  <build type='Pollution' level='" << level_builds;
	file << "' unused='" << unused_builds << "'/>\n";
}

void	Pollution::XMLDisplay(Player *player)
{
	std::ostringstream	buffer;
	buffer << "<s-build-planet-info info='Pollution Control Units: " << total_builds << "'/>\n";
	buffer << "<s-build-planet-info info='  General: " << level_builds << "'/>\n";
	buffer << "<s-build-planet-info info='  Unallocated: " << unused_builds << "'/>\n";
	player->Send(buffer);
}







