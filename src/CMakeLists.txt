set(src
    geometry.cc geometry.h
    random.cc random.h
    core.cc core.h

    vector-utils.cc vector-utils.h
    double.cc double.h

    fillers/scan-line-hachure.cc fillers/scan-line-hachure.h
)

add_library(rough STATIC ${src})
target_link_libraries(rough
    PRIVATE project_options project_warnings
)
target_include_directories(rough
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
source_group("" FILES ${src})
