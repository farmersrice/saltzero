from TrainingManager import TrainingManager
from NeuralNet import NeuralNet

from numba import cuda

import os
#os.environ['CUDA_VISIBLE_DEVICES'] = '-1' # no gpu.


from classical_agents.NaiveMCAgent import NaiveMCAgent
from classical_agents.RandomAgent import RandomAgent
from DuelManager import DuelManager
from SaltZeroAgent import SaltZeroAgent

import tensorflow as tf

conf = tf.compat.v1.ConfigProto()
conf.gpu_options.per_process_gpu_memory_fraction=0.6
session = tf.compat.v1.Session(config=conf)

tf.compat.v1.disable_eager_execution()

temp_manager = TrainingManager()

temp_manager.train_new_network()

cuda.select_device(0)
cuda.close() # hopefully this frees the memory and allows us to use GPU for training without out of memory