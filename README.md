# kernel-status-d-demo
Linux kernel module that create a character device to "simulate/force" uninterruptible sleep state for testing purpose.


## Make
```bash
$ make
```

## Sign (If need)
```bash
$ sudo /usr/src/linux-headers-$(uname -r)/scripts/sign-file sha256 <PRIVATE_KEY> <X509_FILE> mod_statusddemo.ko
```

## Load Module
```bash
$ sudo insmod mod_statusddemo.ko
```

## Run some tests
For this example we use 2 terminals, on the first terminal we try to read "/dev/statusd_demo" with "cat":
```bash
$ sudo cat /dev/statusd_demo
^C^Z
```
At this time the first terminal will become unresponsive, even if you type "ctrl+c" or "ctrl+z".

Now on the second terminal we will check the process states and try to kill this process:
```bash
$ sudo ps aux | grep statusd | grep -v sudo
root      7919  0.0  0.0   7612   764 pts/1    D+   23:47   0:00 cat /dev/statusd_demo
$ sudo kill -9 7919
$ sudo kill -9 7919
$ sudo ps aux | grep statusd | grep -v sudo
root      7919  0.0  0.0   7612   764 pts/1    D+   23:47   0:00 cat /dev/statusd_demo
$ sudo cat /proc/7919/stack 
[<0>] statusd_read+0x67/0xa0 [mod_statusddemo]
[<0>] __vfs_read+0x1b/0x40
[<0>] vfs_read+0x8e/0x130
[<0>] SyS_read+0x5c/0xe0
[<0>] do_syscall_64+0x73/0x130
[<0>] entry_SYSCALL_64_after_hwframe+0x41/0xa6
[<0>] 0xffffffffffffffff
$ sudo sh -c 'echo "" > /dev/statusd_demo'
$ sudo ps aux | grep statusd | grep -v sudo
```
The only way to wake up and kill this process is writen to "/dev/statusd_demo". 
This happens because when you try to read the file, the kernel module put the process in the uninterruptible sleep state. At this state this process cannot receive any kind of signals, the only way to wake up this process is calling "wake_up" function inside the kernel module, which (in this example) is done on the write operation.

## Unload Module
```bash
$ sudo rmmod mod_statusddemo
```

