#!/bin/bash
cd /root/tmp-nilfs
git stash
git clean -f
git pull origin master
cd source
make
rmmod hw1
insmod hw1.ko
cd /root
