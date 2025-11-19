add_subdirectory(GLFW)
add_subdirectory(glm)
add_subdirectory(glad)
add_subdirectory(assimp)
add_subdirectory(stb)

get_target_list_from(assimp_targets "assimp")

add_library(imgui STATIC)
target_include_directories(imgui PUBLIC "imgui")

set(IMGUI_SOURCES 
  "imgui/imconfig.h"
  "imgui/imgui.h"
  "imgui/imgui.cpp"
  "imgui/imgui_draw.cpp"
  "imgui/imgui_tables.cpp"
  "imgui/imgui_internal.h"
  "imgui/imgui_widgets.cpp"
  "imgui/imstb_rectpack.h"
  "imgui/imstb_textedit.h"
  "imgui/imstb_truetype.h"
  "imgui/imgui_demo.cpp"
  "imgui/backends/imgui_impl_opengl3.h"
  "imgui/backends/imgui_impl_opengl3_loader.h"
  "imgui/backends/imgui_impl_opengl3.cpp"
  "imgui/backends/imgui_impl_glfw.h"
  "imgui/backends/imgui_impl_glfw.cpp"
  "imgui/imstb_rectpack.h"
  "imgui/imstb_textedit"
  "imgui/imstb_truetype")

target_sources(imgui PRIVATE ${IMGUI_SOURCES})
target_link_libraries(imgui PUBLIC glfw PRIVATE opengl32)

foreach(target IN LISTS assimp_targets)
  if(TARGET ${target})
      
    set_target_properties(${target} PROPERTIES 
      FOLDER "Assimp"
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>" )

  endif()
endforeach()



