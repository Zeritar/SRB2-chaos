cd src
set CC=gcc
mingw32-make MINGW=1 WINDOWSHELL=1 SDL=1 NOOBJDUMP=1
cd ..
copy F:\srb29chaos\bin\Mingw\Release\srb2win.exe E:\SRB2\srb2chaos.exe