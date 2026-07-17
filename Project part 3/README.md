# Code Logic and Changes Logic
# Project-4-3-1.c Code Logic and Changes
The first change I made to the code was the following:
<p>
"#include <asm/io.h>"
<p>
"#include <linux/io.h>"
<p>
"#include <linux/string.h>"
<p>
These headers provide the functions needed to work with physical memory addresses and to use "memcpy()" for copying data into the kernel buffer.
Next, I updated the proc file operations "struct file_operations" to "static const struct proc_ops" as well as changed the function pointer from ".mmap" to ".proc_mmap". This change was made because newer Linux kernel versions use the "proc_ops" structure for proc file operations instead of "file_operations". I was not getting the results I needed with them. 
<p>
The hardest change was completing the "my_map()" function. First, I copied the contents of the predefined array into the allocated kernel buffer using "memcpy(buffer, array, sizeof(array));" to esnure that it stores the values from "array" in the memory that will later be shared with user space. Next, I converted the kernel virtual address of the buffer into a physical page frame number using "pfn = virt_to_phys(buffer) >> PAGE_SHIFT;"
<p>
Moving onto the "virt_to_phys()" function to convert the virtual address into a physical address, and shifting by "PAGE_SHIFT" converts that physical address into the page frame number required by the kernel.
<p>
After obtaining the page frame number, I mapped the kernel memory into the user process's virtual address space using "remap_pfn_range()". This function associates the user-space virtual memory area ("vma") with the physical page that contains the kernel buffer. If the mapping fails, the function returns "-EAGAIN"; otherwise, it returns "0" to indicate success.
<p>
Another important change that was needed was chaning "proc_create("myinfo", 0, tempdir, &myproc_fops);" to "proc_create("myinfo", 0666, tempdir, &myproc_fops);"
This was made as it allowed the permission value "0666" to both read and write access to the proc entry for all users. This made the proc file easier to access during testing.
<p>
The remaining functions were already implemented. The "allocate_memory()" function allocates one page of kernel memory using "kmalloc()" and marks that page as reserved so it can safely be mapped into user space. The "clear_memory()" function reverses this process by clearing the reserved flag and freeing the allocated memory with "kfree()".
<p>
Finally, the "init_myproc_module()" function creates the "/proc/mydir" directory and the "/proc/mydir/myinfo" file before allocating the kernel memory. When the module is unloaded, the "exit_myproc_module()" function frees the allocated memory and removes both proc entries, ensuring that all resources are properly cleaned up.

# Project-4-3-2.c Code Logic and Changes
The first changes were made to the "main()", giving it less attributes, simplifying it. After opening the proc file, I changed the success to "printf("open successfully by Abass\n");". I was little concerned about it having it only print my name. I was reasrching and reviwing my notes to try to make it display the user's name. After a while I decided to just alter the line as that was a easier to implement.
<p>
Next I needed to address the process's virtual address space. The "PROT_READ | PROT_WRITE" flags allow the mapped memory to be both read from and written to, while "MAP_SHARED" allows changes to be shared between the user program and the kernel. If the mapping fails, the program prints an error message using "perror()", closes the file, and exits. Once the mapping is successful, the program reads the contents of the mapped memory. The original template only contained a comment indicating where this code should be written. I implemented a "for loop" so it can read the first 12 bytes from the mapped memory and prints each value on its own line. At first it was printing each number on one line. I altered the code so that it will indent similar to the example. These values correspond to the contents of the array that was copied into the kernel buffer by the kernel module. I also left the original output code in the file as a commented block. That version printed all twelve values on a single line separated by spaces. I chose to print each value on a separate line because it made the output easier to read during testing. 
<p>
Last I printed the success message and finished the cleanup process by calling the "munmap(p_map, PAGE_SIZE);" to unmap the memory from the process's address space, before closing the proc file.
<p>
For both files I kept some if not all the comments of what the Original code as well as the old functions to allow the user to obersve and see what exactly I did line by line for themsleves. I posted Screenshots of myself running the code as well as the files created after I ran the "make" command.
