all: build run

build:
	clang *.c -o tetris.exe -I "C:/SDL3/include" -L "C:/SDL3/lib/x64" -lSDL3 -lSDL3_ttf

run:
	./tetris.exe

clean:
	rm -rf *.o *.exe