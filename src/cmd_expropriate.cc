/*-----------------------------------------------------------------------
		Copyright (c) Alan Lenton & Interactive Broadcasting 2003-2011
	All Rights Reserved. No part of this software may be reproduced,
	transmitted, transcribed, stored in a retrieval system, or translated
	into any human or computer language, in any form or by any means,
	electronic, mechanical, magnetic, optical, manual or otherwise,
	without the express written permission of the copyright holder.
-----------------------------------------------------------------------*/

#include "cmd_expropriate.h"

#include <string>

#include <cstdlib>

#include "commodities.h"
#include "company.h"
#include "comp_register.h"
#include "depot.h"
#include "factory.h"
#include "fedmap.h"
#include "misc.h"
#include "player.h"
#include "review.h"
#include "tokens.h"

const int	ExpParser::NO_NOUN = 9999;

const std::string	ExpParser::vocab[] =	{ "factory", "depot", ""	};

void	ExpParser::ExpropriateDepot(Player *player,Tokens *tokens,const std::string& line)
{
	static const std::string	not_owner("You don't own this planet!\n");
	static const std::string	no_name("You haven't given a company name!\n");
	static const std::string	no_depot("I can't find the depot you specified.\n");
	static const std::string	no_company("I can't find the company you specified.\n");
	static const std::string	no_owner("I can't find the company owner.\n");
	static const std::string	is_owner("You can't expropriate your own depots!.\n");

	if(!player->CurrentMap()->IsOwner(player))	{ player->Send(not_owner);		return;	}	
	if(tokens->Size() < 3)								{ player->Send(no_name);		return;	}

	std::string	co_name(tokens->GetRestOfLine(line,2,Tokens::COMPANY));
	Depot	*depot = player->CurrentMap()->FindDepot(co_name);
	if(depot == 0)											{ player->Send(no_depot);	return;	}

	Company	*company = Game::company_register->Find(depot->Owner());
	if(company == 0)										{ player->Send(no_company);	return;	}
	Player	*owner = company->CEO();		
	if(owner == 0)											{ player->Send(no_owner);		return;	}
	if(player == owner)									{ player->Send(is_owner);		return;	}

	FedMap	*cur_map = player->CurrentMap();
	company->DeleteDepot(cur_map);
	std::string	company_name = company->Name();

	std::ostringstream	buffer;
	buffer << "You have expropriated ";
 	if(company_name[company_name.length() - 1] == 's')
		buffer<< company_name << "' depot.\n";
	else
		buffer<< company_name << "'s depot.\n";
	player->Send(buffer);

	buffer.str("");
	buffer << player->Name() << " has expropriated your company's depot on " << cur_map->Title() << ".\n";
	owner->Send(buffer);

	buffer.str("");
 	if(company_name[company_name.length() - 1] == 's')
	 	buffer << company_name << "' ";
	else
 		buffer << company_name << "'s ";
	buffer << "depot on " <<  cur_map->Title() << " has been expropriated by the authorities.\n";
	Game::financial->Post(buffer);
}

void	ExpParser::ExpropriateFactory(Player *player,Tokens *tokens,const std::string& line)
{
	static const std::string	not_owner("You don't own this planet!\n");
	static const std::string	not_number("You haven't given a factory number...\n");
	static const std::string	no_name("You haven't given a company name!\n");
	static const std::string	no_factory("I can't find the factory you specified.\n");
	static const std::string	no_company("I can't find the company you specified.\n");
	static const std::string	no_owner("I can't find the company owner.\n");
	static const std::string	no_commod("I can't find the commodity being produced.\n");
	static const std::string	is_owner("You can't expropriate your own factories!.\n");

	if(!player->CurrentMap()->IsOwner(player))	{ player->Send(not_owner);		return;	}	
	if(!isdigit(tokens->Get(2)[0]))					{ player->Send(not_number);	return;	}
	if(tokens->Size() < 4)								{ player->Send(no_name);		return;	}

	std::string	co_name(tokens->GetRestOfLine(line,3,Tokens::COMPANY));
	int	fac_num = std::atoi(tokens->Get(2).c_str());
	Factory	*factory = player->CurrentMap()->FindFactory(co_name,fac_num);
	if(factory == 0)										{ player->Send(no_factory);	return;	}

	Company	*company = Game::company_register->Find(factory->Owner());
	if(company == 0)										{ player->Send(no_company);	return;	}
	Player	*owner = company->CEO();		
	if(owner == 0)											{ player->Send(no_owner);		return;	}
	if(player == owner)									{ player->Send(is_owner);		return;	}

	company->DeleteFactory(std::atoi(tokens->Get(2).c_str()));

	std::string	company_name = company->Name();
	std::ostringstream	buffer;
	buffer << "You have expropriated ";
 	if(company_name[company_name.length() - 1] == 's')
		buffer<< company_name << "' number ";
	else
		buffer<< company_name << "'s number ";
	buffer << fac_num << " factory.\n";
	player->Send(buffer);

	buffer.str("");
	buffer << player->Name() << " has expropriated your company's number " << fac_num << " factory.\n";
	owner->Send(buffer);

	buffer.str("");
 	if(company_name[company_name.length() - 1] == 's')
	 	buffer << company_name << "' #";
	else
 		buffer << company_name << "'s #";
	buffer << fac_num << " factory has been expropriated by the ";
	buffer << player->CurrentMap()->Title() << " authorities.\n";
	Game::financial->Post(buffer);
}

int	ExpParser::FindNoun(const std::string& noun)
{
	for(int count = 0;vocab[count] != "";count++)
	{
		if(vocab[count] == noun)
			return(count);
	}
	return(NO_NOUN);
}

void	ExpParser::Process(Player *player,Tokens *tokens,const std::string& line)
{
	static const std::string	error("I don't know what you want to expropriate!\n");
	
	switch(FindNoun(tokens->Get(1)))
	{
		case  0:	ExpropriateFactory(player,tokens,line);	break;
		case  1:	ExpropriateDepot(player,tokens,line);		break;
		default:	player->Send(error);								break;
	}
}

