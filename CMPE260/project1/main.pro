% mehmet suzer
% 2019400213
% compiling: yes
% complete: yes
:- ['cmpecraft.pro'].

/* Manhattan distance */
abs_sub(X,Y,Result) :- X>Y, !, Result is X-Y.
abs_sub(X,Y,Result) :- Result is Y-X.

manhattan_distance([X1,Y1], [X2,Y2], Distance) :- 
    abs_sub(X1,X2,Xresult), abs_sub(Y1,Y2,Yresult), Distance is Xresult+Yresult.

% finds the manhattan distance of an object to agent
manhattan_distance_of_object(AgentX, AgentY, Object, Distance) :-
    get_dict(x, Object, X),
    get_dict(y, Object, Y),
    manhattan_distance([AgentX,AgentY], [X,Y], Distance).


/* Minimum Of List */
smaller(X,Y,X) :- X<Y, !.
smaller(_,Y,Y).

minimum_of_list([Element], Element) :- !.
minimum_of_list([Head|Rest], Result2) :- 
    minimum_of_list(Rest, Result1), smaller(Head, Result1, Result2).


/* Find Nearest Type */
% Gets an object from the ObjectDict at a given type and checks whether there is one which is closer to the agent.
% If not, then it is the nearest object to the agent at that type.
find_nearest_type(State, ObjectType, ObjKey, Object, Distance) :- 
    State = [AgentDict, ObjectDict, _], 
    get_dict(x, AgentDict, AgentX),
    get_dict(y, AgentDict, AgentY),
    get_dict(ObjKey, ObjectDict, Object),
    get_dict(type, Object, ObjectType),
    manhattan_distance_of_object(AgentX, AgentY, Object, Distance),
    \+ (get_dict(_, ObjectDict, Object2),
        get_dict(type, Object2, ObjectType),
        manhattan_distance_of_object(AgentX, AgentY, Object2, Distance2),
        Distance2 < Distance), !.


/* Navigate to */
% If the initial point and the final point are the same, then no action is required.
go_to(AgentX, AgentY, AgentX, AgentY, [], DepthLimit) :-
    DepthLimit>=0.
% If the agent is on the left side of final point, then go to right.
go_to(AgentX, AgentY, X, Y, [go_right|ActionList], DepthLimit) :-
    AgentX < X,
    DepthLimit>0,
    NewX is AgentX+1,
    NewDepthLimit is DepthLimit-1,
    go_to(NewX, AgentY, X, Y, ActionList, NewDepthLimit).
% If the agent is on the right side of final point, then go to left.
go_to(AgentX, AgentY, X, Y, [go_left|ActionList], DepthLimit) :-
    AgentX > X,
    DepthLimit>0,
    NewX is AgentX-1,
    NewDepthLimit is DepthLimit-1,
    go_to(NewX, AgentY, X, Y, ActionList, NewDepthLimit).
% If the agent is above the final point, then go to down.
go_to(AgentX, AgentY, X, Y, [go_down|ActionList], DepthLimit) :-
    AgentY < Y,
    DepthLimit>0,
    NewY is AgentY+1,
    NewDepthLimit is DepthLimit-1,
    go_to(AgentX, NewY, X, Y, ActionList, NewDepthLimit).
% If the agent is below the final point, then go to up.
go_to(AgentX, AgentY, X, Y, [go_up|ActionList], DepthLimit) :-
    AgentY > Y,
    DepthLimit>0,
    NewY is AgentY-1,
    NewDepthLimit is DepthLimit-1,
    go_to(AgentX, NewY, X, Y, ActionList, NewDepthLimit).

% generates the action list required to navigate to the point given.
navigate_to(State, X, Y, ActionList, DepthLimit) :-
    State = [AgentDict, _, _],
    get_dict(x, AgentDict, AgentX),
    get_dict(y, AgentDict, AgentY),
    go_to(AgentX, AgentY, X, Y, ActionList, DepthLimit), !.


/* Chop Nearest Tree */
% extend_to_end(SourceList1, SourceList2, ResultList).
% adds the second list at the end of the first list.
extend_to_end([], List, List) :- !.
extend_to_end([Head|Rest], List, [Head|Result]) :-
    extend_to_end(Rest, List, Result).

% generates the action list required to chop the nearest tree.
chop_nearest_tree(State, ActionList) :-
    find_nearest_type(State, tree, _, Tree, Distance),
    get_dict(x, Tree, X),
    get_dict(y, Tree, Y),
    navigate_to(State, X, Y, NavigateActionList, Distance),
    extend_to_end(NavigateActionList, [left_click_c, left_click_c, left_click_c, left_click_c], ActionList).


/* Mine Nearest Stone */
% generates the action list required to mine the nearest stone.
mine_nearest_stone(State, ActionList) :-
    find_nearest_type(State, stone, _, Stone, Distance),
    get_dict(x, Stone, X),
    get_dict(y, Stone, Y),
    navigate_to(State, X, Y, NavigateActionList, Distance),
    extend_to_end(NavigateActionList, [left_click_c, left_click_c, left_click_c, left_click_c], ActionList).


/* Mine Nearest Cobblestone */
% generates the action list required to mine the nearest cobblestone.
mine_nearest_cobblestone(State, ActionList) :-
    find_nearest_type(State, cobblestone, _, Stone, Distance),
    get_dict(x, Stone, X),
    get_dict(y, Stone, Y),
    navigate_to(State, X, Y, NavigateActionList, Distance),
    extend_to_end(NavigateActionList, [left_click_c, left_click_c, left_click_c, left_click_c], ActionList).


/* Gather Nearest Food */
% generates the action list required to gather nearest food.
gather_nearest_food(State, ActionList) :-
    find_nearest_type(State, food, _, Food, Distance),
    get_dict(x, Food, X),
    get_dict(y, Food, Y),
    navigate_to(State, X, Y, NavigateActionList, Distance),
    extend_to_end(NavigateActionList, [left_click_c], ActionList).


/* Collect Requirements */
% checks whether the object is in the dict.
% If it is, then it returns it count. If not, it returns 0.
find_count(Object, Dict, Count) :-
    get_dict(Object, Dict, Count), !.
find_count(_,_,0).

% list_of_n_elements(Element, N, List).
% generates a list of N Element.
list_of_n_elements(_, 0, []) :- !.
list_of_n_elements(Element, N, [Element|SubList]) :-
    M is N-1,
    list_of_n_elements(Element, M, SubList).

% Chops nearest trees to collect logs at a given count.
collect_log(State, Count, [], State) :- Count =< 0, !.
collect_log(State, Count, ActionList, FinalState) :-
    chop_nearest_tree(State, ActionList1),
    execute_actions(State, ActionList1, NextState),
    NewCount is Count-3,
    collect_log(NextState, NewCount, ActionList2, FinalState),
    extend_to_end(ActionList1, ActionList2, ActionList).

% mines nearest stones and cobblestons to collect cobblestones at a given count.
collect_cobblestone(State, Count, [], State) :- Count =< 0, !.
collect_cobblestone(State, Count, ActionList, FinalState) :-
    mine_nearest_stone(State, ActionList1), !,
    execute_actions(State, ActionList1, NextState),
    NewCount is Count-3,
    collect_cobblestone(NextState, NewCount, ActionList2, FinalState),
    extend_to_end(ActionList1, ActionList2, ActionList).
collect_cobblestone(State, Count, ActionList, FinalState) :-
    mine_nearest_cobblestone(State, ActionList1),
    execute_actions(State, ActionList1, NextState),
    NewCount is Count-1,
    collect_cobblestone(NextState, NewCount, ActionList2, FinalState),
    extend_to_end(ActionList1, ActionList2, ActionList).

% first, it collects logs, craft sticks at the required count, and updates the state.
% second, it collects logs at the required count and updates the state again.
% finally, it collect cobblestones as many as needed and returns the total action list. 
collect_requirements(State, ItemType, ActionList) :-
    item_info(ItemType, Requirements, _),
    State = [AgentDict1, _, _],
    get_dict(inventory, AgentDict1, Inv1),
    find_count(stick, Requirements, RequiredStick),
    find_count(stick, Inv1, StickCount),
    StickNeeded is RequiredStick-StickCount,
    LogNeeded is ((StickNeeded-1)//4 + 1)*2,
    collect_log(State, LogNeeded, ActionList1, NextState1),
    list_of_n_elements(craft_stick, StickNeeded, ActionList2),
    extend_to_end(ActionList1, ActionList2, ActionList3),
    (
        ActionList2=[] ->
        (
            NextState2 = NextState1
        );
        (
        execute_actions(NextState1, ActionList2, NextState2)
        )
    ),
    NextState2 = [AgentDict2, _, _],
    get_dict(inventory, AgentDict2, Inv2),
    find_count(log, Requirements, RequiredLog),
    find_count(log, Inv2, LogCount),
    LogNeeded2 is RequiredLog-LogCount,
    collect_log(NextState2, LogNeeded2, ActionList4, NextState3),
    extend_to_end(ActionList3, ActionList4, ActionList5),
    find_count(cobblestone, Requirements, RequiredCobblestone),
    find_count(cobblestone, Inv2, CobblestoneCount),
    CobblestoneNeeded is RequiredCobblestone-CobblestoneCount,
    collect_cobblestone(NextState3, CobblestoneNeeded, ActionList6, _),
    extend_to_end(ActionList5, ActionList6, ActionList).


/* Find Castle Location */
% exits successfully, if the given point is occupied by food, tree, cobblestone, or stone.
tile_is_full(X, Y, State) :-
    State = [_, StateDict, _],
    get_dict(_, StateDict, Object),
    get_dict(x, Object, Ox),
    get_dict(y, Object, Oy),
    X = Ox, Y = Oy.

% exits successfully, if the given point is not occupied by food, tree, cobblestone, or stone.
tile_is_empty(X, Y, State) :- 
    tile_is_full(X, Y, State), !, fail.
tile_is_empty(_,_,_).

% exits successfully, if the 3x3 area given is empty.
% X and Y coordinates point the upper left corner of the area.
find_castle_location_helper(State, X, Y) :-
    tile_is_empty(X, Y, State),
    X1 is X+1,
    tile_is_empty(X1, Y, State),
    X2 is X1+1,
    tile_is_empty(X2, Y, State),
    Y1 is Y+1,
    tile_is_empty(X, Y1, State),
    tile_is_empty(X1, Y1, State),
    tile_is_empty(X2, Y1, State),
    Y2 is Y1+1,
    tile_is_empty(X, Y2, State),
    tile_is_empty(X1, Y2, State),
    tile_is_empty(X2, Y2, State).

% Starting from the upper left corner, it checks 3x3 areas recursively.
% If it finds an empty 3x3 area, upper left coordinates are returned.
find_castle_location_rec(State, X, Y, XMin, YMin) :-
    width(W), height(H),
    X < W-1, Y < H-1,
    (   
    	find_castle_location_helper(State, X, Y) ->  
    	(   
        	XMin is X, YMin is Y
        );
    	(   
        	X =:= W-2 ->
        	(   
            	NewY is Y+1,
            	find_castle_location_rec(State, 1, NewY, XMin, YMin)
            );
        	(   
            	NewX is X+1,
            	find_castle_location_rec(State, NewX, Y, XMin, YMin)
            )
        )
    ).

% Finds a 3x3 empty area
find_castle_location(State, XMin, YMin, XMax, YMax) :-
    find_castle_location_rec(State, 1, 1, XMin, YMin),
    XMax is XMin+2,	YMax is YMin+2.

/* Make Castle */
% Finds a castle location, goes there, and places cobblestones at that location
make_castle(State, ActionList) :-
    width(W), height(H),
    MaxDepth is W+H,
    collect_cobblestone(State, 9, ActionList1, NextState1),
    find_castle_location(NextState1, XMin, YMin, _, _),
    navigate_to(NextState1, XMin, YMin, ActionList2, MaxDepth),
    extend_to_end(ActionList2, [place_c, go_right, place_c, go_right, place_c, go_down, place_c, go_left, place_c, go_left, place_c, go_down, place_c, go_right, place_c, go_right, place_c], ActionList3),
    extend_to_end(ActionList1, ActionList3, ActionList).

:- init_from_map.

% 10 points
% manhattan_distance(+A, +B, -Distance) :- .
% 10 points
% minimum_of_list(+List, -Minimum) :- .
% 10 points
% find_nearest_type(+State, +ObjectType, -ObjKey, -Object, -Distance) :- .
% 10 points
% navigate_to(+State, +X, +Y, -ActionList, +DepthLimit) :- .
% 10 points
% chop_nearest_tree(+State, -ActionList) :- .
% 10 points
% mine_nearest_stone(+State, -ActionList) :- .
% 10 points
% gather_nearest_food(+State, -ActionList) :- .
% 10 points
% collect_requirements(+State, +ItemType, -ActionList) :- .
% 5 points
% find_castle_location(+State, -XMin, -YMin, -XMax, -YMax) :- .
% 15 points
% make_castle(+State, -ActionList) :- .
