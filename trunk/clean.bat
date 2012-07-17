for /d /r . %%d in (ipch) do @if exist %%d rd /s %%d
for /d /r . %%d in (lib) do @if exist %%d rd /s %%d
for /d /r . %%d in (bin) do @if exist %%d rd /s %%d
for /d /r . %%d in (build) do @if exist %%d rd /s %%d
del /s *.sdf