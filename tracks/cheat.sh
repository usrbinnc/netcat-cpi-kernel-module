#!/bin/bash
for i in *data.h; do
	cat ${i} |
		sed -re 's/,$//' |
		awk '{printf "%c", strtonum($0)}' > ${i}.ogg
done
