add_library(footbot_acrw SHARED footbot_acrw.h footbot_acrw.cpp)
target_link_libraries(footbot_acrw
  footbot_clw
  argos3core_simulator
  argos3plugin_simulator_footbot
  argos3plugin_simulator_genericrobot)
