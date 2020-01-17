import os
import time

import dotdict
import numpy as np
import tensorflow as tf
from nn_implementations.NeuralNetTensorflow import NeuralNetTensorflow
from tensorflow import keras
from tensorflow.keras import regularizers
from tensorflow.keras.layers import Dense


class NeuralNet:
    def __init__(self):
        self.net = NeuralNetTensorflow()

    def predict(self, board):
        return self.net.predict(board)

    def train(self, examples):
        self.net.train(examples)

    def save(self, folder, filename, include_opt=True):
        self.net.save(folder, filename, include_opt)

    def load(self, folder, filename):
        self.net.load(folder, filename)

    def predict_on_batch(self, inputs):
        # print("wut")
        # print(str(np.asarray(inputs)))
        return self.net.model.predict_on_batch(np.asarray(inputs))
