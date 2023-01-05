/*
File: textGenerator.cpp
Alexis Rodriguez
arodr247
CS 251
Project 2 - Generating Text
Description: A program that reads in a file and stores N keys into a map along 
with N values. Then you have the ability to generate random text with the
keys and values stored in the map. Also the ability to start over and read in 
a different file, size of n, and total amout of words generated.
*/

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <queue>
#include <fstream>          // to read file
#include <algorithm>		// to lower the string
#include "myrandom.h"

using namespace std;

string toLowerCase(string line);
string trimString(string& passedString);
string getValue(map<string, vector<string>> &myMap, string keyString);
string getKeyString(queue<string> window);
int getRandom(int low, int high);
void loopMenu(char &input, string fileName);
void checkFileName(string &fileName);
void checkValeOfN(int &valueOfN);
void checkTotalWords(int &totalWords, int valueOfN);
void startingMenu(string &fileName, int &valueOfN, int &totalWords);
void insert(map<string, vector<string>> &myMap, string key, string word);
void spaceNeeded(int i, int totalWords);
void print(map<string, vector<string>> &myMap);
void splitUp(queue<string>& tempQueue, string keyString);
void loadQueue(vector<string>& keyVector, queue<string>& randomQueue);
void getKeys(map<string, vector<string>> &myMap, vector<string>& keyVector);
void build(map<string, vector<string>> &myMap, string fileName, int valueOfN);
void startOver(map<string, vector<string>> &myMap, vector<string> &keyVector,
string &fileName, int &valueOfN, int &totalWords);
void startingWindow(int nGram, queue<string>& window, ifstream& infile,
vector<string>& extraWords);
void randomText(map<string, vector<string>> &myMap, vector<string>& keyVector,
int totalWords);
void generateText(map<string, vector<string>> &myMap, int totalWords,
vector<string>& keyVector);
void wrapAround(queue<string>& window, map<string, vector<string>> &myMap,
vector<string>& extraWords);

int main() {
	map<string, vector<string>> myMap;
	vector<string> keyVector;  // only has keys from map
	string fileName = "";
	char input = ' ';
	int valueOfN = 0, totalWords = 0;

    startingMenu(fileName, valueOfN, totalWords);
    loopMenu(input, fileName);

    while (tolower(input) != 'x') {
		if (tolower(input) == 'b') {  // builds map
			cout << endl;
			cout << "...Building map: " << fileName << "..." << endl;
			cout << endl;
			build(myMap, fileName, valueOfN);
		}
		if (tolower(input) == 'p') {  // prints map
			cout << endl;
			print(myMap);
			cout << endl;
		}
		if (tolower(input) == 'g') {  // generates text
			cout << endl;
			generateText(myMap, totalWords, keyVector);
			cout << endl;
		}
		if (tolower(input) == 's') {  // start over
			startOver(myMap, keyVector, fileName, valueOfN, totalWords);
		}
		loopMenu(input, fileName);
    }
    return 0;
}

//----------------------------------------------------------------------------->
// function used to get input in while loop
void loopMenu(char &input, string fileName) {
	cout << "Type b-build map, p-print map, g-generate text, s-start over,";
	cout << " x-to exit: ";
	cin >> input;
}
//<-----------------------------------------------------------------------------

//----------------------------------------------------------------------------->
// function to lower any string passed through using <algorithm>
string toLowerCase(string line) {
    transform(line.begin(), line.end(), line.begin(), ::tolower);
    return line;
}
//<-----------------------------------------------------------------------------

//----------------------------------------------------------------------------->
// function that checks for wrong file name
void checkFileName(string &fileName) {
	ifstream infile;
	fileName = toLowerCase(fileName);  // lowers the fileName
	infile.open(fileName);

	while (!infile.is_open()) {  // checks if the file doesn't exists
		cout << "Invalid file, try again: ";
		cin >> fileName;
		infile.open(fileName);  // tries again
	}
	infile.close();
}
//<-----------------------------------------------------------------------------

//----------------------------------------------------------------------------->
// makes sure the value of N entered is greater than 1
void checkValeOfN(int &valueOfN) {
	while (valueOfN <= 1) {
		cout << "N must be > 1, try again: ";
		cin >> valueOfN;
	}
}
//<-----------------------------------------------------------------------------

//----------------------------------------------------------------------------->
// makes sure the total amount of words generated is at least N
void checkTotalWords(int &totalWords, int valueOfN) {
	while (totalWords < valueOfN) {
		cout << "Total words must be at least N, try again: ";
		cin >> totalWords;
	}
}
//<-----------------------------------------------------------------------------

//----------------------------------------------------------------------------->
// the menu when the program is first loaded or starts over
void startingMenu(string &fileName, int &valueOfN, int &totalWords) {
	cout << "Welcome to the Text Generator." << endl;
    cout << "This program makes random text based on a document." << endl;
    cout << "Input file name? ";
    cin >> fileName;
    checkFileName(fileName);  // checks if the fileName is valid
    cout << "Value of N? ";
    cin >> valueOfN;
    checkValeOfN(valueOfN);  // checks if the valueOfN is valid
    cout << "Total words you'd like to generate? ";
    cin >> totalWords;
    checkTotalWords(totalWords, valueOfN);  // checks if totalWords is valid
}
//<-----------------------------------------------------------------------------

//----------------------------------------------------------------------------->
// trims beginning of the passedString if a space is found and then returns it
string trimString(string& passedString) {
    string whitespace = (" \t\n\r");
    passedString.erase(0, passedString.find_first_not_of(whitespace));
    return passedString;
}
//<-----------------------------------------------------------------------------

//----------------------------------------------------------------------------->
// function clears both keyVector and myMap, and gets new input
void startOver(map<string, vector<string>> &myMap, vector<string> &keyVector,
string &fileName, int &valueOfN, int &totalWords) {
	startingMenu(fileName, valueOfN, totalWords);
	myMap.clear();
	keyVector.clear();
}
//<-----------------------------------------------------------------------------

//----------------------------------------------------------------------------->
// function inserts a pair(key, value) if key isn't found in map or
// pushs a value into the map if theres already a key
void insert(map<string, vector<string>> &myMap, string key, string word) {
	map<string, vector<string>>::iterator it;  // used to find key in map
	it = myMap.find(key);
	if (it != myMap.end()) {  // if the key exists in the map
		it->second.push_back(word);
	} else {  // key does not exist in map
		vector<string> myVector;  // make new vector to hold keys
		myVector.push_back(word);
		myMap[key] = myVector;  // makes the pair EX: {to, be} -> {or}
	}
}
//<-----------------------------------------------------------------------------

//----------------------------------------------------------------------------->
// function that checks if space is needed in output
// used in print() and randomText()
void spaceNeeded(int i, int totalWords) {
	if ((i + 1) != totalWords) {  // if the next iteration is not end
		cout << " ";
	}
}
//<-----------------------------------------------------------------------------

//----------------------------------------------------------------------------->
// function that prints the map
void print(map<string, vector<string>> &myMap) {
	size_t i = 0;  // used for spaceNeeded()
	for (auto const& x : myMap) {
		cout << "{" << x.first << "} -> {";  // x.first == keys
		i = 0;
		for (auto const& y : x.second) {  // x.second == values
			cout << y;
			spaceNeeded(i, x.second.size());
			i++;
		}
		cout << "}" << endl;
	}
}
//<-----------------------------------------------------------------------------

//----------------------------------------------------------------------------->
// function to read in n times using (nGram) and adds it to to window(queue)
// and inserts extra words in extraWords vector for the wrap around
void startingWindow(int nGram, queue<string>& window, ifstream& infile,
vector<string>& extraWords) {
	string word;  // holds the file input
	for (int q = 0; q < nGram; q++) {
		infile >> word;
		window.push(word);
		extraWords.push_back(word);
	}
}
//<-----------------------------------------------------------------------------

//----------------------------------------------------------------------------->
// uses randomInteger() to return a random int in between set bounds of low&high
// low usually is 0, and high usually is the size of container - 1
int getRandom(int low, int high) {
	// Fix this to not use myrandom.h
	// int randomNumber = randomInteger(low, high);
	return randomNumber;
}
//<-----------------------------------------------------------------------------

//----------------------------------------------------------------------------->
// makes a vector(keyVector) that stores all the keys in the map(myMap)
void getKeys(map<string, vector<string>> &myMap, vector<string>& keyVector) {
	for (auto const& x : myMap) {
		keyVector.push_back(x.first);
	}
}
//<-----------------------------------------------------------------------------

//----------------------------------------------------------------------------->
// function the gets a random value by searching for keyString in myMap
// then getting the vector of values at keyString
// then getting a random value in the vector using randomNum
// and then returns valueString
string getValue(map<string, vector<string>> &myMap, string keyString) {
	int low = 0, high = 0, randomNum = 0;  // used for getRandom()
	string valueString = "";
	vector<string> valueVector;  // holds the values found at keyString
	valueVector = myMap.find(keyString)->second;  // looks for key in myMap
	high = valueVector.size() - 1;
	randomNum = getRandom(low , high);
	valueString = valueVector[randomNum];  // random value picked out of vector
	return valueString;
}
//<-----------------------------------------------------------------------------

//----------------------------------------------------------------------------->
// splits up  the keyString for example if keyString == "to be"(1 element)
// it goes into the queue as two elements == "to" "be"
void splitUp(queue<string>& tempQueue, string keyString) {
	string word = "";
	while (keyString.find(" ") != string::npos) {
		word = keyString.substr(0, keyString.find(" "));  // finds word by a " "
		keyString = keyString.substr(word.length() + 1, keyString.length());
		// ^erases the word part from keyString to get next word^
		tempQueue.push(word);
	}
	tempQueue.push(keyString);  // at the end of loop
}
//<-----------------------------------------------------------------------------

//----------------------------------------------------------------------------->
// loads the queue at the beginning of randomText() by getting a randomKey
// and uses randomKey to find the keyString at randomKey(index)
void loadQueue(vector<string>& keyVector, queue<string>& randomQueue) {
	string keyString = "";
	int low = 0, high = keyVector.size() - 1, randomKey = 0;
	randomKey = getRandom(low, high);
	keyString = keyVector[randomKey];  // Ex: keyString == "be who" (1 element)
	splitUp(randomQueue, keyString);  // Ex: randomQueue == be, who (2 elements)
	cout << keyString << " ";
}
//<-----------------------------------------------------------------------------

//----------------------------------------------------------------------------->
// return the value(string) for the map by going through the window(queue)
// and adding every element in queue to tempValue
string getKeyString(queue<string> window) {
	string keyString = "";
	string front = "";
	while (!window.empty()) {  // Ex window == {to, be}
		front = window.front();  // Ex: "to"
		keyString = keyString + " " + front;  // EX: "to be" at the end of loop
		window.pop();  // Ex window now == "be" O(1)
	}
	keyString = trimString(keyString);  // trims of any whitespace
	return keyString;
}
//<-----------------------------------------------------------------------------

//----------------------------------------------------------------------------->
// function first load the randomQueue by getting a random key from myMap and
// then just pushes values into the queue that matches the keyString every loop
void randomText(map<string, vector<string>> &myMap, vector<string>& keyVector,
int totalWords) {
	string keyString = "";
	string valueString = "";
	queue <string> randomQueue;  // holds the random values
	loadQueue(keyVector, randomQueue);  // loads the window(randomQueue)

	int i = randomQueue.size();
	// used in while() to only print the total amount of words(totalWords)
	while (i != totalWords) {
		keyString = getKeyString(randomQueue);
		valueString = getValue(myMap, keyString);
		cout << valueString;  // prints random value
		spaceNeeded(i, totalWords);
		randomQueue.push(valueString);
		randomQueue.pop();
		i++;
	}
}
//<-----------------------------------------------------------------------------

//----------------------------------------------------------------------------->
// function to generate random text by calling getKeys() and randomText()
void generateText(map<string, vector<string>> &myMap, int totalWords,
vector<string>& keyVector) {
	cout << "...";
	getKeys(myMap, keyVector);
	randomText(myMap, keyVector, totalWords);
	cout << "..." << endl;
}
//<-----------------------------------------------------------------------------

//----------------------------------------------------------------------------->
// used when window(queue) still has keys with no values left to match
// so we used the vector(extraWords) to get more values instead of reading
// in the file again
void wrapAround(queue<string>& window, map<string, vector<string>> &myMap,
vector<string>& extraWords) {
	int i = 0;
	int remainingPops = window.size();  // keys left in window
	string value = "";
	string keyString = "";

	while (i != remainingPops) {  // O(N)
		value = extraWords[i];  // extra words used as values
		keyString = getKeyString(window);
		insert(myMap, keyString, value);  // makes pair in map
		window.pop();
		window.push(value);
		i++;
	}
}
//<-----------------------------------------------------------------------------

//----------------------------------------------------------------------------->
// builds the map(myMap) by reading in file(fileName) and the amount of keys per
// pair is equal to valueOfN - 1 Ex: valueOfN == 3 - 1 -> {be just}
void build(map<string, vector<string>> &myMap, string fileName, int valueOfN) {
	string word = "";
	int nGram = valueOfN - 1;  // amount of keys in pair {key key}
	string keyString = "";
	ifstream infile;
	infile.open(fileName);
	queue <string> window;  // holds keys
	vector<string> extraWords;  // for wrap around

	// function called to read in n times keys and add to to starting window
	startingWindow(nGram, window, infile, extraWords);
	// -------------------------------------------------------------
	while (!infile.eof()) {
	infile >> word;
		if (!infile.fail()) {
			keyString = getKeyString(window);  // gets the keyString
			insert(myMap, keyString, word);  // makes pair w keyString & value
			window.pop();
			window.push(word);  // push new key
		}
	}
	infile.close();
	// -------------------------------------------------------------
	wrapAround(window, myMap, extraWords);
}
//<-----------------------------------------------------------------------------
