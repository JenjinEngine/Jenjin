#!/usr/bin/env bash

# Check for `tl` and `inotifywait`
if ! [ -x "$(command -v tl)" ]; then
		echo 'Error: `tl` is not installed.' >&2
		exit 1
fi

if ! [ -x "$(command -v inotifywait)" ]; then
		echo 'Error: `inotifywait` is not installed.' >&2
		exit 1
fi

while true; do
    inotifywait -q -e modify,create,delete -m . | while read -r _ _ file
    do
        if [[ $file == *.tl ]]; then
            echo "File changed: $file"
            tl gen *.tl -I ../../
        fi
    done
done

