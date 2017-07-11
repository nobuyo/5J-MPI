#!/bin/bash

for i in {1..30}; do
  res=()

  for j in {1..5}; do
    res+=`mpirun -np $i ex3 | grep time | awk {'print $NF'} | sed -e 's/$/+/'`
    # res+=`mpirun -np $i ex3 | grep time | awk {'print $NF'} | sed -e 's/$/, /'`
  done

  ave=`echo "($res 0) / 5.0" | bc -l`

  echo "$i, 0$ave"
  # echo "$i, ${res[@]}"
  unset res[@]
done