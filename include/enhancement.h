/*-----------------------------------------------------------------------
		Copyright (c) Alan Lenton & Interactive Broadcasting 2003-10
	All Rights Reserved. No part of this software may be reproduced,
	transmitted, transcribed, stored in a retrieval system, or translated
	into any human or computer language, in any form or by any means,
	electronic, mechanical, magnetic, optical, manual or otherwise,
	without the express written permission of the copyright holder.
-----------------------------------------------------------------------*/

#ifndef ENHANCEMENT_H
#define ENHANCEMENT_H

#include <fstream>
#include <list>
#include <string>

class Disaffection;
class	Efficiency;
class	Enhancement;
class FedMap;
class	Player;
class Population;
class	Tokens;

typedef	std::list<Enhancement *>	EnhanceList;

class	Enhancement		// interface for all planet infrastructure builds
{
public:
	enum	// different things that can be built
	{
		HELIOGRAPH,		BASE,			CLINIC,			HOSPITAL,	POLICE,			FAMILY,
		CANAL,			SCHOOL,		AGRI_COLLEGE,	ATMOS,		POLLUTION,		BIODIVERSITY,
		DOLE,				TELEGRAPH,	COAL, 			PENSION,		RAILWAY,			HOUSING,
		RIOT_POLICE,	MINING_SCH,	INSULATION,		SATELLITE,	PORT,				RADIO,
		TECH,				OIL,			ANTIGRAV,		PHONE,		AIRLANE,			FIBRE,
		RESEARCH,		DEFENCE,		WEATHER,			URBAN,		SURVEILLANCE,	FUSION,
		INSURANCE,		FLOATING,	BIOLAB,			TERRAFORM,	GENETIC,			TACHYON,
		TQUARK,			METASTUDIO,	SOLAR,			INFORMER,	LEISURE,			AIRPORT,
		ANTIAGATHICS,
		MAX_BUILD
	};

protected:
	FedMap		*fed_map;			// our home map
	std::string	name;					// proper name of the build
	int			total_builds;
	bool			ok_status;

public:
	Enhancement();
	virtual ~Enhancement();

	int	TotalBuilds()				{ return(total_builds);	}

	virtual const std::string&		Name() = 0;

	virtual bool	Add(Player *player,Tokens *tokens) = 0;
	virtual bool	CanPromote()	{ return(true);		}
	virtual bool	Demolish(Player *player) 							{ --total_builds; return(true);	}
	virtual bool	IsOK()			{ return(ok_status);	}
	virtual bool	RequestResources(Player *player,const std::string& recipient,int quantity = 0)
						{ return(false);	}
	virtual bool	Riot();

	virtual void	LevelUpdate()											{ return;	}
	virtual void	ReleaseAssets(const std::string& which)		{ return;	}
	virtual void	ReleaseAssets(const std::string& who,const std::string& which)		{ return;	}
	virtual void	UpdateDisaffection(Disaffection *discontent)	{ return;	}
	virtual void	UpdateEfficiency(Efficiency *efficiency)		{ return;	}
	virtual void	UpdatePopulation(Population *population)		{ return;	}

	// For derived classes to use if they have a private member
	virtual int	Get()															{ return(0);	}
	virtual int	Set(int value = 0)										{ return(0);	}

	virtual void	Display(Player *player) = 0;
	virtual void	Write(std::ofstream& file) = 0;
	virtual void	XMLDisplay(Player *player);
};

#endif

