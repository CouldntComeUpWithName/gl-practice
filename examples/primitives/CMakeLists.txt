add_executable(primitives)

set(SOURCES src/primitives.cpp)
target_sources(primitives PRIVATE ${SOURCES} )

target_link_libraries(primitives PRIVATE core)

set_target_properties(primitives PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}")

if(MSVC)
  set_property(TARGET primitives PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>")
  
  source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})
endif()
