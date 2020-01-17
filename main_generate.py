import gc
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

# TrainingManager().generate_training_data(100, 10)

TrainingManager().generate_training_data_bulk(50)
