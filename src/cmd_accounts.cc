/*-----------------------------------------------------------------------
		Copyright (c) Alan Lenton 1985-2015
	All Rights Reserved. No part of this software may be reproduced,
	transmitted, transcribed, stored in a retrieval system, or translated
	into any human or computer language, in any form or by any means,
	electronic, mechanical, magnetic, optical, manual or otherwise,
	without the express written permission of the copyright holder.
-----------------------------------------------------------------------*/

#include "cmd_accounts.h"

#include <iomanip>
#include <sstream>
#include <output_filter.h>

#include "company.h"
#include "misc.h"
#include "player.h"


const int 	Accounts::NOT_AVAILABLE = -1;

Accounts::Accounts(const std::string& the_name,const std::string& owner,int number)
{
	name = the_name;
	ceo = owner;
	cycle_number = number;
	share_value = num_shares = share_val_chg = 0;
	assets = assets_chg = cash = cash_chg = rev_income = rev_exp = profit = 0L;
	pe_ratio = pd_ratio = ed_ratio = NOT_AVAILABLE;
}

void	Accounts::Cash(long net_assets,long ass_chg,long money,
								long money_chg,long income,long exp,long prof,long prof_chg)
{
	assets = net_assets;
	assets_chg = ass_chg;
	cash = money;
	cash_chg = money_chg;
	rev_income = income;
	rev_exp = exp;
	profit = prof;
	profit_chg = prof_chg;
}

void	Accounts::Display(Player *player)
{
	static const std::string dashes("------------------------------------------\n");
	std::ostringstream	buffer;
	buffer << name << " Accounts - Cycle Number " << cycle_number << "\n";
	buffer << "   Share value: " << share_value;
	if(share_val_chg == 0)
		buffer << "ig (no change)\n";
	else
	{
		buffer.setf(std::ios::showpos);
		buffer << "ig (" << share_val_chg << ")\n";
		buffer.unsetf(std::ios::showpos);
	}
	buffer << "   A total of " << num_shares << " shares have been issued\n";

	buffer << "   Assets: ";
	MakeMoneyString(assets,buffer);
	if(assets_chg == 0L)
		buffer << " (no change)*\n";
	else
	{
	 	buffer << " (";
		buffer.setf(std::ios::showpos);
		MakeMoneyString(assets_chg,buffer);
		buffer.unsetf(std::ios::showpos);
		buffer << ")*\n";
	}
	buffer << "   Dividend: " << dividend << "ig\n";

	buffer << "   Cash: ";
	MakeMoneyString(cash,buffer);
	if(cash_chg == 0L)
		buffer << " (no change)\n";
	else
	{
	 	buffer << " (";
		buffer.setf(std::ios::showpos);
		MakeMoneyString(cash_chg,buffer);
		buffer.unsetf(std::ios::showpos);
		buffer << ")\n";
	}

	buffer << "   Income: ";
	MakeMoneyString(rev_income,buffer);
	buffer << "   Expenditure: ";
	MakeMoneyString(rev_exp,buffer);
	buffer << "\n";

	buffer << "   Profit: ";
	MakeMoneyString(profit,buffer);
	if(profit_chg == 0L)
		buffer << " (no change)\n";
	else
	{
	 	buffer << " (";
		buffer.setf(std::ios::showpos);
		MakeMoneyString(profit_chg,buffer);
		buffer.unsetf(std::ios::showpos);
		buffer << ")\n";
	}

	if(pe_ratio != Company::NO_EARNINGS)
		buffer << "   Price-Earnings (P/E) ratio is " << pe_ratio << "\n";
	else
		buffer << "   Earnings insufficient to generate a meaningful P/E ratio\n";
	if(pd_ratio != Company::NO_DIVIDENDS)
	{
		buffer << "   Price-Dividend (P/D) ratio is " << pd_ratio << "\n";
		if(ed_ratio != Company::NO_EARNINGS)
			buffer << "   Earnings-Dividend (E/D) ratio is " << ed_ratio << "\n";
		else
			buffer << "   Earnings insufficient to generate a meaningful E/D ratio\n";
	}
	else
		buffer << "   No dividend payment has been recorded\n";
	buffer << "\n*All assets valued at nominal sale prices.\n";
	player->Send(buffer,OutputFilter::DEFAULT);
	player->Send(dashes,OutputFilter::DEFAULT);
}

void	Accounts::Ratios(int pe,int pd,int ed)
{
	pe_ratio = pe;
	pd_ratio = pd;
	ed_ratio = ed;
}

void	Accounts::Shares(int value,int number,int changed,int divi)
{
	share_value = value;
	num_shares = number;
	share_val_chg = changed;
	dividend = divi;
}

void	Accounts::Write(std::ofstream& file)
{
	file << "    <accounts name='" << EscapeXML(name) << "' ceo='" << ceo << "' cycle='" << cycle_number << "'>\n";

	file << "      <share-info value='" << share_value << "' number='";
	file << num_shares << "' change='" << share_val_chg << "' dividend='" << dividend << "'/>\n";

	file << "      <money assets='" << assets << "' assets-chg='" << assets_chg;
	file << "' cash='" << cash << "' cash-chg='" << cash_chg;
	file << "' income='" << rev_income << "' exp='" << rev_exp;
	file << "' profit='" << profit << "' profit-chg='" << profit_chg << "'/>\n";

	file << "      <ratios pe='" << pe_ratio << "' pd='" << pd_ratio << "' ed='" << ed_ratio << "'/>\n";
	file << "    </accounts>\n";
}

