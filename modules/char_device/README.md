# Character Device Driver Module

This module demonstrates how to create a character device driver in the Linux kernel. It creates a device file that can be read from and written to from user space.

## What is a Character Device?

A character device is one of the fundamental device types in Linux. It's a device that can be accessed as a stream of bytes (like a file), and it can be read from and written to. Common examples include:
- Serial ports
- Keyboard
- Mouse
- Sound card

## Features

This module demonstrates:
1. Device file creation and management
2. Reading from and writing to the device
3. Basic device operations (open, close, read, write)
4. User space to kernel space data transfer
5. Device registration and cleanup

## Prerequisites

Same as the hello world module, plus:
```bash
sudo apt-get install udev    # For Debian/Ubuntu
# or
sudo dnf install udev       # For Fedora
```

## Building and Loading

1. Build the module:
```bash
make
```

2. Load the module:
```bash
sudo insmod chardev.ko
```

3. Check that the device was created:
```bash
ls -l /dev/chardev
dmesg | tail
```

## Testing the Device

1. Write to the device:
```bash
echo "Hello from user space" > /dev/chardev
```

2. Read from the device:
```bash
cat /dev/chardev
```

3. Check kernel messages:
```bash
dmesg | tail
```

## Understanding the Code

The module implements four main operations:
- `dev_open`: Called when the device is opened
- `dev_read`: Called when the device is read from
- `dev_write`: Called when the device is written to
- `dev_release`: Called when the device is closed

Key concepts demonstrated:
- Device registration with `register_chrdev`
- Device class and device creation for udev
- Copy between user and kernel space with `copy_to_user`
- Basic device file operations

## Cleanup

To remove the module:
```bash
sudo rmmod chardev
```

## Learning Objectives

After studying this module, you should understand:
1. How character devices work in Linux
2. The basic structure of a device driver
3. How to transfer data between user space and kernel space
4. How to register and manage device files 