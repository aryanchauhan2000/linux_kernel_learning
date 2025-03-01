# Hello World Kernel Module

This is a simple Linux kernel module that prints "Hello World" when loaded and "Goodbye" when unloaded.

## Prerequisites

Before you can build and run this module, you need:

1. Linux kernel headers for your current kernel version
2. Build essentials (gcc, make)

On Ubuntu/Debian:
```bash
sudo apt-get update
sudo apt-get install linux-headers-$(uname -r) build-essential
```

On Fedora:
```bash
sudo dnf install kernel-headers kernel-devel
sudo dnf group install "C Development Tools and Libraries"
```

## Building the Module

To build the module, simply run:
```bash
make
```

## Loading and Unloading the Module

To load the module:
```bash
sudo insmod hello.ko
```

To verify the module is loaded:
```bash
lsmod | grep hello
```

To see the "Hello World" message:
```bash
dmesg | tail
```

To unload the module:
```bash
sudo rmmod hello
```

## Understanding the Code

The module consists of two main functions:
- `hello_init()`: Called when the module is loaded
- `hello_exit()`: Called when the module is unloaded

The module uses `printk()` to write messages to the kernel log buffer, which can be viewed using the `dmesg` command. 