# Installation

By the moment there are only two methods to install yasos compiler in your GNU/Linux.

## IMPORTANT

Yasos only has been tested in amd64 GNU/Linux, other OS or architectures could not work. Besides, neither there are not official precompiled binary nor package, so you will have to compile the project by yourself.

## Debian

You can download the yasos deb package in the releases page of the repository. Then you only have to open the package manager to install or execute in the terminal `apt install ./yasos_YYMMDD-d_arch.deb` > Note, this may will require root privileges

> By the moment, there are not an official way to downloas with apt

## Others

By now, there are not precompiled packages for other GNU/Linux distributions, but you can compile by yourself.

1- Clone the repository with ´git clone https://github.com/DameChocolateYa/yasos.git && cd yasos´

2- Prepare the compilation `cmake -S . -B build/`

3- Compile and copy `cmake --build build/ && cp build/yasos /usr/bin/`

4- Copy the yasos headers `mkdir -p /usr/include/ysinclude/ && cp module/headers/*.ys /usr/include/ysinclude`

5- Compile yasos standard library and copy it `cd modules && mkdir -p /usr/lib/yslib/ && make && cp libys.so /usr/lib/yslib`

6- Remove the local repositoty `cd ../../ && rm -fr yasos`

> This may requires root privileges
