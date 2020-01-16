
from UtttBoard import UtttBoard
from classical_agents.RandomAgent import RandomAgent
from classical_agents.NaiveMCAgent import NaiveMCAgent
from DuelManager import DuelManager

result = DuelManager().play_games(RandomAgent(), NaiveMCAgent(), 10, debug = True, use_gating = False, gating_threshold = 0.0)

print("result is " + str(result[0]) + ' ' + str(result[1]))

'''
game_count = 20

bot0 = RandomAgent()
bot1 = NaiveMCAgent()

score0 = 0
score1 = 1

for i in range(game_count):

	board = UtttBoard()

	to_move = i % 2

	while not board.get_game_ended():

		if to_move == 0:
			board.process_move(bot0.get_best_move(board))
		else:
			board.process_move(bot1.get_best_move(board))

		print("moved " + str(to_move))
		to_move = 1 - to_move

	winner = board.get_win_result()

	if winner == 0:
		score0 += 0.5
		score1 += 0.5
		print("draw")
	else:

		if winner - 1 == i % 2:
			score0 += 1
			print("p0 won")
		else:
			score1 += 1
			print("p1 won")

print("scores are " + str(score0) + " to " + str(score1))
'''