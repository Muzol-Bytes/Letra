# Letra
Letra is a painless text editor written in c++ using the Win32 api and direct2d.
By now its under heavy development but check out its current features.
<!-- It is intended to be --> 
<!-- easy to use and customizable. -->
<!-- Its customization is written in lua script. -->
<!-- Why lua you may ask. Because lua is easy to integrate with c++ and is really simple to code in lua. -->
<!-- So customization should be fairly easy to implement. -->

## Compile and run
Inside root directory
```
$ mkdir build
$ cd build
$ cmake ..
$ cmake -DCMAKE_BUILD_TYPE="Release"
$ cmake --build .
$ cd ..
$ bin\Release\Letra.exe src/main.cpp
```

### Functionalities
* `Esc` to leave
* `Ctrl-S` to save
* `Ctrl-P` to open a file
* `Ctrl-G` to go to `n` line
