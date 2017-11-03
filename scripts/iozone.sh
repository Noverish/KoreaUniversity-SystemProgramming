#!/bin/bash
cd /root/iozone3_414/src/current
./iozone –i 0 –f /root/nilfs_disk/tmp -s 32m
cd /proc/myproc
echo nilfs2 > myproc
cd /root
dmesg
