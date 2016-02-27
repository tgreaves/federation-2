/*-----------------------------------------------------------------------
		Copyright (c) Alan Lenton 1985-2016
	All Rights Reserved. No part of this software may be reproduced,
	transmitted, transcribed, stored in a retrieval system, or translated
	into any human or computer language, in any form or by any means,
	electronic, mechanical, magnetic, optical, manual or otherwise,
	without the express written permission of the copyright holder.
-----------------------------------------------------------------------*/

#ifndef LOCATION_H
#define LOCATION_H

#include <bitset>
#include <fstream>
#include <map>
#include <string>

class DisplayCabinet;
class	EventNumber;
class FedMap;
class	LocRec;
class MsgNumber;
class	Player;

typedef	std::map<const std::string,EventNumber *,std::less<std::string> >	  LocVocab;

class	Location
{
public:
	enum	{ NORTH, NE, EAST, SE, SOUTH, SW, WEST, NW, UP, DOWN, INTO, OUTOF, MAX_EXITS	};
	enum	{ ENTER, NOEXIT, INROOM, SEARCH, MAX_EVENTS	};
	enum
	{
		SPACE, LINK, EXCHANGE, SHIPYARD, REPAIR, HOSPITAL, INSURE,
		PEACE, BAR, COURIER, UNLIT, CUSTOM, TELEPORT, PICKUP, MAX_FLAGS
	};
	enum	{ GLANCE, FULL_DESC, DEFAULT };		// used with look/glance
	enum	{ ADD_DESC, REPLACE_DESC };			// used with AddDesc

	static const int	MAX_NAME = 64;
	static const int	MAX_DESC = 1024;
	static const int	MAX_MSSG = 40;
	static const int	LINE_SIZE = MAX_DESC + 80;
	static const int	INVALID_LOC = -1;
	static const int	NO_EXIT = -1;

private:
	int 			loc_no;
	std::string	name;
	std::string	desc;
	int			exits[MAX_EXITS];
	MsgNumber	*no_exit;
	EventNumber	*events[MAX_EVENTS];
	std::bitset<MAX_FLAGS>	flags;
	LocVocab		vocab;								// vocabulary specific to this location
	DisplayCabinet	*cabinet;						// display cabinet for landing pad

	void	XMLFlags(std::ostringstream& buffer);

public:
	Location();
	Location(int loc_num);
	~Location();

	EventNumber	*GetEvent(int which)										{ return(events[which]);	}
	LocRec	*Move(Player *player,FedMap *home_map,int dir);

	const std::string&	AsciiOnly(const std::string& text);

	int	Number()																{ return(loc_no);				}
	int	MobileExit(int dir)												{ if(dir > NW) return(-1); else return(exits[dir]);	}

	bool	CanTeleport()														{ return(!flags.test(TELEPORT));		}
	bool	FlagIsSet(int which_flag)										{ return(flags.test(which_flag));	}
	bool	IsABar()																{ return(flags.test(BAR));				}
	bool	IsABroker()															{ return(flags.test(INSURE));			}
	bool	IsACourier()														{ return(flags.test(COURIER));		}
	bool	IsAnExchange()														{ return(flags.test(EXCHANGE));		}
	bool	IsAHospital()														{ return(flags.test(HOSPITAL));		}
	bool	IsALink()															{ return(flags.test(LINK));			}
	bool 	IsAPeaceLoc()														{ return(flags.test(PEACE));			}
	bool	IsAPickup()															{ return(flags.test(PICKUP));			}
	bool	IsARepairShop()													{ return(flags.test(REPAIR));			}
	bool	IsAYard()															{ return(flags.test(SHIPYARD));		}
	bool	ProcessVocab(Player *player,const std::string& command);

	void	AddDesc(const std::string& text,int how = ADD_DESC);
	void	AddEvent(int which,EventNumber *ev)							{ events[which] = ev;		}
	void	AddFlags(std::string& text);
	void	AddExit(int dir,int where_to)									{ exits[dir] = where_to;	}
	void	AddName(const std::string& text)								{ name = text;					}
	void	AddNoExit(MsgNumber *msg_num)									{ no_exit = msg_num;			}
	void	AddVocab(const std::string& cmd,EventNumber *ev_num)	{ vocab[cmd] = ev_num;	}
	void	Description(std::ostringstream& buffer,int extent);
	void	Description(Player *player,int extent = DEFAULT);
	void	Dump();
	void	RemoveExit(int dir)												{ exits[dir] = NO_EXIT;	}
	void	Write(std::ofstream& file);
	void	WriteDesc(std::ofstream& file);
	void	WriteEvents(std::ofstream& file);
	void	WriteExits(std::ofstream& file);
	void	WriteFlags(std::ofstream& file);
	void	WriteVocab(std::ofstream& file);
	void	XMLNewLoc(Player *player,int extent = DEFAULT);
};

#endif


