import random

from Agent import Agent


class RandomAgent(Agent):
    def get_best_move(self, board):

        valid = board.get_valid_moves_vector()
        return valid[int(random.random() * len(valid))]
