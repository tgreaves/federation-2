/*-----------------------------------------------------------------------
                          Federation 2
              Copyright (c) 1985-2018 Alan Lenton

This program is free software: you can redistribute it and /or modify 
it under the terms of the GNU General Public License as published by 
the Free Software Foundation: either version 2 of the License, or (at 
your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY: without even the implied warranty of 
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
General Public License for more details.

You can find the full text of the GNU General Public Licence at
           http://www.gnu.org/copyleft/gpl.html

Programming and design:     Alan Lenton (email: alan@ibgames.com)
Home website:                   www.ibgames.net/alan
-----------------------------------------------------------------------*/

#ifndef COMEXGRAPHPARSER_H
#define COMEXGRAPHPARSER_H

#include "xml_parser.h"

class CommodGraphRec;
class	CommodExchGraphs;

class CommodExchangeGraphParser : public XMLParser
{
private:
	CommodExchGraphs		*graphs;
	void	StartElement(const char *element,const char **attrib);

public:
	CommodExchangeGraphParser(CommodExchGraphs *graphs_ptr) : graphs(graphs_ptr)	{	}
	virtual ~CommodExchangeGraphParser();
};

#endif
