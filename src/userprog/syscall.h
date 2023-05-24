#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

#define USER_VIR_ADDR_BOTTOM ((void *)0x08048000)

void syscall_init (void);



// void syscall_init (void);
// static void syscall_handler (struct intr_frame *);

// void halt(void);
// void exit (int status);
// int wait (int pid);
// int exec (const char *cmd_line);
// bool create (const char *file, unsigned initial_size);
// bool remove (const char *file);
// int open (const char *file);
// int filesize (int fd);
// int read (int fd, void *buffer, unsigned length);
// int write (int fd, const void *buffer, unsigned length);
// void seek (int fd, unsigned position);
// unsigned tell (int fd);
// void close (int fd);

// struct file* get_file_by_fd(int target_fd);
// void get_args (struct intr_frame *f, void **args, int num_of_args);
// void get_args (struct intr_frame *f, void **args, int num_of_args);
// void is_ptr_valid (void* vaddr);
// void *get_kernel_ptr (void *user_ptr);

#endif /* userprog/syscall.h */
