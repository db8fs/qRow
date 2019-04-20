#!/bin/bash

function errormsg()
{
   echo "Error: ./serial <device> [baudrate]"
   exit 1
}

function checkDevice()
{
	if [[ $# -eq 1 ]]; then
		if [[ -c $1 ]]; then
			echo "$1"
			return;
		else
	   		echo "Error: invalid device argument"
			exit 1
	   	fi;
	fi

	errormsg
}

function checkBaudrate()
{
	if [[ $# -eq 1 ]]; then
		case "$1" in
		300)	echo "300"
			return
			;;
		600)	echo "600"
			return
			;;
		1200)	echo "1200"
			return
			;;
		2400)	echo "2400"
			return
			;;
		4800)	echo "4800"
			return
			;;
		9600)	echo "9600"
			return
			;;
		14400)	echo "14400"
			return
			;;
		28800)	echo "28800"
			return
			;;
		38400)	echo "38400"
			return
			;;
		57600)	echo "57600"
			return
			;;
		115200)	echo "115200"
			return
			;;
		230400)	echo "230400"
			return
			;;
		460800)	echo "460800"
			return
			;;
		921600) echo "921600"
			return
			;;
		esac
	fi
	
	echo "115200"
}

export serport=$(checkDevice $1) 
export baudrate=$(checkBaudrate $2)


echo "Setting serial interface \"$serport\" to $baudrate bps"
stty -f $serport ispeed $baudrate ospeed $baudrate clocal cread cs8
stty -f $serport
