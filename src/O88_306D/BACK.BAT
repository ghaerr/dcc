copy *.exe released/b
copy *.exe \c88/b
cd d:\ksp\o88
copy b:*.arc/b
pkarc u inc inc\*.*
pkarc u src src\*.*
pkarc u obj obj\*.*
pkarc u released released\*.*
del rest.arc
copy *.arc b:/b
del *.arc
no *.exe pkarc a rest *.*
copy *.arc b:/b
del *.arc
copy src d:src/b
copy inc d:inc/b
copy obj d:obj/b
copy released d:released/b
copy *.* d:/b
