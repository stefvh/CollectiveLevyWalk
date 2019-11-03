#include "footbot_ilw.h"

#include <argos3/plugins/robots/foot-bot/simulator/footbot_entity.h>
#include <argos3/core/utility/configuration/argos_configuration.h>
#include <argos3/core/utility/logging/argos_log.h>

/****************************************/
/****************************************/

CFootBotIndividualLevyWalk::SStateData::SStateData() :
   State(STATE_INITIALIZE),
   ToWalkSimulationTicks(0),
   WalkedSimulationTicks(0),
   ToRotateSimulationTicks(0),
   RotatedSimulationTicks(0) {}

void CFootBotIndividualLevyWalk::SStateData::Init(TConfigurationNode& t_node) 
{
   try {
      GetNodeAttribute(t_node, "levy_alpha_exponent", LevyAlphaExponent);
   }
   catch(CARGoSException& ex) {
      THROW_ARGOSEXCEPTION_NESTED("Error initializing controller state parameters.", ex);
   }
}

/****************************************/
/****************************************/

CFootBotIndividualLevyWalk::SExperimentParams::SExperimentParams() :
   TicksPerSecond(10) {}

/****************************************/
/****************************************/

CFootBotIndividualLevyWalk::SFootbotParams::SFootbotParams() :
   InterwheelDistance(14) {}

/****************************************/
/****************************************/

CFootBotIndividualLevyWalk::SWheelVelocityParams::SWheelVelocityParams() {}

void CFootBotIndividualLevyWalk::SWheelVelocityParams::Init(TConfigurationNode& t_node) 
{
   try {
      GetNodeAttribute(t_node, "walk_velocity", WalkVelocity);
      GetNodeAttribute(t_node, "max_velocity", MaxVelocity);
   }
   catch(CARGoSException& ex) {
      THROW_ARGOSEXCEPTION_NESTED("Error initializing controller wheel velocity parameters.", ex);
   }
}

/****************************************/
/****************************************/

CFootBotIndividualLevyWalk::SStochasticParams::SStochasticParams()
{
   MapAlphaExponentToStdDevOfRandomVariableX[1.0f] = 1.0f;
   MapAlphaExponentToStdDevOfRandomVariableX[1.1f] = 0.938291f;
   MapAlphaExponentToStdDevOfRandomVariableX[1.2f] = 0.878829f;
   MapAlphaExponentToStdDevOfRandomVariableX[1.3f] = 0.819837f;
   MapAlphaExponentToStdDevOfRandomVariableX[1.4f] = 0.759679f;
   MapAlphaExponentToStdDevOfRandomVariableX[1.5f] = 0.696575f;
   MapAlphaExponentToStdDevOfRandomVariableX[1.6f] = 0.628231f;
   MapAlphaExponentToStdDevOfRandomVariableX[1.7f] = 0.55116f;
   MapAlphaExponentToStdDevOfRandomVariableX[1.8f] = 0.458638f;
   MapAlphaExponentToStdDevOfRandomVariableX[1.9f] = 0.333819f;
   MapAlphaExponentToStdDevOfRandomVariableX[1.95f] = 0.241176f;
   MapAlphaExponentToStdDevOfRandomVariableX[1.99f] = 0.110693f;

   RotationAngleProbRange.Set(-CRadians::PI, CRadians::PI);
}

void CFootBotIndividualLevyWalk::SStochasticParams::Init(TConfigurationNode& t_node) 
{
   try {
      GetNodeAttribute(t_node, "normalization_factor_n", NormalizationFactorN);
      GetNodeAttribute(t_node, "max_random_levy_alpha_distributed_value", MaxRandomLevyAlphaDistributedValue);
   }
   catch(CARGoSException& ex) {
      THROW_ARGOSEXCEPTION_NESTED("Error initializing controller stochastic parameters.", ex);
   }
}

/****************************************/
/****************************************/

CFootBotIndividualLevyWalk::SDiffusionParams::SDiffusionParams() {}

void CFootBotIndividualLevyWalk::SDiffusionParams::Init(TConfigurationNode& t_node) {
   try {
      CRange<CDegrees> cGoStraightAngleRangeDegrees(CDegrees(-10.0f), CDegrees(10.0f));
      GetNodeAttribute(t_node, "go_straight_angle_range", cGoStraightAngleRangeDegrees);
      GoStraightAngleRange.Set(ToRadians(cGoStraightAngleRangeDegrees.GetMin()),
                               ToRadians(cGoStraightAngleRangeDegrees.GetMax()));
      GetNodeAttribute(t_node, "delta", Delta);
   }
   catch(CARGoSException& ex) {
      THROW_ARGOSEXCEPTION_NESTED("Error initializing controller diffusion parameters.", ex);
   }
}

/****************************************/
/****************************************/

CFootBotIndividualLevyWalk::SWheelTurningParams::SWheelTurningParams() :
   TurningMechanism(NO_TURN) {}

void CFootBotIndividualLevyWalk::SWheelTurningParams::Init(TConfigurationNode& t_node) {
   try {
      CDegrees cAngle;
      GetNodeAttribute(t_node, "hard_turn_angle_threshold", cAngle);
      HardTurnOnAngleThreshold = ToRadians(cAngle);
      GetNodeAttribute(t_node, "soft_turn_angle_threshold", cAngle);
      SoftTurnOnAngleThreshold = ToRadians(cAngle);
      GetNodeAttribute(t_node, "no_turn_angle_threshold", cAngle);
      NoTurnAngleThreshold = ToRadians(cAngle);
   }
   catch(CARGoSException& ex) {
      THROW_ARGOSEXCEPTION_NESTED("Error initializing controller wheel turning parameters.", ex);
   }
}

/****************************************/
/****************************************/

CFootBotIndividualLevyWalk::CFootBotIndividualLevyWalk():
   m_pcWheels(NULL),
   m_pcProximity(NULL),
   m_pcRNG(NULL) {}

/****************************************/
/****************************************/

void CFootBotIndividualLevyWalk::Init(TConfigurationNode& t_node) {
   try {
      InitActuators();
      InitSensors();
      InitParams(t_node);
      InitUtilities();
   }
   catch(CARGoSException& ex) {
      THROW_ARGOSEXCEPTION_NESTED("Error initalizing robot \"" << GetId() << "\"", ex);
   }

   Reset();
}

void CFootBotIndividualLevyWalk::InitActuators() 
{
   m_pcWheels    = GetActuator<CCI_DifferentialSteeringActuator>("differential_steering");
}

void CFootBotIndividualLevyWalk::InitSensors() 
{
   m_pcProximity = GetSensor  <CCI_FootBotProximitySensor>("footbot_proximity");
}

void CFootBotIndividualLevyWalk::InitParams(TConfigurationNode &t_node) 
{
   m_sStateData.Init(GetNode(t_node, "state"));
   m_sWheelVelocityParams.Init(GetNode(t_node, "wheel_velocity"));
   m_sStochasticParams.Init(GetNode(t_node, "stochastic"));
   m_sDiffusionParams.Init(GetNode(t_node, "diffusion"));
   m_sWheelTurningParams.Init(GetNode(t_node, "wheel_turning"));
}

void CFootBotIndividualLevyWalk::InitUtilities() 
{
   m_pcRNG = CRandom::CreateRNG("argos");
}

/****************************************/
/****************************************/

void CFootBotIndividualLevyWalk::ControlStep() {
   switch(m_sStateData.State) {
      case SStateData::STATE_INITIALIZE: {
         Initialize();
         break;
      }
      case SStateData::STATE_WALK: {
         Walk();
         break;
      }
      case SStateData::STATE_ROTATE: {
         Rotate();
         break;
      }
      case SStateData::STATE_COLLISIONAVOIDANCE: {
         AvoidCollision();
         break;
      }
      default: {
         LOGERR << "Error invalid state \"" << GetId() << "\"" << std::endl;
      }
   }
}

/****************************************/
/****************************************/

void CFootBotIndividualLevyWalk::Initialize() {
   // State transition
   InitWalkState();
}

/****************************************/
/****************************************/

void CFootBotIndividualLevyWalk::Walk() {
   bool bCollision;
   CVector2 cDiffusion = DiffusionVector(bCollision);

   if (bCollision) 
   {
      InitCollisionAvoidanceState();
      DoCollisionAvoidance(cDiffusion);
   } else {
      DoWalk();
   }
}

void CFootBotIndividualLevyWalk::DoWalk() 
{
   // Act
   m_pcWheels->SetLinearVelocity(m_sWheelVelocityParams.WalkVelocity, m_sWheelVelocityParams.WalkVelocity);

   // Update state
   m_sStateData.WalkedSimulationTicks++;

   // State transition
   if (m_sStateData.ToWalkSimulationTicks - m_sStateData.WalkedSimulationTicks <= 0) 
   {
      InitRotateStateAsUniform();
   }
}

/****************************************/
/****************************************/

void CFootBotIndividualLevyWalk::InitWalkState() 
{
   m_sStateData.ToWalkSimulationTicks = Round(GenerateRandomTimeLevyAlphaDistributedVariable());
   m_sStateData.WalkedSimulationTicks = 0;

   m_sStateData.State = SStateData::STATE_WALK;
}

Real CFootBotIndividualLevyWalk::GenerateRandomTimeLevyAlphaDistributedVariable()
{
   Real L;
   do {
      L = GenerateRandomLevyAlphaDistributedVariable();
   } while (Abs(L) > m_sStochasticParams.MaxRandomLevyAlphaDistributedValue);

   Real T = ((Real) m_sExperimentParams.TicksPerSecond) * (1.0 + Abs(L));
   return T;
}

Real CFootBotIndividualLevyWalk::GenerateRandomLevyAlphaDistributedVariable() 
{
   Real L = 0.0;
   for(size_t i = 0; i < m_sStochasticParams.NormalizationFactorN; ++i) {
      Real X = m_pcRNG->Gaussian(1.0, 0.0); // TODO: change stddev based on alpha
      Real Y = m_pcRNG->Gaussian(1.0, 0.0);

      Real V = X / ::pow(Abs(Y), (1.0 / m_sStateData.LevyAlphaExponent));

      L += V;
   }
   L /= ::pow(m_sStochasticParams.NormalizationFactorN, (1.0 / m_sStateData.LevyAlphaExponent));

   return L;
}

/****************************************/
/****************************************/

void CFootBotIndividualLevyWalk::Rotate() {
   // Act
   m_pcWheels->SetLinearVelocity(m_sStateData.RotateVelocity, -m_sStateData.RotateVelocity);

   // Update state
   m_sStateData.RotatedSimulationTicks++;

   // State transition
   if (m_sStateData.ToRotateSimulationTicks - m_sStateData.RotatedSimulationTicks <= 0)
   {
      InitWalkState();
   }
}

/****************************************/
/****************************************/

void CFootBotIndividualLevyWalk::InitRotateStateAsUniform() 
{
   CRadians fAngle = m_pcRNG->Uniform(m_sStochasticParams.RotationAngleProbRange);
   InitRotateStateFromAngle(fAngle);
}

void CFootBotIndividualLevyWalk::InitRotateStateFromAngle(CRadians c_angle) {
   Real A = c_angle.GetValue();
   Real Y = (Abs(A) * (Real)m_sFootbotParams.InterwheelDistance) / (2.0 * (1.0 / (Real)m_sExperimentParams.TicksPerSecond));

   int T = Ceil(Y / m_sWheelVelocityParams.MaxVelocity);

   m_sStateData.ToRotateSimulationTicks = T;
   m_sStateData.RotatedSimulationTicks = 0;
   m_sStateData.RotateVelocity = Sign(A) * (Y / (Real)T);

   m_sStateData.State = SStateData::STATE_ROTATE;
}

/****************************************/
/****************************************/

void CFootBotIndividualLevyWalk::AvoidCollision() {
   bool bCollision;
   CVector2 cDiffusion = DiffusionVector(bCollision);

   if (bCollision) 
   {
      DoCollisionAvoidance(cDiffusion);
   } else {
      InitWalkState();
      DoWalk();
   }
}

void CFootBotIndividualLevyWalk::DoCollisionAvoidance(const CVector2& c_diffusion) 
{
   // Act
   SetWheelSpeedsFromVector(m_sWheelVelocityParams.MaxVelocity * c_diffusion);
}

CVector2 CFootBotIndividualLevyWalk::DiffusionVector(bool& b_collision) {
   const CCI_FootBotProximitySensor::TReadings& tProxReads = m_pcProximity->GetReadings();
   CVector2 cDiffusionVector;
   for(size_t i = 0; i < tProxReads.size(); ++i) {
      cDiffusionVector += CVector2(tProxReads[i].Value, tProxReads[i].Angle);
   }
   /* If the angle of the vector is small enough and the closest obstacle
      is far enough, ignore the vector and go straight, otherwise return
      it */
   if(m_sDiffusionParams.GoStraightAngleRange.WithinMinBoundIncludedMaxBoundIncluded(cDiffusionVector.Angle()) &&
      cDiffusionVector.Length() < m_sDiffusionParams.Delta ) {
      b_collision = false;
      return CVector2::X;
   }
   else {
      b_collision = true;
      cDiffusionVector.Normalize();
      return -cDiffusionVector;
   }
}

void CFootBotIndividualLevyWalk::SetWheelSpeedsFromVector(const CVector2& c_heading) {
   CRadians cHeadingAngle = c_heading.Angle().SignedNormalize();
   Real fHeadingLength = c_heading.Length();
   Real fBaseAngularWheelSpeed = Min<Real>(fHeadingLength, m_sWheelVelocityParams.MaxVelocity);

   // State transition (TurningMechanism)
   if(m_sWheelTurningParams.TurningMechanism == SWheelTurningParams::HARD_TURN) {
      if(Abs(cHeadingAngle) <= m_sWheelTurningParams.SoftTurnOnAngleThreshold) {
         m_sWheelTurningParams.TurningMechanism = SWheelTurningParams::SOFT_TURN;
      }
   }
   if(m_sWheelTurningParams.TurningMechanism == SWheelTurningParams::SOFT_TURN) {
      if(Abs(cHeadingAngle) > m_sWheelTurningParams.HardTurnOnAngleThreshold) {
         m_sWheelTurningParams.TurningMechanism = SWheelTurningParams::HARD_TURN;
      }
      else if(Abs(cHeadingAngle) <= m_sWheelTurningParams.NoTurnAngleThreshold) {
         m_sWheelTurningParams.TurningMechanism = SWheelTurningParams::NO_TURN;
      }
   }
   if(m_sWheelTurningParams.TurningMechanism == SWheelTurningParams::NO_TURN) {
      if(Abs(cHeadingAngle) > m_sWheelTurningParams.HardTurnOnAngleThreshold) {
         m_sWheelTurningParams.TurningMechanism = SWheelTurningParams::HARD_TURN;
      }
      else if(Abs(cHeadingAngle) > m_sWheelTurningParams.NoTurnAngleThreshold) {
         m_sWheelTurningParams.TurningMechanism = SWheelTurningParams::SOFT_TURN;
      }
   }

   // Wheel speeds based on current turning state
   Real fSpeed1, fSpeed2;
   switch(m_sWheelTurningParams.TurningMechanism) {
      case SWheelTurningParams::NO_TURN: {
         // Just go straight
         fSpeed1 = fBaseAngularWheelSpeed;
         fSpeed2 = fBaseAngularWheelSpeed;
         break;
      }
      case SWheelTurningParams::SOFT_TURN: {
         // Both wheels go straight, but one is faster than the other
         Real fSpeedFactor = (m_sWheelTurningParams.HardTurnOnAngleThreshold - Abs(cHeadingAngle)) / m_sWheelTurningParams.HardTurnOnAngleThreshold;
         fSpeed1 = fBaseAngularWheelSpeed - fBaseAngularWheelSpeed * (1.0 - fSpeedFactor);
         fSpeed2 = fBaseAngularWheelSpeed + fBaseAngularWheelSpeed * (1.0 - fSpeedFactor);
         break;
      }
      case SWheelTurningParams::HARD_TURN: {
         // Opposite wheel speeds
         fSpeed1 = -m_sWheelVelocityParams.MaxVelocity;
         fSpeed2 =  m_sWheelVelocityParams.MaxVelocity;
         break;
      }
   }

   // Apply the calculated speeds to the appropriate wheels
   Real fLeftWheelSpeed, fRightWheelSpeed;
   if(cHeadingAngle > CRadians::ZERO) {
      // Turn Left
      fLeftWheelSpeed  = fSpeed1;
      fRightWheelSpeed = fSpeed2;
   }
   else {
      // Turn Right
      fLeftWheelSpeed  = fSpeed2;
      fRightWheelSpeed = fSpeed1;
   }

   // Finally, set the wheel speeds
   m_pcWheels->SetLinearVelocity(fLeftWheelSpeed, fRightWheelSpeed);
}

/****************************************/
/****************************************/

void CFootBotIndividualLevyWalk::InitCollisionAvoidanceState() 
{
   m_sStateData.State = SStateData::STATE_COLLISIONAVOIDANCE;
}

/****************************************/
/****************************************/

REGISTER_CONTROLLER(CFootBotIndividualLevyWalk, "footbot_ilw_controller")
