+++
date = '2026-02-13T18:16:01-08:00'
draft = false
title = 'Practica1: Elementos basicos de los lenguajes de programacion'
+++

#Cola de impresion en C
. uso de memoria estatica (arrays)
. no usar malloc/free
. capacidad fija a 10 trabajos
funciones:
- q_init
- q_is_empty
- q_is_full
- q_enqueue
- q_dequeue
- q_peek
- q_print

. si llena =>no agrega
. si vacia => no peek/dequeue

Estructura de "trabajo":
id:int -> Numero autoincremental
usuario: char [32]-> nombre del usuario que envio la imp
documento: char [42] -> titulo del documento
total_pgs:int -> numero total de paginas
restante_pgs: int -> paginas restabtes por immprimir
copias: int -> copas por imprimir
prioridad: enum -> [NORMAL, URGENTE]
estado: enum -> [EN_COLA, IMPRIMIENDO, COMPLETADO, CANCELADO]