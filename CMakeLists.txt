cmake_minimum_required(VERSION 3.10)

project(image)

add_executable(${PROJECT_NAME} main.c stb_image.h stb_image_resize2.h)

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)

target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${SDL2_LIBRARIES}
    pthread
    m
)

if(PSP)
    create_pbp_file(
        TARGET ${PROJECT_NAME}
        ICON_PATH NULL
        BACKGROUND_PATH NULL
        PREVIEW_PATH NULL
        TITLE ${PROJECT_NAME}
    )
endif()

file(COPY image.jpg DESTINATION ${CMAKE_BINARY_DIR})