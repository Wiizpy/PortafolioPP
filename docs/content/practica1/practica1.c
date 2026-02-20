#include <stdio.h>
#include <string.h>

#define MAX 10
trabajo cola[MAX];
int frente;
int final;
int contador;

typedef enum 
{
     NORMAL, URGENTE 
} 
Prioridad;

typedef enum 
{ 
    EN_COLA, IMPRIMIENDO, COMPLETADO, CANCELADO 
} Estado;

typedef struct
{
    int id;
    char usuario[32];
    char documento[42];
    int total_pgs;
    int restante_pgs;
    int copias;
    Prioridad prioridad;
    Estado estado;

}trabajo;

void q_init()
{
    frente = 0;
    final = -1;
    contador = 1;
}

int q_is_empty()
{

}

int q_is_full()
{

}
