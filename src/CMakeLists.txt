set(src
    rough/geometry.cc rough/geometry.h
    rough/random.cc rough/random.h
    rough/core.cc rough/core.h

    rough/vector-utils.cc rough/vector-utils.h
    rough/double.cc rough/double.h
    rough/renderer.cc rough/renderer.h
    rough/generator.cc rough/generator.h

    rough/svg.cc rough/svg.h

    rough/fillers/filler-interface.cc rough/fillers/filler-interface.h
    rough/fillers/scan-line-hachure.cc rough/fillers/scan-line-hachure.h
    rough/fillers/hachure-filler.cc rough/fillers/hachure-filler.h
    rough/fillers/zigzag-filler.cc rough/fillers/zigzag-filler.h
    rough/fillers/dashed-filler.cc rough/fillers/dashed-filler.h
    rough/fillers/dot-filler.h rough/fillers/dot-filler.cc
    rough/fillers/hatch-filler.cc rough/fillers/hatch-filler.h
    rough/fillers/zigzag-line-filler.cc rough/fillers/zigzag-line-filler.h
    rough/fillers/filler.cc rough/fillers/filler.h
)

add_library(rough STATIC ${src})
target_link_libraries(rough
    PRIVATE project_options project_warnings
)
target_include_directories(rough
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${src})
