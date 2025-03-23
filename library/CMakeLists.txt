add_library(library)

file(GLOB_RECURSE SOURCES src/**.cpp src/**.cc src/**.cxx)
file(GLOB_RECURSE HEADERS include/**.h include/**.hpp include/**.inl include/**.hxx)

target_include_directories(library PUBLIC include)

target_sources(library PRIVATE ${HEADERS} PRIVATE ${SOURCES})

if(CMAKE_CONFIGURATION_TYPES)
  set_target_properties(library PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/$<CONFIG>"
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/$<CONFIG>")
else()
  set_target_properties(library PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}"
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}")
endif()

if(MSVC)
  source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})
endif(MSVC)