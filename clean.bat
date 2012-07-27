for /d /r . %%d in (ipch lib bin build obj) do @if exist %%d rd /s /q %%d
del /s *.sdf
del /s *.dat