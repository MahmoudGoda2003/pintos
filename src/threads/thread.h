#ifndef THREADS_THREAD_H
#define THREADS_THREAD_H

#include <debug.h>
#include <list.h>
#include <stdint.h>
#include "fixedNumber.h"
#include "synch.h"

/* States in a thread's life cycle. */
enum thread_status
{
    THREAD_RUNNING,     /* Running thread. */
    THREAD_READY,       /* Not running but ready to run. */
    THREAD_BLOCKED,     /* Waiting for an event to trigger. */
    THREAD_DYING        /* About to be destroyed. */
};

/* Thread identifier type.
   You can redefine this to whatever type you like. */
typedef int tid_t;
#define TID_ERROR ((tid_t) -1)          /* Error value for tid_t. */

/* Thread priorities. */
#define PRI_MIN 0                       /* Lowest priority. */
#define PRI_DEFAULT 31                  /* Default priority. */
#define PRI_MAX 63                      /* Highest priority. */

/* A kernel thread or user process.
   Each thread structure is stored in its own 4 kB page.  The
   thread structure itself sits at the very bottom of the page
   (at offset 0).  The rest of the page is reserved for the
   thread's kernel stack, which grows downward from the top of
   the page (at offset 4 kB).  Here's an illustration:
        4 kB +---------------------------------+
             |          kernel stack           |
             |                |                |
             |                |                |
             |                V                |
             |         grows downward          |
             |                                 |
             |                                 |
             |                                 |
             |                                 |
             |                                 |
             |                                 |
             |                                 |
             |                                 |
             +---------------------------------+
             |              magic              |
             |                :                |
             |                :                |
             |               name              |
             |              status             |
        0 kB +---------------------------------+
   The upshot of this is twofold:
      1. First, `struct thread' must not be allowed to grow too
         big.  If it does, then there will not be enough room for
         the kernel stack.  Our base `struct thread' is only a
         few bytes in size.  It probably should stay well under 1
         kB.
      2. Second, kernel stacks must not be allowed to grow too
         large.  If a stack overflows, it will corrupt the thread
         state.  Thus, kernel functions should not allocate large
         structures or arrays as non-static local variables.  Use
         dynamic allocation with malloc() or palloc_get_page()
         instead.
   The first symptom of either of these problems will probably be
   an assertion failure in thread_current(), which checks that
   the `magic' member of the running thread's `struct thread' is
   set to THREAD_MAGIC.  Stack overflow will normally change this
   value, triggering the assertion. */
/* The `elem' member has a dual purpose.  It can be an element in
   the run queue (thread.c), or it can be an element in a
   semaphore wait list (synch.c).  It can be used these two ways
   only because they are mutually exclusive: only a thread in the
   ready state is on the run queue, whereas only a thread in the
   blocked state is on a semaphore wait list. */
struct thread
{
    /* Owned by thread.c. */
    tid_t tid;                          /* Thread identifier. */
    enum thread_status status;          /* Thread state. */
    char name[16];                      /* Name (for debugging purposes). */
    uint8_t *stack;                     /* Saved stack pointer. */
    int priority;                       /* Priority. */
    struct list_elem allelem;           /* List element for all threads list. */

    // Added by the students
    int64_t wakeTime;                   // tick need until wakeup*/
    // Shared between thread.c and synch.c.
    struct list_elem elem;              // List element. */
    struct list locks_list;             // list to hold locks which the thread is acquiring */
    int base_priority;                  // Original priority of the thread */
    struct lock *lock_waiting;          // Pointing to the lock that the thread waiting for
    int nice;                           // Nice value, related to the advanced scheduler
    fixedPoint recent_cpu;              // related to the advanced scheduler

    /* Phase 2 Related */
    struct list opened_files;
    struct list child_processes;
    struct thread* parent_thread;
    bool child_creation_success;
    int child_status;
    tid_t waiting_on;
    struct file* executable_file;
    struct semaphore* wait_child;
    struct semaphore* parent_child_sync;
    struct semaphore waitsync;
    int fd_last;
    int pid;
    struct list_elem child_elem;
    bool loaded;
    int exit_status;


#ifdef USERPROG
    /* Owned by userprog/process.c. */
    uint32_t *pagedir;                  /* Page directory. */
#endif

    /* Owned by thread.c. */
    unsigned magic;                     /* Detects stack overflow. */
};

/* If false (default), use round-robin scheduler.
   If true, use multi-level feedback queue scheduler.
   Controlled by kernel command-line option "-o mlfqs". */
extern bool thread_mlfqs;

void thread_init (void);
void thread_start (void);

void thread_tick (void);
void thread_print_stats (void);

typedef void thread_func (void *aux);
tid_t thread_create (const char *name, int priority, thread_func *, void *);

void thread_block (void);
void thread_unblock (struct thread *);

struct thread *thread_current (void);
tid_t thread_tid (void);
const char *thread_name (void);

void thread_exit (void) NO_RETURN;
void thread_yield (void);

/* Performs some operation on thread t, given auxiliary data AUX. */
typedef void thread_action_func (struct thread *t, void *aux);
void thread_foreach (thread_action_func *, void *);

int thread_get_priority (void);
void thread_set_priority (int);

int thread_get_nice (void);
void thread_set_nice (int);
int thread_get_recent_cpu (void);
int thread_get_load_avg (void);

/* Added Functions */
bool compare_ticks(struct list_elem *first, struct list_elem *second, void *aux);
bool compare_less_priority(struct list_elem *elem1, struct list_elem *elem2, void *aux);

void notifyChangeInLocksPriority(struct thread* t);

void calc_load_avg();
void calc_recent_cpu(struct thread *t, void* aux);
void recalculate_recent_cpu_all();
void calc_priority(struct thread *t);
void recalculate_priority_all();
void thread_set_nice (int nice UNUSED);
int thread_get_nice (void);
int thread_get_load_avg (void);
int thread_get_recent_cpu (void);



/* Modified Functions
 *  1 -> thread_create
 *      - Advanced Scheduler related
 *      - inheritance of nice and RCPU
 *  2 -> thread_unblock
 *      - Priority Scheduler related
 *      - For insertion in order in the ready list
 *  3 -> thread_yield
 *      - Priority Scheduler related
 *      - For insertion in order in the ready list
 *  4 -> thread_set_priority
 *      - Priority Scheduler related
 *  5 -> init_thread
 *      - Priority Scheduler related
 *      - To initialize some added attributes to the struct
 *      - such as the base_priority, locks_list, lock_waiting
 */



#endif /* threads/thread.h */