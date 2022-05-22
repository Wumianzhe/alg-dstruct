#!/usr/bin/env bash

tail -n +4 ./inc/hash.h | head -n -1 > submission.c
tail -n +2 ./main.c >> submission.c
tail -n +2 ./src/hash.c >> submission.c
