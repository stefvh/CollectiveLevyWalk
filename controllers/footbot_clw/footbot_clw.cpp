#include "footbot_clw.h"

#include <algorithm>

#include <argos3/core/utility/configuration/argos_configuration.h>
#include <argos3/core/utility/logging/argos_log.h>

/****************************************/
/****************************************/

CFootBotCollectiveLevyWalk::SCommunicationStateData::SCommunicationStateData() {}

void CFootBotCollectiveLevyWalk::SCommunicationStateData::Reset() {
    PreviouslyReceivedMessages.clear();
}

/****************************************/
/****************************************/

CFootBotCollectiveLevyWalk::SCommunicationParams::SCommunicationParams() {}

void CFootBotCollectiveLevyWalk::SCommunicationParams::Init(TConfigurationNode& t_node) {
   try {
      GetNodeAttribute(t_node, "communication_range", CommunicationRange);
   }
   catch(CARGoSException& ex) {
      THROW_ARGOSEXCEPTION_NESTED("Error initializing controller communication parameters.", ex);
   }
}

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
    m_sCommunicationParams.Init(GetNode(t_node,"communication"));
    m_sCollectiveLevyWalkParams.Init(GetNode(t_node,"collective_levy_walk"));
}

/****************************************/
/****************************************/

void CFootBotCollectiveLevyWalk::Reset() {
    CFootBotIndividualLevyWalk::Reset();
    m_pcRABA->ClearData();
}

/****************************************/
/****************************************/

void CFootBotCollectiveLevyWalk::DoWalk() {
    bool bRepulsiveRotation = false;

    if (m_sStateData.ToWalkSimulationTicks > m_sCollectiveLevyWalkParams.ShortLongStepThreshold) {
        // Long step
        CByteArray cByteArray;
        cByteArray << m_uint32Id;
        m_pcRABA->SetData(cByteArray);

        m_sCommunicationStateData.PreviouslyReceivedMessages.clear();
    } else {
        // Short step
        const CCI_RangeAndBearingSensor::TReadings& tPackets = m_pcRABS->GetReadings();
        TMessages tMessages;
        TSources tSources;
        for(size_t i = 0; i < tPackets.size(); ++i) {
            CCI_RangeAndBearingSensor::SPacket tPacket = tPackets[i];
            UInt32 uiMessage = tPacket.Data.PopFront<UInt32>();
            if (std::find(tMessages.begin(), tMessages.end(), uiMessage) == tMessages.end()) {
                tMessages.push_back(uiMessage);
                tSources.push_back(SMessageSource(tPacket.Range, tPacket.HorizontalBearing));
            }
        }

        std::sort(tMessages.begin(), tMessages.end());
        if(!(std::includes( m_sCommunicationStateData.PreviouslyReceivedMessages.begin(),
                            m_sCommunicationStateData.PreviouslyReceivedMessages.end(),
                            tMessages.begin(),
                            tMessages.end())))
        {
            InitRotateStateAsRepulsiveForce(tSources);
            Rotate();

            bRepulsiveRotation = true;
        }

        m_sCommunicationStateData.PreviouslyReceivedMessages = tMessages;
    }

    if (!bRepulsiveRotation) 
    {
        CFootBotIndividualLevyWalk::DoWalk();
    }
}

/****************************************/
/****************************************/

void CFootBotCollectiveLevyWalk::InitRotateStateAsRepulsiveForce(TSources t_sources) {
    CVector2 cAveragedRepulsiveForceVector;

    Real fInverseCommunicationRange = 1.0 / m_sCommunicationParams.CommunicationRange;
    for(size_t i = 0; i < t_sources.size(); ++i) {
        SMessageSource sSource = t_sources[i];
        CVector2 cPosition(sSource.Distance, sSource.Angle);

        Real fInverseLength = 1.0 / sSource.Distance;
        Real fCommonFactor = (fInverseLength - fInverseCommunicationRange) * (fInverseLength * fInverseLength * fInverseLength);

        cAveragedRepulsiveForceVector += -cPosition * fCommonFactor;
    }

    cAveragedRepulsiveForceVector /= t_sources.size();

    InitRotateStateFromAngle(cAveragedRepulsiveForceVector.Angle());
}

/****************************************/
/****************************************/

REGISTER_CONTROLLER(CFootBotCollectiveLevyWalk, "footbot_clw_controller")
