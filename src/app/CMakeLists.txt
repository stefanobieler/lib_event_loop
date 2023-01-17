set(APP_NAME app)
set(APP_SRC app.c)

add_executable(${APP_NAME} ${APP_SRC})

target_include_directories(
    ${APP_NAME}
    PRIVATE
    ./)

target_link_libraries(
    ${APP_NAME}
    PRIVATE
    event_loop)

