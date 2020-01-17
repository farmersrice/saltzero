import random

from Agent import Agent


class NaiveMCAgent(Agent):
    def __init__(self):
        pass

    def get_random_move(self, board):
        valid = board.get_valid_moves_vector()
        return valid[int(random.random() * len(valid))]

    def playout(self, board):
        temp_board = board.deepcopy()
        starting_player = board.cur_player

        # print("start playout")
        while not temp_board.get_game_ended():
            temp_board.process_move(self.get_random_move(temp_board))

        result = temp_board.get_win_result()

        # print("finished playout")

        if result == 0:
            return 0.5
        if result == starting_player:
            # we're the current player
            return 1.0

        return 0.0

    def get_score(self, board):
        answer = 0
        for i in range(10):
            # perform playouts
            answer += self.playout(board)

        return answer

    def get_best_move(self, board):

        valid = board.get_valid_moves_vector()

        scores = [0 for x in range(len(valid))]

        # just for now do some stupid iteration

        max_score = 100000000
        best_pos = 0

        for i in range(len(valid)):
            temp_board = board.deepcopy()
            temp_board.process_move(valid[i])

            scores[i] = self.get_score(temp_board)

            if scores[i] < max_score:
                max_score = scores[i]
                best_pos = valid[i]

        return best_pos
        """
		iterations = 1000

		for asdf in range(iterations):

			for i in range(len(valid)):
				temp_board = board.deepcopy()
				temp_board.process_move(valid[i])

				scores[i] = self.get_score(temp_board)

			#Pick the best one 
		"""
