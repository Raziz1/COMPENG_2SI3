#include "HashTableQuad.h"
#include <iostream>
#include <random>
using namespace std;

/*
* File: HashTableQuad.cpp
* Author: Rahim Aziz
* Course: 2SI3 Lab 5
*
* Summary of File:
* In this lab you will implement a hash table with quadratic probing and perform simulations to observe its performance.
* The hash table will store positive integers.
* The hash function to be used is h(x) = x%M (the remainder after dividing x by M ), where M is the size of the hash table.
* The set of possible keys x is the set of positive integers representable by the int data type.
* You must write the C++ classes HashTableLin and HashTableQuad for testing and for measuring its performance
*/

/*
* HashTableQuad::HashTableQuad(int maxNum, double load)
*
* Parameters: maxNum - represents the maximum number of keys, load - represents the desired load factor of the hash table
*
* Return Value: No Return but modifies the current HastTableLin object
*
* Description: Creates an empty hash table.The amount of memory allocated for the hash table should be large
* enough so that maxNum keys could be stored without its load factor exceeding
* the value of parameter load
*/
HashTableQuad::HashTableQuad(int maxNum, double load)
{
	this->max_load = load;
	double size_table = maxNum / load; // Calculate the size of the table
	int smallest_prime = ceil(size_table); // Round the integer up so that we can find the next smallest prime
	smallest_prime = isPrime(smallest_prime); // Call a function to find the next smallest prime

	this->table.resize(smallest_prime, NULL); // Initialize the vector based on the new size and fill it with 0's
	this->size_table = smallest_prime;
	this->num_keys = 0;
}

/*
* HashTableQuad::isPrime(int n)
*
* Parameters: int n - represents the base integer to find the next closest prime number
*
* Return Value: int n - represents the next closest prime number to the input n
*
* Description: Determines the next closest prime number
*/
int HashTableQuad::isPrime(int n) {

	bool smallest_prime = false;

	// If we haven't reached the closest prime number determine whether the current number is a prime number
	while (!smallest_prime) {
		bool is_prime = true;
		// 0 and 1 are not prime numbers
		if (n == 0 || n == 1) {
			is_prime = false;
		}

		// loop to check if n is prime
		for (int i = 2; i <= n / 2; ++i) {
			if (n % i == 0) {
				is_prime = false;
			}
		}

		// If the integer is not a prime move on to the next digit
		if (!is_prime) {
			n++;
		}
		// Otherwise we have found our prime number so we can break the loop
		else {
			smallest_prime = true;
			break;
		}
	}

	return n;
}

/*
* HashTableQuad::insert(int n)
*
* Parameters: int n - represents the integer to insert into the hash table
*
* Return Value: No Return but modifies the current HastTableQuad object
*
* Description: Inserts an integer into the hash table using quadratic probing
* Run time: Average - Θ(1), Worst-Case - Θ(n)
* Space complexity: S(1)
*/
void HashTableQuad::insert(int n)
{
	// If the HashTable does not contain the element then we can insert it
	if (isIn(n) == false) {
		// Calculate the first hash function value and initialize the quadratic probing index
		int hash_function = n % getTableSize();
		int probe_index = 0;

		// If we have reached a non-zero digit continue the quadratic probing process
		while (this->table[hash_function] != 0) {
			// Increment the probe index and recalculate the hash function using quadratic probing
			probe_index++;
			hash_function = (n + (probe_index*probe_index)) % getTableSize();

			// If the probing index has surpassed the table size then there are no elements free and we can return nothing to the function and insert nothing into the hash table
			if (probe_index > getTableSize()) {
				return;
			}
		}

		// If we have found an index that isn't filled then insert the desired value
		this->table[hash_function] = n;
		this->num_keys++;

		// If inserting the value exceeds the maximum load factor rehash the table
		if ((((double)getNumKeys() + 1) / (double)getTableSize() > getMaxLoadFactor())) {
			this->rehash();
		}
	}

}

/*
* HashTableQuad::rehash()
*
* Parameters: No parameter
*
* Return Value: No Return but modifies the current HastTableQuad object
*
* Description: Expands the current HashTableLin
* 
* Run time: Average - Θ(n)
* Space complexity: S(n)
*/
void HashTableQuad::rehash()
{
	// Calculate the new size of the hash table which is going to be the next closest prime that is twice the size of the original hash table
	int new_hash_size = this->isPrime(this->getTableSize() * 2);

	// Create a copy of this vector and a new vector to store the rehashed table
	vector<int> this_copy_table = this->table;
	vector<int> new_table(new_hash_size, 0);

	// Link the current HastTableLin vector to the newly expanded vector
	this->table = new_table;
	this->size_table = new_hash_size;
	this->num_keys = 0;

	// Copy all the elements of the original table into the new table using the 'insert()' method
	for (int i = 0; i < this_copy_table.size(); i++) {
		if (this_copy_table[i] != 0) {
			this->insert(this_copy_table[i]);

		}
	}
}

/*
* HashTableQuad::isIn(int n)
*
* Parameters: int n - represents the integer to find inside the hash table
*
* Return Value: bool - represents whether the parameter 'n' has been found in the hash table
*
* Description: Returns true if key n is in this hash table. It returns false otherwise
* 
* Run time: Average - Θ(1), Worst-Case - Θ(n)
* Space complexity: S(1)
*/
bool HashTableQuad::isIn(int n)
{
	// Ensure that the hash table is not empty
	if (getTableSize() > 0) {

		// Calculate the initial hash function 
		int hash_function = n % getTableSize();
		int probe_index = 0;

		// If we haven't reached an empty hash table slot continue quadratic probing
		while (this->table[hash_function] != 0) {

			// We have found a slot containing the integer we were searching for
			if (this->table[hash_function] == n) {
				return true;
			}
			// Continue quadratic probing 
			else {
				probe_index++;
				hash_function = (n + (probe_index * probe_index)) % getTableSize();
			}
			// If the probing index has surpassed the table size then there are no elements free and we can return nothing to the function and insert nothing into the hash table
			if (probe_index > getTableSize()) {
				return false;
			}
		}
	}
	return false;
}

void HashTableQuad::printKeys()
{
	for (int i = 0; i < getTableSize(); i++) {
		if (this->table[i] != 0) {
			cout << table[i] << ", ";
		}
	}
}

void HashTableQuad::printKeysAndIndexes()
{
	for (int i = 0; i < getTableSize(); i++) {
		if (this->table[i] != 0) {
			cout << i << "-";
			cout << table[i] << ", ";

		}
	}
}

int HashTableQuad::getNumKeys() {
	return this->num_keys;
}

int HashTableQuad::getTableSize() {
	return this->size_table;
}

double HashTableQuad::getMaxLoadFactor() {
	return this->max_load;
}

/*
* HashTableQuad::insertCount(int n)
*
* Parameters: int n - represents the integer to insert inside the table
*
* Return Value: int probe_index - Represents the amount of quadratic probing steps required
*
* Description: Inserts an integer into a hash table without rehashing and counts the probing steps required
* 
* Run time: Average - Θ(1), Worst-Case - Θ(n)
* Space complexity: S(1)
*/
int HashTableQuad::insertCount(int n) {
	// If the HashTable does contain the element we want to insert then we do not need to insert it 
	if (isIn(n) == true) {
		return 0;
	}

	// Calculate the initial hash function 
	int hash_function = n % getTableSize();
	int probe_index = 1;

	// If the desired position is not found commence quadratic probing until a position is found
	while (this->table[hash_function] != 0) {
		probe_index++;
		hash_function = (n + (probe_index*probe_index)) % getTableSize();
	}

	// Insert the input parameter at the hash function index
	this->table[hash_function] = n;
	this->num_keys++;

	return probe_index;
}

/*
* HashTableQuad::simProbeSuccess()
*
* Parameters: No parameters
*
* Return Value: Returns a std::vector<double> of size 9, which contains the average number of probes for successful search in a table with load factor λ = 0.1, 0.2, · · · , 0.9
*
* Description: Performs simulation to measure the average number of probes for successful search for quadratic probing
*/
std::vector<double> HashTableQuad::simProbeSuccess()
{
	// Initialize some variables to keep track of the average number of probes
	vector<double> result;
	int probes = 0;
	double average_probes = 0;
	double total_average_probes = 0;

	// Loop through the various load factors
	for (double i = 0.1; i < 1; i += 0.1) {
		// Perform the average calculations 100 times to get a more accurate average calculation
		for (int j = 0; j < 100; j++) {

			// Initialize an empty hash table with a minimum number of keys equivalent to 100,000
			HashTableQuad sim_prob_table(10000, i);

			// Reset the probe count
			probes = 0;
			std::default_random_engine generator;
			std::uniform_int_distribution<int> distribution;

			// Randomly generate a number and insert it into the hash table using quadratic probing
			// Return the number of probes required for the above step
			while (sim_prob_table.getNumKeys() != 10000) {
				probes += sim_prob_table.insertCount(distribution(generator));

			}

			// Calculate the average number of probes for all the given insertions above
			average_probes += double(probes) / double(sim_prob_table.getNumKeys());
		}

		// Calculate the average number of probes for all 100 trials
		total_average_probes = average_probes / 100.0;
		average_probes = 0;
		// Push the average into the vector
		result.push_back(total_average_probes);

	}

	return result;

}
