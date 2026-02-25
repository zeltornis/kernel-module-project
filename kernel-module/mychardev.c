#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/version.h>

#define DEVICE_NAME "mychardev"
#define BUF_LEN 80

static dev_t dev_num;             // device number (major+minor)
static struct cdev my_cdev;       // character device structure
static struct class *my_class;    // device class
static char msg[BUF_LEN];
static int msg_len;

static int dev_open(struct inode *inode, struct file *file) { return 0; }
static int dev_release(struct inode *inode, struct file *file) { return 0; }

static ssize_t dev_read(struct file *file, char __user *user_buffer,
                        size_t len, loff_t *offset)
{
    if (*offset >= msg_len)
        return 0;

    if (len > msg_len - *offset)
        len = msg_len - *offset;

    if (copy_to_user(user_buffer, msg + *offset, len))
        return -EFAULT;

    *offset += len;
    return len;
}

static ssize_t dev_write(struct file *file, const char __user *user_buffer,
                         size_t len, loff_t *offset)
{
    if (len > BUF_LEN)
        len = BUF_LEN;

    if (copy_from_user(msg, user_buffer, len))
        return -EFAULT;

    msg_len = len;
    return len;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = dev_open,
    .release = dev_release,
    .read = dev_read,
    .write = dev_write,
};

static int __init mychardev_init(void)
{
    // Step 1: Allocate device number
    if (alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME) < 0) {
        pr_err("Failed to allocate device number\n");
        return -1;
    }

    // Step 2: Initialize and add cdev
    cdev_init(&my_cdev, &fops);
    if (cdev_add(&my_cdev, dev_num, 1) < 0) {
        pr_err("Failed to add cdev\n");
        unregister_chrdev_region(dev_num, 1);
        return -1;
    }

    // Step 3: Create class and device
    #if LINUX_VERSION_CODE >= KERNEL_VERSION(6,4,0)
        my_class = class_create(DEVICE_NAME);
    #else
        my_class = class_create(THIS_MODULE, DEVICE_NAME);
    #endif
    if (IS_ERR(my_class)) {
        pr_err("Failed to create class\n");
        cdev_del(&my_cdev);
        unregister_chrdev_region(dev_num, 1);
        return PTR_ERR(my_class);
    }

    if (device_create(my_class, NULL, dev_num, NULL, DEVICE_NAME) == NULL) {
        pr_err("Failed to create device\n");
        class_destroy(my_class);
        cdev_del(&my_cdev);
        unregister_chrdev_region(dev_num, 1);
        return -1;
    }

    pr_info("%s registered with major %d, minor %d\n",
            DEVICE_NAME, MAJOR(dev_num), MINOR(dev_num));
    return 0;
}

static void __exit mychardev_exit(void)
{
    device_destroy(my_class, dev_num);
    class_destroy(my_class);
    cdev_del(&my_cdev);
    unregister_chrdev_region(dev_num, 1);
    pr_info("Unregistered %s\n", DEVICE_NAME);
}

module_init(mychardev_init);
module_exit(mychardev_exit);

MODULE_AUTHOR("Tim D");
MODULE_DESCRIPTION("Simple char device driver");
MODULE_LICENSE("GPL");
