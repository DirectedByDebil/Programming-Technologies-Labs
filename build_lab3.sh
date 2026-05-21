clear
IS_CACHED=false
if [ $# -gt 0 ]; then
    if [ $1 = "--fast" ]; then
        IS_CACHED=true
    fi
else
    echo "Building..."
    make -f Makefile_lab3 clean
    make -f Makefile_lab3
fi
DIRECTORY=./laba3_results
TARGET_FUNC=targetFunc.csv
SIMULATION_RESULTS=simulationResults.csv
TARGET_FUNCTIONS="rastrigin rosenbrock ackley beale"
for func in $TARGET_FUNCTIONS; do
    echo ""
    echo "Count for: $func"
    tf_name="${func}_${TARGET_FUNC}"
    sim_name="${func}_${SIMULATION_RESULTS}"

    if [ $IS_CACHED = false ]; then        
        ./laba_3 -alpha 0.95 -Tstart 200 -iter 2000 \
            -xmin -10 -xmax 10 -ymin -10 -ymax 10 \
            -dir $DIRECTORY -tf $tf_name -sr $sim_name \
            -tft $func
    fi
    python3 ./core/view/laba3_view.py -tf $DIRECTORY/$tf_name -sr $DIRECTORY/$sim_name -pt $func -sdir $DIRECTORY
done