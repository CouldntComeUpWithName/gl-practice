add_executable(cube)

set(SOURCES src/cube.cpp)
target_sources(cube PRIVATE ${SOURCES})

target_link_libraries(cube PRIVATE core)

set_target_properties(cube PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}")

if(MSVC)
  set_property(TARGET cube PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>")
  
  source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})
endif()
