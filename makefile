OUT=a.out

all: $(OUT)

$(OUT): main.cpp hud.h ship.h map.h shot.h enemy.h sounds.h
	g++ main.cpp -lGL -lfreeimage -lglut -lGLU -lsfml-audio -lsfml-window -lsfml-system -std=c++11 -o $(OUT)

clean:
	rm -rf $(OUT)