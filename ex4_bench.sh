#!/bin/bash

mkdir -p data
d=`date "+%Y%m%d-%H%M%S"`

for i in {1..30}; do
  res1=()

  for j in {1..10}; do
    res=`mpirun -machinefile ~/mymachines -np $i ex4 | tail -n1`
    res1+=`echo $res | sed -e 's/$/+/'`
  done

  ave=`echo "($res1 0) / 10.0" | bc -l`

  echo "$i, $ave" >> data/ex4_average_$d.dat
  unset res1[@]
done