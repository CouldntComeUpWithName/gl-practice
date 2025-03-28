set(EXECUTABLE_NAME app)

add_executable(${EXECUTABLE_NAME})

set(SOURCES src/main.cpp)
file(GLOB_RECURSE RESOURCES res/*)

target_sources(${EXECUTABLE_NAME} PRIVATE ${SOURCES} PRIVATE ${RESOURCES})
target_link_libraries(${EXECUTABLE_NAME} PRIVATE opengl32 PRIVATE glfw PRIVATE glm::glm PRIVATE common PRIVATE glad)

if(CMAKE_CONFIGURATION_TYPES)
  set_target_properties(${EXECUTABLE_NAME} PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/$<CONFIG>"
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/$<CONFIG>")
else()
  set_target_properties(${EXECUTABLE_NAME} PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}"
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}")
endif()

if(MSVC)
  set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${EXECUTABLE_NAME})
  set_property(TARGET ${EXECUTABLE_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>")
  
  source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES} ${RESOURCES})
endif()

set_target_properties(${EXECUTABLE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>")


