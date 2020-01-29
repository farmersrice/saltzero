import os, sys
os.environ['CUDA_VISIBLE_DEVICES'] = '-1' # no gpu.

import tensorflow as tf
tf.compat.v1.disable_eager_execution()


from SaltZeroAgent import SaltZeroAgent
from UtttBoard import UtttBoard
from TrainingManager import TrainingManager


print("hi")
board = UtttBoard()
temp_manager = TrainingManager()
bot = SaltZeroAgent(temp_manager.net, use_temp = False)


print("Type 0 if you want to play first, and 1 if you want to play second")

which_player = int(input())


if which_player == 0:
	while not board.get_game_ended():

		print(board.to_string())

		print("your move: ")

		player_move = 0

		try: 
			player_move = int(input())
		except Exception as e:
			if isinstance(e, KeyboardInterrupt): # sometimes you need this, sometimes you don't
				sys.exit()

			print("Your move is not an integer")
			continue

		if not board.is_valid_move(player_move):
			print("bad move")
			continue

		board.process_move(player_move)

		if board.get_game_ended():
			break

		board.process_move(bot.get_best_move(board))
else:
	while not board.get_game_ended():

		board.process_move(bot.get_best_move(board))

		if board.get_game_ended():
			break

		print(board.to_string())

		print("your move: ")

		player_move = 0

		try: 
			player_move = int(input())
		except Exception as e:
			if isinstance(e, KeyboardInterrupt):
				sys.exit()

			print("Your move is not an integer")
			continue

		if not board.is_valid_move(player_move):
			print("bad move")
			continue

		board.process_move(player_move)

print("done, player " + str(board.get_win_result()) + " is the winner")
print("key: winner 0 means draw, winner 1 means first player won, winner 2 means second player won")