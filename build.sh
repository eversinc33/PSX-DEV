#!/bin/sh

echo "***********************+"
echo "    Create PSX ISO      "
echo "***********************+"

mkdir -p build

make
mkpsxiso cdrom.xml -y -q