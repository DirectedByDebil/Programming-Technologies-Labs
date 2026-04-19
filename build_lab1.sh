clear
make -f Makefile_lab1 clean
make -f Makefile_lab1
find . -name "*.c" -o -name "*.h" | xargs du -ch
./laba_1 demos/matrix_a.txt demos/matrix_b.txt -r 1000
