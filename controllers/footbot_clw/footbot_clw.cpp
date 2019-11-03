#include "footbot_clw.h"

#include <argos3/core/utility/logging/argos_log.h>

/****************************************/
/****************************************/

CFootBotCollectiveLevyWalk::CFootBotCollectiveLevyWalk(): CFootBotIndividualLevyWalk(),
   m_pcRABA(NULL),
   m_pcRABS(NULL) {}

/****************************************/
/****************************************/

void CFootBotCollectiveLevyWalk::InitActuators() {
    CFootBotIndividualLevyWalk::InitActuators();
    m_pcRABA = GetActuator<CCI_RangeAndBearingActuator>("range_and_bearing");
}

void CFootBotCollectiveLevyWalk::InitSensors() {
    CFootBotIndividualLevyWalk::InitSensors();
    m_pcRABS = GetSensor<CCI_RangeAndBearingSensor>("range_and_bearing");
}

/****************************************/
/****************************************/

void CFootBotCollectiveLevyWalk::Reset() {
    m_pcRABA->ClearData();
    //m_pcRABA->SetData(0, LAST_EXPLORATION_NONE);
}

/****************************************/
/****************************************/

void CFootBotCollectiveLevyWalk::ControlStep() {
   switch(m_sStateData.State) {
      case SStateData::STATE_INITIALIZE: {
         Initialize();
         LOG << m_pcRABA->GetSize() << std::endl;
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

REGISTER_CONTROLLER(CFootBotCollectiveLevyWalk, "footbot_clw_controller")