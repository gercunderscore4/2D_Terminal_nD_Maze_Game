#!/bin/bash
rm -f m

for i in {1..10}
do
	echo ' '
done

make all

for i in {1..10}
do
	echo ' '
done

./m
