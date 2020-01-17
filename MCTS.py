import math
import time

import dotdict
import numpy as np

params = dotdict.dotdict(
    {
        "visits": 200,
        "c_puct": 1.1,  # from katago
        "alpha": 0.3,  # dirichlet noise parameter, set to 0.3 since that's what was used for chess,
        # since we can assume ~10 random plays in every game or so similar to chess
        # as explained in https://stats.stackexchange.com/questions/322831/purpose-of-dirichlet-noise-in-the-alphazero-paper
        "eps": 0.25,
    }
)


class MCTS:
    def visit(self, board, use_dirichlet_noise):

        board_hash = hash(board)

        if board_hash not in self.game_ended_cache:
            self.game_ended_cache[board_hash] = (board.get_game_ended(), board.get_win_result())

        if self.game_ended_cache[board_hash][0]:
            result = self.game_ended_cache[board_hash][1]

            if result == 0:
                return 0

            if result == board.cur_player:
                return -1

            return 1

        if board_hash in self.P:

            valid_moves = self.valid_move_cache[board_hash]

            if use_dirichlet_noise:

                # print("valid moves are " + str(valid_moves))
                num_valid_moves = np.sum(valid_moves)
                noise = np.random.dirichlet([params.alpha] * int(num_valid_moves))
                counter = 0

            upper_confidence_bounds = np.full((len(valid_moves)), -100000.0)

            for i in range(len(valid_moves)):
                if valid_moves[i] == 1:

                    edge = (board_hash, i)
                    if edge not in self.Q:
                        self.Q[edge] = 0
                        self.N[edge] = 0

                    this_P = self.P[board_hash][i]

                    if use_dirichlet_noise:
                        this_P = this_P * (1 - params.eps) + noise[counter] * params.eps
                        counter += 1

                    upper_confidence_bounds[i] = self.Q[edge] + params.c_puct * this_P * math.sqrt(
                        self.node_visit_count[board_hash]
                    ) / (1 + self.N[edge])

            best_move = np.argmax(upper_confidence_bounds)

            # next_board = board.deepcopy()

            board.process_move(best_move)

            # print("next board " + str(next_board))
            v = self.visit(board, False)

            edge = (board_hash, best_move)
            self.Q[edge] = (self.N[edge] * self.Q[edge] + v) / (self.N[edge] + 1)
            self.N[edge] += 1
            self.node_visit_count[board_hash] += 1

            return -v

        # leaf node, ask the network
        prediction = self.net.predict(board)
        self.P[board_hash], v = prediction[0][0], prediction[1][0][0]

        valid_moves = board.get_valid_moves()
        self.P[board_hash] = self.P[board_hash] * valid_moves
        total = np.sum(self.P[board_hash])

        self.visit_counter += 1
        # print("made prediction " + str(self.visit_counter))

        if total <= 0:
            print("network didn't predict any valid moves, choosing at random")
            print("valid moves sum is " + str(np.sum(valid_moves)))
            self.P[board_hash] = valid_moves / np.sum(valid_moves)
        else:
            self.P[board_hash] /= total

        self.valid_move_cache[board_hash] = valid_moves
        self.node_visit_count[board_hash] = 0

        return -v

    # According to some kind fellow in the leela zero discord, get_probabilities should be the policy network target

    def get_probabilities(self, board, is_training, temperature):
        start = time.time()
        board_hash = hash(board)

        self.visit_counter = 0
        for i in range(params.visits):
            self.visit(board.deepcopy(), is_training)

        valid_moves = self.valid_move_cache[board_hash]

        local_N = np.zeros(len(valid_moves))

        for i in range(len(valid_moves)):
            local_N[i] = self.N[(board_hash, i)] if (board_hash, i) in self.N else 0

        # print("local_N is " + str(local_N))

        print("get probabilities took " + str(time.time() - start))
        return local_N / np.sum(local_N)

    # THIS is controlled by temperature
    def get_probabilities_and_best_move(self, board, is_training, temperature):

        probabilities = self.get_probabilities(board, is_training, temperature)

        # print("got probabilities as " + str(probabilities))

        move = np.argmax(probabilities)

        if temperature == 1:
            # pick proportional to num visits

            labeled_moves = [i for i in range(81)]
            move = np.random.choice(labeled_moves, p=probabilities)

        # print("found best move " + str(move))

        return (probabilities, move)

    def __init__(self, net):
        self.net = net

        # as seen in the paper
        self.N = {}
        self.Q = {}
        self.P = {}
        self.node_visit_count = {}
        self.valid_move_cache = {}
        self.game_ended_cache = {}
