#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USER 32
#define MAX_DOC 48

typedef enum
{
    NORMAL = 0,
    URGENTE = 1
} Prioridad_t;

typedef enum
{
    EN_COLA = 0,
    IMPRIMIENDO = 1,
    COMPLETADO = 2,
    CANCELADO = 3
} Estado_t;

typedef struct
{
    int id;
    char usuario[MAX_USER];
    char documento[MAX_DOC];
    int paginas_total;
    int paginas_restantes;
    int copias;
    Prioridad_t prioridad;
    Estado_t estado;
    int ms_por_pagina;
} PrintJob_t;

typedef struct Node_t
{
    PrintJob_t job;
    struct Node_t *next;
} Node_t;

typedef struct
{
    Node_t *head;
    Node_t *tail;
    int size;
} QueueDynamic_t;

void qd_init(QueueDynamic_t *q);
int qd_is_empty(const QueueDynamic_t *q);
int qd_enqueue(QueueDynamic_t *q, PrintJob_t job);
int qd_peek(const QueueDynamic_t *q, PrintJob_t *out);
int qd_dequeue(QueueDynamic_t *q, PrintJob_t *out);
void qd_print(const QueueDynamic_t *q);
void qd_destroy(QueueDynamic_t *q);
void limpiar_salto(char *cadena);
void leer_cadena(const char *mensaje, char *destino, int tam);
int leer_entero(const char *mensaje, int minimo);
const char *prioridad_a_texto(Prioridad_t prioridad);
const char *estado_a_texto(Estado_t estado);
PrintJob_t crear_trabajo(int id);
void mostrar_trabajo(const PrintJob_t *job);

int main(void)
{
    QueueDynamic_t cola;
    PrintJob_t job;
    int opcion;
    int siguiente_id = 1;

    qd_init(&cola);

    do
    {
        printf("\n-------------------------------------\n");
        printf("   COLA DE IMPRESION MEMORIA DINAMICA\n");
        printf("-------------------------------------\n");
        printf("1. Agregar trabajo\n");
        printf("2. Ver siguiente trabajo (peek)\n");
        printf("3. Procesar trabajo (dequeue)\n");
        printf("4. Listar cola\n");
        printf("5. Salir\n");

        opcion = leer_entero("Selecciona una opcion: ", 1);

        switch (opcion)
        {
            case 1:
                job = crear_trabajo(siguiente_id);

                if (qd_enqueue(&cola, job))
                {
                    printf("\nTrabajo agregado correctamente\n");
                    mostrar_trabajo(&job);
                    siguiente_id++;
                }
                else
                {
                    printf("\nNo se pudo agregar el trabajo porque malloc regreso NULL\n");
                }
                break;

            case 2:
                if (qd_peek(&cola, &job))
                {
                    printf("\nSiguiente trabajo en la cola:\n");
                    mostrar_trabajo(&job);
                }
                else
                {
                    printf("\nLa cola esta vacia, no hay trabajo para procesar\n");
                }
                break;

            case 3:
                if (qd_dequeue(&cola, &job))
                {
                    printf("\nTrabajo procesado y removido de la cola:\n");
                    mostrar_trabajo(&job);
                }
                else
                {
                    printf("\nLa cola esta vacia, no hay trabajos para procesar\n");
                }
                break;

            case 4:
                qd_print(&cola);
                break;

            case 5:
                printf("\nLiberando memoria y terminando programa\n");
                qd_destroy(&cola);
                break;

            default:
                printf("\nOpcion no valida.\n");
                break;
        }

    } while (opcion != 5);

    return 0;
}

void qd_init(QueueDynamic_t *q)
{
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
}

int qd_is_empty(const QueueDynamic_t *q)
{
    return q->head == NULL;
}

int qd_enqueue(QueueDynamic_t *q, PrintJob_t job)
{
    Node_t *new_node;

    new_node = (Node_t *)malloc(sizeof(Node_t));
    if (new_node == NULL)
    {
        return 0;
    }

    new_node->job = job;
    new_node->next = NULL;

    if (q->tail == NULL)
    {
        q->head = new_node;
        q->tail = new_node;
    }
    else
    {
        q->tail->next = new_node;
        q->tail = new_node;
    }

    q->size++;

    return 1;
}

int qd_peek(const QueueDynamic_t *q, PrintJob_t *out)
{
    if (q->head == NULL)
    {
        return 0;
    }

    *out = q->head->job;
    return 1;
}

int qd_dequeue(QueueDynamic_t *q, PrintJob_t *out)
{
    Node_t *temp;

    if (q->head == NULL)
    {
        return 0;
    }

    temp = q->head;
    *out = temp->job;

    q->head = temp->next;

    if (q->head == NULL)
    {
        q->tail = NULL;
    }

    free(temp);
    q->size--;

    return 1;
}

void qd_print(const QueueDynamic_t *q)
{
    Node_t *current;
    int contador;

    if (qd_is_empty(q))
    {
        printf("\nLa cola esta vacia\n");
        return;
    }

    printf("\n------- COLA ACTUAL -------\n");
    printf("Cantidad de trabajos: %d\n\n", q->size);

    current = q->head;
    contador = 1;

    while (current != NULL)
    {
        printf("Trabajo #%d en la cola\n", contador);
        mostrar_trabajo(&(current->job));
        printf("-------------------------------\n");
        current = current->next;
        contador++;
    }
}

void qd_destroy(QueueDynamic_t *q)
{
    Node_t *current;
    Node_t *temp;

    current = q->head;

    while (current != NULL)
    {
        temp = current;
        current = current->next;
        free(temp);
    }

    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
}

void limpiar_salto(char *cadena)
{
    size_t len;

    len = strlen(cadena);
    if (len > 0 && cadena[len - 1] == '\n')
    {
        cadena[len - 1] = '\0';
    }
}

void leer_cadena(const char *mensaje, char *destino, int tam)
{
    do
    {
        printf("%s", mensaje);

        if (fgets(destino, tam, stdin) == NULL)
        {
            destino[0] = '\0';
            continue;
        }

        limpiar_salto(destino);

        if (strlen(destino) == 0)
        {
            printf("La entrada no puede estar vacia, intenta de nuevo.\n");
        }

    } while (strlen(destino) == 0);
}

int leer_entero(const char *mensaje, int minimo)
{
    char buffer[100];
    char *fin;
    long valor;

    while (1)
    {
        printf("%s", mensaje);

        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
        {
            printf("Error al leer la entrada.\n");
            continue;
        }

        valor = strtol(buffer, &fin, 10);

        if (fin == buffer)
        {
            printf("Debes escribir un numero entero valido.\n");
            continue;
        }

        while (*fin == ' ' || *fin == '\t')
        {
            fin++;
        }

        if (*fin != '\n' && *fin != '\0')
        {
            printf("Entrada invalida, intenta de nuevo\n");
            continue;
        }

        if (valor < minimo)
        {
            printf("El numero debe ser mayor o igual a %d.\n", minimo);
            continue;
        }

        return (int)valor;
    }
}

const char *prioridad_a_texto(Prioridad_t prioridad)
{
    if (prioridad == URGENTE)
    {
        return "URGENTE";
    }

    return "NORMAL";
}

const char *estado_a_texto(Estado_t estado)
{
    switch (estado)
    {
        case EN_COLA:
            return "EN_COLA";
        case IMPRIMIENDO:
            return "IMPRIMIENDO";
        case COMPLETADO:
            return "COMPLETADO";
        case CANCELADO:
            return "CANCELADO";
        default:
            return "DESCONOCIDO";
    }
}

PrintJob_t crear_trabajo(int id)
{
    PrintJob_t job;
    int prioridad_num;

    job.id = id;

    leer_cadena("Nombre del usuario: ", job.usuario, MAX_USER);
    leer_cadena("Nombre del documento: ", job.documento, MAX_DOC);

    job.paginas_total = leer_entero("Numero total de paginas: ", 1);
    job.paginas_restantes = job.paginas_total;

    job.copias = leer_entero("Numero de copias: ", 1);

    do
    {
        prioridad_num = leer_entero("Prioridad (0 = NORMAL, 1 = URGENTE): ", 0);

        if (prioridad_num != 0 && prioridad_num != 1)
        {
            printf("Solo se permite 0 o 1.\n");
        }

    } while (prioridad_num != 0 && prioridad_num != 1);

    job.prioridad = (Prioridad_t)prioridad_num;
    job.estado = EN_COLA;
    job.ms_por_pagina = 300;

    return job;
}

void mostrar_trabajo(const PrintJob_t *job)
{
    printf("ID: %d\n", job->id);
    printf("Usuario: %s\n", job->usuario);
    printf("Documento: %s\n", job->documento);
    printf("Paginas totales: %d\n", job->paginas_total);
    printf("Paginas restantes: %d\n", job->paginas_restantes);
    printf("Copias: %d\n", job->copias);
    printf("Prioridad: %s\n", prioridad_a_texto(job->prioridad));
    printf("Estado: %s\n", estado_a_texto(job->estado));
    printf("Ms por pagina: %d\n", job->ms_por_pagina);
}