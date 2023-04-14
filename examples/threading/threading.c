#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// Optional: use these functions to add debug or error prints to your application
//#define DEBUG_LOG(msg,...)
#define DEBUG_LOG(msg,...) printf("threading: " msg "\n" , ##__VA_ARGS__)
#define ERROR_LOG(msg,...) printf("threading ERROR: " msg "\n" , ##__VA_ARGS__)

void* threadfunc(void* thread_param)
{
    
    // TODO: wait, obtain mutex, wait, release mutex as described by thread_data structure
    // hint: use a cast like the one below to obtain thread arguments from your parameter
    struct thread_data* thread_func_args = (struct thread_data *) thread_param;

    //wait to obtain mutex
    usleep(thread_func_args->wait_to_obtain_ms * 1000);
    //obtain mutex
    int rc = 0;
    rc = pthread_mutex_lock(thread_func_args->mutex);
    if(rc != 0)
    {
        ERROR_LOG("Could not lock thread. Error: %d", rc);
        thread_func_args->thread_complete_success = false;
        return thread_param;
    }
    //wait to release mutex
    usleep(thread_func_args->wait_to_release_ms * 1000);
    //release mutex
    rc = pthread_mutex_unlock(thread_func_args->mutex);
    if(rc != 0)
    {
        ERROR_LOG("Could not unlock thread. Error: %d", rc);
        thread_func_args->thread_complete_success = false;
        return thread_param;
    }
    //log when thread creation is successful by getting thread size
    pthread_attr_t thread_attrs;
    if ( sizeof(thread_attrs.__size) > 0)
    {
        DEBUG_LOG("Thread successfully created.");
        thread_func_args->thread_complete_success = true;
    } else {
        ERROR_LOG("Thread not created. Size is zero.");

        thread_func_args->thread_complete_success = false;
    }

    return thread_param;
}


bool start_thread_obtaining_mutex(pthread_t *thread, pthread_mutex_t *mutex,int wait_to_obtain_ms, int wait_to_release_ms)
{
    /**
     * TODO: allocate memory for thread_data, setup mutex and wait arguments, pass thread_data to created thread
     * using threadfunc() as entry point.
     *
     * return true if successful.
     *
     * See implementation details in threading.h file comment block
     */
    //allocation of thread data
    struct thread_data* thread_func_args = malloc(sizeof(struct thread_data));
    //init thread struct
    thread_func_args->mutex = mutex;
    thread_func_args->wait_to_obtain_ms = wait_to_obtain_ms;
    thread_func_args->wait_to_release_ms = wait_to_release_ms;
    thread_func_args->thread_complete_success = false;
    //slide 12 Synchronization
    int rc = pthread_create(thread, NULL, threadfunc, thread_func_args);
    if(rc == 0)
    {
        DEBUG_LOG("PThread creation Succeeded.");
        return true;
    } else {
        ERROR_LOG("PThread creation unsuccessful. Error: %d\n", rc);
        //always free allocated data
        // free(sizeof(struct thread_data));
        return false;
    }
}

