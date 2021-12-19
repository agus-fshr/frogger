# TPFinal de Programación I

## Características de la implementación
---
- Tres vehículos diferentes (de 1, 2, y 3 bloques de longitud) (limitado por el diseño)*.
- Diferentes longitudes de troncos flotantes (a discreción de la dificultad adecuada, no limitado por el diseño)*.
- Generación aleatoria de niveles (modo Endless), con escalado de dificultad conforme se avanza (velocidad y longitud de elementos).
- Tres vidas iniciales, rellenadas cada vez que se completan los 4 viajes a los diferentes puntos objetivo.
- Al morir, la rana reaparece en la posición inicial.
- Sonidos y música de fondo del juego original.
- Interfaz seleccionable: RPi / software, implementado mediante una abstracción "GameEngine" a la que se le asignan funciones de acuerdo a un if de compilación.
- Menú de pausa que permite retomar el juego, reiniciarlo, ver el puntaje actual (solo en vista de software), volver al menú principal, y controlar el volumen del juego.
- Menú de inicio que da la bienvenida al usuario, permite comenzar una partida, y ver el puntaje máximo obtenido.
- La base del diseño es Allegro en ambos casos de interfaz. Mediante un timer y FSMs se controla la lógica de juego, aplicando la interfaz abstracta al momento de manejar input del usuario y de graficar.
- Puntaje multiplicado por dificultad de nivel.

## Problemas conocidos y/o solucionados
---
- Gran leak de memoria en los gráficos, solucionado con Valgrind+gdb.
- Se conoce que hay un pequeño leak causado por el sistema de audio, ya que se debe esperar a terminar la reproducción para poder liberar la memoria ocupada por una pista. Esto no se pudo resolver ya que no se priorizó.
