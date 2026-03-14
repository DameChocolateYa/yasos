#!/bin/bash
help() {
	printf "\nYASOS install script help menu\n
--install: installs yasos binary, library and the headers in your system\n
--remove: remove the yasos compiler, libreary and headers in your system\n
--clean: remove the compilation results in the cloned repository\n
--help: displays this menu\n

To learn YASOS check the github's docs in YASOS repository\n

Have FUN (and good luck...)\n"
}

install() {
	if [ "$(id -u)" -ne 0 ]; then
	    echo "This script requires root privileges."
	    exec sudo "$0" --install
	fi

	cmake -S . -B build/
	cmake --build build/
	cp build/yasos /usr/bin
	
	cd modules/headers
	mkdir -p /usr/include/ysinclude
	cp *.ys /usr/include/ysinclude
	
	cd ../
	make
	mkdir -p /usr/lib/yslib
	cp libys.so /usr/lib/yslib
	
	cd ../
}

clean() {
	rm -fr build/
	cd modules/
	make clean
	cd ../
}

remove() {
	if [ "$(id -u)" -ne 0 ]; then
	    echo "This script requires root privileges."
	    exec sudo "$0" --remove
	fi

	rm -fr /usr/bin/yasos
	rm -fr /usr/lib/yslib
	rm -fr /usr/include/ysinclude
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
	elif [[ "$arg" == "--clean" ]]; then
		clean
	elif [[ "$arg" == "--remove" ]]; then
		remove
	else
		printf "'$arg' not valid"
		exit 1
	fi
done
