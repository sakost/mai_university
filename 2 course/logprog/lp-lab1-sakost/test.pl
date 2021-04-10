subst(Z1,Z2,X,Res):-X=..[A,B,C],subst(Z1,Z2,B,B1),subst(Z1,Z2,C,C1),Res=..[A,B1,C1],!.
subst(Z1,Z2,Z1,Z2):-!.
subst(Z1,_,X,X):-X\=Z1,!.

del_last([_], []).
del_last([H|T1], [H|T2]) :- del_last(T1, T2).

% prefix
razb(X, L) :- X=..[A, B, C], razb(B, L1), razb(C, L2), string_chars(A, Op), append(Op, L1, Ls), append(Ls, L2, L),!.
razb(X, [X]) :- !.

% postfix
razb(X, L) :- X=..[A, B, C], razb(B, L1), razb(C, L2), string_chars(A, Op), append(L1, L2, Ls), append(Ls, Op, L), !.
razb(X, [X]) :- !.

% Игорь factorial
fact(0,1).
fact(X,R):-fact(X,1,1,R).
fact(X,N,H,R):-X>N,H1 is H*N,N1 is N + 1,fact(X,N1,H1,R).
fact(X,N,H,H1):-X=N,H1 is H*N,!.

% Паша
factorial(0, 1).
factorial(X, Result) :- factorial(Y, Res1), X is Y + 1, Result is (Res1 * (Y+1)).

% fibbonacci
fib(N,R):-fib(N,R,3,1,1).
fib(N,N3,H,N1,N2):-N = H,N3 is N1 + N2,!.
fib(N,R,H,N1,N2):-N>H,N3 is N1 + N2,H1 is H + 1,fib(N,R,H1,N2,N3).


% last element
smth(List, X):- append(_, [X], List).

% sum of n first
sum(L,N):- sum(L,0,N).
sum([X|T],S,N):- S1 is S+X, sum(T,S1,N).
sum([],N,N).

% y^n = x
st(F,1,F):- !.
st(Y,N,X):- N1 is N-1, st(Y,N1,X1), X is X1*Y.


test --> append([_, _]).

% search
move_w([0,w|L],[w,0|L]).
move_w([0,X,w|L],[w,X,0|L]).
move_w([X|L1],[X|L2]):-move_w(L1,L2).

move_b([b,0|L],[0,b|L]).
move_b([b,X,0|L],[0,X,b|L]).
move_b([X|L1],[X|L2]):-move_b(L1,L2).

move(S,N):-move_w(S,N).
move(S,N):-move_b(S,N).
move(S,S).

path(Start,Finish):-path(Start,Finish,[Start],Path),reverse(Path,R),write_path(R).
path(Start,Start,Path,Path).
path(Start,Finish,List,Path):-move(Start,Z),not(member(Z, List)),path(Z,Finish,[Z|List],Path).

write_path([]):-write("-------------\n").
write_path([X|L]):-write(X),write("\n"),write_path(L).
prolong([H|T],[X,H|T]):-move(H,X),not(member(X,[H|T])).
path_min(X,Y):-bdth([[X]],Y,P),reverse(P,R),write_path(R).
bdth([[X|T]|_],X,[X|T]).
bdth([P|QI],X,R):-findall(Z,prolong(P,Z),T),append(QI,T,QO),!,bdth(QO,X,R).

iter(1).
iter(M):-iter(N),M is N + 1.

iter_2(X,M):-iter_2(X,1,M).
iter_2(X,Y,M):-Y<M,X is Y.
iter_2(X,Y,M):-Y<M,Y1 is Y+1,iter_2(X,Y1,M).

search_id(Start,Finish):-iter_2(Level,30),search_id([Start],Finish,Path,Level),reverse(Path,P),write_path(P).
search_id(T,Finish,T,0).
search_id(Path,Finish,R,N):-N>0, N < 30,prolong(Path,NewPath),N1 is N - 1,search_id(NewPath,Finish,R,N1).

call_time(G,T) :-
statistics(runtime,[T0|_]),
G,
statistics(runtime,[T1|_]),
T is T1 - T0.

del_first(L, A) :- append([_, _, _ ], A, L).

func(X, A) :- findall(E, (member(E,X), M is E mod 3, M=0), L), sort(L, [A|_]).