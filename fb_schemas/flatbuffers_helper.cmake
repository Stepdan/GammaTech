set(FLATBUFFERS_FLATC_EXECUTABLE ${CONAN_BIN_DIR}/flatc.exe)

set(FB_SCHEMAS "${CMAKE_CURRENT_LIST_DIR}/shapes.fbs")

list(APPEND FB_SCHEMAS "${CMAKE_CURRENT_LIST_DIR}/rect.fbs")
list(APPEND FB_SCHEMAS "${CMAKE_CURRENT_LIST_DIR}/triangle.fbs")
list(APPEND FB_SCHEMAS "${CMAKE_CURRENT_LIST_DIR}/ellipse.fbs")
list(APPEND FB_SCHEMAS "${CMAKE_CURRENT_LIST_DIR}/line.fbs")

set(FB_GENERATED_INCLUDE_DIR ${CMAKE_BINARY_DIR}/fb_generated)

execute_process(COMMAND ${FLATBUFFERS_FLATC_EXECUTABLE} -o ${FB_GENERATED_INCLUDE_DIR} -c ${FB_SCHEMAS})

add_library(fb_generated INTERFACE)

target_include_directories(fb_generated
    INTERFACE ${FB_GENERATED_INCLUDE_DIR})
