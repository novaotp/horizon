build:
	zig build-exe ./source/main.zig -femit-bin=./bin/main.exe

run:
	.\bin\main.exe
