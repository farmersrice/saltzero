from SaltZeroAgent import SaltZeroAgent
from UtttBoard import UtttBoard
from TrainingManager import TrainingManager

print("hi")
board = UtttBoard()
temp_manager = TrainingManager()
bot = SaltZeroAgent(temp_manager.net, use_temp = False)


while not board.get_game_ended():

	print(board.to_string())

	print("your move: ")

	player_move = int(input())

	if not board.is_valid_move(player_move):
		print("bad move")
		continue

	board.process_move(player_move)

	if board.get_game_ended():
		break

	board.process_move(bot.get_best_move(board))


print("done, player " + str(board.get_win_result()) + " is the winner")