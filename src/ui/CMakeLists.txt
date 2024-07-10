project(gammatech_ui)

if(NOT DEFINED HEADERS)
    file(GLOB HEADERS_BASE *.hpp)
    set(HEADERS ${HEADERS_BASE})
endif()

if(NOT DEFINED SOURCES)
    file(GLOB SOURCES
        *.cpp
        *.ui
    )
endif()

source_group("Headers" FILES ${HEADERS})
source_group("Sources" FILES ${SOURCES})

add_library(${PROJECT_NAME} STATIC)
add_library(gammatech::ui ALIAS ${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    PRIVATE
    ${SOURCES}
    PUBLIC
    FILE_SET headers_base TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} FILES "${HEADERS_BASE}"
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        gammatech::log
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
    GAMMATECH_MODULE_NAME="UI"
    PUBLIC
    BUILD_WITH_EASY_PROFILER
)

# install(TARGETS ${PROJECT_NAME} EXPORT ${INSTALL_TARGET_NAME}
#     COMPONENT ${PROJECT_NAME}
#     FILE_SET headers_base DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_ALIAS}
#     INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
# )