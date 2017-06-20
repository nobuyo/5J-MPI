#!/bin/sh

for i in $(seq 45); do
  expect -c "
  set timeout 3
  spawn env LANG=C /usr/bin/ssh j13426@ayu$(printf %03d $i)
  expect {
      \"(yes/no)?\" {
          send \"yes\n\"
          exp_continue
      }
  }
  expect {
      \"\\\$\" {
          exit 0
      }
  }
  "
done