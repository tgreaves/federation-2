/*-----------------------------------------------------------------------
		Copyright (c) Alan Lenton & Interactive Broadcasting 2003-8
	All Rights Reserved. No part of this software may be reproduced,
	transmitted, transcribed, stored in a retrieval system, or translated
	into any human or computer language, in any form or by any means,
	electronic, mechanical, magnetic, optical, manual or otherwise,
	without the express written permission of the copyright holder.
-----------------------------------------------------------------------*/

#ifndef COMMODEXCHITEM_H
#define COMMODEXCHITEM_H

#include <fstream>
#include <sstream>
#include <string>

class	Commodityexchange;
class	ExchEvent;
class	FedMap;
class	Infrastructure;
class	Player;

class	CommodityExchItem
{
public:
	static const int	CARGO_SIZE = 75;

	static const int	MAX_MAX_STOCK;
	static const int	MIN_MAX_STOCK;

	static const int	MAX_MIN_STOCK;
	static const int	MIN_MIN_STOCK;

	enum	{ BUY, SELL, PRICE };

private:
	std::string	name;		// real name of the commodity
	int	value;			// current value/ton
	int	spread;			//	percentage spread between buy/sell prices
	int	stock;			// quantity in stock
	int	min_stock;		// stop selling at this level
	int	max_stock;		// stop producing at this level
	int	production;		// internal production
	int	consumption;	// internal consumption
	int	base_prod;		// production before disasters etc
	int	base_cons;		// consumption before disasters etc
	int	efficiency;		// percentage overall efficiency of production
	int	cycles;			// number of unused cycles available
	bool	valid;			// true if record is complete

public:
	CommodityExchItem(const std::string&	the_name,int exchange_type);
	CommodityExchItem(const char **attrib);
	~CommodityExchItem()		{	}

	const	std::string&	Name()										{ return(name);	}

	long	Buy(FedMap *exch_map);
	long	DivertJob(const std::string& cartel_name,FedMap *exch_map);
	long	Sell(FedMap *exch_map);
	long	Update(int add_cycles,int cycles_per_update,int deficit);
	long	UpdateStock(int add_cycles,int deficit);
	long	YardPurchase(FedMap *exch_map,int amount,std::ostringstream& buffer,int action);

	int	FinalPrice(int value,int spread,int buy_or_sell);
	int	Spread()															{ return(spread);	}
	int	UpdateValue();
	int	Value()															{ return(value);	}

	bool	IsValid()														{ return(valid);	}

	void	AddStock(int amount)											{ stock += amount;	}
	void	Buy(Player *player,FedMap *exch_map);
	void	ChangeCurrentConsumption(int amount);
	void	ChangeConsumption(int amount);
	void	ChangeProduction(int amount);
	void	ClearStock()													{ stock = 0;		}
	void	DisplayProduction(Player *player,int commod_grp);
	void	DivertTrade(Player *player,const std::string& cartel_name,FedMap *exch_map);
	void	Dump();
	void	GroupDisplay(Player *player,int commod_grp);
	void	LineDisplay(FedMap *home_map,bool send_intro,Player *player = 0);
	void	LineDisplay(Player *player,const std::string& exch_name,const std::string& star_name);
	void	LineDisplay(const std::string& exch_name,const std::string& star_name,
																			std::ostringstream& buffer,int which);
	void	MaxStock(int level);
	void	MinStock(int level);
	void	OwnerDisplay(Player *player,int commod_grp);
	void	Sell(Player *player,FedMap *exch_map);
	void	SetEfficiency(int bonus)									{ efficiency += bonus;	}
	void	Spread(int amount);
	void	Update(std::pair<int,int> exch_event);
	void	Write(std::ofstream&	file);
};

#endif

