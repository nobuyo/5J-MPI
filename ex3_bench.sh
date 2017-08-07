#!/bin/bash

# for i in {1..30}; do
#   res=()

#   for j in {1..5}; do
#     res+=`mpirun  -machinefile ~/mymachines -np $i ex3 | grep time | awk {'print $NF'} | sed -e 's/$/+/'`
#     # res+=`mpirun -np $i ex3 | grep time | awk {'print $NF'} | sed -e 's/$/, /'`
#   done

#   ave=`echo "($res 0) / 5.0" | bc -l`

#   echo "$i, 0$ave"
#   # echo "$i, ${res[@]}"
#   unset res[@]
# done


# alias awk='/usr/bin/awk'
mkdir -p data
d=`date "+%Y%m%d-%H%M%S"`

for i in {1..30}; do
  res1=()
  res2=()
  com=()

  for j in {1..5}; do
    res=`mpirun -machinefile ~/mymachines -np $i ex3 | grep time | awk {'print $NF'} `
    com=`mpirun -machinefile ~/mymachines -np $i ex3 | grep comm | awk {'print $NF'} `
    res1+=`echo $res | sed -e 's/$/+/'`
    res2+=`echo $res | sed -e 's/$/, /'`
    com+=`echo $com | sed -e 's/$/+/'`
  done

  ave=`echo "($res1 0) / 5.0" | bc -l`
  ave_com=`echo "($com 0) / 5.0" | bc -l`

  echo "$i, $ave" >> data/ex3_average_$d.dat
  echo "$i, ${res2[@]}" >> data/ex3_raw_$d.dat
  echo "$i, ${ave_com}" >> data/ex3_com_$d.dat
  unset res1[@]
  unset res2[@]
  unset com[@]
done

# alias awk='/usr/bin/awk'

# for i in {4..30..2}; do
#   echo -n "$i: "
#   mpirun -machinefile ~/mymachines -np $i  ex5 | grep time | awk '{print $NF}'
# done