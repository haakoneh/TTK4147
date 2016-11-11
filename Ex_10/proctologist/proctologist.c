#include <linux/module.h>    // included for all kernel modules
#include <linux/kernel.h>    // included for KERN_INFO
#include <linux/init.h>      // included for __init and __exit macros
#include <linux/proc_fs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("BORG");
MODULE_DESCRIPTION("A Simple Hello World module");

struct proc_dir_entry *proctologist; 

int procfile_read(char *buffer, char **buffer_location, off_t offset, int buffer_length, int *eof, void *data);

static int __init hello_init(void)
{

    proctologist = create_proc_entry("myproc",0644,NULL);
    proctologist->read_proc = &procfile_read;
    return 0;    // Non-zero return means that the module couldn't be loaded.
}

static void __exit hello_cleanup(void)
{
    //printk(KERN_INFO "Cleaning up module.\n");
	remove_proc_entry("myproc",NULL);
}

int procfile_read(char *buffer, char **buffer_location, off_t offset, int buffer_length, int *eof, void *data){
	if (offset > 0){
		return 0;
	}else{
		return sprintf(buffer,"hello world\n");
	}
}

module_init(hello_init);
module_exit(hello_cleanup);
