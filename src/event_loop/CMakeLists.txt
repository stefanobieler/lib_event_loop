set(LIB_NAME event_loop)
set(LIB_SRC 
    event_loop.c
    version.c)

add_library(${LIB_NAME} STATIC ${LIB_SRC})

target_include_directories(
    ${LIB_NAME}
    PRIVATE
    ${CMAKE_BINARY_DIR}/linux/include/
    ./)

target_link_libraries(${LIB_NAME} PRIVATE pthread)
