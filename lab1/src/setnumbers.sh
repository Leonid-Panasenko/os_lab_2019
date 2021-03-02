#! /bin/bash
for ((i=0; i<150; i++))
do
od -An -N1 -i /dev/urandom >> numbers.txt
done