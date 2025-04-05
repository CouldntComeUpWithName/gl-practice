add_executable(model-loading)

set(SOURCES src/model-loading.cpp)
target_sources(model-loading PRIVATE ${SOURCES} )

target_link_libraries(model-loading PRIVATE core)

set_target_properties(model-loading PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}")

if(MSVC)
  set_property(TARGET model-loading PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>")
  
  source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})
endif()
