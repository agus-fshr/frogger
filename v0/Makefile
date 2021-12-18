frogger: main.o Frog.o Level.o Lane.o AllegroEngine.o sound.o GameEngine.o
	gcc main.o Frog.o Level.o Lane.o AllegroEngine.o sound.o GameEngine.o -o frogger `pkg-config allegro-5 allegro_font-5 allegro_primitives-5 allegro_audio-5 allegro_acodec-5 allegro_image-5 allegro_ttf-5 --libs --cflags` -g

main.o: main.c objects/Lane.h objects/Level.h objects/GameEngine.h config.h libs/sound.h
	gcc -c main.c -o main.o -D ALLEGRO

Frog.o: objects/Frog.c objects/Frog.h libs/position.h
	gcc -c objects/Frog.c -o Frog.o

Lane.o: objects/Lane.c objects/Lane.h
	gcc -c objects/Lane.c -o Lane.o -D ALLEGRO

Level.o: objects/Level.c objects/Level.h objects/Frog.h objects/Lane.h config.h
	gcc -c objects/Level.c -o Level.o -D ALLEGRO

AllegroEngine.o: objects/AllegroEngine.c objects/AllegroEngine.h libs/sound.h objects/GameEngine.h
	gcc -c objects/AllegroEngine.c -o AllegroEngine.o

GameEngine.o: objects/GameEngine.c objects/GameEngine.h
	gcc -c objects/GameEngine.c -o GameEngine.o

sound.o: libs/sound.c libs/sound.h
	gcc -c libs/sound.c -o sound.o

clean:
	rm *.o