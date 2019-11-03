#include "footbot_acrw.h"

/****************************************/
/****************************************/

CFootBotAdaptiveCollectiveRandomWalk::SAdaptationStateData::SAdaptationStateData() :
   DecreasedLevyAlphaExponentSimulationTicks(0) {}

void CFootBotAdaptiveCollectiveRandomWalk::SAdaptationStateData::Reset() {
   DecreasedLevyAlphaExponentSimulationTicks = 0;
}

/****************************************/
/****************************************/

CFootBotAdaptiveCollectiveRandomWalk::SAdaptationParams::SAdaptationParams() : 
   ToDecreaseLevyAlphaExponentSimulationTicks(50),
   ErfcTransitionInterval(2) {
       InitCalculatedParams();
   }

void CFootBotAdaptiveCollectiveRandomWalk::SAdaptationParams::Init(TConfigurationNode& t_node) {
   try {
      GetNodeAttribute(t_node, "to_decrease_exponent_ticks", ToDecreaseLevyAlphaExponentSimulationTicks);
      GetNodeAttribute(t_node, "erfc_transition_interval", ErfcTransitionInterval);
   }
   catch(CARGoSException& ex) {
      THROW_ARGOSEXCEPTION_NESTED("Error initializing controller adaptation parameters.", ex);
   }

   InitCalculatedParams();
}

void CFootBotAdaptiveCollectiveRandomWalk::SAdaptationParams::InitCalculatedParams() {
    ErfcTransitionFactor = ErfcTransitionInterval / (Real)ToDecreaseLevyAlphaExponentSimulationTicks;
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

REGISTER_CONTROLLER(CFootBotAdaptiveCollectiveRandomWalk, "footbot_acrw_controller")