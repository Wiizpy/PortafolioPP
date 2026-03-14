#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

typedef struct
{
    int trabajos_completados;
    int trabajos_cancelados;
    int paginas_impresas;
} Estadisticas_t;

void qd_init(QueueDynamic_t *q);
int qd_is_empty(const QueueDynamic_t *q);
int qd_enqueue(QueueDynamic_t *q, PrintJob_t job);
int qd_peek(const QueueDynamic_t *q, PrintJob_t *out);
int qd_dequeue(QueueDynamic_t *q, PrintJob_t *out);
void qd_print(const QueueDynamic_t *q);
void qd_destroy(QueueDynamic_t *q);
int qd_cancel_by_id(QueueDynamic_t *q, int id, PrintJob_t *cancelado);
void limpiar_salto(char *cadena);
void leer_cadena(const char *mensaje, char *destino, int tam);
int leer_entero_simple(const char *mensaje);
const char *prioridad_a_texto(Prioridad_t prioridad);
const char *estado_a_texto(Estado_t estado);
PrintJob_t crear_trabajo(int id);
void mostrar_trabajo(const PrintJob_t *job);
void mostrar_menu(void);
void esperar_ms(int ms);
void simular_impresion(QueueDynamic_t *q, Estadisticas_t *stats);
void mostrar_estadisticas(const Estadisticas_t *stats);

int main(void)
{
    QueueDynamic_t cola;
    Estadisticas_t stats;
    PrintJob_t job;
    PrintJob_t cancelado;
    int opcion;
    int siguiente_id;
    int id_cancelar;

    qd_init(&cola);

    stats.trabajos_completados = 0;
    stats.trabajos_cancelados = 0;
    stats.paginas_impresas = 0;

    siguiente_id = 1;

    do
    {
        mostrar_menu();
        opcion = leer_entero_simple("Selecciona una opcion: ");

        switch (opcion)
        {
            case 1:
                job = crear_trabajo(siguiente_id);

                if (qd_enqueue(&cola, job))
                {
                    printf("\nTrabajo agregado correctamente.\n");
                    mostrar_trabajo(&job);
                    siguiente_id++;
                }
                else
                {
                    printf("\nNo se pudo agregar el trabajo.\n");
                }
                break;

            case 2:
                if (qd_peek(&cola, &job))
                {
                    printf("\n===== SIGUIENTE TRABAJO =====\n");
                    mostrar_trabajo(&job);
                }
                else
                {
                    printf("\nLa cola esta vacia.\n");
                }
                break;

            case 3:
                if (qd_dequeue(&cola, &job))
                {
                    printf("\nTrabajo removido del frente de la cola\n");
                    mostrar_trabajo(&job);
                }
                else
                {
                    printf("\nLa cola esta vacia.\n");
                }
                break;

            case 4:
                qd_print(&cola);
                break;

            case 5:
                if (qd_is_empty(&cola))
                {
                    printf("\nLa cola esta vacia, no hay nada que cancelar\n");
                }
                else
                {
                    id_cancelar = leer_entero_simple("Ingresa el ID del trabajo a cancelar: ");

                    if (qd_cancel_by_id(&cola, id_cancelar, &cancelado))
                    {
                        cancelado.estado = CANCELADO;
                        stats.trabajos_cancelados++;

                        printf("\nTrabajo cancelado correctamente\n");
                        mostrar_trabajo(&cancelado);
                    }
                    else
                    {
                        printf("\nNo se encontro un trabajo con ese ID\n");
                    }
                }
                break;

            case 6:
                if (qd_is_empty(&cola))
                {
                    printf("\nLa cola esta vacia, no hay trabajos para simular\n");
                }
                else
                {
                    simular_impresion(&cola, &stats);
                }
                break;

            case 7:
                mostrar_estadisticas(&stats);
                break;

            case 8:
                printf("\nLiberando memoria y terminando programa\n");
                qd_destroy(&cola);
                break;

            default:
                printf("\nError seleccione una opcion correcta\n");
                break;
        }

    } while (opcion != 8);

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

    /* mejora 1: prioridad */
    if (job.prioridad == URGENTE)
    {
        if (q->head == NULL)
        {
            q->head = new_node;
            q->tail = new_node;
        }
        else
        {
            new_node->next = q->head;
            q->head = new_node;
        }
    }
    else
    {
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
        printf("\nLa cola esta vacia.\n");
        return;
    }

    printf("\n-------------------------------------------------\n");
    printf("                 COLA ACTUAL\n");
    printf("--------------------------------------------------\n");
    printf("Cantidad de trabajos: %d\n\n", q->size);

    current = q->head;
    contador = 1;

    while (current != NULL)
    {
        printf("Trabajo #%d en la cola\n", contador);
        mostrar_trabajo(&(current->job));
        printf("--------------------------------------------------\n");
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

int qd_cancel_by_id(QueueDynamic_t *q, int id, PrintJob_t *cancelado)
{
    Node_t *current;
    Node_t *previous;

    if (q->head == NULL)
    {
        return 0;
    }

    current = q->head;
    previous = NULL;

    while (current != NULL)
    {
        if (current->job.id == id)
        {
            *cancelado = current->job;

            if (previous == NULL)
            {
                q->head = current->next;
            }
            else
            {
                previous->next = current->next;
            }

            if (current == q->tail)
            {
                q->tail = previous;
            }

            free(current);
            q->size--;
            return 1;
        }

        previous = current;
        current = current->next;
    }

    return 0;
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
    printf("%s", mensaje);

    if (fgets(destino, tam, stdin) != NULL)
    {
        limpiar_salto(destino);
    }
    else
    {
        destino[0] = '\0';
    }
}

int leer_entero_simple(const char *mensaje)
{
    char buffer[100];
    int numero;

    printf("%s", mensaje);
    fgets(buffer, sizeof(buffer), stdin);
    sscanf(buffer, "%d", &numero);

    return numero;
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

    job.paginas_total = leer_entero_simple("Numero total de paginas: ");
    if (job.paginas_total < 1)
    {
        job.paginas_total = 1;
    }

    job.paginas_restantes = job.paginas_total;

    job.copias = leer_entero_simple("Numero de copias: ");
    if (job.copias < 1)
    {
        job.copias = 1;
    }

    prioridad_num = leer_entero_simple("Prioridad (0 = NORMAL, 1 = URGENTE): ");
    if (prioridad_num != 1)
    {
        prioridad_num = 0;
    }

    job.prioridad = (Prioridad_t)prioridad_num;
    job.estado = EN_COLA;

    job.ms_por_pagina = leer_entero_simple("Milisegundos por pagina: ");
    if (job.ms_por_pagina < 1)
    {
        job.ms_por_pagina = 300;
    }

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

void mostrar_menu(void)
{
    printf("\n--------------------------------------------------\n");
    printf("       COLA DE IMPRESION         \n");
    printf("---------------------------------------------------\n");
    printf("1. Agregar trabajo\n");
    printf("2. Ver siguiente trabajo (peek)\n");
    printf("3. Remover trabajo del frente (dequeue)\n");
    printf("4. Listar cola\n");
    printf("5. Cancelar trabajo por ID\n");
    printf("6. Simular impresion de toda la cola\n");
    printf("7. Ver estadisticas\n");
    printf("8. Salir\n");
}

void esperar_ms(int ms)
{
    clock_t inicio;
    double tiempo_objetivo;

    inicio = clock();
    tiempo_objetivo = (double)ms / 1000.0;

    while (((double)(clock() - inicio) / CLOCKS_PER_SEC) < tiempo_objetivo)
    {
    }
}

void simular_impresion(QueueDynamic_t *q, Estadisticas_t *stats)
{
    PrintJob_t job;
    int pagina_actual;
    int total_paginas_real;

    printf("---------INICIANDO SIMULACION-------\n");

    while (qd_dequeue(q, &job))
    {
        total_paginas_real = job.paginas_total * job.copias;
        job.paginas_restantes = total_paginas_real;
        job.estado = IMPRIMIENDO;

        printf("\n******************************************\n");
        printf("Imprimiendo trabajo ID %d\n", job.id);
        printf("Usuario: %s\n", job.usuario);
        printf("Documento: %s\n", job.documento);
        printf("Prioridad: %s\n", prioridad_a_texto(job.prioridad));
        printf("Estado actual: %s\n", estado_a_texto(job.estado));
        printf("Paginas totales a imprimir: %d\n", total_paginas_real);
        printf("******************************************\n");

        for (pagina_actual = 1; pagina_actual <= total_paginas_real; pagina_actual++)
        {
            printf("Progreso -> pagina %d de %d\n", pagina_actual, total_paginas_real);
            job.paginas_restantes--;
            esperar_ms(job.ms_por_pagina);
        }

        job.estado = COMPLETADO;

        printf("Trabajo completado.\n");
        printf("Estado final: %s\n", estado_a_texto(job.estado));

        stats->trabajos_completados++;
        stats->paginas_impresas += total_paginas_real;
    }
    printf("      SIMULACION FINALIZADA CORRECTAMENTE\n");
}

void mostrar_estadisticas(const Estadisticas_t *stats)
{
    printf("\n==================================================\n");
    printf("                ESTADISTICAS\n");
    printf("==================================================\n");
    printf("Trabajos completados: %d\n", stats->trabajos_completados);
    printf("Trabajos cancelados: %d\n", stats->trabajos_cancelados);
    printf("Paginas impresas: %d\n", stats->paginas_impresas);
    printf("==================================================\n");
}