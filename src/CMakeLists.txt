set(src
    geometry.cc geometry.h
    random.cc random.h
    core.cc core.h

    vector-utils.cc vector-utils.h
    double.cc double.h
    renderer.cc renderer.h
    generator.cc generator.h

    svg.cc svg.h

    fillers/filler-interface.cc fillers/filler-interface.h
    fillers/scan-line-hachure.cc fillers/scan-line-hachure.h
    fillers/hachure-filler.cc fillers/hachure-filler.h
    fillers/zigzag-filler.cc fillers/zigzag-filler.h
    fillers/dashed-filler.cc fillers/dashed-filler.h
    fillers/dot-filler.h fillers/dot-filler.cc
    fillers/hatch-filler.cc fillers/hatch-filler.h
    fillers/zigzag-line-filler.cc fillers/zigzag-line-filler.h
    fillers/filler.cc fillers/filler.h
)

add_library(rough STATIC ${src})
target_link_libraries(rough
    PRIVATE project_options project_warnings
)
target_include_directories(rough
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${src})
