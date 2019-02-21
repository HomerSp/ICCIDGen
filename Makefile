PROJECT = iccidgen.exe

CC = i686-w64-mingw32-g++-win32

CFLAGS = -fpermissive
INCLUDES = -Iinclude

LFLAGS = -static
LIBS = -lssl -lcrypto -lws2_32

SRCS = main.cpp encrypt.cpp hash.cpp utils.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(PROJECT)
	@echo Compiled $(PROJECT)

$(PROJECT): $(OBJS) 
	$(CC) $(CFLAGS) $(INCLUDES) -o $(PROJECT) $(OBJS) $(LFLAGS) $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) *.o *~ $(PROJECT)
