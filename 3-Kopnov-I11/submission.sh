#!/bin/bash

tail -n +7 ./inc/btree.h | head -n -1 > submission.c
tail -n +2 ./main.c >> submission.c
tail -n +27 ./src/btree.c >> submission.c
