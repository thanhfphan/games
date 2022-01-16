windows:
	g++ -Wall -std=c++17 *.cpp -lSDL2 -o game `sdl2-config --cflags --libs`
macos:
	g++ -std=c++17 *.cpp -Wall -I/opt/homebrew/include -D_THREAD_SAFE -L/opt/homebrew/lib -lSDL2 -o game