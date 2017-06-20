#!/bin/bash

touch putLiveMachine.c

echo '#include <stdio.h>' > putLiveMachine.c
echo 'int main(int argc,char* argv[]) {' >> putLiveMachine.c
echo '    int a = atoi(argv[1]);' >> putLiveMachine.c
echo '    printf("ayu%03d cpu=1\\n",a);' >> putLiveMachine.c
echo '    return 0;' >> putLiveMachine.c
echo '}' >> putLiveMachine.c

make putLiveMachine

rm mymachines
for i in $(seq 1 45); do
    x=$i
    i=`printf %03d $i`
    timeout 0.2 ssh j13406@ayu$i ./putLiveMachine $x >> mymachines
    x=`expr $x + 1`
    echo -ayu$i
done

rm putLiveMachine putLiveMachine.c