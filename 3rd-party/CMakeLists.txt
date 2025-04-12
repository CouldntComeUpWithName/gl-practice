add_subdirectory(GLFW)
add_subdirectory(glm)
add_subdirectory(glad)
add_subdirectory(assimp)
add_subdirectory(stb)

get_target_list_from(assimp_targets "assimp")

foreach(target IN LISTS assimp_targets)
  if(TARGET ${target})
      
    # set(folder_tree "Deps")
    # get_target_property(folder ${target} FOLDER)

    # if(folder)
    #   set(folder_tree "${folder_tree}/${folder}")
    # endif()

    set_target_properties(${target} PROPERTIES 
      FOLDER "Assimp"
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>" )

  endif()
endforeach()



