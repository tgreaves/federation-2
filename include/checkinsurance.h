/*-----------------------------------------------------------------------
		Copyright (c) Alan Lenton & Interactive Broadcasting 2003-9
	All Rights Reserved. No part of this software may be reproduced,
	transmitted, transcribed, stored in a retrieval system, or translated
	into any human or computer language, in any form or by any means,
	electronic, mechanical, magnetic, optical, manual or otherwise,
	without the express written permission of the copyright holder.
-----------------------------------------------------------------------*/

#ifndef CHECKSINSURANCE_H
#define CHECKSINSURANCE_H

#include "script.h"

class FedMap;
class EventNumber;
class Player;

class CheckInsurance : public Script
{
private:
	EventNumber	*pass;
	EventNumber	*fail;

public:
	CheckInsurance(const char **attrib,FedMap *fed_map);
	virtual	~CheckInsurance();

	int	Process(Player *player);
};

#endif
