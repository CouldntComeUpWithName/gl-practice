add_executable(triangle)

set(SOURCES src/hello-triangle.cpp)
target_sources(triangle PRIVATE ${SOURCES} )

target_link_libraries(triangle PRIVATE core)

set_target_properties(triangle PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}")

if(MSVC)
  set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT triangle)
  set_property(TARGET triangle PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>")
  
  source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})
endif()
