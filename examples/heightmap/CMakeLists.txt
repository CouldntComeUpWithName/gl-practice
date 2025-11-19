add_executable(heightmap)

set(SOURCES PRIVATE src/main.cpp 
  src/terrain_demo_scene.hpp 
  src/terrain_demo_scene.cpp
  src/terrain_generator.hpp
  src/terrain_generator.cpp)

file(GLOB_RECURSE RESOURCES res/**)

target_sources(heightmap PRIVATE ${SOURCES} PRIVATE ${RESOURCES})

target_link_libraries(heightmap PRIVATE core)

set_target_properties(heightmap PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}")

if(MSVC)
  set_property(TARGET heightmap PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>")
  
  source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})
  source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${RESOURCES})
endif()
