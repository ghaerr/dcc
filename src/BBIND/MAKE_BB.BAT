c88 bbind -px %1
if errorlevel 1 goto stop

c88 bb2 -px %1
if errorlevel 1 goto stop

c88 bb3 -px %1
if errorlevel 2 goto stop

c88 bb4 -px %1
if errorlevel 1 goto stop

if .%1.==.c. goto generate_chk

bind bbind bb2 bb3 bb4 -omy_bbind
goto stop

:generate_chk
bind bbind bb2 bb3 bb4 -o my_bbind -c

:stop
