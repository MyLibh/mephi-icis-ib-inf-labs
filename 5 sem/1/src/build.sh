#!/bin/bash

echo "[RAND DATA]"
echo "Compiling"
gcc -o lab1 -fopenmp lab1.c
echo "Working..."
./lab1 > log.txt
echo "Making plots"
python3 script.py

echo "[SAME DATA]"
echo "Compiling"
gcc -o lab1 -D __SAME_DATA__ -fopenmp lab1.c
echo "Working..."
./lab1 > log.txt
echo "Making plots"
python3 script.py _same

# echo "Constructing report"
# latex ../report.tex