/**************************************************************
 * Class:  CSC-415-03 Fall 2021
 * Name: Sedric Conneally
 * Student ID: 921030432
 * GitHub UserID: sedricconneally
 * Project: Assignment 6 – Device Driver
 *
 * File: conneally_sedric_HW6_module.c
 *
 * Description: This file defines the kernel side routines
 * that our test program will then call. I used code from the
 * BOPV youtube video (mentioned in the writeup) to aid in
 * creating my ioctl function as well as the other functions in
 * this file.
 *
 **************************************************************/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>

// picking a major number that I know is out of the way of all the others
#define MAJOR_NUM 333

// holds user input for ioctl
int userMode = 0;
// tracks the number of times the Device Driver has been opened
int numOpens = 0;
// holds the phrase sent in by the user
char input[50];

// opens the Device Driver
int myDriver_open(struct inode *pinode, struct file *pfile)
{
    numOpens++;
    printk(KERN_INFO "Device has been opened %d times\n", numOpens);
    return 0;
}

// copies the user input to the user space buffer
ssize_t myDriver_read(struct file *pfile, char __user *buffer, size_t length, loff_t *offset)
{
    // stores the reversed input
    char output[50];
    // used to increment the output position
    int outputI = 0;

    printk(KERN_INFO "Reading...\n");

    // keep the input the same and copy it to the user space buffer
    if (userMode == 0)
    {
        copy_to_user(buffer, input, length);
    }
    // reverse the input and copy the result to the userspace buffer
    else
    {
        long i;
        for (i = strlen(input) - 1; i >= -1; i--)
        {
            output[outputI] = input[i];
            outputI++;
        }

        copy_to_user(buffer, output, length);
    }
    printk(KERN_INFO "Read: %s Reversed value is: %s\n", buffer, output);
    return 0;
}

// writes the user space buffer to the input local variable
ssize_t myDriver_write(struct file *pfile, const char __user *buffer, size_t length, loff_t *offset)
{
    printk(KERN_INFO "Writing...\n");
    copy_from_user(input, buffer, length);
    printk(KERN_INFO "Wrote: %s\n", input);
    return length;
}

// closes the Device Driver
int myDriver_close(struct inode *pinode, struct file *pfile)
{
    printk(KERN_INFO "Releasing...\n");
    return 0;
}

// Changes the mode between forward and reversed text
long myDriver_ioctl(struct file *pfile, unsigned int ioctlVal, unsigned long p)
{
    switch (ioctlVal)
    {
    case 0:
        userMode = 0;
        break;

    case 1:
        userMode = 1;
        break;

    default:
        break;
    }
    printk(KERN_INFO "userMode set to %d", userMode);

    return 0;
}

// used to link my kernel routines to the system calls
struct file_operations myDriver_fops =
{
    .owner = THIS_MODULE,
    .open = myDriver_open,
    .read = myDriver_read,
    .write = myDriver_write,
    .unlocked_ioctl = myDriver_ioctl,
    .release = myDriver_close,
};

// registers the Device Driver with the OS
static int __init myDriver_module_init(void)
{
    printk("Initializing Driver...\n");
    register_chrdev(MAJOR_NUM, "Sedric's Device Driver", &myDriver_fops);

    return 0;
}

// unregisters the Device Driver from the OS
static void __exit myDriver_module_exit(void)
{
    unregister_chrdev(MAJOR_NUM, "Sedric's Device Driver");
    printk("Driver Exiting...");
}

// calls the init and exit routines
module_init(myDriver_module_init);
module_exit(myDriver_module_exit);