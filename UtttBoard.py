import numpy as np


class BoardHelper:
    @staticmethod
    def rotate90(board, use_floats=False):
        # board should be length 81
        # you may wonder, why does this look so silly?

        # actually i just rotated the matrix in numpy and converted it to assignments

        # this is much faster than having 2d arrays and going back and forth and little objects and such
        """
		[ 0.,  1.,  2.,  9., 10., 11., 18., 19., 20.],
		[ 3.,  4.,  5., 12., 13., 14., 21., 22., 23.],
		[ 6.,  7.,  8., 15., 16., 17., 24., 25., 26.],
		[27., 28., 29., 36., 37., 38., 45., 46., 47.],
		[30., 31., 32., 39., 40., 41., 48., 49., 50.],
		[33., 34., 35., 42., 43., 44., 51., 52., 53.],
		[54., 55., 56., 63., 64., 65., 72., 73., 74.],
		[57., 58., 59., 66., 67., 68., 75., 76., 77.],
		[60., 61., 62., 69., 70., 71., 78., 79., 80.]


		to

		[20., 23., 26., 47., 50., 53., 74., 77., 80.],
		[19., 22., 25., 46., 49., 52., 73., 76., 79.],
		[18., 21., 24., 45., 48., 51., 72., 75., 78.],
		[11., 14., 17., 38., 41., 44., 65., 68., 71.],
		[10., 13., 16., 37., 40., 43., 64., 67., 70.],
		[ 9., 12., 15., 36., 39., 42., 63., 66., 69.],
		[ 2.,  5.,  8., 29., 32., 35., 56., 59., 62.],
		[ 1.,  4.,  7., 28., 31., 34., 55., 58., 61.],
		[ 0.,  3.,  6., 27., 30., 33., 54., 57., 60.]

		then

		for i in range(9):
			for j in range(9):
				print('temp[' + str(int(old_array[i][j])) + '] = self.board[' + str(int(new_array[i][j])) + ']')
		
		"""

        if use_floats:
            temp = np.zeros(81)
        else:
            temp = np.zeros(81, dtype=int)

        temp[0] = board[20]
        temp[1] = board[23]
        temp[2] = board[26]
        temp[9] = board[47]
        temp[10] = board[50]
        temp[11] = board[53]
        temp[18] = board[74]
        temp[19] = board[77]
        temp[20] = board[80]
        temp[3] = board[19]
        temp[4] = board[22]
        temp[5] = board[25]
        temp[12] = board[46]
        temp[13] = board[49]
        temp[14] = board[52]
        temp[21] = board[73]
        temp[22] = board[76]
        temp[23] = board[79]
        temp[6] = board[18]
        temp[7] = board[21]
        temp[8] = board[24]
        temp[15] = board[45]
        temp[16] = board[48]
        temp[17] = board[51]
        temp[24] = board[72]
        temp[25] = board[75]
        temp[26] = board[78]
        temp[27] = board[11]
        temp[28] = board[14]
        temp[29] = board[17]
        temp[36] = board[38]
        temp[37] = board[41]
        temp[38] = board[44]
        temp[45] = board[65]
        temp[46] = board[68]
        temp[47] = board[71]
        temp[30] = board[10]
        temp[31] = board[13]
        temp[32] = board[16]
        temp[39] = board[37]
        temp[40] = board[40]
        temp[41] = board[43]
        temp[48] = board[64]
        temp[49] = board[67]
        temp[50] = board[70]
        temp[33] = board[9]
        temp[34] = board[12]
        temp[35] = board[15]
        temp[42] = board[36]
        temp[43] = board[39]
        temp[44] = board[42]
        temp[51] = board[63]
        temp[52] = board[66]
        temp[53] = board[69]
        temp[54] = board[2]
        temp[55] = board[5]
        temp[56] = board[8]
        temp[63] = board[29]
        temp[64] = board[32]
        temp[65] = board[35]
        temp[72] = board[56]
        temp[73] = board[59]
        temp[74] = board[62]
        temp[57] = board[1]
        temp[58] = board[4]
        temp[59] = board[7]
        temp[66] = board[28]
        temp[67] = board[31]
        temp[68] = board[34]
        temp[75] = board[55]
        temp[76] = board[58]
        temp[77] = board[61]
        temp[60] = board[0]
        temp[61] = board[3]
        temp[62] = board[6]
        temp[69] = board[27]
        temp[70] = board[30]
        temp[71] = board[33]
        temp[78] = board[54]
        temp[79] = board[57]
        temp[80] = board[60]

        return temp

    @staticmethod
    def reflect_across_horizontal_axis(board, use_floats=False):
        # see above for explanation on how
        # flipud in numpy
        """
		[60., 61., 62., 69., 70., 71., 78., 79., 80.],
		[57., 58., 59., 66., 67., 68., 75., 76., 77.],
		[54., 55., 56., 63., 64., 65., 72., 73., 74.],
		[33., 34., 35., 42., 43., 44., 51., 52., 53.],
		[30., 31., 32., 39., 40., 41., 48., 49., 50.],
		[27., 28., 29., 36., 37., 38., 45., 46., 47.],
		[ 6.,  7.,  8., 15., 16., 17., 24., 25., 26.],
		[ 3.,  4.,  5., 12., 13., 14., 21., 22., 23.],
		[ 0.,  1.,  2.,  9., 10., 11., 18., 19., 20.]]
		"""

        if use_floats:
            temp = np.zeros(81)
        else:
            temp = np.zeros(81, dtype=int)

        temp[0] = board[60]
        temp[1] = board[61]
        temp[2] = board[62]
        temp[9] = board[69]
        temp[10] = board[70]
        temp[11] = board[71]
        temp[18] = board[78]
        temp[19] = board[79]
        temp[20] = board[80]
        temp[3] = board[57]
        temp[4] = board[58]
        temp[5] = board[59]
        temp[12] = board[66]
        temp[13] = board[67]
        temp[14] = board[68]
        temp[21] = board[75]
        temp[22] = board[76]
        temp[23] = board[77]
        temp[6] = board[54]
        temp[7] = board[55]
        temp[8] = board[56]
        temp[15] = board[63]
        temp[16] = board[64]
        temp[17] = board[65]
        temp[24] = board[72]
        temp[25] = board[73]
        temp[26] = board[74]
        temp[27] = board[33]
        temp[28] = board[34]
        temp[29] = board[35]
        temp[36] = board[42]
        temp[37] = board[43]
        temp[38] = board[44]
        temp[45] = board[51]
        temp[46] = board[52]
        temp[47] = board[53]
        temp[30] = board[30]
        temp[31] = board[31]
        temp[32] = board[32]
        temp[39] = board[39]
        temp[40] = board[40]
        temp[41] = board[41]
        temp[48] = board[48]
        temp[49] = board[49]
        temp[50] = board[50]
        temp[33] = board[27]
        temp[34] = board[28]
        temp[35] = board[29]
        temp[42] = board[36]
        temp[43] = board[37]
        temp[44] = board[38]
        temp[51] = board[45]
        temp[52] = board[46]
        temp[53] = board[47]
        temp[54] = board[6]
        temp[55] = board[7]
        temp[56] = board[8]
        temp[63] = board[15]
        temp[64] = board[16]
        temp[65] = board[17]
        temp[72] = board[24]
        temp[73] = board[25]
        temp[74] = board[26]
        temp[57] = board[3]
        temp[58] = board[4]
        temp[59] = board[5]
        temp[66] = board[12]
        temp[67] = board[13]
        temp[68] = board[14]
        temp[75] = board[21]
        temp[76] = board[22]
        temp[77] = board[23]
        temp[60] = board[0]
        temp[61] = board[1]
        temp[62] = board[2]
        temp[69] = board[9]
        temp[70] = board[10]
        temp[71] = board[11]
        temp[78] = board[18]
        temp[79] = board[19]
        temp[80] = board[20]

        return temp


class TicTacToe:
    """
	small board helper, the big board is a 3x3 of small boards.
	zero represents unoccupied, one represents first player, two represents second player

	"""

    @staticmethod
    def get_game_ended(board):
        if TicTacToe.get_win_result(board) != 0:
            return True

        for i in range(9):
            if board[i] == 0:
                return False

        return True

    @staticmethod
    def get_win_result(board):
        # return 0 for no wins, 1 for first win, 2 for second win

        for i in range(3):
            # likely faster than a for loop
            if board[i * 3] == board[i * 3 + 1] and board[i * 3 + 1] == board[i * 3 + 2] and board[i * 3] != 0:
                return board[i * 3]

            if board[i] == board[3 + i] and board[3 + i] == board[6 + i] and board[i] != 0:
                return board[i]

        # check diagonals

        if board[0] == board[4] and board[4] == board[8] and board[0] != 0:
            return board[0]

        if board[2] == board[4] and board[4] == board[6] and board[2] != 0:
            return board[2]

        return 0

    @staticmethod
    def get_valid_moves(board):
        answer = np.zeros(9, dtype=int)

        if TicTacToe.get_game_ended(board):
            return answer

        for i in range(9):
            answer[i] = 1 if board[i] == 0 else 0

        return answer

    @staticmethod
    def mark_valid_moves(board, answer, offset):
        if TicTacToe.get_game_ended(board[offset : offset + 9]):
            return

        for i in range(9):
            answer[i + offset] = 1 if board[i + offset] == 0 else 0

    @staticmethod
    def process_move(board, position, player):
        if TicTacToe.get_win_result(board) != 0:
            return

        board[position] = player


class UtttBoard:
    def deepcopy(self):
        answer = UtttBoard()

        answer.next_square = self.next_square

        # answer.history = [0 for x in range(len(self.history))]

        # for i in range(len(self.history)):
        # 	answer.history[i] = self.history[i]

        answer.cur_player = self.cur_player

        for i in range(81):
            answer.board[i] = self.board[i]

        return answer

    def __init__(self):
        self.next_square = -1  # -1 for play anywhere, else 0-8 for target square

        # self.history = []
        self.cur_player = 1

        self.board = np.zeros(81, dtype=int)

    def get_flattened_board(self):
        return self.board

    def is_valid_move(self, position):
        return self.get_valid_moves()[position] == 1

    def get_valid_moves_vector(self):
        valid_moves = self.get_valid_moves()

        valid_count = 0
        for i in range(81):
            valid_count += valid_moves[i]

        answer = []

        for i in range(81):
            if valid_moves[i] == 1:
                answer += [i]

        return answer

    def get_valid_moves(self):  # binary vector
        answer = np.zeros(81, dtype=int)

        counter = 0

        for i in range(9):
            if i == self.next_square or self.next_square == -1:
                TicTacToe.mark_valid_moves(self.board, answer, i * 9)
                # small_valid_moves = TicTacToe.get_valid_moves(self.board[i * 9 : i * 9 + 9])

                # for k in range(0, 9):
                # 	answer[i * 9 + k] = small_valid_moves[k]

        return answer

    def get_game_ended(self):
        win_results = self.get_small_win_results()

        if TicTacToe.get_win_result(win_results) != 0:
            return True

        for i in range(9):
            has_open = False
            for j in range(9):
                if self.board[i * 9 + j] == 0:
                    has_open = True
                    break

            if win_results[i] == 0 and has_open:
                return False

        return True

    # here, position is range from [0, 81)
    def process_move(self, position):
        if self.get_game_ended():
            return

        self.board[position] = self.cur_player

        self.next_square = position % 9

        if TicTacToe.get_game_ended(self.board[self.next_square * 9 : self.next_square * 9 + 9]):
            self.next_square = -1

        # self.history += [position]
        if self.cur_player == 1:
            self.cur_player = 2
        else:
            self.cur_player = 1

    """
	def undo(self):
		# will error out if we haven't made any moves in history yet
		if self.cur_player == 1:
			self.cur_player = 2
		else:
			self.cur_player = 1

		last_played = self.history.pop()

		self.board[last_played] = 0
	"""

    def get_small_win_results(self):
        win_results = np.zeros(9, dtype=int)

        for i in range(9):
            win_results[i] = TicTacToe.get_win_result(self.board[i * 9 : i * 9 + 9])

        return win_results

    def get_win_result(self):
        # return 0 for no wins, 1 for first win, 2 for second win

        win_results = self.get_small_win_results()

        return TicTacToe.get_win_result(win_results)

    def rotate90(self):
        self.board = BoardHelper.rotate90(self.board)

        if self.next_square == 0:
            self.next_square = 6
        elif self.next_square == 1:
            self.next_square = 3
        elif self.next_square == 2:
            self.next_square = 0
        elif self.next_square == 3:
            self.next_square = 7
        elif self.next_square == 5:  # 4 stays in place
            self.next_square = 1
        elif self.next_square == 6:
            self.next_square = 8
        elif self.next_square == 7:
            self.next_square = 5
        elif self.next_square == 8:
            self.next_square = 2

    def reflect_across_horizontal_axis(self):
        self.board = BoardHelper.reflect_across_horizontal_axis(self.board)

        if self.next_square == 6:
            self.next_square = 0
        elif self.next_square == 7:
            self.next_square = 1
        elif self.next_square == 8:
            self.next_square = 2
        elif self.next_square == 0:
            self.next_square = 6
        elif self.next_square == 1:
            self.next_square = 7
        elif self.next_square == 2:
            self.next_square = 8

    def get_symmetries(self, probability_vector):
        # return vector containing all symmetries
        # should be only the 8 existing ones

        answer = []

        for i in range(4):
            self.rotate90()
            probability_vector = BoardHelper.rotate90(probability_vector, use_floats=True)

            answer += [(self.to_nn_input_vector(), probability_vector)]

        self.reflect_across_horizontal_axis()
        probability_vector = BoardHelper.reflect_across_horizontal_axis(probability_vector, use_floats=True)

        for i in range(4):
            self.rotate90()
            probability_vector = BoardHelper.rotate90(probability_vector, use_floats=True)

            answer += [(self.to_nn_input_vector(), probability_vector)]

        self.reflect_across_horizontal_axis()
        probability_vector = BoardHelper.reflect_across_horizontal_axis(probability_vector, use_floats=True)

        return answer

    def to_nn_input_vector(self):
        # see neural net for input description

        answer = np.zeros(189, dtype=int)

        for i in range(81):
            answer[i] = 1 if self.board[i] == self.cur_player else 0

        opponent = 1 if self.cur_player == 2 else 2

        for i in range(81):
            answer[i + 81] = 1 if self.board[i] == opponent else 0

        win_results = self.get_small_win_results()

        for i in range(9):
            answer[i + 162] = 1 if win_results[i] == self.cur_player else 0

        for i in range(9):
            answer[i + 171] = 1 if win_results[i] == opponent else 0

        for i in range(9):
            answer[i + 180] = (
                1
                if (
                    (self.next_square == i or self.next_square == -1)
                    and not TicTacToe.get_game_ended(self.board[i * 9 : i * 9 + 9])
                )
                else 0
            )

        return answer

    def __eq__(self, other):
        return self.__hash__() == other.__hash__()

    def __hash__(self):
        answer = np.int64(0)

        for i in range(81):
            answer = answer * 31 + self.board[i] + 1
            answer %= 1000000007

        return int(answer)

    """
	def to_history_string(self):
		answer = ""
		for i in range(len(self.history)):
			answer += self.history[i]
			answer += " "

		return answer
	"""

    def to_string(self):
        """
		0 1 2   9 10 11   18 19 20
		3 4 5   12 13 14  21 22 23
		6 7 8   15 16 17  24 25 26

		and so on is the internal representation. We want to convert it to normal matrix representation.

		"""

        array_human = [[0 for x in range(9)] for y in range(9)]
        """
		array_human is in format:

		0 1 2 3 4 5 6 7 8
		9 10 .....
		"""

        for i in range(81):
            big = i // 9
            small = i % 9

            human_row = 3 * (big // 3) + (small // 3)
            human_col = 3 * (big % 3) + (small % 3)

            array_human[human_row][human_col] = self.board[i]

        answer = ""

        for i in range(9):
            for j in range(9):
                if array_human[i][j] == 0:
                    answer += "."
                elif array_human[i][j] == 1:
                    answer += "X"
                else:
                    answer += "O"

                if j == 2 or j == 5:
                    answer += " "

            answer += "\n"

            if i == 2 or i == 5:
                answer += "\n"

        return answer
