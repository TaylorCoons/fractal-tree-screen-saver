#!/bin/bash

make
sudo cp main.o /usr/lib/xscreensaver/ftree
sudo cp ftree.xml /usr/share/xscreensaver/config/ftree.xml 
