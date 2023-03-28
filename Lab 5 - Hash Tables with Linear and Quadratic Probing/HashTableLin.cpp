#include "HashTableLin.h"
#include <iostream>
using namespace std;

HashTableLin::HashTableLin(int maxNum, double load)
{
    // TODO
    this->max_load = load;
    double size_table = maxNum / load;
    int smallest_prime = ceil(size_table);

    smallest_prime = isPrime(smallest_prime);

    this->table.resize(smallest_prime, NULL);
    this->size_table = smallest_prime;
}

int HashTableLin::isPrime(int n) {
    bool smallest_prime = false;

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
            

            if (!is_prime) {
                n++;
            }
            else {
                smallest_prime = true;
                break;
            }
        }
        return n;
}

void HashTableLin::insert(int n)
{
    // TODO
    // If the HashTable does not contain the element
    if (isIn(n) == false) {
        int hash_function = n % getTableSize();
        int probe_index = 0;

        while (this->table[hash_function] != 0) {
            probe_index++;
            hash_function = (n + probe_index) % getTableSize();

            if (probe_index > getTableSize()) {
                return;
            }
        }

        this->table[hash_function] = n;
        this->num_keys++;

        if ((((double)getNumKeys() + 1) / (double)getTableSize() > getMaxLoadFactor())) {
            this->rehash();
        }
    }
  
}

void HashTableLin::rehash()
{
    // TODO
    int new_hash_size = this->isPrime(this->getTableSize() * 2);

    vector<int> this_copy_table = this->table;
    vector<int> new_table(new_hash_size,0);
    
    this->table = new_table;
    this->size_table = new_hash_size;
    this->num_keys = 0;

    for (int i = 0; i < this_copy_table.size(); i++) {
        if (this_copy_table[i] != 0) {
            this->insert(this_copy_table[i]);

        }
    }

}

bool HashTableLin::isIn(int n)
{
    // TODO, change following code after completing this function
    if (getTableSize() > 0) {
        int hash_function = n % getTableSize();
        int i = 0;
        while (this->table[hash_function] != 0) {
            if (this->table[hash_function] == n) {
                return true;
            }
            else {
                i++;
                hash_function = (n + i) % getTableSize();
            }
        }
    }
    return false;
}

void HashTableLin::printKeys()
{
    // TODO
    for (int i = 0; i < getTableSize(); i++) {
        if (this->table[i] != 0) {
            cout << table[i]<<", ";
        }
    }
}

void HashTableLin::printKeysAndIndexes()
{
    // TODO
    for (int i = 0; i < getTableSize(); i++) {
        if (this->table[i] != 0) {
            cout << i<<"-";
            cout << table[i] << ", ";

        }
    }
}

int HashTableLin::getNumKeys() {
	// TODO, change following code after completing this function
    return this->num_keys;
}

int HashTableLin::getTableSize() {
	// TODO, change following code after completing this function
    return this->size_table;
}

double HashTableLin::getMaxLoadFactor() {
	// TODO, change following code after completing this function
    return this->max_load;
}

std::vector<double> HashTableLin::simProbeSuccess()
{
    // TODO, change following code after completing this function
    vector<double> result(9);
    return result;
}

// for bonus
std::vector<double> HashTableLin::simProbeUnsuccess()
{
    // TODO, change following code after completing this function
    vector<double> result(9);
    return result;
}
