add_library(footbot_clw SHARED footbot_clw.h footbot_clw.cpp)
target_link_libraries(footbot_clw
  footbot_ilw
  argos3core_simulator
  argos3plugin_simulator_footbot
  argos3plugin_simulator_genericrobot)
