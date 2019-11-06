#ifndef ILW_LOOP_FUNCTIONS_H
#define ILW_LOOP_FUNCTIONS_H

#include <controllers/footbot_ilw/footbot_ilw.h>

#include <argos3/core/simulator/loop_functions.h>
#include <argos3/plugins/robots/foot-bot/simulator/footbot_entity.h>

using namespace argos;

class CIndividualLevyWalkLoopFunctions : public CLoopFunctions {

public:
   struct SSwarmParams {
       std::string RobotControllerId;
       UInt32 Size;
       bool Nested;
       Real NestAreaBufferFactor;
       Real NestAreaBufferSqrtFactor;

       void Init(TConfigurationNode& t_node);
       void InitCalculatedParams();
   };
   struct SForagingParams {
       std::string TargetPositionsFileName;
       std::vector<CVector2> TargetPositions;
       Real TargetRadius;
       Real TargetSquareRadius;

       void Init(TConfigurationNode& t_node);
       void InitCalculatedParams();
       void InitFileInputParams();
   };
   struct SOutputParams {
       std::string FileName;

       void Init(TConfigurationNode& t_node);
   };
   struct SArenaParams {
       Real ArenaUnit;
       CRange<CVector3> DistributionAreaRange;
       CRange<Real> DistributionAreaRangeX;
       CRange<Real> DistributionAreaRangeY;

       explicit SArenaParams() : 
            ArenaUnit(0.17) {}
   };
   struct STargetFindingData {
       UInt32 SimulationTick;
       UInt32 TargetId;

       explicit STargetFindingData(UInt32 i_simulation_tick,
                                    UInt32 i_target_id) :
            SimulationTick(i_simulation_tick),
            TargetId(i_target_id) {}
   };
   typedef std::vector<STargetFindingData> TTargetFindingsData;
   struct SRobotData {
       CFootBotEntity* FootBotEntity;
       CFootBotIndividualLevyWalk* FootBotController;

       TTargetFindingsData TargetFindings;

       explicit SRobotData() :
          FootBotEntity(NULL),
          FootBotController(NULL) {}
    
       explicit SRobotData(CFootBotEntity* c_foot_bot_entity,
                           CFootBotIndividualLevyWalk* c_foot_bot_controller):
          FootBotEntity(c_foot_bot_entity),
          FootBotController(c_foot_bot_controller) {}
   };
   typedef std::vector<SRobotData> TSwarmData;
public:
   CIndividualLevyWalkLoopFunctions();
   virtual ~CIndividualLevyWalkLoopFunctions() {}

   virtual void Init(TConfigurationNode& t_node);
   virtual void Reset();
   virtual void Destroy();
   virtual void PreStep();
   virtual void PostStep();

   virtual CColor GetFloorColor(const CVector2& c_position_on_plane);

private:
   SSwarmParams m_sSwarmParams;
   SForagingParams m_sForagingParams;
   SOutputParams m_sOutputParams;
   SArenaParams m_sArenaParams;

   TSwarmData m_tSwarmData;

   CFloorEntity* m_pcFloor;
   CRandom::CRNG* m_pcRNG;

private:
   void InitParams(TConfigurationNode& t_node);
   void InitUtilities();

   void InitSwarm();
   SRobotData InitRobot(UInt32 id);
   void InitArenaParams();
   void DistributeSwarmInArena();
};


#endif