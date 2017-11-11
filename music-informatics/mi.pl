%  for music parsing

% runs under sicstus Prolog on DICE

% start Prolog ( command ' sicstus '  on DICE ),  and consult this file:
%
%  | ?- [mi].
%

% example on front page of http://abcnotation.com/,
% paddy O'rafferty.
%
% dff cee|def gfe|dff cee|dfe dBA|
% dff cee|def gfe|faf gfe|dfe dBA

% as input
% assumed key: D major
% bar line in abc given as " | ', in Prolog input as " bl ".
% Uppercase symbols in abc need to have single quotes, eg " 'B' ".

%
% Line1 = [d,f,f,c,e,e,bl,d,e,f,g,f,e,bl,d,f,f,c,e,e,bl,d,f,e,d,'B','A',bl].
% Line2 = [d,f,f,c,e,e,bl,d,e,f,g,f,e,bl,f,a,f,g,f,e,bl,d,f,e,d,'B','A',bl].
% append(Line1,Line2,Input).

% less interesting version:
% Line1 = [d,f,f,d,b,d,bl,d,f,a,f,f,a,bl,d,g,'B',d,e,d,bl,d,f,'A',d,b,d,bl]
% Line2 = [d,f,f,d,b,d,bl,d,f,a,f,f,a,bl,d,g,'B',d,e,d,bl,d,f,'A',d,b,d,bl]
% append(Line1,Line2,Input).

input(Simple) :-
  Line1 = [d,f,f,d,b,d,bl,d,f,a,f,f,a,bl,d,g,'B',d,e,d,bl,d,f,'A',d,b,d,bl],
  Line2 = [d,f,f,d,b,d,bl,d,f,a,f,f,a,bl,d,g,'B',d,e,d,bl,d,f,'A',d,b,d,bl],
  append(Line1,Line2,Simple).

% show results up to 100 depth terms:

?- set_prolog_flag(toplevel_print_options,[quoted(true),numbervars(true),portrayed(true),max_depth(0)]).

% make dynamic

:- dynamic tune/2, line/2, bar1/2, bar/2, bar4/2, tonic/2, dominant/2.
:- dynamic subdominant/2, ton/2, by_ton/2, dom/2, by_dom/2, subd/2, by_subd/2.

% grammar for less interesting melody


tune --> line, line.
line --> bar1, bar, bar, bar4.

bar1 --> tonic.
bar  --> tonic.
bar  --> subdominant.
bar  --> dominant.
bar4 --> tonic.

tonic --> ton,by_ton,ton,ton,by_ton,ton,[bl].
dominant --> dom,by_dom,dom,dom,by_dom,dom,[bl].
subdominant --> subd,by_subd,subd,subd,by_subd,subd,[bl].

ton --> [a].
ton --> [d].
ton --> [f].
ton --> ['A'].
by_ton --> [b].
by_ton --> ton.

dom --> [a].
dom --> [c].
dom --> [e].
dom --> ['A'].
by_dom --> [f].
by_dom --> dom.

subd --> [b].
subd --> [d].
subd --> [g].
subd --> ['B'].
by_subd --> [e].
by_subd --> subd.

%% end of grammar

% Next procedure for random generation of melody following grammar;
%  NB: only works when each grammar symbol has a corresponding
%  "dynamic" declaration, as given above for the initial grammar

% randomised meta-interpreter
% runs under Sicstus 4.3.2

% NB: the clauses of object programs must be declared with
% the dynamic directive, eg " :- dynamic tune/2. "

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% random path through the search space --
% no back-tracking!

:- use_module(library(random)).
:- use_module(library(lists)).

ran_solve( true ) :- !.
ran_solve( (A,B) ) :- !, ran_solve(A), ran_solve(B).
ran_solve( P ) :- predicate_property(P, built_in), !, call(P).
ran_solve( HH ) :-  random_clause( HH, B ),
                    ran_solve(B).
random_clause( H, B ) :- setof( (H,B), clause(H,B), S),
                         random_pick( (H,B), S).
random_pick(X,List) :- length(List,N), rp(X,List,N).
rp(X,[X],_) :- !.
rp(X,List,N) :- random(0,N,Nth), nth0(Nth,List,X).

% special version for grammar top-level symbol.

random_tune( X, Y ) :- ran_solve( tune(X,Y) ).

% procedure to output query results to file

saveResult(Result,File) :-
    tell(File), write(Result), nl, told, nl,
    write('Result saved in '), write(File), write(' .'), nl.

% how to use:
%  the second argument is normally a sequence of alphanumeric characters,
% starting with a lower case letter;  to do anything else, put
% single quotes before and after the filename: eg  '~/results/r1' .
%
% for the first argument, use the variable that you use for the
% result you are computing, eg:
%
%  ?- tune( X, [] ), saveResult( X, myRandomMelody23 ).
%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% sample queries:

% to check input is parsed by this grammar:
% | ?- input(Input), tune(Input,[]).
% Input = [d,f,f,d,b,d,bl,d,f,a, ...
% yes
%
% for generation:
%
% | ?- tune( X, [] ).
% X = [a,b,a,a,b,a,bl,a,b,a,a,b,a,bl,a,b,a,a,b,a,bl,a,b,a,a,b,a,bl,a,b,a,a,b,a,bl,a,b,a,a,b,a,bl,a,b,a,a,b,a,bl,a,b,a,a,b,a,bl] ? ;
% X = [a,b,a,a,b,a,bl,a,b,a,a,b,a,bl,a,b,a,a,b,a,bl,a,b,a,a,b,a,bl,a,b,a,a,b,a,bl,a,b,a,a,b,a,bl,a,b,a,a,b,a,bl,a,b,a,a,b,d,bl]


% to call randomised:

% ?- random_tune( X, [] ).
% X = ['A',b,a,f,f,a,bl,f,b,f,a,d,'A',bl,c,e,a,e,e,a,bl,a,b,d,f,b,'A',bl,'A',b,f,'A',b,'A',bl,b,d,g,d,g,'B',bl,g,b,'B',g,e,d,bl,f,b,'A',d,b,f,bl] ? 
% yes
