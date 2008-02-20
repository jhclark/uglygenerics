gcc -I.. -I../.. ../../../Debug/libgenc.a -P -E -CC main.c > main.pre
gcc -g3 -O0 -I.. -I../.. ../../../Debug/libgenc.a main.c -o main