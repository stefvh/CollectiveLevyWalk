#include "footbot_ilw.h"

#include <argos3/plugins/robots/foot-bot/simulator/footbot_entity.h>
#include <argos3/core/utility/configuration/argos_configuration.h>
#include <argos3/core/utility/math/vector2.h>
#include <argos3/core/utility/logging/argos_log.h>

/****************************************/
/****************************************/

CFootBotIndividualLevyWalk::SStateData::SStateData() :
   LevyAlphaExponent(1.0f) {}

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

CFootBotIndividualLevyWalk::SWheelVelocityParams::SWheelVelocityParams() :
   WalkVelocity(17.0),
   MaxVelocity(20.0) {}

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

   NormalizationFactorN = 100;
   MaxRandomLevyAlphaDistributedValue = 59;

   RotationAngleProbRange.Set(-M_PI, M_PI);
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
      m_pcWheels    = GetActuator<CCI_DifferentialSteeringActuator>("differential_steering");
      m_pcProximity = GetSensor  <CCI_FootBotProximitySensor      >("footbot_proximity"    );

      m_sDiffusionParams.Init(GetNode(t_node, "diffusion"));
      m_sWheelTurningParams.Init(GetNode(t_node, "wheel_turning"));
   }
   catch(CARGoSException& ex) {
      THROW_ARGOSEXCEPTION_NESTED("ERROR CFootBotIndividualLevyWalk - Init \"" << GetId() << "\"", ex);
   }

   m_pcRNG = CRandom::CreateRNG("argos");
   Reset();
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
      }
      default: {
         LOGERR << "ERROR CFootBotIndividualLevyWalk - Invalid State" << std::endl;
      }
   }
}

/****************************************/
/****************************************/

void CFootBotIndividualLevyWalk::Initialize() {

}

/****************************************/
/****************************************/

void CFootBotIndividualLevyWalk::Walk() {
   m_sStateData.RemainingWalkSimulationTicks = Round(GenerateRandomTimeLevyAlphaDistributedVariable());
   m_pcWheels->SetLinearVelocity(m_sWheelVelocityParams.WalkVelocity, m_sWheelVelocityParams.WalkVelocity);
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

Real CFootBotIndividualLevyWalk::GenerateRandomTimeLevyAlphaDistributedVariable()
{
   Real L;
   do {
      L = GenerateRandomLevyAlphaDistributedVariable();
   } while (Abs(L) > m_sStochasticParams.MaxRandomLevyAlphaDistributedValue);

   Real T = ((Real) m_sExperimentParams.TicksPerSecond) * (1.0 + Abs(L));
   return T;
}

/****************************************/
/****************************************/

void CFootBotIndividualLevyWalk::Rotate() {
   Real U = m_pcRNG->Uniform(m_sStochasticParams.RotationAngleProbRange);
   Real Y = (Abs(U) * (Real)m_sFootbotParams.InterwheelDistance) / (2.0 * (Real)m_sExperimentParams.TicksPerSecond);
   
   int T = Ceil(Y / m_sWheelVelocityParams.MaxVelocity);
   
   Real fRotateVelocity = Sign(U) * (Y / (Real)T);

   m_sStateData.RemainingRotateSimulationTicks = T;
}

/****************************************/
/****************************************/

void CFootBotIndividualLevyWalk::AvoidCollision() {

}

/****************************************/
/****************************************/

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

/****************************************/
/****************************************/

void CFootBotIndividualLevyWalk::SetWheelSpeedsFromVector(const CVector2& c_heading) {
   CRadians cHeadingAngle = c_heading.Angle().SignedNormalize();
   Real fHeadingLength = c_heading.Length();
   Real fBaseAngularWheelSpeed = Min<Real>(fHeadingLength, m_sWheelTurningParams.MaxSpeed);
   /* State transition logic */
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
   /* Wheel speeds based on current turning state */
   Real fSpeed1, fSpeed2;
   switch(m_sWheelTurningParams.TurningMechanism) {
      case SWheelTurningParams::NO_TURN: {
         /* Just go straight */
         fSpeed1 = fBaseAngularWheelSpeed;
         fSpeed2 = fBaseAngularWheelSpeed;
         break;
      }
      case SWheelTurningParams::SOFT_TURN: {
         /* Both wheels go straight, but one is faster than the other */
         Real fSpeedFactor = (m_sWheelTurningParams.HardTurnOnAngleThreshold - Abs(cHeadingAngle)) / m_sWheelTurningParams.HardTurnOnAngleThreshold;
         fSpeed1 = fBaseAngularWheelSpeed - fBaseAngularWheelSpeed * (1.0 - fSpeedFactor);
         fSpeed2 = fBaseAngularWheelSpeed + fBaseAngularWheelSpeed * (1.0 - fSpeedFactor);
         break;
      }
      case SWheelTurningParams::HARD_TURN: {
         /* Opposite wheel speeds */
         fSpeed1 = -m_sWheelTurningParams.MaxSpeed;
         fSpeed2 =  m_sWheelTurningParams.MaxSpeed;
         break;
      }
   }
   /* Apply the calculated speeds to the appropriate wheels */
   Real fLeftWheelSpeed, fRightWheelSpeed;
   if(cHeadingAngle > CRadians::ZERO) {
      /* Turn Left */
      fLeftWheelSpeed  = fSpeed1;
      fRightWheelSpeed = fSpeed2;
   }
   else {
      /* Turn Right */
      fLeftWheelSpeed  = fSpeed2;
      fRightWheelSpeed = fSpeed1;
   }
   /* Finally, set the wheel speeds */
   m_pcWheels->SetLinearVelocity(fLeftWheelSpeed, fRightWheelSpeed);
}

/****************************************/
/****************************************/

/****************************************/
/****************************************/

REGISTER_CONTROLLER(CFootBotIndividualLevyWalk, "footbot_ilw_controller")
