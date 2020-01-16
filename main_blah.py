from TrainingManager import TrainingManager
from NeuralNet import NeuralNet

import os
os.environ['CUDA_VISIBLE_DEVICES'] = '-1' # no gpu


from classical_agents.NaiveMCAgent import NaiveMCAgent
from classical_agents.RandomAgent import RandomAgent
from DuelManager import DuelManager
from SaltZeroAgent import SaltZeroAgent

import gc

import tensorflow as tf

tf.compat.v1.disable_eager_execution()

TrainingManager().play_many_games(1, False)