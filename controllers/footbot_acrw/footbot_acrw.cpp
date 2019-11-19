#include "footbot_acrw.h"

#include <limits>

/****************************************/
/****************************************/

CFootBotAdaptiveCollectiveRandomWalk::SAdaptationStateData::SAdaptationStateData() :
   LevyAlphaExponent(1.0),
   NoTargetFoundConsecutivelySimulationTicks(50) {}

void CFootBotAdaptiveCollectiveRandomWalk::SAdaptationStateData::Reset() {
   LevyAlphaExponent = 1.0;
   NoTargetFoundConsecutivelySimulationTicks = 50;
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
    m_sAdaptationStateData.NoTargetFoundConsecutivelySimulationTicks = m_sAdaptationParams.AdaptationIntervalSimulationTicks;
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
        m_sStateData.RandomWalk = SStateData::BROWNIAN_WALK;
        m_sAdaptationStateData.LevyAlphaExponent = 2.0;
        m_sAdaptationStateData.NoTargetFoundConsecutivelySimulationTicks = 0;
    } else {
        m_sAdaptationStateData.NoTargetFoundConsecutivelySimulationTicks++;
        if (m_sStateData.RandomWalk != SStateData::PURE_LEVY_WALK) {
            // Switch to Levy walk by decreasing Levy alpha exponent to a minimal value: 1
            Real fX = m_sAdaptationParams.ErfcTransitionFactor*(m_sAdaptationStateData.NoTargetFoundConsecutivelySimulationTicks - m_sAdaptationParams.AdaptationIntervalSimulationTicks);
            if (fX >= std::numeric_limits<Real>::epsilon()) {
                m_sStateData.RandomWalk = SStateData::PURE_LEVY_WALK;
                m_sAdaptationStateData.LevyAlphaExponent = 1.0;
            } else {
                m_sStateData.RandomWalk = SStateData::ALPHA_LEVY_WALK;
                m_sAdaptationStateData.LevyAlphaExponent = Max(1.0, erfc(fX));
            }
        }
    }
}

/****************************************/
/****************************************/

Real CFootBotAdaptiveCollectiveRandomWalk::GenerateRandomStepLengthVariable() {
    if (m_sStateData.RandomWalk == SStateData::BROWNIAN_WALK) {
        return m_pcRNG->Gaussian(1.0, 0.0);
    } else {
        return GenerateRandomLevyAlphaDistributedVariable(m_sAdaptationStateData.LevyAlphaExponent);
    }
}

/****************************************/
/****************************************/

REGISTER_CONTROLLER(CFootBotAdaptiveCollectiveRandomWalk, "footbot_acrw_controller")