#!/bin/bash

alias awk='/usr/bin/awk'

for i in {2..30..2}; do
  res1=()
  res2=()

  for j in {1..5}; do
    res=`mpirun -np $i ex5 | grep time | awk {'print $NF'} `
    res1+=`echo $res | sed -e 's/$/+/'`
    res2+=`echo $res | sed -e 's/$/, /'`
  done

  ave=`echo "($res1 0) / 5.0" | bc -l`

  echo "$i, $ave" >> ex5_ave.dat
  echo "$i, ${res2[@]}" >> ex5_raw.dat
  unset res1[@]
  unset res2[@]
done