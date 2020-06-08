#!/bin/bash
${CC} src/freelist/main.c -O2 -DFIRST_MATCH -o first_match
${CC} src/freelist/main.c -O2 -DBEST_MATCH -o best_match