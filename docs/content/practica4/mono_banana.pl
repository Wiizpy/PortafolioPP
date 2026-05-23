
% Estado:
% estado(PosicionMono, AlturaMono, PosicionCaja, TieneBanana)

% Lugares posibles
lugar(puerta).
lugar(ventana).
lugar(centro).

% El mono puede caminar de un lugar a otro si está en el suelo
mover(
    estado(X, suelo, Caja, Tiene),
    caminar(X, Y),
    estado(Y, suelo, Caja, Tiene)
) :-
    lugar(X),
    lugar(Y),
    X \== Y.

% El mono puede empujar la caja si está en el mismo lugar que la caja
mover(
    estado(X, suelo, X, Tiene),
    empujar_caja(X, Y),
    estado(Y, suelo, Y, Tiene)
) :-
    lugar(X),
    lugar(Y),
    X \== Y.

% El mono puede subirse a la caja si está en el mismo lugar que la caja
mover(
    estado(X, suelo, X, Tiene),
    subir_caja,
    estado(X, arriba, X, Tiene)
).

% El mono puede tomar la banana si está en el centro, arriba de la caja y aún no la tiene
mover(
    estado(centro, arriba, centro, no),
    tomar_banana,
    estado(centro, arriba, centro, si)
).

% Inicio de la búsqueda
puede_alcanzar(EstadoInicial, Acciones) :-
    buscar(EstadoInicial, [], Acciones).

% Caso base: si ya tiene la banana, termina
buscar(
    estado(_, _, _, si),
    _,
    []
).

% Caso recursivo: busca acciones evitando repetir estados
buscar(
    EstadoActual,
    Visitados,
    [Accion | RestoAcciones]
) :-
    mover(EstadoActual, Accion, EstadoNuevo),
    \+ member(EstadoNuevo, Visitados),
    buscar(EstadoNuevo, [EstadoNuevo | Visitados], RestoAcciones).