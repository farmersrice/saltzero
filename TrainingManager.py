from BatchedMCTS import BatchedMCTS
from MCTS import MCTS
from UtttBoard import UtttBoard
from NeuralNet import NeuralNet
from SaltZeroAgent import SaltZeroAgent
import dotdict
import pickle
import time
import sys
from DuelManager import DuelManager
from mem_top import mem_top




import gc
import glob
import os.path

params = dotdict.dotdict({
	'temperature_threshold': 12,
	'game_generation_batch_size': 10, # how many games to generate before we save a file
	'train_game_size': 500, # how many games before we train a new network
	'duel_game_count': 40,
	'gating_threshold': 0.51,
	'play_batch_size': 100,
	'visits': 200
})

class TrainingManager():

	def __init__(self):
		self.load_latest_network()
	
	def play_many_games(self, num_games = params.play_batch_size, is_training = True):
		boards = [UtttBoard() for i in range(num_games)]

		move_counter = 0
		training_data = [[] for i in range(num_games)]

		unfinished_games = num_games

		mcts_items = [BatchedMCTS() for i in range(num_games)] #put here for tree reuse

		while unfinished_games > 0:

			start = time.time()
			#mcts_items = [BatchedMCTS() for i in range(num_games)]

			for i in range(params.visits):

				batched_queries = []
				no_input = [False for j in range(num_games)]

				for j in range(num_games):

					board = boards[j]

					if board.get_game_ended():
						no_input[j] = True
						continue

					query = mcts_items[j].before_visit(board, is_training)

					#print("got query " + query.to_string() if query is not None else "None")

					if query is not None:
						batched_queries.append(query.to_nn_input_vector())
					else:
						no_input[j] = True

				
				if len(batched_queries) > 0:
					#mini_start = time.time()
					results = self.net.predict_on_batch(batched_queries)
					#print("evaluated positions in " + str(time.time() - mini_start))
					#print("got results " + str(results))
				

				counter = 0
				for j in range(num_games):
					board = boards[j]

					if board.get_game_ended():
						continue

					if no_input[j]:
						mcts_items[j].after_visit(board, None, None)
					else:
						mcts_items[j].after_visit(board, results[0][counter], results[1][counter][0])
						counter += 1


			for i in range(num_games):
				board = boards[i]

				if not board.get_game_ended():
					pi, move = mcts_items[i].get_probabilities_and_best_move(board, (1 if move_counter < params.temperature_threshold else 0) if is_training else 0)

					symmetries = board.get_symmetries(pi)
					training_data[i] += symmetries

					board.process_move(move)

					#print("game appears")
					#print(board.to_string())

					if board.get_game_ended():
						unfinished_games -= 1

			print("finished move " + str(move_counter) + " in time " + str(time.time() - start))
			move_counter += 1


		gc.collect()

		answer = [[], [], []]
		for i in range(num_games):
			board = boards[i]
			this_training_data = training_data[i]

			print(board.to_string())

			result = board.get_win_result()

			print("result is " + str(result))

			if result == 2:
				result = -1 # format result to be -1, 0, or 1 in accordance with paper
				# -1 means second player won, 1 means first won

			# augment training data with the result

			# also reformat the training data while we're at it

			training_input = [None] * len(this_training_data)
			training_policy_output = [None] * len(this_training_data)
			training_value_output = [None] * len(this_training_data)

			for j in range(len(this_training_data)):
				whose_turn = (j // 8) % 2

				#win_result = -result if whose_turn == 0 else result
				win_result = result if whose_turn == 0 else -result

				training_input[j] = this_training_data[j][0]
				training_policy_output[j] = this_training_data[j][1]
				training_value_output[j] = win_result

			answer[0].extend(training_input)
			answer[1].extend(training_policy_output)
			answer[2].extend(training_value_output)
			#return [training_input, training_policy_output, training_value_output]

		return answer

	def play_one_game(self):
		board = UtttBoard()

		move_counter = 0

		training_data = []

		print("playing new game")
		while not board.get_game_ended():
			# in original paper they randomly rotate board, doubt that's necessary

			pi, move = MCTS(self.net).get_probabilities_and_best_move(board, True, 1 if move_counter < params.temperature_threshold else 0)
			move_counter += 1

			#print("pi is " + str(pi))

			print("move " + str(move_counter))
			#print(board.to_string())

			symmetries = board.get_symmetries(pi)
			training_data += symmetries

			#print("symmetries " + str(symmetries))

			board.process_move(move)

			#print("training_data size is " + str(sys.getsizeof(training_data)))

		gc.collect()

		#print("mem top")
		#print(mem_top())

		print(board.to_string())

		result = board.get_win_result()

		print("result is " + str(result))

		if result == 2:
			result = -1 # format result to be -1, 0, or 1 in accordance with paper
			# -1 means second player won, 1 means first won

		# augment training data with the result

		# also reformat the training data while we're at it

		training_input = [None] * len(training_data)

		training_policy_output = [None] * len(training_data)

		training_value_output = [None] * len(training_data)

		for i in range(len(training_data)):

			whose_turn = (i // 8) % 2

			#win_result = -result if whose_turn == 0 else result
			win_result = result if whose_turn == 0 else -result

			training_input[i] = training_data[i][0]
			training_policy_output[i] = training_data[i][1]
			training_value_output[i] = win_result

		return [training_input, training_policy_output, training_value_output]

	def generate_training_data_bulk(self, num_games):
		games_played = 0
		answer = [[], [], []]

		while games_played < num_games:
			data = self.play_many_games()

			self.save_data(data, params.play_batch_size)

			games_played += params.play_batch_size



	def generate_training_data(self, num_games, save_interval = 10): 

		#training_data = [[], [], []]
		save_interval_data = [[], [], []]

		for i in range(num_games):
			print("generating game " + str(i))

			start = time.time()

			new_data = self.play_one_game()
			for j in range(3):
				#training_data[j].extend(new_data[j])
				save_interval_data[j].extend(new_data[j])

			print("done, took " + str(time.time() - start))

			if (i + 1) % save_interval == 0:
				self.save_data(save_interval_data, save_interval)
				save_interval_data = [[], [], []]

		#return training_data

	def save_data(self, data, num_games):
		file_counter = 0

		if not os.path.exists('games'):
			os.makedirs('games')

		string_prefix = 'games/' + str(num_games) + '_'
		file_string = string_prefix + str(file_counter) + '.traintactoe'

		while os.path.isfile(file_string):
			file_counter += 1
			file_string = string_prefix + str(file_counter) + '.traintactoe'

		pickle.dump(data, open(file_string, 'wb'))

	def load_latest_network(self):
		file_counter = 0

		self.net = NeuralNet()

		if not os.path.exists('models') or not os.path.isfile("models/best_0.h5"):
			os.makedirs('models')

			self.net.save('models', "best_0.h5")
			self.model_string = "best_0.h5"
			return

		string_prefix = 'models/best_'
		file_string = string_prefix + str(file_counter) + '.h5'

		while os.path.isfile(file_string):
			file_counter += 1
			file_string = string_prefix + str(file_counter) + '.h5'

		# decrement to find existing one

		file_counter -= 1
		file_string = string_prefix + str(file_counter) + '.h5'

		print("loaded best network " + file_string[7:-3])

		self.net.load("models", file_string[7:])
		self.model_string = file_string[7:-3]
		self.model_counter = file_counter

	def count_num_saved_games(self):
		# return num games
		num_games = 0

		if not os.path.exists('games'):
			os.makedirs('games')
			return num_games

		for file in glob.glob("games/" + self.model_string + "*"):
			this_games = int(file.split("_")[2])

			num_games += this_games

		return num_games

	def parse_cpp_file(self, file):
		answer = [[], [], []]

		with open (file, "r") as f:
			raw_data = f.readlines()

			counter = 0
			for example in raw_data:

				split_example = example.split(' ')

				training_input = [(1 if split_example[0][i] == '1' else 0) for i in range(189)]
				training_policy_output = [float(split_example[i + 1]) for i in range(81)]
				training_value_output = float(split_example[1 + 81])

				answer[0].append(training_input)
				answer[1].append(training_policy_output)
				answer[2].append(training_value_output)

				counter += 1

				if (counter % 1000 == 0):
					print("training example " + str(training_input) + " output " + str(training_policy_output) + " value " + str(training_value_output))
					print("at " + str(counter))


		return answer;


	def load_training_data(self):

		# return num games, training data

		num_games = 0
		training_data = [[], [], []]

		if not os.path.exists('games'):
			os.makedirs('games')
			return num_games, training_data

		for file in glob.glob("games/*"):

			print("Loading training data from file " + file)

			this_games = int(file.split("games\\")[1].split('_')[0])

			parse_cpp = file.split('.')[1] == "cpptactoe"

			num_games += this_games

			#if num_games > max_games:
			#	return num_games - this_games, training_data

			if parse_cpp:
				temp = self.parse_cpp_file(file)
			else:
				temp = pickle.load(open(file, "rb"))


			for i in range(3):
				training_data[i].extend(temp[i])

			# now delete the file

			os.remove(file)



		return num_games, training_data


	def save_model_no_optimizer(self):
		# needed for frugally deep so we can generate games in C++ and kill the insane python overhead
		# when running 200 games in parallel the python overhead is literally > 98% of the time spent, 85 seconds for first move, 0.4-0.6 spent on neural net

		self.net.save("models", "best_" + str(self.model_counter) + "_no_opt.h5", False)


	def train_and_eval_new_network(self):
		#training_data = self.generate_training_data(params.train_game_size)

		if self.count_num_saved_games() < params.train_game_size:
			print("Not enough data to train")
			return

		num_games, training_data = self.load_training_data()

		print("Loaded " + str(num_games) + " games")

		self.net.save('models', "temp.h5")
		new_net = NeuralNet()
		new_net.load('models', "temp.h5")

		new_net.train(training_data)

		# compare 2 nets

		result = DuelManager().play_games(SaltZeroAgent(new_net), SaltZeroAgent(self.net), params.duel_game_count, debug = True, \
			use_gating = True, gating_threshold = params.gating_threshold)

		print("finished dueling, result is " + str(result[0]) + " to " + str(result[1]) + " new to old")

		if (result[0] / (result[0] + result[1]) >= params.gating_threshold):
			# Passed!

			print("Passed! saving new net")
			new_net.save("models", "best_" + str(self.model_counter + 1) + ".h5")
			self.model_counter += 1
			self.net = new_net

	def train_new_network(self):
		num_games, training_data = self.load_training_data()
		self.net.train(training_data)

		print("Finished training! saving new net")
		self.net.save("models", "best_" + str(self.model_counter + 1) + ".h5")
		self.model_counter += 1

	def compare_networks(self):
		# Use this just for now to check
		result = DuelManager().play_games(SaltZeroAgent(new_net), SaltZeroAgent(self.net), params.duel_game_count, debug = True, \
			use_gating = True, gating_threshold = params.gating_threshold)

		print("finished dueling, result is " + str(result[0]) + " to " + str(result[1]) + " new to old")