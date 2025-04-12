add_library(stb STATIC)


set(stb_sources include/stb_image.h src/stb_image.cpp)

target_sources(stb PRIVATE ${stb_sources})

target_include_directories(stb PUBLIC include)

if(MSVC)
  source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${stb_sources})
endif(MSVC)