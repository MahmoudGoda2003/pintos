#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  printf ("system call!\n");
  switch(number){
    case SYS_HALT:
        halt_wrapper(void);break;
    case SYS_EXIT:
        exit_wrapper(int status);break;
    case SYS_EXEC:
        exec_wrapper(const char *file);break;
    case SYS_WAIT:
        wait_wrapper(pid_t pid);break;
    case SYS_CREATE:
        create_wrapper(const char *file, unsigned initial_size);break;
    case SYS_REMOVE:
        remove_wrapper(const char *file);break;
    case SYS_OPEN:
        open_wrapper(const char *file);break;
    case SYS_FILESIZE:
        filesize_wrapper(int fd);break;
    case SYS_READ:
        read_wrapper(int fd, void *buffer, unsigned size);break;
    case SYS_WRITE:
        write_wrapper(int fd, const void *buffer, unsigned size);break;
    case SYS_SEEK:
        seek_wrapper(int fd, unsigned position);break;
    case SYS_TELL:
        tell_wrapper(int fd);break;
    case SYS_CLOSE:
        close_wrapper(int fd);break;
  }
  thread_exit ();
}

void halt_wrapper(){
    sys_halt();
}

void exit_wrapper(){
    int exitcode;
    memread_user(f->esp + 4, &exitcode, sizeof(exitcode));
    sys_exit(exitcode);
}

void sys_halt(){
    shutdown_power_off();
}

void sys_exit(int status) {
    printf("%s: exit(%d)\n", thread_current()->name, status);

    // The process exits.
    // wake up the parent process (if it was sleeping) using semaphore,
    // and pass the return code.
    struct process_control_block *pcb = thread_current()->pcb;
    if(pcb != NULL) {
        pcb->exitcode = status;
    }
    else {
        // pcb == NULL probably means that previously
        // page allocation has failed in process_execute()
    }

    thread_exit();
}
