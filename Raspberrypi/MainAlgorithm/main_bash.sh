#!/bin/bash

echo "0" > ../calibration/restart.txt
while true; do
echo "Empieza el codigo de nuevo"
python /home/pi/LARC_2.0/Raspberrypi/MainAlgorithm/MainRasp.py
done
