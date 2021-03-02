#! /bin/bash
summ=0
count=0
for el in $@
do
let summ+=el
let count+=1
done
echo $count $((summ/count))