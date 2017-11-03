#!/bin/bash
cd /root/tmp-nilfs
git stash
git clean -f
git pull origin master
chmod 777 -R scripts
cd source
make
rmmod hw1
insmod hw1.ko
cd /root
