add_library(glad STATIC)

set(SOURCES src/glad.c include/glad/glad.h include/KHR/khrplatform.h)
target_sources(glad PRIVATE ${SOURCES})

target_include_directories(glad PUBLIC include)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})
