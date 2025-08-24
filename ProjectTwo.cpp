//----------------------------------------------------------------------------
// Name        : ProjectTwo.cpp
// Author      : Elric
// Version     : 1.0
// Description : Project Two ABCU advicing assistance software
//----------------------------------------------------------------------------

#include <algorithm>
#include <climits>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

// Global definitions visible to all methods and classes

const unsigned int DEFAULT_SIZE = 179;

// define a structure to hold course information
struct Course {
	string courseId; // unique identifier
	string courseTitle;
	vector<string> prereqs;
};

/*
 *Hash Table class definition
 */
class HashTable {

private:
	// Define structures to hold bids
	struct Node {
		Course course;
		unsigned int key;
		Node* next;

		// default constructor
		Node() {
			key = UINT_MAX;
			next = nullptr;
		}

		// initialize with a bid
		Node(Course aCourse) : Node() {
			course = aCourse;
		}

		// initialize with a bid and a key
		Node(Course aCourse, unsigned int aKey) : Node(aCourse) {
			key = aKey;
		}
	};

	vector<Node> nodes;

	unsigned int tableSize = DEFAULT_SIZE;

	unsigned int hash(const string& key);

public:
	HashTable();
	HashTable(unsigned int size);
	void PrintCourse(Course course);
	void PrintAll();
	void Insert(Course course);
	Course Search(string courseId);
};

/**
 * Default constructor
 */
HashTable::HashTable() {
	// Initialize the structures used to hold bids
	// Initalize node structure by resizing tableSize
	nodes.resize(tableSize);
}

/**
 * Constructor for specifying size of the table
 * Use to improve efficiency of hashing algorithm
 * by reducing collisions without wasting memory.
 */
HashTable::HashTable(unsigned int size) {
	// invoke local tableSize to size with this->
	// resize nodes size
	this->tableSize = size;
	nodes.resize(size);
}

/**
 * @param key The key to hash
 * @return The calculated hash
 */
unsigned int HashTable::hash(const string& key) {

	unsigned int hashValue = 0;
	for (char ch : key) {
		hashValue = (hashValue * 31 + ch) % tableSize;
	}
	return hashValue;
}

/**
 * Insert course
 *
 * @param course The course to insert
 */
void HashTable::Insert(Course course) {
	unsigned int tempKey = hash(course.courseId);

	//test to make sure data is being inserted
	cout << "Inserting: " << course.courseId << endl;

	if (nodes.at(tempKey).key == UINT_MAX) {
		Node newNode = Node(course, tempKey);
		nodes.at(tempKey) = newNode;
	}
	else {
		Node* currNode = &nodes.at(tempKey);
		Node* newNode = new Node(course, tempKey);

		//If the next node isn't null, continue
		while (currNode->next != nullptr) {
			currNode = currNode->next;
		}

		//add new node to end
		currNode->next = newNode;
	}
}

// Open and read the file to load data
void loadCourses(HashTable* hashTable) {

	std::cout << "Loading file " << endl;

	// Define file variable
	string file;

	// Get filename
	// Use ANSI file encoding
	file = "CS 300 ABCU_Advising_Program_Input.csv";

	// Open file
	ifstream infile;
	infile.open(file);
	unordered_map<string, Course> courses;

	string line;
	while (getline(infile, line)) {
		stringstream ss(line);
		string courseId, courseTitle, prereq;

		// Parse CourseId
		getline(ss, courseId, ',');

		// Parse Title
		getline(ss, courseTitle, ',');

		// Parse prerequisites (split by comma)
		vector<string> prereqs;
		while (getline(ss, prereq, ',')) {
			prereqs.push_back(prereq);
		}

		// Store in hash table
		Course course;
		course.courseId = courseId;
		course.courseTitle = courseTitle;
		course.prereqs = prereqs;
		hashTable->Insert(course);

	}

	infile.close();
}

void HashTable::PrintCourse(Course course) {
	std::cout << course.courseId << ", " << course.courseTitle << endl;
	std::cout << "Prerequisites: ";

	for (const auto& p : course.prereqs) {
		std::cout << p << " ";
	}
	std::cout << endl;
	return;
}

/**
 * Print all courses
 */
void HashTable::PrintAll() {
	// Implement logic to print all courses
	// for node begin to end iterate
	for (unsigned int i = 0; i < tableSize; ++i) {
		Node* currNode = &nodes.at(i);

		// Skip empty buckets
		if (currNode->key == UINT_MAX) continue;

		// Traverse the linked list at this bucket
		while (currNode != nullptr) {
			std::cout << currNode->course.courseId << ", "
				<< currNode->course.courseTitle;

			if (!currNode->course.prereqs.empty()) {
				std::cout << " | Prerequisites: ";
				for (const auto& p : currNode->course.prereqs) {
					std::cout << p << " ";
				}
			}
			std::cout << endl;

			currNode = currNode->next;
		}
	}
}

/**
 * Search for the specified courseId
 *
 * @param courseId The course ID to search for
 */
Course HashTable::Search(string courseId) {
	Course course;

	//  Implement logic to search for and return course

	// create the key for the given course
	unsigned int tempKey = hash(courseId);
	Node* currNode = &nodes.at(tempKey);

	while (currNode != nullptr) {

		// if entry found for the key
		if ((currNode->key != UINT_MAX) &&
			(currNode->course.courseId == courseId)) {
			return currNode->course;
		}
		currNode = currNode->next;
	}

	return course;
}

int main() {

	// Define a hash table to hold all the bids
	HashTable* courseTable;
	Course course;
	courseTable = new HashTable();

	int userInput = 0;
	while (userInput != 9) {
		std::cout << "Menu:" << endl;
		std::cout << "  1. Load Course Data" << endl;
		std::cout << "  2. Print Course List" << endl;
		std::cout << "  3. Print Course" << endl;
		std::cout << "  9. Exit" << endl;
		std::cout << "Enter option: ";
		cin >> userInput;

		if ((userInput != 1) && (userInput != 2) && (userInput != 3) && (userInput != 9)) {
			std::cout << "Please enter a number listed in the menu." << endl;
		}

		switch (userInput) {

		case 1:
			// Complete the method call to load the bids
			loadCourses(courseTable);
			std::cout << "Data loaded." << endl;
			break;

		case 2:
			courseTable->PrintAll();
			break;

		case 3:

			string courseId;

			std::cout << "What course are you interested in?" << endl;
			cin >> courseId;

			Course result = courseTable->Search(courseId);
			if (!result.courseId.empty()) {
				PrintCourse(result);
			}
			else {
				std::cout << "Course ID " << courseId << " not found." << endl;
			}

			break;

		}
	}

	std::cout << "Good bye." << endl;

	return 0;
}
