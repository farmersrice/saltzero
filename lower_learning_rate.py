from TrainingManager import TrainingManager
from NeuralNet import NeuralNet

import os
os.environ['CUDA_VISIBLE_DEVICES'] = '-1' # no gpu.

import sys

import tensorflow as tf

conf = tf.compat.v1.ConfigProto()
conf.gpu_options.per_process_gpu_memory_fraction=0.6
session = tf.compat.v1.Session(config=conf)

tf.compat.v1.disable_eager_execution()


learning_rate = float(sys.argv[1])
model_num = int(sys.argv[2])

temp_manager = TrainingManager()


model = temp_manager.net.net.model

import tensorflow.keras.backend as K 

print("Current learning rate: " + str(K.get_value(model.optimizer.lr)))
print("Setting learning rate to " + str(learning_rate))
K.set_value(model.optimizer.lr, learning_rate)

print("Done")

print("Overwriting " + "models/best_" + str(model_num) + ".h5")
model.save("models/best_" + str(model_num) + ".h5")