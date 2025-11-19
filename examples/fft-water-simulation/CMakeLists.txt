add_executable(fft-water)

set(SOURCES PRIVATE "src/fft-water-sim.cpp")

file(GLOB_RECURSE RESOURCES res/**)

target_sources(fft-water PRIVATE ${SOURCES} PRIVATE ${RESOURCES})

target_link_libraries(fft-water PRIVATE core)

set_target_properties(fft-water PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}")

if(MSVC)
  set_property(TARGET fft-water PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>")
  
  source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})
  source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${RESOURCES})
endif()