#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// Optional: use these functions to add debug or error prints to your application
#define DEBUG_LOG(msg,...) printf("threading: " msg "\n" , ##__VA_ARGS__)
#define ERROR_LOG(msg,...) printf("threading ERROR: " msg "\n" , ##__VA_ARGS__)

void* threadfunc(void* thread_param)
{
    // TODO: wait, obtain mutex, wait, release mutex as described by thread_data structure
    // hint: use a cast like the one below to obtain thread arguments from your parameter
    //struct thread_data* thread_func_args = (struct thread_data *) thread_param;
    struct thread_data * thread_func_args = (struct thread_data *)thread_param;
    thread_func_args->thread_complete_success = false;

    usleep(thread_func_args->time_for_obtaining_mutex_ms * 1000);
    int rc = pthread_mutex_lock(thread_func_args->mutex);
    if (rc != 0) {
      ERROR_LOG("Obtaining mutex has failed: %d!\r\n", rc);
      return thread_func_args; 
    }

    usleep(thread_func_args->time_for_releasing_mutex_ms * 1000); 
    rc = pthread_mutex_unlock(thread_func_args->mutex);
    if (rc != 0) {
      ERROR_LOG("Releasing mutex has failed: %d!\r\n", rc);
      return thread_func_args; 
    }

    thread_func_args->thread_complete_success = true; 
    return thread_func_args;
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
    int rc = 0;
    struct thread_data *p_thread_data = (struct thread_data *)malloc(sizeof(struct thread_data));
    if (p_thread_data == NULL) {
      ERROR_LOG("Malloc has failed!\r\n"); 
      return false; 
    }
 
    p_thread_data->mutex = mutex; 
    p_thread_data->time_for_obtaining_mutex_ms = wait_to_obtain_ms; 
    p_thread_data->time_for_releasing_mutex_ms = wait_to_release_ms;
    p_thread_data->thread_complete_success = false;   

    rc = pthread_create(&p_thread_data->thread, NULL, threadfunc,(void *)p_thread_data);
    if (rc != 0) {
      ERROR_LOG("Thread creation has failed: %d!\r\n", rc);
      free(p_thread_data);
      return false; 
    }
    
    *thread = p_thread_data->thread; 
    return true;
}

