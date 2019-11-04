#include "footbot_acrw.h"

/****************************************/
/****************************************/

CFootBotAdaptiveCollectiveRandomWalk::SAdaptationStateData::SAdaptationStateData() :
   NoTargetFoundConsecutivelySimulationTicks(0) {}

void CFootBotAdaptiveCollectiveRandomWalk::SAdaptationStateData::Reset() {
   NoTargetFoundConsecutivelySimulationTicks = 0;
}

/****************************************/
/****************************************/

CFootBotAdaptiveCollectiveRandomWalk::SAdaptationParams::SAdaptationParams() : 
   AdaptationIntervalSimulationTicks(50),
   ErfcTransitionInterval(2) {
       InitCalculatedParams();
   }

void CFootBotAdaptiveCollectiveRandomWalk::SAdaptationParams::Init(TConfigurationNode& t_node) {
   try {
      GetNodeAttribute(t_node, "adaptation_interval_ticks", AdaptationIntervalSimulationTicks);
      GetNodeAttribute(t_node, "erfc_transition_interval", ErfcTransitionInterval);
   }
   catch(CARGoSException& ex) {
      THROW_ARGOSEXCEPTION_NESTED("Error initializing controller adaptation parameters.", ex);
   }

   InitCalculatedParams();
}

void CFootBotAdaptiveCollectiveRandomWalk::SAdaptationParams::InitCalculatedParams() {
    ErfcTransitionFactor = ErfcTransitionInterval / (Real)AdaptationIntervalSimulationTicks;
}

/****************************************/
/****************************************/

CFootBotAdaptiveCollectiveRandomWalk::CFootBotAdaptiveCollectiveRandomWalk(): CFootBotCollectiveLevyWalk() {}

/****************************************/
/****************************************/

void CFootBotAdaptiveCollectiveRandomWalk::InitParams(TConfigurationNode& t_node) {
    CFootBotCollectiveLevyWalk::InitParams(t_node);
    m_sAdaptationParams.Init(GetNode(t_node,"adaptation"));
}

/****************************************/
/****************************************/

void CFootBotAdaptiveCollectiveRandomWalk::Reset() {
    CFootBotCollectiveLevyWalk::Reset();
}

/****************************************/
/****************************************/

void CFootBotAdaptiveCollectiveRandomWalk::UpdateStateFromExploration(bool b_target_found) {
    CFootBotIndividualLevyWalk::UpdateStateFromExploration(b_target_found);
    if (b_target_found) {
        // Switch to Brownian walk by Gaussian distributed step lengths
        m_sStateData.LevyAlphaExponent = 2.0;
    } else {
        // Switch to Levy walk by decreasing Levy alpha exponent to a minimal value: 1
        m_sAdaptationStateData.NoTargetFoundConsecutivelySimulationTicks++;
        m_sStateData.LevyAlphaExponent = Max(1.0,erfc(m_sAdaptationParams.ErfcTransitionFactor*(m_sAdaptationStateData.NoTargetFoundConsecutivelySimulationTicks - m_sAdaptationParams.AdaptationIntervalSimulationTicks)));
    }
}

/****************************************/
/****************************************/

REGISTER_CONTROLLER(CFootBotAdaptiveCollectiveRandomWalk, "footbot_acrw_controller")