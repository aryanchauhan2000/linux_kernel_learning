#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "chardev"
#define CLASS_NAME "char_class"
#define BUFFER_SIZE 1024

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Aryan Chauhan");
MODULE_DESCRIPTION("A simple character device driver");
MODULE_VERSION("0.1");

static int major_number;
static char message[BUFFER_SIZE] = {0};
static short size_of_message;
static int counter = 0;
static struct class* char_device_class = NULL;
static struct device* char_device = NULL;

// Function prototypes
static int dev_open(struct inode *, struct file *);
static int dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

static struct file_operations fops = {
    .open = dev_open,
    .read = dev_read,
    .write = dev_write,
    .release = dev_release,
};

static int __init chardev_init(void) {
    printk(KERN_INFO "CharDev: Initializing the character device\n");

    // Register the character device
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        printk(KERN_ALERT "CharDev failed to register a major number\n");
        return major_number;
    }
    printk(KERN_INFO "CharDev: registered with major number %d\n", major_number);

    // Register the device class
    char_device_class = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(char_device_class)) {
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ALERT "Failed to register device class\n");
        return PTR_ERR(char_device_class);
    }

    // Register the device driver
    char_device = device_create(char_device_class, NULL, MKDEV(major_number, 0), NULL, DEVICE_NAME);
    if (IS_ERR(char_device)) {
        class_destroy(char_device_class);
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ALERT "Failed to create the device\n");
        return PTR_ERR(char_device);
    }
    printk(KERN_INFO "CharDev: device class created correctly\n");
    return 0;
}

static void __exit chardev_exit(void) {
    device_destroy(char_device_class, MKDEV(major_number, 0));
    class_unregister(char_device_class);
    class_destroy(char_device_class);
    unregister_chrdev(major_number, DEVICE_NAME);
    printk(KERN_INFO "CharDev: Goodbye from the character device!\n");
}

static int dev_open(struct inode *inodep, struct file *filep) {
    counter++;
    printk(KERN_INFO "CharDev: Device has been opened %d time(s)\n", counter);
    return 0;
}

static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset) {
    int error_count = 0;
    
    // Copy to user buffer
    error_count = copy_to_user(buffer, message, size_of_message);
    if (error_count == 0) {
        printk(KERN_INFO "CharDev: Sent %d characters to the user\n", size_of_message);
        return (size_of_message = 0);  // Clear the position to the start
    } else {
        printk(KERN_INFO "CharDev: Failed to send %d characters to the user\n", error_count);
        return -EFAULT;
    }
}

static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset) {
    sprintf(message, "%s(%zu letters)", buffer, len);
    size_of_message = strlen(message);
    printk(KERN_INFO "CharDev: Received %zu characters from the user\n", len);
    return len;
}

static int dev_release(struct inode *inodep, struct file *filep) {
    printk(KERN_INFO "CharDev: Device successfully closed\n");
    return 0;
}

module_init(chardev_init);
module_exit(chardev_exit); 