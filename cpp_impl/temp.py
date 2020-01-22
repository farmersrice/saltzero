import struct
import time
import random

items = [random.random() for i in range(82000)]

start = time.time()
temp = " ".join(f'{i:.9f}' for i in items)
end = time.time()

print("string join time " + str(end - start))

float_string = 'f' * 82000

start = time.time()
temp2 = struct.pack(float_string, *items)
end = time.time()
print("bytes time " + str(end - start))