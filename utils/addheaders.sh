#!/bin/bash

for i in $(find -name *.cpp -o -name *.h); do
	if grep "This software is governed by the CeCILL" $i >/dev/null
		then cat /dev/null
		else echo Adding header to file $i; cat HEADER $i > temp; mv temp $i;
	fi
done
