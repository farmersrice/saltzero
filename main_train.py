
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


temp_manager = TrainingManager()

temp_manager.train_and_eval_new_network()

# This is to compare with stupid MC

result = DuelManager().play_games(SaltZeroAgent(temp_manager.net, use_temp = False), NaiveMCAgent(), 10, debug = True, use_gating = False, gating_threshold = 0.0)

print("after mc is " + str(result[0]) + ' ' + str(result[1]))
