#include "Poly.h"
#include <iostream>

using namespace std;
/*
* File: Poly.cpp
* Author: Rahim Aziz
* Course: 2SI3 Lab 3
*
* Summary of File:
* In this lab you will implement polynomials and operations on polynomials using a sorted singly linked list with a dummy header. 
* To implement the linked list nodes you must use the C++ calss PolyNode provided in this lab. 
* Additionally, you need to implement the C++ class Poly (to represent polynomials). 
* In your implementation you are not allowed to use std::list! 
* Please compute the asymptotic run time and space complexity of all methods and be ready to present them to the TA with explanations at your demo.
*/

/*
* Poly::Poly()
*
* Parameters: N/A
*
* Return Value: No Return but modifies the current Poly object
*
* Description: Creates the zero polynomial with degree equals -1
*/
Poly::Poly()
{
	// Create an empty list with a dummy node
	this->head = new PolyNode(-1,0, nullptr);

	// The size of an empty polynomial is 0
	this->number_terms = 0;
}

/*
* Poly::Poly(const std::vector<int>& deg, const std::vector<double>& coeff)
*
* Parameters: A vector of integers & a vector of doubles
*
* Return Value: No Return but modifies the current Poly object
*
* Description: Creates a polynomial out of the two input parameters as follows: vector 'deg' contains the degrees of non-negative terms.
* vector 'coeff' contains the coefficients of non-zero terms. Both vectors have the same size and both are sorted in descending order
*/
Poly::Poly(const std::vector<int>& deg, const std::vector<double>& coeff)
{
	// Create an empty list with the dummy node
	this->head = new PolyNode(-1, 0, nullptr);

	// Create a temporary reader node
	PolyNode* previous_node = this->head;
	
	number_terms = deg.size();

	// Loop through the vector of polynomial terms and create new nodes with the data
	for (int i = 0; i < number_terms; i++) {

		previous_node->next = new PolyNode(deg[i],coeff[i], nullptr);

		// Move to the next node
		previous_node = previous_node->next;
	}
}


/*
* Poly::~Poly()
* 
* Parameters: N/A
*
* Return Value: N/A
*
* Description: Linked List Destructor
*/
Poly::~Poly()
{
	// Create a temporary node starting from the dummy node
	PolyNode* redear_head = head;
	while (redear_head->next != nullptr) {
		// Create another temporary node to be able to delete the current node
		PolyNode* next_reader = redear_head->next;
		delete redear_head;
		redear_head = next_reader;
	}
}


/*
* void Poly::addMono(int i, double c)
*
* Parameters: An integer 'i' representing the degree and a double 'c' representing the coefficient
*
* Return Value: No Return but modifies the current Poly object
*
* Description: Adds a single monomial to this polynomial. This function assumes the input degree is non negative.
*/
void Poly::addMono(int i, double c)
{
	PolyNode* prev_node = this->head;
	PolyNode* current_node = this->head->next;
	
	// Loop through the entire linked list
	while(current_node!=nullptr){
		// If the degree is bigger than the current node degree, then add it to the list in between the previous and current node;
		if (i > current_node->deg) {
			PolyNode* temp_node = new PolyNode(i, c, current_node);
			prev_node->next = temp_node;
			this->number_terms++;
			return;
		}
		// If the degree already exists add the coefficients and increment the list size
		else if (i==current_node->deg) {
			current_node->coeff += c;

			// If the sum of the coefficients is zero then delete the node and decrement the list size
			if (current_node->coeff == 0) {
				prev_node->next = current_node->next;
				delete current_node;
				this->number_terms--;
			}
			return;
		}

		// Move the previous and current reader nodes to the next linked node
		prev_node = prev_node->next;
		current_node = current_node->next;
	}

	// If we are at the end of the list and haven't found the value yet add it to the end of the list
	PolyNode* temp_node = new PolyNode(i, c, nullptr);
	prev_node->next = temp_node;
	this->number_terms++;
}


/*
* void Poly::addPoly(const Poly& p)
*
* Parameters: A linked list representing the polynomial 'p'
*
* Return Value: No Return but modifies the current Poly object
*
* Description: Adds a polynomial to this polynomial.
*/
void Poly::addPoly(const Poly& p)
{
	// Create a temporary reader head 
	PolyNode* temp_node = p.head->next;

	// Loop through every monomial in the input polynomial and add it to this polynomial
	while (temp_node != nullptr) {
		this->addMono(temp_node->deg, temp_node->coeff);
		temp_node = temp_node->next;
	}
}

/*
* void Poly::multiplyMono(int i, double c)
*
* Parameters: An integer 'i' representing the degree and a double 'c' representing the coefficient
*
* Return Value: No Return but modifies the current Poly object
*
* Description: Multiplies this polynomial by a single monomial. This function assumes that the degree is non-negative and the coefficient can be zero
*/
void Poly::multiplyMono(int i, double c)
{
	PolyNode* current_node = this->head->next;

	//If one of the terms is zero then delete the list
	if (c == 0) {
		this->number_terms = 0;
		// Deletes the list by unlinking the header node
		this->head->next = nullptr;
	}

	// Loop through the entire linked list
	while (current_node != nullptr) {
		//If one of the terms is zero then delete the list
		if (c == 0) {
			// Deallocate ever node in the linked list
			PolyNode* temp_node = current_node->next;
			delete current_node;
			current_node = temp_node;
		}
		else {
			current_node->coeff *= c;
			current_node->deg += i;
			current_node = current_node->next;
		}
	}
}

/*
* void Poly::multiplyPoly(const Poly& p)
*
* Parameters: A linked list representing the polynomial 'p'
*
* Return Value: No Return but modifies the current Poly object
*
* Description: Multiplies this polynomial by a polynomial 'p'
*/
void Poly::multiplyPoly(const Poly& p)
{
	// SANITY CHECK
	if (this->number_terms == 0) {
		this->head->deg = -1;
		return;
	}

	// SANITY CHECK
	if (p.number_terms == 0) {
		multiplyMono(0, 0); // multiply all terms by zero removing all the elements
		return;
	}

	// Create a copy of this polynomial
	Poly* copy_this = new Poly();
	this->duplicate(*copy_this);

	// Create a new linked list to store the sum of the sum of all the monomial terms
	Poly* product_terms = new Poly();

	// Create a reader head to loop through the monomial terms of the input polynomial
	PolyNode* input_current_node = p.head->next;

	// Loop through all the monomial terms of the input linked list
	while (input_current_node != nullptr) {

		// Multiply this entire polynomial by one monomial term of the input polynomial and store it in the copy of the original polynomial
		copy_this->multiplyMono(input_current_node->deg, input_current_node->coeff);
		
		// Sum the product of the monomial terms and store them 
		product_terms->addPoly(*copy_this);

		// Reset the value of the copied polynomial to the value of the original polynomial
		this->duplicate(*copy_this);

		// Move to the next node
		input_current_node = input_current_node->next;
	}

	// Clear this monomial
	this->multiplyMono(0, 0);

	// Store the product of both polynomials in this polynomial
	this->addPoly(*product_terms);
	
}


/*
* void Poly::duplicate(Poly& outputPoly)
*
* Parameters: A linked list representing the polynomial 'outPoly'
*
* Return Value: No Return but modifies the pointer to the input polynomial
*
* Description: Copies this polynomial to outputPoly. The incoming outputPoly can be zero polynomial or non-zero polynomial. 
* The linked list corresponding to the outgoing outputPoly must be a duplicate of this linked list.
*/
void Poly::duplicate(Poly& outputPoly)
{
	vector<int> degree;
	vector<double> coefficients;

	// Create a reader node for this linkded list
	PolyNode* current_node = this->head->next;
	
	// Loop through the entire list and push the corresponding coefficient and degrees into the vectors
	for (int i = 0; i < getTermsNo(); i++) {
		degree.push_back(current_node->deg);
		coefficients.push_back(current_node->coeff);
		current_node = current_node->next;
	}

	// Create a new linked list based on the extracted coefficients and degrees
	Poly *tempList = new Poly(degree, coefficients);

	// Set the input polynomial to the copy of this polynomial
	outputPoly = *tempList;
}


/*
* int Poly::getDegree()
*
* Parameters: N/A
*
* Return Value: Returns an integer
*
* Description: Returns a number representing the highest degree of this polynomial
*/
int Poly::getDegree()
{
	int degree = -1;
	// Return the highest degree of the polynomial by checking the very first element
	if (getTermsNo() > 0 && this->head->next != nullptr) {
		degree = this->head->next->deg;
	}
	return degree;
}

/*
* int Poly::getTermsNo()
*
* Parameters: N/A
*
* Return Value: Returns an integer
*
* Description: Returns the number of terms in this polynomial
*/
int Poly::getTermsNo()
{
	return number_terms;
}


/*
* double Poly::evaluate(double x)
*
* Parameters: A double 'x' representing the independent variable of the polynomial
*
* Return Value: Returns a double
*
* Description: Evaluates the polynomial expression for the value x of the variable
*/
double Poly::evaluate(double x)
{
	double evaluation = 0;

	// Create a reader node that starts on the node after the dummy
	PolyNode* current_node = this->head->next;

	// Loop through every monomial term and evaluate the term at 'x'
	for (int i = 0; i < getTermsNo(); i++) {
		evaluation += (current_node->coeff) * pow(x, current_node->deg);
		current_node = current_node->next;
	}
	return evaluation;
}


/*
* std::string Poly::toString()
*
* Parameters: N/A
*
* Return Value: Returns a string
*
* Description: returns a string representation of the polynomial. 
* The string should specify the degree of the polynomial and the coefficients of all non-zero terms, listed in decreasing order of terms degrees.
*/
std::string Poly::toString()
{

	int highest_degree = getDegree();
	string output_string = "degree="+to_string(highest_degree)+"; ";

	// Create a reader node 
	PolyNode* current_node = this->head->next;

	// Loop through the entire linked list of monomial terms 
	for (int i = 0; i < getTermsNo(); i++) {
		output_string += "a(" + to_string(current_node->deg)+")"+"="+to_string(current_node->coeff)+"; ";
		current_node = current_node->next;
	}
	return output_string;
}
