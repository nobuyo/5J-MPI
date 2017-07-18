#!/bin/bash

if [[ $# -eq 0 ]]; then
  echo "Usage: $(basename $0) <number>"
  exit 1
fi

for i in {1..30}; do
  res=()

  for j in {1..5}; do
    res+=`mpirun -machinefile ~/mymachines -np $i ex2 $1 | cut -d' ' -f2 | sed -e 's/$/, /g'`
  done

  echo "$i, ${res[@]}"
  unset res[@]
done