file (GLOB sources CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/lib/*.c")

file (GLOB headers CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/include/*.h")

add_library(lib STATIC ${sources} ${headers})

target_link_libraries(lib PRIVATE raylib)
target_include_directories(lib PUBLIC ${PROJECT_SOURCE_DIR}/include PRIVATE ${raygui_SOURCE_DIR}/src)
