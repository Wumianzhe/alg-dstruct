#!/bin/bash

tail -n +3 ./inc/btree.h | head -n -1 > submission.c
tail -n +2 ./main.c >> submission.c
tail -n +25 ./src/btree.c >> submission.c
