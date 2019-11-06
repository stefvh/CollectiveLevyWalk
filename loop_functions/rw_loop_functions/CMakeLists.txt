link_directories(${CMAKE_BINARY_DIR}/controllers/footbot_ilw)

add_library(rw_loop_functions SHARED rw_loop_functions.h rw_loop_functions.cpp)

target_link_libraries(rw_loop_functions
  footbot_ilw
  argos3core_simulator
  argos3plugin_simulator_dynamics2d
  argos3plugin_simulator_entities
  argos3plugin_simulator_footbot
  argos3plugin_simulator_genericrobot
  argos3plugin_simulator_media)

