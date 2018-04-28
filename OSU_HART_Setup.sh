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
git clone https://github.com/upthagrg/100kRocketryAvionics2017-18
echo
echo "done"
echo
#configure
echo
echo "running configure"
echo
cd 100kRocketryAvionics2017-18
./configure
echo
echo "done"
echo
#make
echo
echo "Making programs"
echo
cd 3d_trace
make
cd ..
cd logging
make
cd data_handling
make
cd ..
cd data_generator
make
cd ..
cd conversion
make
cd ..
echo
echo "done"
echo
