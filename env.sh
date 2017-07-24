function killall() { ps | grep $1 | grep -v grep | cut -d' ' -f 1 | xargs kill -9}
