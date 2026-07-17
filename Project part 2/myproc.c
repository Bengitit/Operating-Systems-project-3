#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#define PROC_NAME "myproc"
#define MAX_LEN 4096

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Abass");
MODULE_DESCRIPTION("CSE 3502 Project 3 - Part 2 (Proc File System Read/Write Example)");

static struct proc_dir_entry *proc_entry;
static char *info;


ssize_t read_proc(struct file *f, char __user *user_buf,
                  size_t count, loff_t *off)
{
    int len;

    /* output the content of info to user's buffer */
    len = strlen(info);

    if (*off > 0 || count < len)
        return 0;

    if (copy_to_user(user_buf, info, len))
        return -EFAULT;

    *off = len;

    printk(KERN_INFO "procfs_read: read %d bytes\n", len);

    return len;
}


ssize_t write_proc(struct file *f, const char __user *user_buf,
                   size_t count, loff_t *off)
{
    /* copy the written data from user space and save it in info */
    if (count > MAX_LEN - 1)
        count = MAX_LEN - 1;

    if (copy_from_user(info, user_buf, count))
        return -EFAULT;

    info[count] = '\0';

    printk(KERN_INFO "procfs_write: write %lu bytes\n", count);

    return count;
}

//operated better with my system
static const struct proc_ops proc_fops = {
    .proc_read = read_proc,
    .proc_write = write_proc,
};


int init_module(void)
{
    int ret = 0;

    /* allocate memory space for the proc entry */
    info = vmalloc(MAX_LEN);
    if (!info)
        return -ENOMEM;

    memset(info, 0, MAX_LEN);

    /* create the proc entry */
    proc_entry = proc_create(PROC_NAME, 0666, NULL, &proc_fops);
    if (!proc_entry) {
        vfree(info);
        return -ENOMEM;
    }

    printk(KERN_INFO "test_proc created.\n");

    return ret;
}


void cleanup_module(void)
{
    /* remove the proc entry and free info space */
    remove_proc_entry(PROC_NAME, NULL);

    if (info)
        vfree(info);

    printk(KERN_INFO "test_proc deleted.\n");
}
