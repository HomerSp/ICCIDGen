all: iccidgen.exe

iccidgen.exe: main.o
	 i686-w64-mingw32-g++-win32 -o iccidgen.exe -static main.o -lssl -lcrypto

main.o: main.cpp
	 i686-w64-mingw32-g++-win32 -c main.cpp

clean:
	 rm main.o iccidgen.exe