#!/bin/bash

if [[ $# -eq 0 ]]; then
  echo "Usage: $(basename $0) <number>"
  exit 1
fi

# for i in {1..30}; do
#   res=()

#   for j in {1..5}; do
#     res+=`mpirun -machinefile ~/mymachines -np $i ex2 $1 | cut -d' ' -f2 | sed -e 's/$/, /g'`
#   done

#   echo "$i, ${res[@]}"
#   unset res[@]
# done

# alias awk='/usr/bin/awk'
mkdir -p data
d=`date "+%Y%m%d-%H%M%S"`

for i in {1..30}; do
  res1=()
  res2=()
  comm=()

  for j in {1..5}; do
    res=`mpirun -machinefile ~/mymachines -np $i ex2 $1 | cut -d',' -f2`
    com=`mpirun -machinefile ~/mymachines -np $i ex2 $1 | cut -d',' -f3`
    res1+=`echo $res | sed -e 's/$/+/'`
    res2+=`echo $res | sed -e 's/$/, /'`
    comm+=`echo $com | sed -e 's/$/+/'`
  done

  ave=`echo "($res1 0) / 5.0" | bc -l`
  ave_comm=`echo "($comm 0) / 5.0" | bc -l`

  echo "$i, $ave" >> data/ex2_average_$d.dat
  echo "$i, ${res2[@]}" >> data/ex2_raw_$d.dat
  echo "$i, $ave_comm" >> data/ex2_com_$d.dat
  unset res1[@]
  unset res2[@]
  unset comm[@]
done