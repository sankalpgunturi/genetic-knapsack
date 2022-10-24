from hwcounter import Timer, count, count_end

#from time import sleep
#from math import sqrt


# 1. Manually count cycles elapsed between two points

start = count()
#sqrt(144) / 12

elapsed = count_end() - start
print(elapsed)
# with Timer() as t:
# 	sleep(1)

# print(el)