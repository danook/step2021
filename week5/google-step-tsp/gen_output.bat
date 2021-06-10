% echo off
for /l %%i in (0, 1, 6) do (
    solver.exe input_%%i.csv output_%%i.csv
)