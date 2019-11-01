add_library(footbot_ilw SHARED footbot_ilw.h footbot_ilw.cpp)
target_link_libraries(footbot_ilw
  argos3core_simulator
  argos3plugin_simulator_footbot
  argos3plugin_simulator_genericrobot)
