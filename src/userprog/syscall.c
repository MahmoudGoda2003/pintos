#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "devices/shutdown.h"
#include "userprog/process.h"
#include "userprog/pagedir.h"
#include "filesys/filesys.h"
#include "filesys/file.h"
#include "../lib/kernel/stdio.h"
#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/malloc.h"
#include "threads/synch.h"
#include "threads/vaddr.h"
#include "userprog/pagedir.h"
#include "userprog/process.h"
#include <user/syscall.h>
#include "devices/input.h"
#include "devices/shutdown.h"
#include "filesys/file.h"
#include "filesys/filesys.h"

#define SYS_HALT 0
#define SYS_EXIT 1
#define SYS_EXEC 2
#define SYS_WAIT 3
#define SYS_CREATE 4
#define SYS_REMOVE 5
#define SYS_OPEN 6
#define SYS_FILESIZE 7
#define SYS_READ 8
#define SYS_WRITE 9
#define SYS_SEEK 10
#define SYS_TELL 11
#define SYS_CLOSE 12

#define STD_INPUT 0
#define STD_OUTPUT 1
#define ERROR -1

struct file * get_file_by_fd(int target_fd);
void* get_kernel_ptr (void* user_ptr);
int write (int fd, const void *buffer, unsigned length);
int syscall_write (int filedes, const void * buffer, unsigned byte_size);
struct lock file_system_lock;
static void syscall_handler (struct intr_frame *);

struct file* get_file(int filedes);
struct file* get_file (int filedes);

struct process_file {
    struct file *file;
    int fd;
    struct list_elem elem;
};

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
  lock_init(&file_system_lock);
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  void *args[3];
  void* stackPointer = f->esp;
  int sysCallNumber = *((int*) stackPointer);
  stackPointer += sizeof(int);
  // printf("######################################33\n");
  // printf ("system call!\n");
  // printf("%d\n",sysCallNumber);
  
  switch(sysCallNumber){
    // HALT
		case SYS_HALT:
			halt();
			break;
  	
		// // TERMINATE PROCESS
		// case SYS_EXIT:
		// 	get_args(f, &args[0], 1); 
		// 	exit(*((int*)args[0]));
		// 	break;
  	
		//START ANOTHER PROCESS
		// case SYS_EXEC:
		// 	get_args(f, &args[0], 1);
		// 	/*Transform arg from user vaddr. to kernel vaddr*/
		// 	args[0] = get_kernel_ptr (args[0]);
		// 	f->eax = exec((char *)args[0]);
		// 	break;

		// case SYS_CREATE: // bool create (const char *file, unsigned initial_size);
		// 	get_args(f, &args[0], 2);
		// 	args[0] = get_kernel_ptr (args[0]);
		// 	f->eax = create ((const char *)args[0], *((int*)args[1]));
		// 	break;

		// case SYS_REMOVE: // bool remove (const char *file);
		// 	get_args(f, &args[0], 1);
		// 	args[0] = get_kernel_ptr (args[0]);
		// 	f->eax = remove ((const char *)args[0]);
		// 	break;

		// case SYS_OPEN: // int open (const char *file);
		// 	get_args(f, &args[0], 1);
		// 	args[0] = get_kernel_ptr (args[0]);
		// 	f->eax = open ((const char *)args[0]);
		// 	break;

		// case SYS_FILESIZE: // int filesize (int fd);
		// 	get_args(f, &args[0], 1);
		// 	f->eax = filesize (*((int*)args[0]));
		// 	break;

		// case SYS_READ: // int read (int fd, void *buffer, unsigned length);
		// 	get_args(f, &args[0], 3);
		// 	args[1] = get_kernel_ptr (args[1]);
		// 	f->eax = read (*((int*)args[0]), args[1], *((unsigned*)args[2]));
		// 	break;

		case SYS_WRITE: // int write (int fd, const void *buffer, unsigned length);
			// printf("form sys 9\n");
      get_args(f, &args[0], 3);
			args[1] = get_kernel_ptr (args[1]);
      // printf("the fd is %d\n",*((int*)args[0]));
      printf("/////////////////////////////////");
      printf("first %d, third %d ",((int*)args[0]),((unsigned*)args[2]));
      printf("/////////////////////////////////");
			f->eax = syscall_write(*((int*)args[0]), args[1], *((unsigned*)args[2]));
      // printf("after execution\n");
			break;

		// case SYS_SEEK: // void seek (int fd, unsigned position);
		// 	get_args(f, &args[0], 2);
		// 	seek (*((int*)args[0]), *((unsigned*)args[1]));
		// 	break;

		// case SYS_TELL: // unsigned tell (int fd);
		// 	get_args(f, &args[0], 1);
		// 	f->eax = tell (*((int*)args[0]));
		// 	break;

		// case SYS_CLOSE: // void close (int fd);
		// 	get_args(f, &args[0], 1);
		// 	close (*((int*)args[0]));
		// 	break;
		
		// default:
		// 	exit(-1);
		// 	break;

  }
}

////////////////////implementation of Sys calls/////////////////////////////

void halt(void){
	shutdown_power_off();
}

int write (int fd, const void *buffer, unsigned length){
  if(fd == 1){
    putbuf(buffer, length);
    return length;
  }
	struct file* file_ptr = get_file_by_fd(fd);
	if ((file_ptr != NULL) && (buffer != NULL))
	{
		return file_write(file_ptr, buffer, (off_t)length);
	}
	return -1;
}

// int sys_write(int fd, const void *buffer, unsigned size)
// {
//   if (fd == 1)
//   { // fd is 1, writes to the console
//     lock_acquire(&required_lock);
//     putbuf(buffer, size);
//     lock_release(&required_lock);
//     return size;
//   }

//   struct open_file *file = sys_file(fd);
//   if (file == NULL)
//   { // fail
//     return -1;
//   }
//   else
//   {
//     int ans = 0;
//     lock_acquire(&required_lock);
//     ans = file_write(file->file, buffer, size);
//     lock_release(&required_lock);
//     return ans;
//   }
// }

int syscall_write (int filedes, const void * buffer, unsigned byte_size)
{
  printf("fd is %d\n",filedes);
    if (byte_size <= 0)
    {
      return byte_size;
    }
    if (filedes == STD_OUTPUT)
    {
      putbuf (buffer, byte_size); // from stdio.h
      return byte_size;
    }
    
    // start writing to file
    lock_acquire(&file_system_lock);
    struct file *file_ptr = get_file(filedes);
    if (!file_ptr)
    {
      lock_release(&file_system_lock);
      return ERROR;
    }
    int bytes_written = file_write(file_ptr, buffer, byte_size); // file.h
    lock_release (&file_system_lock);
    return bytes_written;
}
////////////////helper functions/////////////////////////////

struct file*
get_file (int filedes)
{
  struct thread *t = thread_current();
  struct list_elem* next;
  struct list_elem* e = list_begin(&t->opened_files);
  
  for (; e != list_end(&t->opened_files); e = next)
  {
    next = list_next(e);
    struct process_file *process_file_ptr = list_entry(e, struct process_file, elem);
    if (filedes == process_file_ptr->fd)
    {
      return process_file_ptr->file;
    }
  }
  return NULL; // nothing found
}

void 
is_ptr_valid (void* vaddr)
{
	if(vaddr < USER_VIR_ADDR_BOTTOM || !is_user_vaddr(vaddr))
	{
		thread_exit();
		// virtual memory address is not reserved for the user
		// TO BE ADDED LATER : system call exit
	}
}


void get_args (struct intr_frame *f, void **args, int num_of_args){
	void *ptr;
	int i;
	
	for (i = 0; i < num_of_args; i++)
	{
		// +1 ignores the first element in stack (the one used for switching between syscalls)
		ptr = (void *)((f->esp) + i + 1); 
		is_ptr_valid (ptr);
		args[i] = ptr; 
	}
}

 /*convert user ptr to kernel ptr*/
void* get_kernel_ptr (void* user_ptr)
{
	is_ptr_valid(user_ptr);
	
	/*userptr -> kernelptr*/
	void *kernel_ptr = pagedir_get_page(thread_current()->pagedir, user_ptr);
	
	// Ensure kernel is not NULL
	// if(kernel_ptr == NULL)
	// 	exit(-1);
	return kernel_ptr;
}

struct file * 
get_file_by_fd(int target_fd)
{
	struct thread *t = thread_current();
	struct open_file *of = NULL;
	/*loop over all files opened by this process and search for the fd*/
	for(struct list_elem *e = list_begin(&(t->opened_files)); 
	e != &((t->opened_files).tail);
	e = e->next)
	{
		of = list_entry(e, struct open_file, open_files_elem);
		if((of->fd) == target_fd)
		{
			return (of->file_ptr);
		}
	}
	return NULL;
}

//////////////////////////////////////////



// int 
// exec (const char *cmd_line)
// {
// 	struct thread* parent = thread_current();
//  	(parent->child_creation_success) = false;
//  	if(cmd_line == NULL)
// 	{
// 		return -2; // cannot run
// 	}
//  	/*create new process*/
//  	int child_tid = process_execute(cmd_line);
// 	if (child_tid != TID_ERROR)
// 	{
// 		(parent->child_creation_success) = true;
// 	}
//  	return child_tid;
// }

/* is_ptr_valid: checks the validity of the pointer */


// /**
//  * exit: Terminates the current userprog
//  * @status: 0 in case success
//  *         otherwise in case errors
//  */
// void 
// exit (int status)
// {
// 	struct thread *t = thread_current();
//  	(t->exit_status) = status;
// 	//closing opened files
// 	if(&(t->open_files_list) != NULL)
// 	{
// 		struct open_file *of = NULL;
// 		for (struct list_elem *e = &((t->open_files_list).head.next); e != &((t->open_files_list).tail); e = (e->next))
//   		{
// 			of=list_entry(e,struct open_file,open_files_elem);
// 			close(of->fd);
//   		}
// 	}
// 	//check if there are any child processes running and wait on them
// 	if(&(t->children_list) != NULL)
// 	{
// 		struct thread *cp;
// 		for (struct list_elem *e = &((t->children_list).head.next); e != &((t->children_list).tail); e = (e->next))
//   		{
// 			cp = list_entry(e,struct thread, child_elem);
// 			wait(cp->pid);
//   		}
// 	}
// 	//check if the child process is blocking the parent 
// 	if((t->parent_thread->waiting_on) == t->pid)
// 	{
// 		sema_up(&t->sem_wait_on_child);
// 	}
	
// 	file_allow_write(t->executable_file);
//  	thread_exit();
// }
 
//  /* wait */
// int 
// wait (int pid)
// {
// 	struct thread *parent = thread_current(); /* current process */
// 	struct thread *child = NULL;              /* Child process */

// 	/* Ensure there is child process needed to wait for */
// 	if (list_empty(&parent->children_list))
// 	{
// 		return -2;
// 	}
    

// 	/* Search parent list of children for child_tid */
// 	child = get_child(parent, pid);

// 	/* If child not found, return -1 */
// 	if (child == NULL)
// 	{
// 		return -1;
// 	}
	
// 	/* Remove child for which we are waiting from the list*/
// 	list_remove(&child->child_elem);

// 	/* Make parent wait till child finishes executing */
// 	(parent->waiting_on) = (child->pid);
// 	sema_down(&(child->sem_wait_on_child));

// 	/* Return exit status of child when terminated*/
// 	return (child->exit_status);
// }
 
// void 
// close (int fd)
// {
// 	struct open_file *of = NULL; 
// 	(of->file_ptr) = get_file_by_fd(fd);
// 	if ((of->file_ptr) != NULL)
// 	{
// 		file_close(of->file_ptr);
// 		lock_release(&file_lock);
// 	}
// }

// int
// open (const char *file_name)
// {
// 	struct thread *t = thread_current();
// 	struct open_file *of = NULL; 
	
// 	lock_acquire(&file_lock);
	
// 	of->file_ptr = filesys_open (file_name);
	
// 	if ((of->file_ptr) != NULL)
//     {
// 		(t->fd_last)++;
// 		(of->fd) = (t->fd_last);
// 		list_push_back (&(t->open_files_list), &(of->open_files_elem));
//     }
// 	else
// 	{
// 		lock_release(&file_lock); 
// 	}
// 	return (of->fd);
// }

// int
// filesize (int fd)
// {
// 	struct open_file *of = NULL; 
// 	int file_size = -1;
	
// 	lock_acquire(&file_lock);
	
// 	(of->file_ptr) = get_file_by_fd(fd);
	
// 	if ((of->file_ptr) != NULL)
// 	{
// 		file_size = file_length (of->file_ptr);
// 		lock_release(&file_lock);
// 	}
// 	return file_size;
// }

// bool
// create (const char *file_name, unsigned size)
// {
// 	bool is_file_creation_successful;
// 	if (file_name == NULL)
// 	{
// 		is_file_creation_successful = false;
// 	}
// 	else 
// 	{
// 		lock_acquire(&file_lock);
		
// 		is_file_creation_successful = filesys_create(file_name, size); 
		
// 		lock_release(&file_lock);
// 	}
	
// 	return is_file_creation_successful;
// }

// bool 
// remove (const char *file_name)
// {
// 	bool is_file_removal_successful;
	
// 	if (file_name == NULL)
// 	{
// 		is_file_removal_successful = false;
// 	}
// 	else
// 	{
// 		lock_acquire(&file_lock);
		
// 		is_file_removal_successful = filesys_remove(file_name);
		
// 		lock_release(&file_lock);
// 	}  
	
// 	return is_file_removal_successful;
// }

// void 
// seek (int fd, unsigned position)
// {
// 	struct open_file *of = NULL; 
	
// 	lock_acquire(&file_lock);
	
// 	(of->file_ptr) = get_file_by_fd(fd);
	
// 	if ((of->file_ptr) != NULL)
// 	{
// 		file_seek(of->file_ptr, position);
// 	}
	
// 	lock_release(&file_lock);
// }

// unsigned 
// tell (int fd)
// {
// 	struct open_file *of = NULL; 
// 	unsigned position_in_file = -1;
  
// 	lock_acquire(&file_lock);
  
// 	(of->file_ptr) = get_file_by_fd(fd);
  
// 	if ((of->file_ptr) != NULL)
// 	{
// 		position_in_file = file_tell(of->file_ptr); 
// 	}
	
// 	lock_release(&file_lock);
	
// 	return position_in_file;
// }

// int 
// read (int fd, void *buffer, unsigned length)
// {
// 	struct file* file_ptr = get_file_by_fd(fd);
	
// 	if ((file_ptr != NULL) && (buffer != NULL))
// 	{
// 		return file_read(file_ptr, buffer, (off_t)length);
// 	}
	
// 	return -1;
// }


// /***/