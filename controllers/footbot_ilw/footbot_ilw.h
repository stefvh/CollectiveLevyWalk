/*
 * AUTHOR: Stef Van Havermaet <stefvanhavermaet@gmail.com>
 *
 * Individual Levy Walk (ILW) controller for the foot-bot.
 *
 */

 #ifndef FOOTBOT_ILW_H
 #define FOOTBOT_ILW_H

using namespace argos;

class CFootBotIndividualLevyWalk : public CCI_Controller {
public:
  struct SStateData {
     enum EState {
        STATE_INITIALIZE = 0,
        STATE_WALK,
        STATE_ROTATE,
        STATE_COLLISIONAVOIDANCE
     } State;

     SStateData();
     void Init(TConfigurationNode& t_node);
     void Reset();
   };
public:
  CFootBotIndividualLevyWalk();
  virtual ~CFootBotIndividualLevyWalk() {}

  virtual void Init(TConfigurationNode& t_node);
};

#endif
