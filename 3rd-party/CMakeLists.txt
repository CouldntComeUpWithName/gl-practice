# 'add_subdirectory' of your external libraries or git submodules

if(MSVC)
  get_scoped_target_list(scoped_targets)
  
  foreach(target IN LISTS scoped_targets)
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
endif()