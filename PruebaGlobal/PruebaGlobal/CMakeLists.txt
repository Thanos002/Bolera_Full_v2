cmake_minimum_required(VERSION 3.21)
project(PruebaGlobal C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(PruebaGlobal
        Bolera.c
        Bolera.h
        ConfiguracionV1.c
        ConfiguracionV1.h
        Displays.c
        Displays.h
        Elevadores.c
        Elevadores.h
        IncFile1.h
        Lanzador.c
        Lanzador.h
        main.c
        Sensores_bolos.c
        Sensores_bolos.h)
