#!/bin/bash
/root/iozone3_414/src/current –i 0 –f /root/nilfs_disk/tmp
cd /proc/myproc
echo nilfs2 > hw1
dmesg
