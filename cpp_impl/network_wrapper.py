import tensorflow as tf
import numpy as np
import time
import gc
import struct
from mem_top import mem_top

# Unfortunately tensorflow doesn't allow us to release the used gpu memory in an easy fashion, so we have to create a separate executable


import os,sys,inspect,psutil # if it crashes for no reason, might need to pip install psutil
currentdir = os.path.dirname(os.path.abspath(inspect.getfile(inspect.currentframe())))
parentdir = os.path.dirname(currentdir)
sys.path.insert(0,parentdir)

from NeuralNet import NeuralNet
from UtttBoard import UtttBoard

#os.environ['CUDA_VISIBLE_DEVICES'] = '-1' # no gpu

# do it this way since we only have one python interpreter at a time
model0 = NeuralNet()
model1 = NeuralNet()

tf.compat.v1.disable_eager_execution()

float_strings = ['f' * 82 * i for i in range(1001)];

print(float_strings[10])

print("preprocess done")

def load_model(network_id, index):
	
	if index == 0:
		model0.load('../models', 'best_' + str(network_id) + '.h5')
	else:
		model1.load('../models', 'best_' + str(network_id) + '.h5')

	print("called load model " + str(network_id))


def predict(vector_string, index):
	start = time.time()
	num_queries = len(vector_string) / (int(189))

	num_queries = int(num_queries)

	#fastest way
	queries = [+(c=='1') for c in vector_string]
	queries = [queries[189 * i : 189 * (i + 1)] for i in range(num_queries)]

	mini_time = time.time()

	#print("before query time " + str(mini_time - start))
	#print(queries)
	result = model0.predict_on_batch(queries) if index == 0 else model1.predict_on_batch(queries)
	#print("predict only time " + str(time.time() - mini_time))


	items = [0] * 82 * num_queries

	counter = 0
	for i in range(num_queries):
		items[counter: counter + 81] = result[0][i]
		counter += 81
		items[counter] = result[1][i][0]
		counter += 1

	#print("result get " + str(items[0]) + " length is " + str(len(items)))
	
	str_time = time.time()

	#try:
	answer = struct.pack(float_strings[num_queries], *items)
	#except Exception as e: 
	#	print(e)

	#print("string time " + str(time.time() - str_time))

	end = time.time()

	#print("answer length " + str(len(answer)))
	#assert(len(answer) == 82 * 1000 * 4)
	#print("total python time taken " + str(end - start))
	return answer
