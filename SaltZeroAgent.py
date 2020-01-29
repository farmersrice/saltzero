from MCTS import MCTS
from Agent import Agent

class SaltZeroAgent(Agent):
	
	def __init__(self, net, use_temp = True):
		self.net = net
		self.move_counter = 0
		self.use_temp = use_temp
		self.tree_search = MCTS(self.net)

	def get_best_move(self, board):
		self.move_counter += 1

		pi, v = self.tree_search.get_probabilities_and_best_move(board, False, 1 if self.move_counter <= 2 and self.use_temp else 0) # not training, temp = 0 (pick one with most visits)

		#temp 1 for first 2 moves to give variation

		print("moved " + str(v) + " with probability " + str(pi[v]))

		#print("probabilities are " + str(pi))
		
		return v

	def reset(self):
		self.move_counter = 0