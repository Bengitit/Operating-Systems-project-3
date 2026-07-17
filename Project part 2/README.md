# Code Logic and Changes

After observing the original code, I needed to make some changes to code to be able to perform the required functions as intended. There where some structures that needed no change, while some aspects of it worked as intended.
<p>
The first change I made was adding the following metadata:
<p>
```c
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Abass");
MODULE_DESCRIPTION("CSE 3502 Project 3 - Part 2 (Proc File System Read/Write Example)");
```
<p>
Similar to part 1, I needed these lines to be able to run the "make" command in the terminal. Next, I defined the proc file name using:
<p>
```c
#define PROC_NAME "myproc"
```
<p>
instead of hardcoding the file name throughout the program. This makes the code easier to maintain as the file name only needs to be changed in one location.
<p>
I added a global character pointer:
<p>
```c
static char *info;
```
<p>
which acts as a buffer to store the data written to the proc file.
<p>
The `read_proc()` function was the first function to alter so that it can return data to user space properly. It first determines the length of the stored string using `strlen(info)`. It then checks whether the file has already been read or if the user's buffer is too small. If those checks pass, `copy_to_user()` copies the contents of `info` into the user's buffer. The file offset is updated so the data is only returned once per read operation, and the function prints a message to the kernel log before returning the number of bytes read. `return count;` was change to `return len;` to better give the user a proper answer to the input.
<p>
The second function that was altered was the `write_proc()`. It was changed to ensure that the data being written does not exceed the maximum buffer size. The function then uses `copy_from_user()` to safely copy data from user space into the kernel buffer. A null terminator is added so the data becomes a valid C string, and the function logs how many bytes were written before returning the number of bytes written. Another important change was replacing:
```c
struct file_operations
```
 with
```c
static const struct proc_ops
``` 
 as well as changing the members from `read` and `write` to `.proc_read` and `.proc_write`. This modification was made to ensure that the newer Linux kernel versions use `proc_ops` instead of `file_operations` for proc file system entries. This allowed the module to compile correctly on my system.
<p>
In the `init_module()` function, I implemented the missing initialization code. Memory is allocated for the buffer using `vmalloc(MAX_LEN)`. If the allocation fails, the function returns `-ENOMEM`. The allocated memory is initialized to zero using `memset()`. The proc file is then created using `proc_create()`, which registers the file `/proc/myproc` and associates it with the read and write functions. If the proc entry cannot be created, the allocated memory is released using `vfree()` before returning an error.
<p>
Finally, I completed the `cleanup_module()` function. When the module is unloaded, `remove_proc_entry()` removes the proc file from the `/proc` directory, and `vfree()` releases the dynamically allocated memory. This ensures that no kernel memory is leaked after the module is removed.
