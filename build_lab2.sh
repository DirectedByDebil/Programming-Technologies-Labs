clear
make -f Makefile_lab2 clean
make -f Makefile_lab2
#find . -name "*.cpp" -o -name "*.h" | xargs du -ch
OUTPUT="laba2_results.csv"
DIRECTORY="./laba2_results"
./laba_2 -mo 3 -f $OUTPUT -d $DIRECTORY
python3 ./core/view/plot_csv.py $DIRECTORY $OUTPUT 