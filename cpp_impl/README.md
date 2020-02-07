
Libraries like frugally-deep were tested to predict directly in C++ but end up being significantly slower (by a factor of about 5x on every 1000 queries) than calling the python interpreter (probably because batching is not possible in frugally-deep, but is possible in tensorflow). 

For comparing/playing other ultimate tic-tac-toe bots:

`bot.cpp` when compiled will produce an executable that is capable of playing one game of ultimate tic-tac-toe, provided that the python and model files are in the appropriate locations. 
Inputs consist of the last move made by the opponent as two space-separated integers, the row and column of that move. If the executable is the first player, input `-1 -1` as the first input. The executable will terminate when the game is finished. The response to every input that does not terminate the game will be two space-separated integers representing the row and column of the calculated best move.

`arbiter.cpp` when compiled will produce an arbiter capable of playing games between two bots in the same format that `bot.cpp` outputs. Follow the prompts it outputs. You will need boost to compile.