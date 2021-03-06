:- ['four.pl'].

% First task

average(List, Res) :- length(List, Len), sum_list(List, Sum), Len > 0, Res is Sum/Len.
scores(Student, Res) :- subject(_, X), member(grade(Student, Res), X).
average_score_by_student(Student, Res) :- findall(N, scores(Student, N), L), average(L, Res).
average_score_every_student(Group, Res) :- group(Group, List), member(S, List), average_score_by_student(S, Res).

average_score_by_group(Group, Ans) :- group(Group, _), findall(Res, average_score_every_student(Group, Res), ListAverages), average(ListAverages, Ans).

% Second task

with_score_by_subject(Sub, Score, Name) :- subject(Sub, Students), member(grade(Name, Score), Students).
failed_students_by_subject(Sub, Name) :- with_score_by_subject(Sub, 2, Name).

list_failed_students_by_subject(Sub, ListNames) :- subject(Sub, _), findall(Stud, failed_students_by_subject(Sub, Stud), ListNames).

% Third task

with_score_by_group(Group, Score, Name) :- group(Group, Students), member(Name, Students), with_score_by_subject(_, Score, Name).
failed_students_by_group(Group, Name) :- with_score_by_group(Group, 2, Name).

count_failed_students_by_group(Group, Count) :- group(Group, _), setof(Name, failed_students_by_group(Group, Name), Students), length(Students, Count).
