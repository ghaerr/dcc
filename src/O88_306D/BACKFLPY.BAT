del b:*.arc
del *.arc
pkarc a src src\*.*
copy *.arc b:
del *.arc
pkarc a inc inc\*.*
copy *.arc b:
del *.arc
pkarc a lint lint\*.*
copy *.arc b:
del *.arc
pkarc a released released\*.*
copy *.arc b:
del *.arc
pkarc a obj obj\*.*
copy *.arc b:
del *.arc
no *.exe pkarc a rest *.*
copy *.arc b:
del *.arc
