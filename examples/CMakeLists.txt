add_subdirectory(hello-triangle)
add_subdirectory(cube)
add_subdirectory(model-loading)

get_target_list_from(examples ${CMAKE_CURRENT_SOURCE_DIR})

foreach(example IN LISTS examples)
  set_target_properties(${example} PROPERTIES 
    FOLDER "Examples"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}" )
endforeach()