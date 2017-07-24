#!/bin/bash

# alias awk='/usr/bin/awk'

mkdir -p data
d=`date "+%Y%m%d-%H%M%S"`

for i in {4..30..2}; do
  res1=()
  res2=()

  for j in {1..5}; do
    res=`mpirun -machinefile ~/mymachines -np $i ex6_for_bench | grep time | awk {'print $NF'} `
    res1+=`echo $res | sed -e 's/$/+/'`
    res2+=`echo $res | sed -e 's/$/, /'`
  done

  ave=`echo "($res1 0) / 5.0" | bc -l`
  echo "$i, $ave" >> data/ex6_average_$d.dat
  echo "$i, ${res2[@]}" >> data/ex6_raw_$d.dat
  unset res1[@]
  unset res2[@]
done

# alias awk='/usr/bin/awk'

# for i in {4..30..2}; do
#   echo -n "$i: "
#   mpirun -machinefile ~/mymachines -np $i  ex5 | grep time | awk '{print $NF}'
# done