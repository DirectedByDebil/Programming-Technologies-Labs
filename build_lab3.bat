cls
REM make -f Makefile_lab3_windows clean
REM make -f Makefile_lab3_windows
REM .\laba_3
set TARGET_FUNCTIONS=rastrigin rosenbrock ackley

for %%n in (%TARGET_FUNCTIONS%) do (
    echo Имя: %%n
)