/*-----------------------------------------------------------------------
		Copyright (c) Alan Lenton & Interactive Broadcasting 2003-7
	All Rights Reserved. No part of this software may be reproduced,
	transmitted, transcribed, stored in a retrieval system, or translated
	into any human or computer language, in any form or by any means,
	electronic, mechanical, magnetic, optical, manual or otherwise,
	without the express written permission of the copyright holder.
-----------------------------------------------------------------------*/

#include "galactic_parser.h"

#include <fstream>
#include <iostream>

#include <cstring>

#include "galaxy.h"
#include "misc.h"
#include "star.h"

const char	*GalacticParser::el_names[] = { "galaxy", "star", "map", ""	};

GalacticParser::GalacticParser(Galaxy *our_galaxy)
{
	galaxy = our_galaxy;
	directory[0] = '\0';
}

GalacticParser::~GalacticParser()
{
	// don't mess with inline virtuals...
}


void	GalacticParser::EndElement(const char *element)
{
	int	which;
	for(which = 0;el_names[which][0] != '\0';which++)
	{
		if(std::strcmp(el_names[which],element) == 0)
			break;
	}
	switch(which)
	{
		case 1:	galaxy->EndStar();	break;
	}
}

void	GalacticParser::MapStart(const char **attrib)
{
	const std::string	*name_str = FindAttrib(attrib,"name");
	if(name_str == 0)
		return;

	char	buffer[Star::NAME_SIZE];
	std::strncpy(buffer,name_str->c_str(),Star::NAME_SIZE);
	buffer[Star::NAME_SIZE -1] = '\0';
	galaxy->AddMap(buffer);
}

void	GalacticParser::StarStart(const char **attrib)
{
	const std::string	*name_str = FindAttrib(attrib,"name");
	if(name_str == 0)
		return;	

	std::string	name = *name_str;
	const std::string	*dir_str  = FindAttrib(attrib,"directory");
	if(dir_str == 0)
		return;
	std::string dir = *dir_str;
	std::strncpy(directory,dir.c_str(),MAXNAMLEN);
	directory[MAXNAMLEN -1] = '\0';

	Star	*star = new Star(name,dir);
	galaxy->AddStar(star);
	star->Load();
}

void	GalacticParser::StartElement(const char *element,const char **attrib)
{
	int	which;
	for(which = 0;el_names[which][0] != '\0';which++)
	{
		if(std::strcmp(el_names[which],element) == 0)
			break;
	}
	
	switch(which)
	{
		case 1:	StarStart(attrib);	break;
		case 2:	MapStart(attrib);		break;
	}
}


