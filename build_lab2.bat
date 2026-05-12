cls
make -f Makefile_lab2_windows clean
make -f Makefile_lab2_windows
set OUTPUT="laba2_results.csv"
set DIRECTORY=".\laba2_results"
.\laba_2 -mo 5 -f %OUTPUT% -d %DIRECTORY%
.\core\view\.venv\Scripts\python.exe .\core\view\plot_csv.py %DIRECTORY% %OUTPUT%