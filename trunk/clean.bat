for /d /r . %%d in (ipch lib bin build obj Debug Release) do @if exist %%d rd /s /q %%d
del /s *.sdf
del /s *.dat