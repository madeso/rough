
set(svg_src
    svg/rectangle.cc

    svg/tests.h

    svg/main.cc
)

add_executable(svg ${svg_src})

target_link_libraries(svg
    PUBLIC rough
    PRIVATE project_options project_warnings
)
target_include_directories(svg
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/svg FILES ${svg_src})
