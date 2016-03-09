import sys

#takes 2 integers and returns 
#common denominator greatest
def gcd(a, b):
	while b != 0:
		temp = b 
		b = a % b
		a = temp
	return a

def compare(a, b):
	x = (a[0] * b[1])
	y = (b[0] * a[1])
	if (x < y):
		return -1
	elif (x == y):
		return 0
	else:
		return 1
