set O88.1=+aefjmpst
md obj

cc src\cc -oobj\cc -nOLD_ASM88 %1
if errorlevel 2 goto exit

cc src\gather -oobj\gather %1
if errorlevel 2 goto exit

cc src\o88 -oobj\o88 %1
if errorlevel 2 goto exit

cc src\parse -oobj\parse %1
if errorlevel 2 goto exit

cc src\utils -oobj\utils %1
if errorlevel 2 goto exit

cc src\8088-pt1 -oobj\8088-pt1 %1
if errorlevel 2 goto exit

cc src\8088-pt2 -oobj\8088-pt2 %1
if errorlevel 2 goto exit

cc src\8088-pt3 -oobj\8088-pt3 %1
if errorlevel 2 goto exit

cc src\80188 -oobj\80188 -nOLD_ASM88 %1
if errorlevel 2 goto exit

cc src\8087 -oobj\8087 -nPRODUCTION %1
if errorlevel 2 goto exit

cc src\labels -oobj\labels %1
if errorlevel 2 goto exit

cc src\asmlib -oobj\asmlib %1
if errorlevel 2 goto exit

cc src\allocate -oobj\allocate %1
if errorlevel 2 goto exit

cc src\squeeze -oobj\squeeze %1
if errorlevel 2 goto exit

cc src\jumps -oobj\jumps %1
if errorlevel 2 goto exit

cc src\folding -oobj\folding %1
if errorlevel 2 goto exit

cc src\buffers -oobj\buffers %1
if errorlevel 2 goto exit

cc src\cleanup -oobj\cleanup %1
if errorlevel 2 goto exit

cc src\linklist -oobj\linklist %1
if errorlevel 2 goto exit

cc src\data -oobj\data %1
if errorlevel 2 goto exit

bind -ffiles -omy_o88 -po88.map -_
if errorlevel 2 goto exit

bind exec obj\cc -s1000 -omy_cc -pcc.map -_

:exit
