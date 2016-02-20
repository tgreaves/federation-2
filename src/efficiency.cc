/*-----------------------------------------------------------------------
		Copyright (c) Alan Lenton 1985-2016
	All Rights Reserved. No part of this software may be reproduced,
	transmitted, transcribed, stored in a retrieval system, or translated
	into any human or computer language, in any form or by any means,
	electronic, mechanical, magnetic, optical, manual or otherwise,
	without the express written permission of the copyright holder.
-----------------------------------------------------------------------*/

#include "efficiency.h"

#include <sstream>

#include "fedmap.h"
#include "infra.h"
#include "misc.h"


Efficiency::Efficiency(Infrastructure *infrastructure)
{
	infra = infrastructure;

	agri_coll_pts = all_pts = coal_pts = housing_project_pts = canal_pts = 0;
	railway_pts = riot_police_pts = mining_school_pts = insulation_pts = insurance_pts = 0;
	port_pts = tech_inst_pts = research_inst_pts = defence_pts = weather_pts = 0;
	urban_pts = floating_pts = biolab_pts = tquark_pts = metastudio_pts = leisure_pts = 0;

	agri_pc = all_pc = resource_pc = ind_pc = tech_pc = bio_pc = leisure_pc = 0;
	bulk_pc = consumer_pc = defence_pc = energy_pc = sea_pc = 0;
}


void	Efficiency::CalculateCanalPoints()
{
	agri_pc += canal_pts;
	resource_pc += canal_pts;
}

void	Efficiency::CalculateHousing()
{
	int	level = infra->Economy();
	int	needed;

	switch(level)
	{
		case Infrastructure::RESOURCE:	needed = 3;	break;
		case Infrastructure::INDUSTRIAL:	needed = 6;	break;
		default:									needed = 0;	break;
	}

	int deficit = needed - housing_project_pts;
	if(deficit > 0)
	{
		agri_pc -= (deficit * 5);
		resource_pc -= (deficit * 5);
		ind_pc -= (deficit * 5);
		tech_pc -= (deficit * 5);
		bio_pc -= (deficit * 5);
		leisure_pc -= (deficit * 5);
	}
}

void	Efficiency::CalculateRailway()
{
	resource_pc += railway_pts;
	ind_pc += railway_pts;
	tech_pc += railway_pts;
}

void	Efficiency::CalculateRiotPolicePoints()
{
	agri_pc -= riot_police_pts;
	resource_pc -= riot_police_pts;
	ind_pc -= riot_police_pts;
	tech_pc -= riot_police_pts;
	bio_pc -= riot_police_pts;
	leisure_pc -= riot_police_pts;
}

void	Efficiency::CalculateUrbanPoints()
{
	tech_pc += urban_pts;
	leisure_pc += urban_pts;
}

void	Efficiency::CalculateWeatherPts()
{
	agri_pc += weather_pts;
	sea_pc += weather_pts;
}

void	Efficiency::Dump()
{
	if((agri_pc + resource_pc + ind_pc + tech_pc + bio_pc + leisure_pc +
						bulk_pc + consumer_pc + defence_pc + energy_pc + sea_pc + all_pc) == 0)
		return;

	std::ostringstream buffer;
	buffer << infra->Home()->Title() << ": " << agri_pc << "/" << resource_pc << "/" << ind_pc;
	buffer << "/" << tech_pc << "/" << bio_pc << "/" << leisure_pc << "/" << bulk_pc << "/";
	buffer << consumer_pc << "/" << defence_pc << "/" << energy_pc << "/" << sea_pc << "/" << all_pc;
	WriteLog(buffer);
}

void	Efficiency::Update()
{
	CalculateAgriCollege();
	CalculateBioLab();
	CalculateCanalPoints();
	CalculateCoal();
	CalculateDefence();
	CalculateFloating();
	CalculateHousing();
	CalculateInsulation();
	CalculateInsurance();
	CalaculateLeisure();
	CalculateMetaStudio();
	CalculateMiningSchool();
	CalculatePort();
	CalculateRailway();
	CalculateResearchInst();
	CalculateRiotPolicePoints();
	CalculateTechInst();
	CalculateTQuark();
	CalculateUrbanPoints();
	CalculateWeatherPts();

	CalculateAll();
}

