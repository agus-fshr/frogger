machine := $(shell uname -m)
gccflags := -Wall
cheatflags := -D CHEAT


ifeq ($(machine), armv7l)
	target = froggerRPi
	maintarget = mainRPi.o
	dep = LEDMatrixEngine.o LEDlib.o
	linkedobj = $(dep) libs/joydisp/disdrv.o libs/joydisp/joydrv.o
	flag = -D LEDMAT
else 
	target = froggerAllegro
	maintarget = mainAllegro.o
	dep = sound.o AllegroEngine.o
	linkedobj = $(dep) `pkg-config allegro-5 allegro_font-5 allegro_primitives-5 allegro_audio-5 allegro_acodec-5 allegro_image-5 allegro_ttf-5 --libs --cflags`
	flag = -D ALLEGRO
endif

$(target): $(maintarget) Frog.o Level.o Lane.o $(dep) GameEngine.o
	gcc $(maintarget) Frog.o Level.o Lane.o $(linkedobj) GameEngine.o -o $(target)  -g

$(maintarget): main.c objects/Lane.h objects/Level.h objects/GameEngine.h config.h $(dep)
	gcc -c main.c -o $(maintarget) $(flag) $(gccflags)

Frog.o: objects/Frog.c objects/Frog.h
	gcc -c objects/Frog.c -o Frog.o $(gccflags)

Lane.o: objects/Lane.c objects/Lane.h
	gcc -c objects/Lane.c -o Lane.o $(flag) $(gccflags)

Level.o: objects/Level.c objects/Level.h objects/Frog.h objects/Lane.h config.h
	gcc -c objects/Level.c -o Level.o $(flag) $(gccflags)

AllegroEngine.o: objects/AllegroEngine.c objects/AllegroEngine.h libs/allegro_sound.h objects/GameEngine.h
	gcc -c objects/AllegroEngine.c -o AllegroEngine.o $(gccflags) $(cheatflags)

GameEngine.o: objects/GameEngine.c objects/GameEngine.h
	gcc -c objects/GameEngine.c -o GameEngine.o $(gccflags)

LEDMatrixEngine.o: objects/LEDMatrixEngine.c objects/LEDMatrixEngine.h objects/GameEngine.h libs/LEDlib.h
	gcc -c objects/LEDMatrixEngine.c -o LEDMatrixEngine.o $(gccflags)

LEDlib.o: libs/LEDlib.c libs/LEDlib.h libs/joydisp/disdrv.h
	gcc -c libs/LEDlib.c -o LEDlib.o $(gccflags)
	
allegro_sound.o: libs/allegro_sound.c libs/allegro_sound.h
	gcc -c libs/allegro_sound.c -o allegro_sound.o $(gccflags)

clean:
	rm *.o