% 2 задание

% Нужно использовать compound term figure(X, Y, Z), где X - большая фигура, Y - маленькая, Z - её цвет.
% Матрица - matrix(figure(...), X, Y), X - x координата, Y - y
matrix(figure(cir, tri, bl), 0, 0).
matrix(figure(tri, sq, wh), 1, 0).
matrix(figure(sq, cir, gr), 2, 0).
matrix(figure(tri, sq, gr), 0, 1).
matrix(figure(sq, cir, bl), 1, 1).
matrix(figure(cir, tri, wh), 2, 1).
matrix(figure(sq, cir, wh), 0, 2).
matrix(figure(cir, tri, gr), 1, 2).

find_ans(A) :- member(X, [cir, tri, sq]), member(Y, [cir, tri, sq]),
member(Z, [wh, gr, bl]), matrix(F, _, _), A=figure(X, Y, Z), F\=A, !.
