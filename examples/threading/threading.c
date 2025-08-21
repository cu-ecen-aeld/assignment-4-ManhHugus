#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// Optional: use these functions to add debug or error prints to your application
#define DEBUG_LOG(msg,...) printf("threading: " msg "\n" , ##__VA_ARGS__)
#define ERROR_LOG(msg,...) printf("threading ERROR: " msg "\n" , ##__VA_ARGS__)

static thread_data *p_first_thread; 
void* threadfunc(void* thread_param)
{
    // TODO: wait, obtain mutex, wait, release mutex as described by thread_data structure
    // hint: use a cast like the one below to obtain thread arguments from your parameter
    //struct thread_data* thread_func_args = (struct thread_data *) thread_param;
    thread_data * thread_func_args = (thread_data *)thread_param; 
    sleep(thread_func_args->time_for_obtaining_mutex_ms);
    int rc = pthread_mutex_lock(thread_func_args->&mutex);
    if (rc != 0) {
      ERROR_LOG("Obtaining mutex has failed: %d!\r\n", rc);
      return NULL; 
    }
    sleep(thread_func_args->time_for_releasing_mutex_ms); 
    rc = pthread_mutex_unlock(thread_func_args->&mutex);
    if (rc != 0) {
      ERROR_LOG("Releasing mutex has failed: %d!\r\n", rc); 
      return NULL; 
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
    int rc = 0;
	
    p_first_thread = (thread_data *)malloc(sizeof(thread_data)); 
    if (p_first_thread == NULL) {
      ERROR_LOG("Malloc has failed!\r\n");
      p_first_thread->thread_complete_success = false; 
      return false; 
    }

    p_first_thread->thread = *thread; 
    p_first_thread->mutex = *mutex; 
    p_first_thread->time_for_obtaining_mutex_ms = wait_to_obtain_ms; 
    p_first_thread->time_for_releasing_mutex_ms = wait_to_release_ms;
    p_first_thread->thread_complete_success = true;   

    rc = pthread_mutex_init(p_first_thread->&mutex, NULL);
    if (rc != 0) {
      ERROR_LOG("Mutex initialization has failed: %d!\r\n", rc);
      p_first_thread->thread_complete_success = false; 
      return false; 
    }

    rc = pthread_create(p_first_thread->&thread, NULL, threadfunc, NULL);
    if (rc != 0) {
      ERROR_LOG("Thread creation has failed: %d!\r\n", rc);
      p_first_thread->thread_complete_success = false;
      return false; 
    }

    return true;
}

