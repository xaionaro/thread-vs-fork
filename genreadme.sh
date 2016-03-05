#!/bin/bash

(
	echo -e 'system\n======\n'
	echo '```'
	grep 'model name' /proc/cpuinfo | head -1
	echo -n "cpus: "; grep -c processor /proc/cpuinfo
	uname -a
	echo -e '```\n'
	make build > /dev/null && \
		for p in *[dk]; do
			echo "$p"
			echo "$p" | tr "[a-z\-]" "="
			echo -e '\n```'
			(time ./$p > /dev/null) 2>&1 | grep -v "^$"
			echo '```'
			echo
		done
) | tee -a README.md

