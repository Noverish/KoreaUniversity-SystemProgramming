#!/bin/bash
cd /root/tmp-nilfs
sudo cp blk-core.c /usr/src/linux-4.4/block/
sudo cp segbuf.c /usr/src/linux-4.4/fs/nilfs2/
cd /usr/src/linux-4.4
make
make modules_install
make install
cd /root
