
% :- op(100, xfy, bis).

trans(X, Left+Right) :- atom(Left), trans(X1, Left), trans(X2, Right).
trans(X, Left+Right) :- trans(X1, Left), trans(X2, Right).
trans(X, Left*Right) :- trans(X1).



trans(L, L+L) :- atom(L).
trans(R+L, R+L) :- atom(L), atom(R).

trans(L, L*L) :- atom(L).
trans(L*R, L*R) :- atom(L), atom(R).

trans(L, L) :- atom(L).

bis(X, R) :- trans(X, R).

subst(Z1,Z2,X,Res):-X=..[A,B,C],subst(Z1,Z2,B,B1),subst(Z1,Z2,C,C1),Res=..[A,B1,C1],!.
subst(Z1,Z2,Z1,Z2):-!.
subst(Z1,_,X,X):-X\=Z1,!.
