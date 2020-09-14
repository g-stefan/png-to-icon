@echo off
rem Public domain
rem http://unlicense.org/
rem Created by Grigore Stefan <g_stefan@yahoo.com>

echo -^> sign png-to-icon

pushd bin
for /r %%i in (*.exe) do call grigore-stefan.sign "PNG to Icon" "%%i"
popd
