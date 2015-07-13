/*-----------------------------------------------------------------------
		Copyright (c) Alan Lenton 1985-2015
	All Rights Reserved. No part of this software may be reproduced,
	transmitted, transcribed, stored in a retrieval system, or translated
	into any human or computer language, in any form or by any means,
	electronic, mechanical, magnetic, optical, manual or otherwise,
	without the express written permission of the copyright holder.
-----------------------------------------------------------------------*/

#include "build_terraform.h"

#include <sstream>

#include "fedmap.h"
#include "infra.h"
#include "misc.h"
#include "output_filter.h"
#include "player.h"
#include "population.h"
#include "tokens.h"
#include "xml_parser.h"

const std::string	Terraform::success =
"A new area is cleared and terraformed, providing space for housing and feeding an increased population.\n";

Terraform::Terraform(FedMap *the_map,const std::string& the_name,const char **attribs)
{
	fed_map = the_map;
	name = the_name;
	total_builds = XMLParser::FindNumAttrib(attribs,"points",0);
	ok_status = true;
}

Terraform::Terraform(FedMap *the_map,Player *player,Tokens *tokens)
{
	static const std::string	too_soon("Advanced terraforming can only \
take place on biological level and above planets.\n");

	fed_map = the_map;
	name = tokens->Get(1);
	name[0] = std::toupper(name[0]);

	if(the_map->Economy() < Infrastructure::BIOLOGICAL)
	{
		 player->Send(too_soon,OutputFilter::DEFAULT);
		 ok_status = false;
	}
	else
	{
		total_builds = 1;
		fed_map->AddTotalLabour(10);
		fed_map->AddLabour(10);
		player->Send(success,OutputFilter::DEFAULT);
		ok_status = true;
	}
}

Terraform::~Terraform()
{

}

bool	Terraform::Add(Player *player,Tokens *tokens)
{
	if(total_builds++ < 20)
	{
		fed_map->AddTotalLabour(10);
		fed_map->AddLabour(10);
		player->Send(success,OutputFilter::DEFAULT);
		return(true);
	}
	else
	{
		std::ostringstream	buffer;
		buffer <<"In a surprisingly short space of time more virgin territory is terraformed. ";
		buffer << "Unfortunately, it becomes clear that you have reached diminishing returns on ";
		buffer << fed_map->Title() << "\n";
		player->Send(buffer);
		return(true);
	}
}

void	Terraform::Display(Player *player)
{
	std::ostringstream	buffer;
	buffer << "    Terraforming: " << total_builds << " clearances\n";
	player->Send(buffer);
}

void	Terraform::UpdatePopulation(Population *population)
{
	population->TotalTerraformPoints((total_builds < 20) ? total_builds : 20);
}

void	Terraform::Write(std::ofstream& file)
{
	file << "  <build type='Terraform' points='" << total_builds<< "'/>\n";
}

void	Terraform::XMLDisplay(Player *player)
{
	std::ostringstream	buffer;
	buffer << "<s-build-planet-info info='Terraforming: " << total_builds << "'/>\n";
	player->Send(buffer);
}

