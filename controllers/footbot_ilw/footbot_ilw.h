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
#include <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_motor_ground_sensor.h>
#include <argos3/core/utility/math/rng.h>
#include <argos3/core/utility/math/vector2.h>

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

    enum ERandomWalk
    {
      PURE_LEVY_WALK = 0,
      ALPHA_LEVY_WALK,
      BROWNIAN_WALK
    } RandomWalk;

    int ToWalkSimulationTicks;
    int WalkedSimulationTicks;

    int ToRotateSimulationTicks;
    int RotatedSimulationTicks;
    Real RotateVelocity;

    bool TargetFound;

    SStateData();
    void Reset();
  };
  struct SExperimentParams 
  {
    int TicksPerSecond;

    SExperimentParams();
  };
  struct SFootbotParams 
  {
    int InterwheelDistance;
    int BodyDiameter;

    SFootbotParams();
  };
  struct SWheelVelocityParams 
  {
    Real WalkVelocity;
    Real MaxVelocity;

    SWheelVelocityParams();
    void Init(TConfigurationNode &t_node);
  };
  struct SStochasticParams 
  {
    int NormalizationFactorN;
    int MaxStepLength;

    CRange<CRadians> RotationAngleUniformRange;

    SStochasticParams();
    void Init(TConfigurationNode &t_node);
  };
  struct SDiffusionParams
  {
    Real Delta;
    CRange<CRadians> GoStraightAngleRange;

    SDiffusionParams();
    void Init(TConfigurationNode &t_node);
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

    SWheelTurningParams();
    void Init(TConfigurationNode &t_node);
  };

public:
  CFootBotIndividualLevyWalk();
  virtual ~CFootBotIndividualLevyWalk() {}

  virtual void Init(TConfigurationNode &t_node);
  virtual void ControlStep();
  virtual void Reset();

  inline bool IsWalking() { return m_sStateData.State == SStateData::STATE_WALK; }
  inline bool IsRotating() { return m_sStateData.State == SStateData::STATE_ROTATE; }
  inline bool IsAvoidingCollision() { return m_sStateData.State == SStateData::STATE_COLLISIONAVOIDANCE; }

  inline SStateData GetStateData() { return m_sStateData; }

  inline SFootbotParams GetFootbotParams() { return m_sFootbotParams; }

protected:
  UInt32 m_uint32Id;

  CCI_DifferentialSteeringActuator *m_pcWheels;
  CCI_FootBotProximitySensor *m_pcProximity;
  CCI_FootBotMotorGroundSensor* m_pcGround;

  SStateData m_sStateData;

  SExperimentParams m_sExperimentParams;
  SFootbotParams m_sFootbotParams;
  SWheelVelocityParams m_sWheelVelocityParams;
  SStochasticParams m_sStochasticParams;
  SDiffusionParams m_sDiffusionParams;
  SWheelTurningParams m_sWheelTurningParams;

  CRandom::CRNG *m_pcRNG;

protected:
  void InitId();
  virtual void InitActuators();
  virtual void InitSensors();
  virtual void InitParams(TConfigurationNode &t_node);
  virtual void InitUtilities();

  void Initialize();
  void Walk();
  void Rotate();
  void AvoidCollision();

  void InitWalkState();
  void InitRotateStateAsUniform();
  void InitRotateStateFromAngle(CRadians c_angle);
  void InitCollisionAvoidanceState();

  void DetectTargets();
  virtual void UpdateStateFromExploration(bool b_target_found);

  virtual void DoWalk();
  void DoCollisionAvoidance(const CVector2& c_diffusion);

  int GenerateToWalkSimulationTicks();
  Real GenerateRandomTimeVariable();
  virtual Real GenerateRandomStepLengthVariable();
  Real GenerateRandomLevyAlphaDistributedVariable(Real f_levy_alpha_exponent);
  Real GetStdDevGaussianVariableX(Real f_levy_alpha_exponent);

private:
  CVector2 DiffusionVector(bool& b_collision);
  void SetWheelSpeedsFromVector(const CVector2& c_heading);
};

#endif
