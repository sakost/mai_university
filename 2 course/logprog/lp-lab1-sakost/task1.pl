mylength([], 0):-!.
mylength([_H|Tail], Len):-
    mylength(Tail, TailLen),
    Len is TailLen + 1.

mymember(X, [X|_Tail]) :- !.
mymember(X, [_H|Tail]):-
   mymember(X, Tail).

myappend([], L2, L2).
myappend([H1|T1], L2, [H1|TailRes]) :- append(T1, L2, TailRes).

% removes first occurrence of Elem
myremove([], _Elem, []) :- !.
% if it needs to remove all occurrences, just uncomment next line and comment the next of next line
% myremove([Elem|T], Elem, ResultT) :- myremove(T, Elem, ResultT), !. 
myremove([Elem|T], Elem, T) :- !. % <--- this line to comment
myremove([OtherElem|T], Elem, [OtherElem|ResultT]) :- myremove(T, Elem, ResultT).

mypermute([], []).
mypermute([X|T], L) :- 
    mypermute(T, L1),
    select(X, L, L1).

mysublist([], _).
mysublist([X|XS], [X|XSS]) :- mysublist(XS, XSS), !.
mysublist([X|XS], [_|XSS]) :- mysublist([X|XS], XSS), !.
    

% first task part 1
% custom

mynth0(0, [Head|_], Head).
mynth0(N, [_|Tail], Elem) :-
    nonvar(N),
    M is N-1,
    mynth0(M, Tail, Elem).
mynth0(N, [_|Tail], Elem) :-
    var(N),
    mynth0(M, Tail, Elem),
    N is M+1.


% using built-in
mynth0_2(N, List, Elem) :- nth0(N, List, Elem).


% first task part 2
arithmetic_progression([], _, _) :- true, !.
% arithmetic_progression([X], Prev, D) :- X0 is Prev + D, X0 = X, !.
arithmetic_progression([X|Tail], Prev, D) :- X0 is Prev + D, X0 = X, arithmetic_progression(Tail, X, D), !.

arithmetic_progression([]) :- true, !.
arithmetic_progression([_]) :- arithmetic_progression([]), !.
arithmetic_progression([X, Y|Tail]) :- D is Y - X, arithmetic_progression([Y|Tail], X, D), !.

