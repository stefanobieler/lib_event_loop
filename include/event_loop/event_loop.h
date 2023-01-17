#ifndef EVENT_LOOP_H
#define EVENT_LOOP_H

#include <pthread.h>

typedef void (*event_callback)(void*);

typedef enum { EV_LOOP_IDLE, EV_LOOP_BUSY } EV_LOOP_STATE;

typedef struct task_t {
    event_callback event;
    void* args;
    struct task_t* left;
    struct task_t* right;
} task;

typedef struct event_loop_t {
    task* task_head;
    pthread_mutex_t mutex;
    pthread_cond_t mutex_cv;
    EV_LOOP_STATE state;
    pthread_t* thread;
    task* current_task;
} event_loop;

void event_loop_init(event_loop* ev_loop);
void event_loop_run(event_loop* ev_loop);
task* event_loop_create_task(event_loop* ev_loop, event_callback callback, void* args);

#endif
