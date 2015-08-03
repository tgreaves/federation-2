/*-----------------------------------------------------------------------
		Copyright (c) Alan Lenton 1985-2015
	All Rights Reserved. No part of this software may be reproduced,
	transmitted, transcribed, stored in a retrieval system, or translated
	into any human or computer language, in any form or by any means,
	electronic, mechanical, magnetic, optical, manual or otherwise,
	without the express written permission of the copyright holder.
-----------------------------------------------------------------------*/

#include "build_oil.h"

#include "commodities.h"
#include	"disaffection.h"
#include "fedmap.h"
#include "infra.h"
#include "misc.h"
#include "output_filter.h"
#include "player.h"
#include "tokens.h"
#include "xml_parser.h"

Oil::Oil(FedMap *the_map,const std::string& the_name,const char **attribs)
{
	fed_map = the_map;
	name = the_name;
	total_builds = XMLParser::FindNumAttrib(attribs,"points",0);
	ok_status = true;
}

Oil::Oil(FedMap *the_map,Player *player,Tokens *tokens)
{
	static const std::string	not_allowed("Oil wells can only be drilled at industrial and tech levels.\n");
	static const std::string	ok("Crowds turn out to watch the first gusher on the planet. The oil-droids \
soon have it under control and crude oil is pumped through to the refineries.\n");

	int	economy = the_map->Economy();
	if((economy < Infrastructure::INDUSTRIAL) || (economy > Infrastructure::TECHNICAL))
	{
		player->Send(not_allowed,OutputFilter::DEFAULT);
		ok_status = false;
	}
	else
	{
		if(!CheckCommodity(player,tokens))
			ok_status =false;
		else
		{
			fed_map = the_map;
			name = tokens->Get(1);
			name[0] = std::toupper(name[0]);
			total_builds = 1;

			if(!fed_map->AddProductionPoint(player,tokens->Get(2),2))
				ok_status = false;
			else
			{
				std::ostringstream	buffer;
			 	buffer << "Your drilling/refinery complex comes on-line as planned. Its ";
				buffer << "output enhances the production of " << tokens->Get(2) << ".\n";
				player->Send(buffer);
				ok_status = true;
			}
		}
	}
}



Oil::~Oil()
{

}

bool	Oil::Add(Player *player,Tokens *tokens)
{
	static const std::string	not_allowed("Oil wells can only be drilled at industrial and tech levels.\n");

	int	economy = fed_map->Economy();
	if((economy < Infrastructure::INDUSTRIAL) || (economy > Infrastructure::TECHNICAL))
	{
		player->Send(not_allowed,OutputFilter::DEFAULT);
		return(false);
	}

	std::ostringstream	buffer;
	if(total_builds < 5)
	{
		if(!CheckCommodity(player,tokens))
			return(false);

		if(fed_map->AddProductionPoint(player,tokens->Get(2),2))
		{
		 	buffer << "Your drilling/refinery complex come on-line as planned. Its ";
			buffer << "output enhances the production of " << tokens->Get(2) << ".\n";
			player->Send(buffer);
			total_builds++;
			return(true);
		}
		return(false);
	}

 	buffer << "Your drilling/refinery complex come on-line as planned, but its ";
 	buffer << "start up seems to have little additional effect on production.\n";
	player->Send(buffer);
	total_builds++;
	return(true);
}

bool	Oil::CheckCommodity(Player *player,Tokens *tokens)
{
	static const std::string	unknown("I don't know which commodity production you want to improve!\n");
	static const std::string	no_commod("You haven't said what commodity to add the production points to!\n");

	if(tokens->Size() < 3)
	{
		player->Send(no_commod,OutputFilter::DEFAULT);
		return(false);
	}

	if(Game::commodities->Find(tokens->Get(2)) == 0)
	{
		player->Send(unknown,OutputFilter::DEFAULT);
		return(false);
	}

	std::ostringstream	buffer;
	if(!Game::commodities->IsType(tokens->Get(2),Commodities::IND))
	{
		buffer << "You cannot allocate production to " << tokens->Get(2) << ", only to industrial commodities.\n";
		player->Send(buffer);
		return(false);
	}
	return(true);
}

bool	Oil::Demolish(Player *player)
{
	if(fed_map->Economy() > Infrastructure::TECHNICAL)
	{
		--total_builds;
		return(true);
	}
	else
	{
		player->Send("Unfortunately, the Society for the Preservation of Ancient \
Artifacts and Relics (SPAAR) manages to persuade the Galactic Administration to \
issue a preservation order and your plans are frustrated...\n",OutputFilter::DEFAULT);
		return(false);
	}
}


void	Oil::Display(Player *player)
{
	std::ostringstream	buffer;
	buffer << "    " << name << ": " << total_builds << " refiner";
	buffer << ((total_builds == 1) ? "y" : "ies") << " built\n";
	player->Send(buffer);
}

bool	Oil::RequestResources(Player *player,const std::string& recipient,int quantity)
{
	if(recipient == "Insulation")
		return(true);
	else
		return(false);
}

void	Oil::UpdateDisaffection(Disaffection *discontent)
{
	int	economy = fed_map->Economy();
	if(economy < Infrastructure::LEISURE)
		discontent->TotalOilPoints(total_builds * 2);
}

void	Oil::Write(std::ofstream& file)
{
	file << "  <build type='Oil' points='" << total_builds << "'/>\n";
}

void	Oil::XMLDisplay(Player *player)
{
	std::ostringstream	buffer;
	buffer << name << ": " << total_builds << " refiner";
	buffer << ((total_builds == 1) ? "y" : "ies") << " built'/>\n";
	AttribList attribs;
	std::pair<std::string,std::string> attrib(std::make_pair("info",buffer.str()));
	attribs.push_back(attrib);
	player->Send("",OutputFilter::BUILD_PLANET_INFO,attribs);
}



