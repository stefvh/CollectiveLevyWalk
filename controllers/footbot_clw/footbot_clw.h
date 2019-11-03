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
  typedef std::vector<UInt8*> TMessages;
  struct SCommunicationStateData 
  {
    TMessages PreviouslyReceivedMessages;

    SCommunicationStateData();
    void Reset();
  };
  struct SCommunicationParams
  {
    int CommunicationRange;

    SCommunicationParams();
    void Init(TConfigurationNode &t_node);
  };
  struct SCollectiveLevyWalkParams
  {
    Real ShortLongStepThreshold;

    SCollectiveLevyWalkParams();
    void Init(TConfigurationNode &t_node);
  }; 
public:
  CFootBotCollectiveLevyWalk();
  virtual ~CFootBotCollectiveLevyWalk() {}

  virtual void Reset();

protected:
  CCI_RangeAndBearingActuator*  m_pcRABA;
  CCI_RangeAndBearingSensor* m_pcRABS;
  
  SCommunicationStateData m_sCommunicationStateData;
  SCommunicationParams m_sCommunicationParams;
  SCollectiveLevyWalkParams m_sCollectiveLevyWalkParams;
  
  struct SMessageSource
  {
    Real Distance;
    CRadians Angle;

    explicit SMessageSource(Real f_distance, CRadians c_angle): 
        Distance(f_distance),
        Angle(c_angle) {}
  };
  typedef std::vector<SMessageSource> TSources;

protected:
  virtual void InitActuators();
  virtual void InitSensors();
  virtual void InitParams(TConfigurationNode &t_node);

  void InitRotateStateAsRepulsiveForce(TSources t_sources);

  virtual void DoWalk();

};

#endif