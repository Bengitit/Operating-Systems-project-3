
# Code Logic and Changes Logic

# project-4-3-1.c
The original program creates a Linux kernel module that uses the "/proc" file system and memory mapping ("mmap") to allow a user-space program to access a page of kernel memory. The provided code already created the "/proc/mydir/myinfo" entry and allocated one page of kernel memory, but the "my_map()" function, which performs the memory mapping, was left incomplete.

The first change I made was adding the following header files:

"""c
#include <asm/io.h>
#include <linux/io.h>
#include <linux/string.h>
"""

These headers provide the functions needed to work with physical memory addresses and to use "memcpy()" for copying data into the kernel buffer.

Next, I updated the proc file operations from:

"""c
struct file_operations
"""

to:

"""c
static const struct proc_ops
"""

and changed the function pointer from ".mmap" to ".proc_mmap". This change was required because newer Linux kernel versions use the "proc_ops" structure for proc file operations instead of "file_operations". Without this change, the module would not compile correctly on my system.

The largest change was completing the "my_map()" function. First, I copied the contents of the predefined array into the allocated kernel buffer using:

"""c
memcpy(buffer, array, sizeof(array));
"""

This stores the values from "array" in the memory that will later be shared with user space.

Next, I converted the kernel virtual address of the buffer into a physical page frame number using:

"""c
pfn = virt_to_phys(buffer) >> PAGE_SHIFT;
"""

The "virt_to_phys()" function converts the virtual address into a physical address, and shifting by "PAGE_SHIFT" converts that physical address into the page frame number required by the kernel.

After obtaining the page frame number, I mapped the kernel memory into the user process's virtual address space using "remap_pfn_range()". This function associates the user-space virtual memory area ("vma") with the physical page that contains the kernel buffer. If the mapping fails, the function returns "-EAGAIN"; otherwise, it returns "0" to indicate success.

Another change I made was modifying the "proc_create()" call from:

"""c
proc_create("myinfo", 0, tempdir, &myproc_fops);
"""

to

"""c
proc_create("myinfo", 0666, tempdir, &myproc_fops);
"""

The permission value "0666" allows both read and write access to the proc entry for all users. This made the proc file easier to access during testing.

The remaining functions were already implemented. The "allocate_memory()" function allocates one page of kernel memory using "kmalloc()" and marks that page as reserved so it can safely be mapped into user space. The "clear_memory()" function reverses this process by clearing the reserved flag and freeing the allocated memory with "kfree()".

Finally, the "init_myproc_module()" function creates the "/proc/mydir" directory and the "/proc/mydir/myinfo" file before allocating the kernel memory. When the module is unloaded, the "exit_myproc_module()" function frees the allocated memory and removes both proc entries, ensuring that all resources are properly cleaned up.
