project(gammatech_proc)

if(NOT DEFINED HEADERS)
    file(GLOB HEADERS *.hpp)
endif()

if(NOT DEFINED SOURCES)
    file(GLOB SOURCES *.cpp)
endif()

source_group("Headers" FILES ${HEADERS})
source_group("Sources" FILES ${SOURCES})

add_library(${PROJECT_NAME} STATIC)
add_library(gammatech::proc ALIAS ${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    PRIVATE
    ${SOURCES}
    PUBLIC
    FILE_SET headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} FILES "${HEADERS}"
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        flatbuffers::flatbuffers
        fb_generated
        gammatech::threading
        gammatech::utils
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
    GAMMATECH_MODULE_NAME="PROC"
    GIT_INFO="${GIT_INFO}"
)

# install(TARGETS ${PROJECT_NAME} EXPORT ${INSTALL_TARGET_NAME}
#     COMPONENT ${PROJECT_NAME}
#     FILE_SET headers DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_ALIAS}
#     INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
# )
