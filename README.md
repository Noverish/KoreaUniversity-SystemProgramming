# Korea University System Programming Assignments

Thank you for [arkainoh](https://github.com/arkainoh/FileSystem-Profiling), and [kimdog](https://github.com/kimdog/SystemProgramming-2016)

## Environment
Ubuntu 16.04.3 LTS (64bit)  
Kernel [linux-4.4.0](https://www.kernel.org/pub/linux/kernel/v4.x/linux-4.4.tar.gz) (4.4.0-36-generic)  
IOzone [iozone-3-414](http://iozone.org/src/current/iozone3_414.tar)  

# HW1
1. Copy blk-core.c to [your kernel path]/block/    directory
1. Copy segbuf.c   to [your kernel path]/fs/nilfs2 directory
1. Compile and install kernel. Then restart your vm
1. Compile hw1.c and install that to kernel
1. Create nilfs2 disk and mount it to some directory
1. Run iozone to directory which mounts above nilfs2 disk
1. Run below
    ```
    $ echo asdf > /proc/myproc/myproc
    ```
1. Check dmesg and /tmp/result.csv

### Result
![hw1_1.png](https://github.com/Noverish/KU-SP-2017-Fall/blob/master/images/hw1_1.PNG?raw=true)  
x-axis : time (microsecond)  
y-axis : sector number  

![hw1_2.png](https://github.com/Noverish/KU-SP-2017-Fall/blob/master/images/hw1_2.PNG?raw=true)  
dmesg  

![hw1_3.png](https://github.com/Noverish/KU-SP-2017-Fall/blob/master/images/hw1_3.PNG?raw=true)  
/tmp/result.csv  
