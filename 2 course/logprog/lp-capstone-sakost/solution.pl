:- load_files(['database.pl']).
% :- debug.

% Num. 3
% 6 option. mother-in-law
motherinlaw(X, Y) :- 
    % determine a gender of the target
    sex(X, m),
    % find a child of the target(to determine wife)
    parent(X, Z),
    % find a wife(by finding a second parent of child of the target)
    parent(W, Z),
    sex(W, f),
    % clip backprop to not to use another children
    !,
    % find her(wife) parent(mother)
    parent(Y, W),
    % determine a gender to find the mother-in-law
    sex(Y, f).

% Num. 4
% Who is Y for X?

relative(X,Y,Z):-relative(X,Y,Z,'deep').

relative(["father"], X, Y, _) :- parent(Y, X), sex(Y, m).
relative(["mother"], X, Y, _) :- parent(Y, X), sex(Y, f).
relative(["daughter"], X, Y, _) :- parent(X, Y), sex(Y, f).
relative(["son"], X, Y, _) :- parent(X, Y), sex(Y, m).
relative(["sister"], X, Y, _) :- sex(Y, f), 
    parent(A, X), parent(B, X), parent(A, Y), parent(B, Y),
    sex(A, f), sex(B, m), X\=Y.
relative(["brother"], X, Y, _) :- sex(Y, m), 
    parent(A, X), parent(B, X), parent(A, Y), parent(B, Y),
    sex(A, f), sex(B, m), X\=Y.
relative(["wife"], X, Y, _) :- sex(Y, f), sex(X, m), parent(X, C), parent(Y, C).
relative(["husband"], Y, X, _) :- sex(Y, f), sex(X, m), parent(X, C), parent(Y, C).
relative(["motherinlaw"], X, Y, _) :- motherinlaw(X, Y).

relative([H1|Deep], X, Y, N) :- N = 'deep',
    relative(Deep, X, Int, 'deep'), X \= Int,
    relative(H, Int, Y, 'head'), Y \= Int, X \= Y, [H1|[]]=H.


transform_names(H-Deep, Name, [Ans|Ans2]):- transform_names(Deep, Name, Ans2), relative(H, Ans3, Ans), [Ans3|_]=Ans2.
transform_names(_-_, _, _) :- !, fail.
transform_names(H, Name, [Ans]) :- relative(H, Name, Ans) .

convert_relative(H-T, [H|Res]) :- convert_relative(T, Res), !.
convert_relative(H, [H]) :- !.