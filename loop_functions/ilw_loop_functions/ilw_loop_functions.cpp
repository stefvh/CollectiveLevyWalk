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
        m_tSwarmData.push_back(InitRobot(i));
    }

    if (m_sSwarmParams.Size > 0) {
        InitArenaParams();
        DistributeSwarmInArena();
    }
}

CIndividualLevyWalkLoopFunctions::SRobotData CIndividualLevyWalkLoopFunctions::InitRobot(UInt32 id) {
    std::string sStringId = std::to_string(id);
    CFootBotEntity* cFootBotEntity = new CFootBotEntity(sStringId, m_sSwarmParams.RobotControllerId);
    CFootBotIndividualLevyWalk* cFootBotController = &dynamic_cast<CFootBotIndividualLevyWalk&>(cFootBotEntity->GetControllableEntity().GetController());

    AddEntity(*cFootBotEntity);

    return SRobotData(cFootBotEntity, cFootBotController);
}

void CIndividualLevyWalkLoopFunctions::InitArenaParams() {
    m_sArenaParams.ArenaUnit = (Real)m_tSwarmData[0].FootBotController->GetFootbotParams().BodyDiameter / 100.0;

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
            GetSpace().GetArenaLimits().GetMax() - cBuffer);
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
    for(UInt32 i = 0; i < m_sSwarmParams.Size; i++) {
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
            m_tSwarmData[i].FootBotEntity->GetEmbodiedEntity(),
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

void CIndividualLevyWalkLoopFunctions::PreStep() {}

/****************************************/
/****************************************/

void CIndividualLevyWalkLoopFunctions::PostStep() {
    for(UInt32 i = 0; i < m_sSwarmParams.Size; i++) {
        if(m_tSwarmData[i].FootBotController->GetStateData().TargetFound) {
            //TODO
        }
    }
}

/****************************************/
/****************************************/

REGISTER_LOOP_FUNCTIONS(CIndividualLevyWalkLoopFunctions, "ilw_loop_functions")