add_library(core STATIC)

file(GLOB_RECURSE SOURCES src/**.cpp src/**.cc src/**.cxx)
file(GLOB_RECURSE HEADERS include/**.h include/**.hpp include/**.inl include/**.hxx)

target_include_directories(core PUBLIC include)

target_sources(core PUBLIC ${HEADERS} PRIVATE ${SOURCES})

target_link_libraries(core PRIVATE opengl32 
                           PUBLIC glfw 
                           PUBLIC glm::glm 
                           PUBLIC glad 
                           PUBLIC assimp::assimp 
                           PUBLIC stb
                           PUBLIC imgui)

if(CMAKE_CONFIGURATION_TYPES)
  set_target_properties(core PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/$<CONFIG>"
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/$<CONFIG>")
else()
  set_target_properties(core PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}"
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}")
endif()

if(MSVC)
  source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})
endif(MSVC)