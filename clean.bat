for /d /r . %%d in (ipch lib bin build) do @if exist %%d rd /s /q %%d
del /s *.sdf