#*
# * yasos - a compiler for yasos language
# * Copyright (c) 2025-2026 DameChocolateYa
# * Licensed under the BSD 3-Clause License.
# * See LICENSE file in the project root for full license text.
#*

#!/bin/bash

help() {
	printf "\nYASOS install script help menu\n
--install: installs yasos binary, library and the headers in your system\n
--remove: remove the yasos compiler, libreary and headers in your system\n
--help: displays this menu\n

To learn YASOS check the github's docs in YASOS repository\n

Have FUN (and good luck...)\n"
}

install() {
	if [ "$(id -u)" -ne 0 ]; then
            echo "This script requires root privileges."
            exec sudo "$0" --install
        fi

	cp yasos /usr/bin
	mkdir -p /usr/include/ysinclude
	cp headers/*.ys /usr/include/ysinclude
	mkdir -p /usr/lib/yslib/
	cp libys.so /usr/lib/yslib

	mkdir -p /usr/share/yasos/
	cp install.sh /usr/share/yasos/
}

remove() {
	if [ "$(id -u)" -ne 0 ]; then
	    echo "This script requires root privileges."
	    exec sudo "$0" --remove
	fi

	rm /usr/bin/yasos
	rm -fr /usr/lib/yslib/
	rm -fr /usr/include/ysinclude/
	rm -fr /usr/share/yasos/
}

if [ "$#" -eq 0 ]; then
	help
	exit 0
fi

for arg in "$@"; do
	if [[ "$arg" == "--help" ]]; then
		help
	elif [[ "$arg" == "--install" ]]; then
		install
	elif [[ "$arg" == "--remove" ]]; then
		remove
	else
		printf "'$arg' not valid"
		exit 1
	fi
done
