// ----- created by Inho (16. 10. 25) ----- //
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/syscalls.h>
#include <linux/fcntl.h>
#include <asm/uaccess.h>
#include <asm/segment.h>
#include <linux/buffer_head.h>

#define SP_BUFFER_SIZE 1000
#define PROC_DIRNAME "myproc"
#define PROC_FILENAME "myproc"

static struct proc_dir_entry *proc_dir;
static struct proc_dir_entry *proc_file;

extern unsigned long long sp_sector_arr[SP_BUFFER_SIZE];
extern unsigned long long sp_time_arr[SP_BUFFER_SIZE];
extern int sp_arr_index;

static int my_open(struct inode *inode, struct file *file) {
    printk(KERN_INFO "Simple Module Open!!\n");
    return 0;
}

static ssize_t my_write(struct file *file, const char __user *user_buffer, size_t count, loff_t *ppos) {
    int i, sp_arr_index_origin = sp_arr_index;
    struct file *filp;
    char tmp[17];

    mm_segment_t old_fs = get_fs();
    set_fs(KERNEL_DS);

    printk(KERN_INFO "Simple Module Write!!\n");

    filp = filp_open("/tmp/result.csv", O_WRONLY|O_CREAT, 0644);
    if(IS_ERR(filp)) {
	    set_fs(old_fs);
	    return count;
    }

    for(i = sp_arr_index_origin + 1; i != sp_arr_index_origin; i++) {
        i %= SP_BUFFER_SIZE;

        if(sp_time_arr[i] == 0)
            continue;

        printk("i : %d\n", i);

        snprintf(tmp, 17, "%llu", sp_time_arr[i]);
        vfs_write(filp, tmp, strlen(tmp), &filp->f_pos);
        vfs_write(filp, ",", 1, &filp->f_pos);
        printk("sp_time_arr[i] : %llu\n", sp_time_arr[i]);

        snprintf(tmp, 17, "%llu", sp_sector_arr[i]);
        vfs_write(filp, tmp, strlen(tmp), &filp->f_pos);
        vfs_write(filp, "\n", 1, &filp->f_pos);
        printk("sp_sector_arr[i] : %llu\n", sp_sector_arr[i]);

        sp_time_arr[i] = 0;
        sp_sector_arr[i] = 0;
    }

    filp_close(filp, NULL);
    set_fs(old_fs);

    return count;
}

static const struct file_operations myproc_fops = {
    .owner = THIS_MODULE,
    .open = my_open,
    .write = my_write,
};

static int __init simple_init(void) {
    printk(KERN_INFO "Simple Module Init!!\n");
    proc_dir = proc_mkdir(PROC_DIRNAME, NULL);
    proc_file = proc_create(PROC_FILENAME, 0600, proc_dir, &myproc_fops);
    return 0;
}

static void __exit simple_exit(void) {
    remove_proc_entry(PROC_FILENAME, NULL);
    remove_proc_entry(PROC_DIRNAME, NULL);
    printk(KERN_INFO "Simple Module Exit!!\n");
    return;
}

module_init(simple_init);
module_exit(simple_exit);

MODULE_AUTHOR("Korea University");
MODULE_DESCRIPTION("It's Simple!!");
MODULE_LICENSE("GPL");
MODULE_VERSION("NEW");
