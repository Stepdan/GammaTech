find_package(Qt6 REQUIRED)

# get absolute path to qmake, then use it to find windeployqt executable

get_target_property(_qmake_executable Qt6::qmake IMPORTED_LOCATION)
get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)

set(windeployqt_build_param "--release")
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(windeployqt_build_param "--debug")
endif()

function(windeployqt target)

    # POST_BUILD step
    # - after build, we have a bin/lib for analyzing qt dependencies
    # - we run windeployqt on target and deploy Qt libs

    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND "${_qt_bin_dir}/windeployqt6.exe"
                --verbose 1
                ${windeployqt_build_param}
                #--no-svg
                --no-opengl
                --no-opengl-sw
                --no-compiler-runtime
                --no-system-d3d-compiler
                --no-translations
                --qmldir ${QML_FILES_PATH}
                --qmlimport ${QML_IMPORT_PATH}
                \"$<TARGET_FILE:${target}>\"
        COMMENT "Deploying Qt libraries using windeployqt for compilation target '${target}' ..."
    )

endfunction()