#include "footbot_clw.h"

#include <argos3/core/utility/configuration/argos_configuration.h>
#include <argos3/core/utility/logging/argos_log.h>

/****************************************/
/****************************************/

CFootBotCollectiveLevyWalk::SCollectiveLevyWalkParams::SCollectiveLevyWalkParams() {}

void CFootBotCollectiveLevyWalk::SCollectiveLevyWalkParams::Init(TConfigurationNode& t_node) {
   try {
      GetNodeAttribute(t_node, "short_long_step_threshold", ShortLongStepThreshold);
   }
   catch(CARGoSException& ex) {
      THROW_ARGOSEXCEPTION_NESTED("Error initializing controller collective levy walk parameters.", ex);
   }
}

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

void CFootBotCollectiveLevyWalk::InitParams(TConfigurationNode& t_node) {
    CFootBotIndividualLevyWalk::InitParams(t_node);
    m_sCollectiveLevyWalkParams.Init(GetNode(t_node,"collective_levy_walk"));
}

/****************************************/
/****************************************/

void CFootBotCollectiveLevyWalk::Reset() {
    m_pcRABA->ClearData();
    //m_pcRABA->SetData(0, LAST_EXPLORATION_NONE);
}

/****************************************/
/****************************************/

void CFootBotCollectiveLevyWalk::DoWalk() {
    if (m_sStateData.ToWalkSimulationTicks > 15) {
        // Long step
        CByteArray cByteArray;
        cByteArray << GetId();
        m_pcRABA->SetData(cByteArray);
    } else {
        // Short step
        const CCI_RangeAndBearingSensor::TReadings& tPackets = m_pcRABS->GetReadings();
        for(size_t i = 0; i < tPackets.size(); ++i) {
            //LOG << i << "->Data:" << tPackets[0].Data << std::endl;
        }
    }

    CFootBotIndividualLevyWalk::DoWalk();
}

/****************************************/
/****************************************/

REGISTER_CONTROLLER(CFootBotCollectiveLevyWalk, "footbot_clw_controller")