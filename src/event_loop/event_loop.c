#include "event_loop/event_loop.h"
#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static task* event_loop_get_next_task(event_loop* ev_loop) {
    task* task;

    if (!ev_loop->task_head)
        return NULL;

    task = ev_loop->task_head;
    ev_loop->task_head = task->right;

    if (ev_loop->task_head)
        ev_loop->task_head->left = NULL;

    task->left = NULL;
    task->right = NULL;
    return task;
}

static void event_loop_add_task(event_loop* ev_loop, task* new_task) {
    task* last_task = NULL;

    last_task = ev_loop->task_head;

    while (last_task->right) {
        last_task = last_task->right;
    }

    if (last_task) {
        last_task->right = new_task;
        new_task->left = last_task;
    } else {
        ev_loop->task_head = new_task;
    }
}

static bool event_loop_has_task(const task* task_list) {
    return !(task_list->right == NULL && task_list->left == NULL);
}

static void* event_loop_thread(void* args) {

    task* current_task;
    event_loop* ev_loop = (event_loop*)args;

    while (1) {
        pthread_mutex_lock(&ev_loop->mutex);
        while((current_task = event_loop_get_next_task(ev_loop)) == NULL) {
            ev_loop->state = EV_LOOP_IDLE;
            ev_loop->current_task = NULL;
            pthread_cond_wait(&ev_loop->mutex_cv, &ev_loop->mutex);
        }
        pthread_mutex_unlock(&ev_loop->mutex);

        ev_loop->state = EV_LOOP_IDLE;
        ev_loop->current_task = current_task;
        current_task->event(current_task->args);
        free(current_task);
    }

    return NULL;
}

static void event_loop_schedule_task(event_loop* ev_loop, task* new_task) {
    pthread_mutex_lock(&ev_loop->mutex);

    event_loop_add_task(ev_loop, new_task);


    if(ev_loop->state == EV_LOOP_BUSY) {
        pthread_mutex_unlock(&ev_loop->mutex);
        return;
    }

    pthread_cond_signal(&ev_loop->mutex_cv);

    pthread_mutex_unlock(&ev_loop->mutex);

}

void event_loop_init(event_loop* ev_loop) {
    ev_loop->task_head = NULL;
    pthread_mutex_init(&ev_loop->mutex, NULL);
    pthread_cond_init(&ev_loop->mutex_cv, NULL);
    ev_loop->state = EV_LOOP_IDLE;
    ev_loop->current_task = NULL;
    ev_loop->thread = NULL;
}

void event_loop_run(event_loop* ev_loop) {
    pthread_attr_t thread_attributes;
    assert(ev_loop->thread == NULL);

    ev_loop->thread = (pthread_t*)calloc(1, sizeof(pthread_t));
    pthread_attr_init(&thread_attributes);
    pthread_attr_setdetachstate(&thread_attributes, PTHREAD_CREATE_DETACHED);

    pthread_create(ev_loop->thread, &thread_attributes, event_loop_thread,
                   (void*)ev_loop);
}

task* event_loop_create_task(event_loop *ev_loop, event_callback callback, void *args) {
    task* new_task = (task*) calloc(1, sizeof(task));
    new_task->event = callback;
    new_task->args = args;

    event_loop_schedule_task(ev_loop, new_task);
    return new_task;
}
