# YASOS
## What is yasos?
The yasos is a new language, but a the same time is the compiler for this language. It is a project with learning purpuoses to learn more about
compilers and how our machines understand our codes.

> This project is barely advanced and it is not recommended to use in a mid-advanced project, it is usable in examples or even easy learning about low level programming languages

# Documentation
By the moment, the project has not an official documentation, but I am working in that, so the documentation could last a few weeks to month to appear. When the docs are available there will be a link right here. Thank you for your patience

## How to compile the project?
To compile the project you must first download the source code:
`git clone https://github.com/DameChocolateYa/yasos.git`

Then enter in the directory:
`cd yasos`

Create make compilation files with cmake:
`cmake -S . -B build/`

Compile:
`make --build build/`

By the moment, yasos does not include a installation script, so you must copy all the files manually:
> This may require root privileges
`cp build/yasos /usr/bin`

**__This only will install the compiler, to make the whole installation:__**
```
cd modules/headers
mkdir -p /usr/include/ysinclude
cp *.ys /usr/include/ysinclude
cd ../
mkdir -p /usr/lib/yslib
cp libys.so /usr/lib/yslib
```
> This may require root privileges

This will install modules and headers

## License

This project is licensed under the BSD 3-Clause License - see the [LICENSE](LICENSE) file for details.
