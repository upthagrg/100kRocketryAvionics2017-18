#!/bin/bash

#git
echo
echo "installing git"
echo
sudo apt-get install git -y
echo
echo "done"
echo
#clone
echo
echo "cloning repository"
echo
cd ~
git clone https://github.com/upthagrg/100kRocketryAvionics2017-18
echo
echo "done"
echo
#configure
echo
echo "running setup"
echo
cd 100kRocketryAvionics2017-18
./configure
echo
echo "done"
echo
