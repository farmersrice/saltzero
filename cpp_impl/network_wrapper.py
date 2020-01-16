import tensorflow as tf
import numpy as np
import time
import gc
from mem_top import mem_top


import os,sys,inspect,psutil # if it crashes for no reason, might need to pip install psutil
currentdir = os.path.dirname(os.path.abspath(inspect.getfile(inspect.currentframe())))
parentdir = os.path.dirname(currentdir)
sys.path.insert(0,parentdir)

from NeuralNet import NeuralNet
from UtttBoard import UtttBoard

os.environ['CUDA_VISIBLE_DEVICES'] = '-1' # no gpu

# do it this way since we only have one python interpreter at a time
model0 = NeuralNet()
model1 = NeuralNet()

tf.compat.v1.disable_eager_execution()

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

	#several alternative, slower ways below

	#for i in range(num_queries):
	#	for j in range(189):
	#		if vector_string[i * 189 + j] == '1':
	#			queries[i][j] = 1

	#queries = np.array([+(c=='1') for c in vector_string])
	#queries = [queries[189 * i : 189 * (i + 1)] for i in range(num_queries)]


	#bstr = vector_string.encode('ascii')
	#queries = [c - 48 for c in bstr]
	#queries = [queries[189 * i : 189 * (i + 1)] for i in range(num_queries)]

	#bstr = vector_string.encode('ascii')
	#queries = [[bstr[i * 189 + j] - 48 for j in range(189)] for i in range(num_queries)]

	#print(queries)
	#queries = [[1 if vector_string[i * 189 + j] == '1' else 0 for j in range(189)] for i in range(num_queries)]

	#print("got query string " + vector_string)
	#print("num queries is " + str(num_queries))

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



	
	str_time = time.time()

	answer = " ".join(f'{i:.9f}' for i in items)
	#answer = " ".join("{:.9f}".format(i) for i in items)

	#print("string time " + str(time.time() - str_time))

	#print(answer)
	end = time.time()

	#gc.collect()

	#print("total python time taken " + str(end - start))
	return answer
