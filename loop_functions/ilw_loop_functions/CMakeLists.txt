link_directories(${CMAKE_BINARY_DIR}/controllers/footbot_ilw)
set(ilw_loop_functions ilw_loop_functions.cpp)

add_library(ilw_loop_functions MODULE ${ilw_loop_functions_SOURCES})
target_link_libraries(ilw_loop_functions
  footbot_ilw
  argos3core_simulator
  argos3plugin_simulator_dynamics2d
  argos3plugin_simulator_entities
  argos3plugin_simulator_footbot
  argos3plugin_simulator_genericrobot
  argos3plugin_simulator_media)

