import tensorflow as tf
from tensorflow import keras
from tensorflow.keras import regularizers
from tensorflow.keras.layers import Dense, Add, Activation

import dotdict

import time

import numpy as np

import os

params = dotdict.dotdict({
	'learning_rate': 0.01,
	'epochs': 1,
	'c': 0.0001, # from original 
	'momentum': 0.9 # from original
})



# We are not using a convolutional net because ultimate tic tac toe is inherently not-convolutional:
# if you have a convolution of 3x3, for example, then you might be taking positions from
# different boards, in which case there is almost no correlation between them, and the output is near-useless.
# So simple densely connected layers is probably better.
# Also it's a lot faster.

# Input vector will be 81 * 2 + 9 * 2, the first 81 are binary encodings of whether we have moved there or not,
# second 81 are whether opponent has moved there or not, and finally 9 telling us whether or not we have won
# the mini-boards, and the other 9 for whether the opponent has won the mini-boards.

# Last 9 are whether or not you can play on that mini-board.


class NeuralNetTensorflow():
	def new_network(self):
		def resnet_block(x):
			identity = x
			x = Dense(1024, activation = 'relu', kernel_regularizer = regularizers.l2(params.c))(x)
			x = Dense(1024, kernel_regularizer = regularizers.l2(params.c))(x) # no activation
			x = Add()([x, identity])
			x = Activation('relu')(x)

			return x


		visible = keras.layers.Input(shape=(189))
		x = Dense(1024, activation = 'relu', kernel_regularizer = regularizers.l2(params.c))(visible)

		# 7 resnet blocks
		x = resnet_block(x)
		x = resnet_block(x)
		x = resnet_block(x)
		x = resnet_block(x)
		x = resnet_block(x)
		x = resnet_block(x)
		x = resnet_block(x)

		policy_hidden = Dense(512, activation = 'relu', kernel_regularizer = regularizers.l2(params.c))(x)
		policy_output = Dense(81, activation = 'softmax', kernel_regularizer = regularizers.l2(params.c))(policy_hidden)

		value_hidden = Dense(512, activation = 'relu', kernel_regularizer = regularizers.l2(params.c))(x)
		value_output = Dense(1, activation = 'tanh', kernel_regularizer = regularizers.l2(params.c))(value_hidden)

		self.model = keras.models.Model(inputs = visible, outputs = [policy_output, value_output])

		opt = keras.optimizers.SGD(learning_rate = params.learning_rate, momentum = params.momentum)
		#opt = keras.optimizers.Adam(learning_rate = params.learning_rate)

		self.model.compile(loss = ['categorical_crossentropy', 'mean_squared_error'],
			optimizer = opt, 
			metrics = ['categorical_accuracy', 'mse'], loss_weights = [1, 0.1])

		print(self.model.summary())

		# Old network architecture below

		'''
		visible = keras.layers.Input(shape=(189))

		x = Dense(512, activation = 'relu', kernel_regularizer = regularizers.l2(params.c))(visible)
		x = Dense(512, activation = 'relu', kernel_regularizer = regularizers.l2(params.c))(x)
		x = Dense(512, activation = 'relu', kernel_regularizer = regularizers.l2(params.c))(x)
		x = Dense(512, activation = 'relu', kernel_regularizer = regularizers.l2(params.c))(x)
		x = Dense(512, activation = 'relu', kernel_regularizer = regularizers.l2(params.c))(x)
		x = Dense(512, activation = 'relu', kernel_regularizer = regularizers.l2(params.c))(x)

		policy_hidden = Dense(512, activation = 'relu', kernel_regularizer = regularizers.l2(params.c))(x)
		policy_output = Dense(81, activation = 'softmax', kernel_regularizer = regularizers.l2(params.c))(policy_hidden)

		value_hidden = Dense(512, activation = 'relu', kernel_regularizer = regularizers.l2(params.c))(x)
		value_output = Dense(1, activation = 'tanh', kernel_regularizer = regularizers.l2(params.c))(value_hidden)

		self.model = keras.models.Model(inputs = visible, outputs = [policy_output, value_output])

		opt = keras.optimizers.SGD(learning_rate = params.learning_rate, momentum = params.momentum)
		#opt = keras.optimizers.Adam(learning_rate = params.learning_rate)

		self.model.compile(loss = ['categorical_crossentropy', 'mean_squared_error'],
			optimizer = opt, 
			metrics = ['categorical_accuracy', 'mse'], loss_weights = [1, 1])

		print(self.model.summary())
		'''
	
	def predict(self, board):
		#	print(np.asarray(board.to_nn_input_vector()).reshape(-1, 189))
		#start = time.time()

		prediction = self.model.predict_on_batch(np.asarray(board.to_nn_input_vector()).reshape(-1, 189))
		#prediction = np.array(self.model.predict_on_batch(np.asarray(board.to_nn_input_vector()).reshape(-1, 189)))
		#prediction = self.model.predict(np.asarray(board.to_nn_input_vector()).reshape(-1, 189))
		#print("time elapsed for net: " + str(time.time() - start))
		return prediction

	def train(self, examples):
		#for fit: desired target y format is [nparray of nparray of policy outputs , nparray of nparray of value outputs]

		#input format is array [nn inputs, policy outputs, value outputs]

		self.model.fit(np.asarray(examples[0]), [np.asarray(examples[1]), np.asarray(examples[2])], epochs = params.epochs)

	#these only work for 1 level of foldering
	def save(self, folder, filename, include_opt = True):
		if not os.path.exists(folder):
			os.makedirs(folder)
		self.model.save(folder + '/' + filename, include_optimizer = include_opt)

	def load(self, folder, filename):
		if not os.path.exists(folder):
			os.makedirs(folder)
		self.model = keras.models.load_model(folder + '/' + filename)


	# Formerly planned on using this as an ID, but it's useless. Each time we load it's different, even when loading from the same file.
	# Probably has something to do with precision errors or something like that.

	#def get_network_hash(self):
	#	return hash(str(self.model.weights))