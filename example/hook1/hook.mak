!include <C:\Program Files\Microsoft SDKs\Windows\v6.0A\Include\win32.mak>
#------------------------
# HELLOWIN.MAK make file
#------------------------

Invoke.exe : Invoke.obj Hook.dll
    $(link) $(guilflags) /out:Invoke.exe Invoke.obj $(guilibs)
#$(link) $(conlflags) /out:Invoke.exe Invoke.obj $(conlibs)
    
Invoke.obj : Invoke.cpp
    $(cc) $(cflags) /DUNICODE /D_UNICODE Invoke.cpp

Hook.dll : Hook.obj
    $(link) $(dllflags) /out:Hook.dll Hook.obj /def:Hook.def $(conlibs)
     
Hook.obj : Hook.cpp
    $(cc) $(cflags) /DUNICODE /D_UNICODE Hook.cpp
    
clean:
    del *.obj *.dll *.lib *.exe *.exp
