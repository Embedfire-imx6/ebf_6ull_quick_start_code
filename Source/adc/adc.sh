#!/bin/bash

echo "Press Ctrl+C for quit"

while true
do
		vol_raw=$(cat /sys/bus/iio/devices/iio\:device0/in_voltage3_raw)
		vol_scale=$(cat /sys/bus/iio/devices/iio\:device0/in_voltage_scale)
		vol=$(echo "scale=4; $vol_raw*$vol_scale/1000" | bc )
		echo "vol_raw:$vol_raw,vol_scale:$vol_scale,vol:$vol V"
		sleep 1s
done