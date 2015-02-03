@echo off

if exist %1.pl goto work
echo Usage: %0 name
echo where name is the base name of a perl file to turn into a .exe file
goto exit

:work
perl -u %1.pl
emxbind -q -x perl.exe %1
emxbind -q -cperldump %1
del perldump %1
echo %1.exe created

:exit
