/*
 * AUTHOR: Stef Van Havermaet <stefvanhavermaet@gmail.com>
 *
 * Random Walk loop functions for the ILW, CLW and ACRW controllers.
 *
 */

#ifndef ILW_LOOP_FUNCTIONS_H
#define ILW_LOOP_FUNCTIONS_H

#include "../../controllers/footbot_ilw/footbot_ilw.h"

#include <argos3/core/simulator/loop_functions.h>
#include <argos3/plugins/robots/foot-bot/simulator/footbot_entity.h>

using namespace argos;

class CRandomWalkLoopFunctions : public CLoopFunctions {

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
       bool SaveTrajectory;

       void Init(TConfigurationNode& t_node);
   };
   struct SArenaParams {
       Real ArenaUnit;
       CRange<Real> UnboundedBufferRangeX;
       CRange<Real> UnboundedBufferRangeY;
       CRange<Real> RepositionRangeX;
       CRange<Real> RepositionRangeY;
       CRange<Real> ContextRangeX;
       CRange<Real> ContextRangeY;
       CRange<Real> DistributionAreaRangeX;
       CRange<Real> DistributionAreaRangeY;

       explicit SArenaParams() : 
            ArenaUnit(0.17) {}
   };
   struct STargetFindingData {
       UInt32 SimulationTick;
       UInt32 RobotId;
       UInt32 TargetId;

       explicit STargetFindingData( UInt32 i_simulation_tick,
                                    UInt32 i_robot_id,
                                    UInt32 i_target_id) :
            SimulationTick(i_simulation_tick),
            RobotId(i_robot_id),
            TargetId(i_target_id) {}
   };
   struct SRobotData {
       CFootBotEntity* FootBotEntity;
       CFootBotIndividualLevyWalk* FootBotController;

       UInt32 TicksInWalkState;
       UInt32 TicksInRotateState;
       UInt32 TicksInCollisionAvoidanceState;

       bool WasWalkingPreviousTick;
       CVector2 MostRecentWalkStartPosition;
       CVector2 MostRecentWalkEndPosition;

       explicit SRobotData() :
          FootBotEntity(NULL),
          FootBotController(NULL),
          TicksInWalkState(0),
          TicksInRotateState(0),
          TicksInCollisionAvoidanceState(0),
          WasWalkingPreviousTick(false) {}
    
       explicit SRobotData(CFootBotEntity* c_foot_bot_entity,
                           CFootBotIndividualLevyWalk* c_foot_bot_controller):
          FootBotEntity(c_foot_bot_entity),
          FootBotController(c_foot_bot_controller),
          TicksInWalkState(0),
          TicksInRotateState(0),
          TicksInCollisionAvoidanceState(0),
          WasWalkingPreviousTick(false) {}
   };
   struct SSwarmData {
       std::vector<SRobotData> Robots;
       std::vector<STargetFindingData> TargetFindings;
       std::vector<Real> WalkDistances;

       void Reset();
   };
public:
   CRandomWalkLoopFunctions();
   virtual ~CRandomWalkLoopFunctions() {}

   virtual void Init(TConfigurationNode& t_node);
   virtual void Reset();
   virtual void Destroy();
   virtual void PreStep();
   virtual void PostStep();

   virtual CColor GetFloorColor(const CVector2& c_position_on_plane);

private:
   SSwarmData m_sSwarmData;

   SSwarmParams m_sSwarmParams;
   SForagingParams m_sForagingParams;
   SOutputParams m_sOutputParams;
   SArenaParams m_sArenaParams;

   CFloorEntity* m_pcFloor;
   CRandom::CRNG* m_pcRNG;

private:
   void InitParams(TConfigurationNode& t_node);
   void InitUtilities();

   void InitSwarm();
   void InitArenaParams();
   void DistributeSwarmInArena();

   void UpdateWalkDistances(CVector2 c_end_pos, CVector2 c_start_pos);

   void UpdateUnboundedArena(SRobotData* s_robot_data);
};


#endif