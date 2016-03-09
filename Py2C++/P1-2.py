# Program: CS 115 Lab 6a
# Author: Your name
# Description: This program will ask the user for a value
# and tell the user whether the value is even or odd.


def main():

	N = int(input('Enter a number: '))

	if N % 2 == 1:
		print(N, 'is odd.')
	else:
		print(N, 'is even.')


main()
