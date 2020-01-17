import os

import tensorflow as tf
from classical_agents.NaiveMCAgent import NaiveMCAgent
from classical_agents.RandomAgent import RandomAgent
from DuelManager import DuelManager
from NeuralNet import NeuralNet
from SaltZeroAgent import SaltZeroAgent
from TrainingManager import TrainingManager

os.environ["CUDA_VISIBLE_DEVICES"] = "-1"  # no gpu

tf.compat.v1.disable_eager_execution()

# test against random and naive mc

temp_manager = TrainingManager()

temp_manager.train_new_network()

# temp_manager.play_many_games(1, False)
