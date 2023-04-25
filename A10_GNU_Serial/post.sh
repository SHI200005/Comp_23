#!/bin/bash
rm -f humanswin.dat zombieswin.dat

while read -r line; do
    words=($line)
    if [ "${words[6]}" == "0" ]; then
        if [ -r "${words[17]}" ]; then
            survival=$(sed -n '12p' "${words[17]}")
            if [ "${survival#*=}" != "0%" ]; then
                echo "${words[13]} ${words[14]}" >> humanswin.dat
            else
                echo "${words[13]} ${words[14]}" >> zombieswin.dat
            fi
        fi
    fi
done < joblog.txt
