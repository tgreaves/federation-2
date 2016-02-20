/*-----------------------------------------------------------------------
		Copyright (c) Alan Lenton 1985-2016
	All Rights Reserved. No part of this software may be reproduced,
	transmitted, transcribed, stored in a retrieval system, or translated
	into any human or computer language, in any form or by any means,
	electronic, mechanical, magnetic, optical, manual or otherwise,
	without the express written permission of the copyright holder.
-----------------------------------------------------------------------*/

#include "notices.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include <cstdlib>
#include <ctime>

#include "fedmap.h"
#include "misc.h"
#include "output_filter.h"
#include "player.h"


NoticeBoard::~NoticeBoard()
{
	std::ostringstream	buffer("");
	buffer << HomeDir() << "/log/notice_board.log";
	std::string	file_name(buffer.str());
	std::ofstream	notices_file(file_name.c_str());

	std::time_t	now;
	std::tm		*gm_time;
	std::time(&now);
	gm_time = std::gmtime(&now);
	notices_file << std::asctime(gm_time);
	notices_file << "\nNotice Board:" << std::endl;
	if(notices.size() == 0)
		notices_file << "  none" << std::endl;
	else
	{
		for(Notices::iterator iter = notices.begin();iter !=  notices.end();iter++)
			notices_file << *iter;
	}
	notices_file.close();
}


void	NoticeBoard::Post(Player *player,std::string& text)
{
	std::ostringstream	buffer("");
	buffer << Stardate() << " - " << player->Name() << ": " << text << std::endl;
	notices.push_back(buffer.str());
	player->Send(Game::system->GetMessage("notices","post",1),OutputFilter::DEFAULT);
}

void	NoticeBoard::Read(Player *player,unsigned how_many)
{
	static const std::string	none("There aren't any messages on the board, yet.\n");
	
	if(notices.size() == 0)
	{
		player->Send(none,OutputFilter::DEFAULT);
		return;
	}
	
	player->Send(Game::system->GetMessage("notices","read",1),OutputFilter::DEFAULT);
	if((how_many == 0) ||  (notices.size() <= how_many))
	{
		for(Notices::iterator iter = notices.begin();iter != notices.end();iter++)
			player->Send(*iter,OutputFilter::DEFAULT);
	}
	else
	{
		int index = notices.size() - how_many;
		Notices::iterator iter;
		int count = 0;
		for(iter = notices.begin();(iter != notices.end());iter++,count++)
		{
			if(count >= index)
				player->Send(*iter,OutputFilter::DEFAULT);
		}
	}
}

void	NoticeBoard::UnPost(Player *player,const std::string& name)
{
	std::string	text("");
	bool	man_remove = (name != "");
	if(man_remove)
		text = " - " + name + ": ";
	else
		text = " - " + player->Name() + ": ";

	int total = 0;
	for(Notices::iterator iter = notices.begin();iter !=  notices.end();)
	{
		if((*iter).find(text) != std::string::npos)
		{
			if(man_remove)
			{
				std::string item(*iter);
				item[item.length() - 1] = ' ';
				WriteLog(item);
				WriteNavLog(item);
			}
			notices.erase(iter++);
			total++;
		}
		else
			iter++;
	}

	std::ostringstream	buffer("");
	if(total == 1)
		buffer << total << " message located and removed.\n";
	else
		buffer << total << " messages located and removed.\n";
	player->Send(buffer,OutputFilter::DEFAULT);

	if(man_remove)
	{
		buffer.str("");
		buffer << player->Name() << " has removed " << total << " post(s) from " << name;

		WriteLog(buffer);
		WriteNavLog(buffer.str());
	}
}


