from UtttBoard import UtttBoard


class DuelManager:
    def play_game(self, bot0, bot1, debug=False):

        score0 = 0
        score1 = 0

        board = UtttBoard()

        to_move = 0

        while not board.get_game_ended():

            if to_move == 0:
                board.process_move(bot0.get_best_move(board))
            else:
                board.process_move(bot1.get_best_move(board))

            # if debug:
            # print("moved " + str(to_move))
            # print(board.to_string())

            # print("small win results " + str(board.get_small_win_results()))

            to_move = 1 - to_move

        winner = board.get_win_result()

        print(board.to_string())

        if winner == 0:
            score0 += 0.5
            score1 += 0.5
            if debug:
                print("draw")
        else:

            if winner == 1:
                score0 += 1
                if debug:
                    print("p0 won")
            else:
                score1 += 1
                if debug:
                    print("p1 won")

        if debug:
            print("scores are " + str(score0) + " to " + str(score1))

        return (score0, score1)

    def play_games(self, bot0, bot1, num_games, debug=False, use_gating=False, gating_threshold=0.0):
        score0 = 0
        score1 = 0

        if debug:
            print("starting ,gating threshold is " + str(gating_threshold))
        for i in range(num_games):

            bot0.reset()
            bot1.reset()

            if debug:
                print("playing " + str(i) + "th game")

            if i % 2 == 0:
                result = self.play_game(bot0, bot1, debug)
                score0 += result[0]
                score1 += result[1]
            else:
                result = self.play_game(bot1, bot0, debug)
                score1 += result[0]
                score0 += result[1]

            if debug:
                print("current scores " + str(score0) + " " + str(score1))

            if use_gating:
                # Assuming player 1 wins all remaining games, can we still pass the gating threshold? If not then break early

                remaining_games = num_games - i - 1

                if (score0 + remaining_games) / num_games < gating_threshold:
                    # rip, we can never pass the threshold
                    break

                # Assuming player 2 wins all remaining games, are we still ahead of the gating threshold? if so then break early

                if score0 / num_games >= gating_threshold:
                    break

        return (score0, score1)
