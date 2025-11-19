add_executable(scene)

set(SOURCES src/main.cpp src/sphere_scene.h src/sphere_scene.cpp)
target_sources(scene PRIVATE ${SOURCES} )

target_link_libraries(scene PRIVATE core)

set_target_properties(scene PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}")

if(MSVC)
  set_property(TARGET scene PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>")
  
  source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})
endif()
