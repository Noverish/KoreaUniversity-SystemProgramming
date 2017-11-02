// ----- created by Inho (16. 10. 25) ----- //
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/syscalls.h>
#include <linux/fcntl.h>
#include <asm/uaccess.h>
#include <asm/segment.h>
#include <linux/buffer_head.h>

#define PROC_DIRNAME "myproc"
#define PROC_FILENAME "hw1"

static struct proc_dir_entry *proc_dir;
static struct proc_dir_entry *proc_file;

extern unsigned long long hw1_buffer[1024];
extern long long int hw1_time[1024];
extern int hw1_index;
extern const char* hw1_file_system_type[1024];

static int hw1_open(struct inode *inode, struct file *file) {
    printk(KERN_ALERT "<STATUS> hw1_open!\n");
    return 0;
}

static ssize_t hw1_write(struct file *file, const char __user *user_buffer, size_t count, loff_t *ppos) {
    int i, startindexpoint = hw1_index;
    struct file *filp;
    char tmp[19];

    printk("<hw1_index> : %d", hw1_index);

	//set kernel memory to access user area's file
    mm_segment_t old_fs = get_fs();
    set_fs(KERNEL_DS);

    filp = filp_open("/tmp/result.csv", O_WRONLY|O_CREAT, 0644);
    if(IS_ERR(filp)) {
        printk("file open error\n");
	    set_fs(old_fs);

	    return count;
    } else {
        printk("file open success\n");
    }

    printk(KERN_ALERT "<STATUS> hw1_write!\n");

    for(i = startindexpoint + 1; i != startindexpoint; i++) {
        i %= 2000;

        printk("<i> : %d", i);

        if(hw1_file_system_type[i] == NULL) {
            printk("<hw1_file_system_type[i]> : NULL");
            continue;
        }
        printk("<hw1_file_system_type[i]> : %s", hw1_file_system_type[i]);

        if(user_buffer == NULL) {
            printk("<user_buffer> : NULL");
            continue;
        }
        printk("<user_buffer> : %s", user_buffer);

        if(strncmp(hw1_file_system_type[i], user_buffer, count-1) != 0) {
            continue;
        }

        if(hw1_buffer[i] == NULL) {
            printk("<hw1_buffer[i]> : NULL");
            continue;
        }
        printk("<hw1_buffer[i]> : %lld", hw1_buffer[i]);

        printk(KERN_INFO "%d:[%lld] %lld\n", i, hw1_time[i], hw1_buffer[i]);

        snprintf(tmp, 19, "%lld", hw1_time[i]);
        vfs_write(filp, tmp, strlen(tmp), &filp->f_pos);
        vfs_write(filp, ", ", 2, &filp->f_pos);

        snprintf(tmp, 19, "%lld", hw1_buffer[i]);

        vfs_write(filp, tmp, strlen(tmp), &filp->f_pos);
        vfs_write(filp, ", ", 2, &filp->f_pos);

        printk("%s\n", hw1_file_system_type[i]);

        snprintf(tmp, 19, "%s", hw1_file_system_type[i]);
        vfs_write(filp, tmp, strlen(tmp), &filp->f_pos);
        vfs_write(filp, "\n", 1, &filp->f_pos);

        hw1_time[i]=0;
        hw1_buffer[i]=0;
        hw1_file_system_type[i]=NULL;
    }

    printk(KERN_ALERT "<STATUS> hw1_write complete.\n");

	//close file
    filp_close(filp, NULL);

    //restore kernel memory setting
    set_fs(old_fs);

    return count;
}

static const struct file_operations hw1_proc_fops = {
    .owner = THIS_MODULE,
    .open = hw1_open,
    .write = hw1_write,
};

static int __init hw1_init(void) {
    printk(KERN_ALERT "<STATUS> hw1 module up...\n");
    proc_dir = proc_mkdir(PROC_DIRNAME, NULL);
    proc_file = proc_create(PROC_FILENAME, 0600, proc_dir, &hw1_proc_fops);
    return 0;
}

static void __exit hw1_exit(void) {
    remove_proc_entry(PROC_FILENAME, NULL);
    printk(KERN_ALERT "<STATUS> hw1 module down...\n");
    return;
}

module_init(hw1_init);
module_exit(hw1_exit);

MODULE_AUTHOR("Kim Inho 2012130888");
MODULE_DESCRIPTION("System Programming - hw1");
MODULE_LICENSE("GPL");
MODULE_VERSION("NEW");
