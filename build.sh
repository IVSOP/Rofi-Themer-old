#!/bin/bash

DIR="build"

if [[ -z $1 ]]
then # do not change socket path

	printf ">> Socket path not set, using default\n"

	if cmake -S . -B $DIR; then
	printf "\n"
	if ! make -j$(nproc) -C $DIR
	then
		printf ">> build failed\n"
	fi
	else
		printf ">> configure failed\n"
	fi

else

	if cmake -S . -B $DIR -DSOCK_PATH="$1"; then
		printf "\n"
		if ! make -j$(nproc) -C $DIR
		then
			printf ">> build failed\n"
		fi
	else
		printf ">> configure failed\n"
	fi

fi


