#include "footbot_ilw.h"

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
    
}

/****************************************/
/****************************************/

void CFootBotIndividualLevyWalk::Rotate() {

}

/****************************************/
/****************************************/

void CFootBotIndividualLevyWalk::AvoidCollision() {

}

/****************************************/
/****************************************/

CVector2 CFootBotForaging::DiffusionVector(bool& b_collision) {
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

void CFootBotForaging::SetWheelSpeedsFromVector(const CVector2& c_heading) {
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
