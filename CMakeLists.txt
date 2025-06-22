cmake_minimum_required(VERSION 3.11)

project(sdl3)

add_executable(${PROJECT_NAME} ./src/main.c)

include(FindPkgConfig)
pkg_search_module(SDL3 REQUIRED sdl3)

target_include_directories(${PROJECT_NAME} PRIVATE ${SDL3_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${SDL3_LIBRARIES}
)

if(PSP)
    create_pbp_file(
        TARGET ${PROJECT_NAME}
        ICON_PATH NULL
        BACKGROUND_PATH NULL
        PREVIEW_PATH NULL
        TITLE ${PROJECT_NAME}
        VERSION 01.00
    )
endif()

