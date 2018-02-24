#!/bin/bash

for((i=1;i<=20;i++))
do
	output=$(xinput list-props $i 2>/dev/null)
	
	if [[ "$output" =~ Touchpad ]]
	then
		xinput set-prop $i "Device Enabled" 0
	fi

	if [[ "$output" =~ 'NTRG0001:00' ]]
	then
		xinput set-prop $i "Device Enabled" 0
	fi
done

