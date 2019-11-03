/*
 * AUTHOR: Stef Van Havermaet <stefvanhavermaet@gmail.com>
 *
 * Collective Levy Walk (CLW) controller for the foot-bot.
 *
 */

#ifndef FOOTBOT_CLW_H
#define FOOTBOT_CLW_H

#include "../footbot_ilw/footbot_ilw.h"

#include <argos3/plugins/robots/generic/control_interface/ci_range_and_bearing_actuator.h>
#include <argos3/plugins/robots/generic/control_interface/ci_range_and_bearing_sensor.h>

using namespace argos;

class CFootBotCollectiveLevyWalk : public CFootBotIndividualLevyWalk 
{
public:
  CFootBotCollectiveLevyWalk();
  virtual ~CFootBotCollectiveLevyWalk() {}

  virtual void Reset();
  virtual void ControlStep();

protected:
  CCI_RangeAndBearingActuator*  m_pcRABA;
  CCI_RangeAndBearingSensor* m_pcRABS;

protected:
  virtual void InitActuators();
  virtual void InitSensors();
};

#endif