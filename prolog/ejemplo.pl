likes(sam, pizza).
likes(sam, burgers).
likes(alex, pizza).

friend(X, Y) :- likes(X, Z), likes(Y, Z).