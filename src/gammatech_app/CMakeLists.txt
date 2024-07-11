project(gammatech_app)

add_executable(gammatech_app main.cpp)
target_link_libraries(gammatech_app
    PUBLIC
    gammatech::application
    gammatech::ui
)

target_compile_definitions(gammatech_app
    PRIVATE
    GAMMATECH_MODULE_NAME="GAMMATECH_APP"
    PUBLIC
    BUILD_WITH_EASY_PROFILER
)

file(GLOB CONAN_BIN_FILES ${CONAN_BIN_DIR}/*.dll)
foreach(ConanBinFile IN LISTS CONAN_BIN_FILES)
    add_custom_command(
                TARGET gammatech_app POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different ${ConanBinFile} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE}
    )
endforeach()

if (WIN32)
    windeployqt(gammatech_app)
endif ()

install(TARGETS gammatech_app RUNTIME DESTINATION ${GAMMA_BIN_DIR})