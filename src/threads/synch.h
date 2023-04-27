#ifndef THREADS_SYNCH_H
#define THREADS_SYNCH_H

#include <list.h>
#include <stdbool.h>

/* A counting semaphore. */
struct semaphore
{
    unsigned value;             /* Current value. */
    struct list waiters;        /* List of waiting threads. */
};

void sema_init (struct semaphore *, unsigned value);
void sema_down (struct semaphore *);
bool sema_try_down (struct semaphore *);
void sema_up (struct semaphore *);
void sema_self_test (void);

/* Lock. */
struct lock
{
    struct thread *holder;      /* Thread holding lock (for debugging). */
    struct semaphore semaphore; /* Binary semaphore controlling access. */
    struct list_elem lock_position;
    int greatestPriorityInWaiters;
};


void lock_init (struct lock *);
void lock_acquire (struct lock *);
bool lock_try_acquire (struct lock *);
void lock_release (struct lock *);
bool lock_held_by_current_thread (const struct lock *);

/* Condition variable. */
struct condition
{
    struct list waiters;        /* List of waiting threads. */
};

void cond_init (struct condition *);
void cond_wait (struct condition *, struct lock *);
void cond_signal (struct condition *, struct lock *);
void cond_broadcast (struct condition *, struct lock *);

// Added Functions
void lock_update_greatest_priority_in_waiters(struct semaphore* sema);

bool compare_locks_priority(struct list_elem *first, struct list_elem *second, void *aux);
bool compare_max_priority(struct list_elem *elem1, struct list_elem *elem2, void *aux);
bool compare_semaphore_priority(struct list_elem *first, struct list_elem *second, void *aux);

updateNestedPriority(struct thread* t);

/*
 * Modified Functions
 *  1 -> sema_down
 *      - Related to Priority Scheduler
 *      - Notifies the holder thread when a semaphore's priority is changed
 *  2 -> sena_up
 *      - Related to Priority Scheduler
 *      - Pops out the thread with the greatest priority among the waiters
 *      - makes it acquire the semaphore.
 *  3 -> lock_acquire
 *      - Related to Priority Scheduler
 *      - Changes the lock_waiting of the thread
 *      - Changes the lock greatest priority
 *      - Insert the lock in the holder locks list
 *  4 -> lock_release
 *      - Related to Priority Scheduler
 *      - Notifies the thread that used to hold the lock of change
 *      - in priority, in case of priority donation that should no longer exist
 *      - Also resets the lock's greatest priority.
 */



/* Optimization barrier.
   The compiler will not reorder operations across an
   optimization barrier.  See "Optimization Barriers" in the
   reference guide for more information.*/
#define barrier() asm volatile ("" : : : "memory")

#endif /* threads/synch.h */