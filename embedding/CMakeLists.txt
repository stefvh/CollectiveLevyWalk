add_executable(main main.cpp)

target_link_libraries(main
    footbot_ilw
    rw_loop_functions
    argos3core_simulator)

add_executable(seeds seeds.cpp)