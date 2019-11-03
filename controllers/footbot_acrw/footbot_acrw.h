/*
 * AUTHOR: Stef Van Havermaet <stefvanhavermaet@gmail.com>
 *
 * Adaptive Collective Random Walk (CLW) controller for the foot-bot.
 *
 */

#ifndef FOOTBOT_ACRW_H
#define FOOTBOT_ACRW_H

#include "../footbot_clw/footbot_clw.h"


using namespace argos;

class CFootBotAdaptiveCollectiveRandomWalk : public CFootBotCollectiveLevyWalk 
{
};

#endif