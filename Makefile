all: iccidgen.exe

iccidgen.exe: main.o encrypt.o hash.o utils.o
	 i686-w64-mingw32-g++-win32 -o iccidgen.exe -static main.o encrypt.o hash.o utils.o -lssl -lcrypto -lws2_32

main.o: main.cpp
	 i686-w64-mingw32-g++-win32 -c main.cpp -fpermissive

encrypt.o: encrypt.cpp
	 i686-w64-mingw32-g++-win32 -c encrypt.cpp -fpermissive

hash.o: hash.cpp
	 i686-w64-mingw32-g++-win32 -c hash.cpp -fpermissive

utils.o: utils.cpp
	 i686-w64-mingw32-g++-win32 -c utils.cpp -fpermissive

clean:
	 rm main.o encrypt.o hash.o utils.o iccidgen.exe