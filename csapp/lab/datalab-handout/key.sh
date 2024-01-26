#!/usr/bin/env bash

arg="$1"

#1:test, 2:make, 3:run, 4:split

if [ "$arg" = "3" ]; then
    ./btest;
elif [ "$arg" = "1" ]; then
    ./dlc bits.c;
    ./dlc -e bits.c;
elif [ "$arg" = "2" ]; then
    make clean;
    make btest;
elif [ "$arg" = "4" ]; then
    tmux split-window -v -p 70 
    tmux send "fhs" ENTER
elif [ "$arg" = "5" ]; then
    tmux last-window
else
    echo "you have binding wrong key!"
fi

