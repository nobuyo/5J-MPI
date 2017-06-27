#!/bin/bash

for i in {1..30}; do
  res=()

  for j in {1..5}; do
    res+=`mpirun -np $i ex3 | cut -d' ' -f2 | sed -e 's/$/, /g'`
  done

  echo "$i, ${res[@]}"
  unset res[@]
done