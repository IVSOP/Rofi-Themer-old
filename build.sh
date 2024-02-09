#!/bin/bash

DIR="build"

if cmake -S . -B $DIR; then
	printf "\n"
	if ! make -j$(nproc) -C $DIR
	then
		printf ">> build failed\n"
	fi
else
	printf ">> configure failed\n"
fi
