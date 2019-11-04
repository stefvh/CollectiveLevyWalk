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
public:
  struct SAdaptationStateData 
  {
    int NoTargetFoundConsecutivelySimulationTicks;

    SAdaptationStateData();
    void Reset();
  };
  struct SAdaptationParams 
  {
    int AdaptationIntervalSimulationTicks;
    Real ErfcTransitionInterval;
    Real ErfcTransitionFactor;

    SAdaptationParams();
    void Init(TConfigurationNode &t_node);
    void InitCalculatedParams();
  };

public:
  CFootBotAdaptiveCollectiveRandomWalk();
  virtual ~CFootBotAdaptiveCollectiveRandomWalk() {}

  virtual void Reset();

protected:
  SAdaptationStateData m_sAdaptationStateData;
  SAdaptationParams m_sAdaptationParams;

protected:
  virtual void InitParams(TConfigurationNode &t_node);

  virtual void UpdateStateFromExploration(bool b_target_found);
};

#endif