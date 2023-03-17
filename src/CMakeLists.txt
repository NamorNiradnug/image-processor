file(GLOB filters filters/*.cpp)
file(GLOB cmdline cmdline/*.cpp)

set(LIB_SRC
    bmploader.cpp
    image.cpp
    rgb.cpp
    ${cmdline}
    ${filters}
)

add_library(
    image_processor_lib OBJECT
    ${LIB_SRC}
)

add_executable(
    image_processor
    image_processor.cpp
)
target_link_libraries(image_processor image_processor_lib)

