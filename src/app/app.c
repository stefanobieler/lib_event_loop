#include "event_loop/event_loop.h"
#include "version.h"
#include <stdio.h>

event_loop ev_loop;

int main(int argc, char** argv) {
    printf("%s v%s\n", get_project_name(), get_project_version());

    event_loop_init(&ev_loop);
    event_loop_run(&ev_loop);

    scanf("\n");
    return 0;
}
