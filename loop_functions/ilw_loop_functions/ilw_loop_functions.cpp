#include "ilw_loop_functions.h"

/****************************************/
/****************************************/

void CIndividualLevyWalkLoopFunctions::SSwarmParams::Init(TConfigurationNode& t_node) {
    try {
        GetNodeAttribute(t_node, "robot_controller", RobotControllerId);
        GetNodeAttribute(t_node, "size", Size);
        GetNodeAttribute(t_node, "nested", Nested);
        GetNodeAttribute(t_node, "nest_area_buffer_factor", NestAreaBufferFactor);
    }
    catch(CARGoSException& ex) {
        THROW_ARGOSEXCEPTION_NESTED("Error parsing loop functions swarm parameters!", ex);
    }

    InitCalculatedParams();
}

void CIndividualLevyWalkLoopFunctions::SSwarmParams::InitCalculatedParams() {
    NestAreaBufferFactor += 1.0;
    NestAreaBufferSqrtFactor = Sqrt(NestAreaBufferFactor);
}

/****************************************/
/****************************************/

void CIndividualLevyWalkLoopFunctions::SForagingParams::Init(TConfigurationNode& t_node) {
    try {
        GetNodeAttribute(t_node, "target_positions_file", TargetPositionsFileName);
        GetNodeAttribute(t_node, "target_radius", TargetRadius);
    }
    catch(CARGoSException& ex) {
        THROW_ARGOSEXCEPTION_NESTED("Error parsing loop functions foraging parameters!", ex);
    }

    InitCalculatedParams();
}

void CIndividualLevyWalkLoopFunctions::SForagingParams::InitCalculatedParams() {
    TargetSquareRadius = TargetRadius * TargetRadius;
}

void CIndividualLevyWalkLoopFunctions::SForagingParams::InitFileInputParams() {
    //TODO read TargetPositions from file (if filename not empty/null!)
}

/****************************************/
/****************************************/

void CIndividualLevyWalkLoopFunctions::SOutputParams::Init(TConfigurationNode& t_node) {
    try {
        GetNodeAttribute(t_node, "file", FileName);
        GetNodeAttribute(t_node, "save_trajectory", SaveTrajectory);
    }
    catch(CARGoSException& ex) {
        THROW_ARGOSEXCEPTION_NESTED("Error parsing loop functions output parameters!", ex);
    }
}

/****************************************/
/****************************************/

CIndividualLevyWalkLoopFunctions::CIndividualLevyWalkLoopFunctions() :
   m_pcFloor(NULL),
   m_pcRNG(NULL) {}

/****************************************/
/****************************************/

void CIndividualLevyWalkLoopFunctions::Init(TConfigurationNode& t_node) {
    try {
        InitParams(t_node);
        InitUtilities();

        InitSwarm();
   }
   catch(CARGoSException& ex) {
      THROW_ARGOSEXCEPTION_NESTED("Error initializing loop functions!", ex);
   }
}

void CIndividualLevyWalkLoopFunctions::InitParams(TConfigurationNode& t_node) {
    try {
        m_sSwarmParams.Init(GetNode(t_node, "swarm"));
        m_sForagingParams.Init(GetNode(t_node, "foraging"));
        m_sOutputParams.Init(GetNode(t_node, "output"));
    }
    catch(CARGoSException& ex) {
        THROW_ARGOSEXCEPTION_NESTED("Error parsing loop functions parameters!", ex);
    }
}

void CIndividualLevyWalkLoopFunctions::InitUtilities() {
    try {
        m_pcFloor = &GetSpace().GetFloorEntity();
        m_pcRNG = CRandom::CreateRNG("argos");
    }
    catch(CARGoSException& ex) {
        THROW_ARGOSEXCEPTION_NESTED("Error initializing loop functions utilities!", ex);
    }
}

/****************************************/
/****************************************/

void CIndividualLevyWalkLoopFunctions::InitSwarm() {
    for(UInt32 i = 0; i < m_sSwarmParams.Size; i++) {
        std::string sStringId = std::to_string(i);
        CFootBotEntity* cFootBotEntity = new CFootBotEntity(sStringId, m_sSwarmParams.RobotControllerId);
        CFootBotIndividualLevyWalk* cFootBotController = &dynamic_cast<CFootBotIndividualLevyWalk&>(cFootBotEntity->GetControllableEntity().GetController());

        AddEntity(*cFootBotEntity);

        m_sSwarmData.Robots.push_back(SRobotData(cFootBotEntity, cFootBotController));
    }

    if (m_sSwarmParams.Size > 0) {
        InitArenaParams();
        DistributeSwarmInArena();
    }
}

void CIndividualLevyWalkLoopFunctions::InitArenaParams() {
    m_sArenaParams.ArenaUnit = (Real)m_sSwarmData.Robots[0].FootBotController->GetFootbotParams().BodyDiameter / 100.0;

    Real fUnboundedBuffer = 1.5 * m_sArenaParams.ArenaUnit;
    m_sArenaParams.UnboundedBufferRangeX.Set(
        GetSpace().GetArenaLimits().GetMax().GetX() - fUnboundedBuffer,
        GetSpace().GetArenaLimits().GetMax().GetX()
    );
    m_sArenaParams.UnboundedBufferRangeY.Set(
        GetSpace().GetArenaLimits().GetMax().GetY() - fUnboundedBuffer,
        GetSpace().GetArenaLimits().GetMax().GetY()
    );

    m_sArenaParams.RepositionRangeX.Set(
        m_sArenaParams.UnboundedBufferRangeX.GetMin() - fUnboundedBuffer,
        m_sArenaParams.UnboundedBufferRangeX.GetMin()
    );
    m_sArenaParams.RepositionRangeY.Set(
        m_sArenaParams.UnboundedBufferRangeY.GetMin() - fUnboundedBuffer,
        m_sArenaParams.UnboundedBufferRangeY.GetMin()
    );

    m_sArenaParams.ContextRangeX.Set(
        -m_sArenaParams.UnboundedBufferRangeX.GetMin(),
        m_sArenaParams.UnboundedBufferRangeX.GetMin()
    );
    m_sArenaParams.ContextRangeY.Set(
        -m_sArenaParams.UnboundedBufferRangeY.GetMin(),
        m_sArenaParams.UnboundedBufferRangeY.GetMin()
    );

    CRange<CVector3> cDistributionAreaRange;
    if(m_sSwarmParams.Nested) {
        Real fNestSize = m_sSwarmParams.NestAreaBufferSqrtFactor * m_sArenaParams.ArenaUnit * Sqrt((Real)m_sSwarmParams.Size);
        Real fNestHalveSize = fNestSize / 2.0;
        cDistributionAreaRange.Set(
            CVector3(-fNestHalveSize, -fNestHalveSize, 0.0),
            CVector3(fNestHalveSize, fNestHalveSize, 0.0)
        );
    } else {
        Real fBuffer = 3.0 * m_sArenaParams.ArenaUnit;
        CVector3 cBuffer(fBuffer, fBuffer, 0.0);
        cDistributionAreaRange.Set(
            GetSpace().GetArenaLimits().GetMin() + cBuffer,
            GetSpace().GetArenaLimits().GetMax() - cBuffer
        );
    }

    m_sArenaParams.DistributionAreaRangeX.Set(
        cDistributionAreaRange.GetMin().GetX(),
        cDistributionAreaRange.GetMax().GetX()
    );
    m_sArenaParams.DistributionAreaRangeY.Set(
        cDistributionAreaRange.GetMin().GetY(),
        cDistributionAreaRange.GetMax().GetY()
    );
}

void CIndividualLevyWalkLoopFunctions::DistributeSwarmInArena() {
    CVector3 cPosition;
    CQuaternion cQuaternion;
    for(UInt32 i = 0; i < m_sSwarmParams.Size; ++i) {
        do {
            cPosition.Set(
                m_pcRNG->Uniform(m_sArenaParams.DistributionAreaRangeX),
                m_pcRNG->Uniform(m_sArenaParams.DistributionAreaRangeY),
                0.0);
            cQuaternion.FromEulerAngles(
                CRadians(m_pcRNG->Uniform(CRadians::SIGNED_RANGE)),
                CRadians::ZERO,
                CRadians::ZERO
            );
        } while (!MoveEntity(
            m_sSwarmData.Robots[i].FootBotEntity->GetEmbodiedEntity(),
            cPosition,
            cQuaternion));
    }
}

/****************************************/
/****************************************/

CColor CIndividualLevyWalkLoopFunctions::GetFloorColor(const CVector2& c_position_on_plane) {
   for(UInt32 i = 0; i < m_sForagingParams.TargetPositions.size(); ++i) {
      if((c_position_on_plane - m_sForagingParams.TargetPositions[i]).SquareLength() < m_sForagingParams.TargetSquareRadius) {
         return CColor::BLACK;
      }
   }
   return CColor::WHITE;
}

/****************************************/
/****************************************/

void CIndividualLevyWalkLoopFunctions::Reset() {
    DistributeSwarmInArena();
}

/****************************************/
/****************************************/

void CIndividualLevyWalkLoopFunctions::Destroy() {}

/****************************************/
/****************************************/

void CIndividualLevyWalkLoopFunctions::PreStep() {
    for(UInt32 i = 0; i < m_sSwarmParams.Size; ++i) {
        SRobotData* sRobot = &m_sSwarmData.Robots[i];
        if (sRobot->FootBotController->IsWalking()) {
            ++sRobot->TicksInWalkState;
            if (!sRobot->WasWalkingPreviousTick) {
                sRobot->MostRecentWalkStartPosition.Set(
                    sRobot->FootBotEntity->GetEmbodiedEntity().GetOriginAnchor().Position.GetX(),
                    sRobot->FootBotEntity->GetEmbodiedEntity().GetOriginAnchor().Position.GetY()
                ); 
                sRobot->WasWalkingPreviousTick = true;
            }
            sRobot->MostRecentWalkEndPosition.Set(
                    sRobot->FootBotEntity->GetEmbodiedEntity().GetOriginAnchor().Position.GetX(),
                    sRobot->FootBotEntity->GetEmbodiedEntity().GetOriginAnchor().Position.GetY()
            ); 
        } else if (sRobot->FootBotController->IsRotating()) {
            ++sRobot->TicksInRotateState;
            if (sRobot->WasWalkingPreviousTick) {
                sRobot->MostRecentWalkEndPosition.Set(
                    sRobot->FootBotEntity->GetEmbodiedEntity().GetOriginAnchor().Position.GetX(),
                    sRobot->FootBotEntity->GetEmbodiedEntity().GetOriginAnchor().Position.GetY()
                ); 
                sRobot->WasWalkingPreviousTick = false;
                UpdateWalkDistances(sRobot->MostRecentWalkEndPosition, sRobot->MostRecentWalkStartPosition);
            }
        } else if (sRobot->FootBotController->IsAvoidingCollision()) {
            ++sRobot->TicksInCollisionAvoidanceState;
            if (sRobot->WasWalkingPreviousTick) {
                sRobot->WasWalkingPreviousTick = false;
                UpdateWalkDistances(sRobot->MostRecentWalkEndPosition, sRobot->MostRecentWalkStartPosition);
            }
        } else {}
    }
}

void CIndividualLevyWalkLoopFunctions::UpdateWalkDistances(CVector2 c_end_pos, CVector2 c_start_pos) {
    if (m_sOutputParams.SaveTrajectory) {
        m_sSwarmData.WalkDistances.push_back((c_end_pos - c_start_pos).Length());
    }
}

/****************************************/
/****************************************/

void CIndividualLevyWalkLoopFunctions::PostStep() {
    for(UInt32 i = 0; i < m_sSwarmParams.Size; ++i) {
        SRobotData* sRobot = &m_sSwarmData.Robots[i];
        if(sRobot->FootBotController->GetStateData().TargetFound) {
            CVector2 cRobotPosition(
                sRobot->FootBotEntity->GetEmbodiedEntity().GetOriginAnchor().Position.GetX(),
                sRobot->FootBotEntity->GetEmbodiedEntity().GetOriginAnchor().Position.GetY()
            );
            bool bTargetIdentified = false;
            for(UInt32 j = 0; j < m_sForagingParams.TargetPositions.size() && !bTargetIdentified; ++j) {
                if((cRobotPosition - m_sForagingParams.TargetPositions[j]).SquareLength() < m_sForagingParams.TargetSquareRadius) {
                    m_sSwarmData.TargetFindings.push_back(
                        STargetFindingData(GetSpace().GetSimulationClock(),i,j)
                    );
                    bTargetIdentified = true;
                }
            }
        }
        UpdateUnboundedArena(sRobot);
    }
}

void CIndividualLevyWalkLoopFunctions::UpdateUnboundedArena(SRobotData* s_robot_data) {
    CVector3 cRobotPosition(s_robot_data->FootBotEntity->GetEmbodiedEntity().GetOriginAnchor().Position);
    if (m_sArenaParams.UnboundedBufferRangeX.WithinMinBoundExcludedMaxBoundIncluded(Abs(cRobotPosition.GetX()))) {
        UInt32 iTrials = 0;
        CVector3 cNewPosition;
        do {
            if (iTrials < 1) {
                cNewPosition.Set(
                    -m_pcRNG->Uniform(m_sArenaParams.RepositionRangeX), 
                    cRobotPosition.GetY(), 
                    cRobotPosition.GetZ()
                );
            } else {
                cNewPosition.Set(
                    -m_pcRNG->Uniform(m_sArenaParams.RepositionRangeX), 
                    m_pcRNG->Uniform(m_sArenaParams.ContextRangeY), 
                    cRobotPosition.GetZ()
                );
            }
        } while (
            !MoveEntity(
                s_robot_data->FootBotEntity->GetEmbodiedEntity(),
                cNewPosition,
                s_robot_data->FootBotEntity->GetEmbodiedEntity().GetOriginAnchor().Orientation
            ) && ++iTrials < 100);
    } else if (m_sArenaParams.UnboundedBufferRangeY.WithinMinBoundExcludedMaxBoundIncluded(Abs(cRobotPosition.GetY()))) {
        UInt32 iTrials = 0;
        CVector3 cNewPosition;
        do {
            if (iTrials < 1) {
                cNewPosition.Set(
                    cRobotPosition.GetX(), 
                    -m_pcRNG->Uniform(m_sArenaParams.RepositionRangeY), 
                    cRobotPosition.GetZ()
                );
            } else {
                cNewPosition.Set(
                    m_pcRNG->Uniform(m_sArenaParams.ContextRangeX), 
                    -m_pcRNG->Uniform(m_sArenaParams.RepositionRangeY), 
                    cRobotPosition.GetZ()
                );
            }
        } while (
            !MoveEntity(
                s_robot_data->FootBotEntity->GetEmbodiedEntity(),
                cNewPosition,
                s_robot_data->FootBotEntity->GetEmbodiedEntity().GetOriginAnchor().Orientation
            ) && ++iTrials < 100);
    } else {}
}

/****************************************/
/****************************************/

REGISTER_LOOP_FUNCTIONS(CIndividualLevyWalkLoopFunctions, "ilw_loop_functions")