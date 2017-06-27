#!/bin/bash

living() {
  ping -c 1 -w 1 "ayu$1" &>/dev/null \
  && echo "ayu$1 cpu=1"
}

for i in {001..045}; do
  living $i &
done
