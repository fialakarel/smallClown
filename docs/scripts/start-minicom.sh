#!/bin/bash


for t in 0 1
do
	tty="/dev/ttyUSB$t"
	printf "\nTesting -- $tty"
	if [ -r "$tty" ]
	then
		echo " -- [OK]"
		sudo chmod 777 $tty
		minicom --device $tty
	fi
done

echo

