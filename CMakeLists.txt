cmake_minimum_required(VERSION 3.14..3.24)

if(XCODE)
  message(FATAL_ERROR "XCODE isn't supported yet")
endif()

project( MyProject 
  VERSION 0.0.0
  DESCRIPTION "template-project"
  LANGUAGES CXX C )

set(CMAKE_CXX_STANDARD 20) # not the best practice, but who cares
set(CMAKE_CXX_STANDARD_REQUIRED OFF)

include(CTest)
include(FetchContent)

include(cmake/target.cmake)

add_subdirectory(app)
add_subdirectory(library)

add_subdirectory(3rd-party)
add_subdirectory(tools)
add_subdirectory(tests)

if(MSVC) 
  add_compile_options(/MP /W4)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

if(MSVC OR XCODE)

  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
  set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake")
  
  get_target_list_from(third_party_targets "3rd-party")
  get_target_list_from(test_targets "tests")
  get_target_list_from(tools_targets "tools")
  
  foreach(target IN LISTS third_party_targets)
    if(TARGET ${target})
      
      set(folder_tree "Deps")
      get_target_property(folder ${target} FOLDER)

      if(folder)
        set(folder_tree "${folder_tree}/${folder}")
      endif()
      
      set_target_properties(${target} PROPERTIES 
        FOLDER "${folder_tree}"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>" )

    endif()
  endforeach()

  foreach(target IN LISTS test_targets)
    if(TARGET ${target})
      
      set(folder_tree "Tests")
      get_target_property(folder ${target} FOLDER)

      if(folder)
        set(folder_tree "${folder_tree}/${folder}")
      endif()
      
      set_target_properties(${target} PROPERTIES 
        FOLDER "${folder_tree}"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/tests/" )

    endif()
  endforeach()

  foreach(target IN LISTS tools_targets)
    if(TARGET ${target})
      
      set(folder_tree "Tools")
      get_target_property(folder ${target} FOLDER)

      if(folder)
        set(folder_tree "${folder_tree}/${folder}")
      endif()
      
      set_target_properties(${target} PROPERTIES 
        FOLDER "${folder_tree}"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/tools/$<CONFIG>")

    endif()
  endforeach()
endif()