#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/unistd.h>
#include <linux/syscalls.h>
#include <linux/sched.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Aviral");

int ret = 0;
module_param(ret, int, 0);

static int __init KernelModuleInit(void)
{
    struct task_struct *task;
    task = pid_task(find_vpid(ret), PIDTYPE_PID);

    printk(KERN_INFO "The pid is: %d\n", t->pid);
    printk(KERN_INFO "the user_id is: %d\n", t->cred->uid.val);
    printk(KERN_INFO "The process group id is: %d\n", t->group_leader->pid);
    printk(KERN_INFO "The process name is  : %s\n", t->comm);

    return 0;
}

static void __exit KernelModuleExit(void)
{
    printk(KERN_INFO "Exiting!!!! \n");
}

module_init(KernelModuleInit);
module_exit(KernelModuleExit);
