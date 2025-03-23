
macro(get_all_scoped_targets_recursively targets dir)

  get_property(subdirectories DIRECTORY ${dir} PROPERTY SUBDIRECTORIES)
  foreach(subdir ${subdirectories})
    get_all_scoped_targets_recursively(${targets} ${subdir})
  endforeach()

  get_property(current_targets DIRECTORY ${dir} PROPERTY BUILDSYSTEM_TARGETS)
  
  list(APPEND ${targets} ${current_targets})

endmacro()

function(get_target_list_from list root)
  get_all_scoped_targets_recursively(list ${root})
  set(${list} PARENT_SCOPE)
endfunction()

function(get_scoped_target_list list_name)
  get_all_scoped_targets_recursively(list_name ${CMAKE_CURRENT_SOURCE_DIR})
  set(${list_name} PARENT_SCOPE)
endfunction()

