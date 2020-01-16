SaltZero
=======

Machine learning bot for ultimate tic-tac-toe based on DeepMind's AlphaGo Zero paper. No human knowledge provided.

Ultimate tic-tac-toe is a game involving a 3 by 3 grid, each cell of which is itself a regular tic-tac-toe game, for a total board size of 9 by 9. Read the [Wikipedia page](https://en.wikipedia.org/wiki/Ultimate_tic-tac-toe) for a concise summary of the rules of the game.

For the general idea of the mechanism behind the bot, read the original paper, [Mastering the Game of Go without Human Knowledge](https://discovery.ucl.ac.uk/id/eprint/10045895/1/agz_unformatted_nature.pdf).

The goal is to produce a strong bot with easily understandable code – balancing speed with readability. There are a variety of other implementations of the AlphaGo Zero / AlphaZero idea online for a wide range of games. Other implementations usually fall into two categories: targeting either education and readability or pure speed. In the first case, implementations are often written in pure Python, and as a result are incredibly slow and unusable. In the second case, implementations are extremely large and convoluted and are difficult to understand. This implementation aims to strike a balance, although it focuses more on the readability side than the optimization side.

For a speed reference: more than 2500 games an hour can be played on an i7-8550U at 200 visits per move, using a network with 6 shared hidden dense layers of 512 nodes each. Please note that the current "whole_pipeline.cpp" is single-threaded and therefore is closer to 1800 games an hour.

# Features

- Concise C++ implementation for a speed advantage over Python

- Additional Python implementation for readability

- Machine learning is all conducted in Python and interfaces to C++ through the Python C API

    - Allows for neural networks and other machine learning models to be coded in Python while still putting the grunt work of game processing on C++. In the Python implementation, Python's slowness is the main bottleneck (ML takes < 1% of time spent), while in the C++ implementation, the neural network and Python/C++ communication are the main bottlenecks.

    - This C++/Python hybrid system is > 15 times faster than the pure Python implementation.

- Moderately strong pre-trained network (although not yet superhuman)

- Automatic deletion of games to save space (every 1000 games takes 200 mb)

- Batched gameplay for speed

    - Since batched queries to the neural network are often much faster than individual queries, both on CPU and GPU, multiple games are run in a pseudo-parallel fashion. In short, N games (in the code, N = 1000) are simultaneously played. Whenever a new visit in the game tree needs to occur, all games are queried for the positions to be evaluated and these are sent to the neural network in a batch. This provides a huge speed advantage. 

# Usage
Compile `whole_pipeline.cpp` and run it to train the network. 

Run `HumanVsRobotTest.py` in order to play against the neural network. In this setting the bot always plays the best move. Currently, playing as the second player is unsupported (you always make the first move). Your move must be an integer from 0 to 80, inclusive. Integers corresponding to board positions can be seen in the diagram below: 


		[ 0.,  1.,  2.,  9., 10., 11., 18., 19., 20.],
		[ 3.,  4.,  5., 12., 13., 14., 21., 22., 23.],
		[ 6.,  7.,  8., 15., 16., 17., 24., 25., 26.],
		[27., 28., 29., 36., 37., 38., 45., 46., 47.],
		[30., 31., 32., 39., 40., 41., 48., 49., 50.],
		[33., 34., 35., 42., 43., 44., 51., 52., 53.],
		[54., 55., 56., 63., 64., 65., 72., 73., 74.],
		[57., 58., 59., 66., 67., 68., 75., 76., 77.],
		[60., 61., 62., 69., 70., 71., 78., 79., 80.]


# Differences

This section lists some differences between this implementation and the original paper, as well as other implementations online. For anything not mentioned here, you can generally assume that the implementation follows the original paper.

- Rotations are added to training data, but random rotations are not used in the Monte Carlo Tree Search

- Rotations are not used in evaluator games; instead, temperature is turned on for the first 4 moves

- Evaluator uses > 50% gating at 200 games; in other words, a new net must win more than 100 out of 200 games against an old net to pass

- Uses Dirichlet noise as in the original paper, but this time with alpha = 0.3 to compensate for fewer possible moves (this is seen to be optimal based on the AlphaZero paper)

- Current neural network does not use convolutions

    - This is because the game rules clearly delineate the differences between small games and the big game. If a convolution overlaps multiple small games, its data will not be very useful, because cells next to each other are not inherently linked unless they are in the same small board (unlike in go where stones in adjacent cells become unified groups or snuff out liberties). Therefore only densely connected layers are used.

# Other notes

I lost once to the neural network, but since then I have beaten it in every game despite its increased training. Every time I beat the network I tested the strongest net against older nets in order to verify its strength, and it held up and beat them all. The network probably hasn't been trained for long enough to gain strength yet. 

There are a variety of ad-hoc changes I've made to the learning rate and the networks; there's a small text-only readme in the models folder detailing these changes.
