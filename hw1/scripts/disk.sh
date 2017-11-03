#!/bin/bash
cd /root
rm diskfile
rm -rf nilfs_disk
dd if=/dev/zero of=./diskfile bs=1024 count=200000
mkfs.nilfs2 ./diskfile
losetup /dev/loop0 ./diskfile
mkdir nilfs_disk
mount -t nilfs2 /dev/loop0 nilfs_disk/
mount | grep nilfs
