/*
 * AUTHOR: Stef Van Havermaet <stefvanhavermaet@gmail.com>
 *
 * Individual Levy Walk (ILW) controller for the foot-bot.
 *
 */

#ifndef FOOTBOT_ILW_H
#define FOOTBOT_ILW_H

#include <argos3/core/control_interface/ci_controller.h>
#include <argos3/plugins/robots/generic/control_interface/ci_differential_steering_actuator.h>
#include <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_proximity_sensor.h>

using namespace argos;

class CFootBotIndividualLevyWalk : public CCI_Controller
{

public:
  struct SStateData
  {
    enum EState
    {
      STATE_INITIALIZE = 0,
      STATE_WALK,
      STATE_ROTATE,
      STATE_COLLISIONAVOIDANCE
    } State;

    SStateData();
    void Reset();
  };
  struct SDiffusionParams
  {
    Real Delta;
    CRange<CRadians> GoStraightAngleRange;

    void Init(TConfigurationNode &t_tree);
  };
  struct SWheelTurningParams
  {
    enum ETurningMechanism
    {
      NO_TURN = 0, // go straight
      SOFT_TURN,   // both wheels are turning forwards, but at different speeds
      HARD_TURN    // wheels are turning with opposite speeds
    } TurningMechanism;

    CRadians HardTurnOnAngleThreshold;
    CRadians SoftTurnOnAngleThreshold;
    CRadians NoTurnAngleThreshold;

    Real MaxSpeed;

    void Init(TConfigurationNode &t_tree);
  };

public:
  CFootBotIndividualLevyWalk();
  virtual ~CFootBotIndividualLevyWalk() {}

  virtual void Init(TConfigurationNode &t_node);
  virtual void ControlStep();

private:
  CCI_DifferentialSteeringActuator *m_pcWheels;
  CCI_FootBotProximitySensor *m_pcProximity;

  SStateData m_sStateData;
  SDiffusionParams m_sDiffusionParams;
  SWheelTurningParams m_sWheelTurningParams;

  CRandom::CRNG *m_pcRNG;

private:
  void Initialize();
  void Walk();
  void Rotate();
  void AvoidCollision();
};

#endif
