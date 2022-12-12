@echo off
cd cdimage
del game.bin
cd..
del MAIN.EXE
cls
psymake
pause
if exist MAIN.EXE goto :run
exit

:run
.\mkpsxiso\mkpsxiso.exe -o cdimage\game.bin -y .\cuesheet.xml
C:/epsxe/epsxe.exe -loadbin C:/psyq/Projects/Tutorial/cdimage/game.bin -nogui
exit