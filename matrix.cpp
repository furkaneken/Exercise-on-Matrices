#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<sstream>
using namespace std;

//this function calculates the factorial of the given number
int fact(const int & a)
{
	if (a == 1 || a == 0)
		return 1;
	else
		return a * fact(a - 1);
}

//this function finds all possibilities of N choose k and stores in a 2D vector
//the path followed is, creating k positions and incrementing values starting from pos 0, a combination is done when the increment is done on the k-1th pos
//the position incremented will be the k-1th pos until it reaches maximum value
//when the current position reaches its maximum value (matrixcount-(k-pos)) previous position is incremented
//incrementing will continue until there are N choose k combinations
vector<vector<int>> combinator(const vector<vector<vector<int>>> & pool, int k) 
{
	vector<vector<int>> com;
	vector<int> sidecom (k);

	int numcom = fact(pool.size()) / (fact(k) * fact(pool.size() - k)) ;
	bool finished = false; //are all the combinations calculated?
	int coor = 0, value = 0; //first is the value, second is the coordinate of the value in the combination

	while (!finished)
	{
		while (coor != k && !finished) //if the last increment is made on last position, it means this combination is done
		{
			if (numcom != com.size())
			{
				sidecom[coor] = value;
				value++;
				coor++;
			}
			else
				finished = true;
		}
		coor--; //coordinate needs a decrement to stay on the last position
		while (value > pool.size() - (k - coor) && coor > 0) //increments the previous positions value if the current value is the highest possible
			{
				value = sidecom[coor-1] + 1;
				coor--; //coordinate moves to the previous position
			}
		if (!finished)
			com.push_back(sidecom);
	}

	return com;
}

//this function adds matrices according to the combinations list
vector<vector<int>> calculator(const vector<int> & combination, const vector<vector<vector<int>>> & pool)
{
	vector<vector<int>> sum = vector<vector<int>>(pool[0].size(), vector<int>(pool[0][0].size()));

	for (int j = 0; j < pool[0].size(); j++)
		for (int i = 0; i < pool[0][0].size(); i++)
			for (int c = 0;  c < combination.size(); c++)
				sum[j][i] += pool[combination[c]][j][i];

	return sum;
}

//this function returns true if the matrices given are the same, returns false otherwise
bool isMatch(vector<vector<int>> vec1, vector<vector<int>> vec2)
{
	bool match = true;
	for (int i = 0; i < vec1.size() && match; i++)
		for (int j = 0; j < vec1[0].size() && match; j++)
			if (vec1[i][j] != vec2[i][j])
				match = false;

	return match;
}

//main function prompts filename and k, reads the file, stores in matrices, sums up the combinations, compares with the matrices and gives an appropriate message
int main()
{
	int matrices;
	int rows;
	int cols;
	int k;
	fstream input;
	string filename;
	cout << "Please enter the input file name: ";
	cin >> filename;
	input.open(filename);
	while (input.fail())
	{
		cout << "Invalid file name. Please enter again: ";
		cin >> filename;
		input.open(filename.c_str());
	}

	string line = "";

	input >> matrices >> rows >> cols;

	vector<vector<vector<int>>> pool = vector<vector<vector<int>>>(matrices, vector<vector<int>>(rows, vector<int>(cols))); //3D vector decleration

	while (line == "")
		getline(input, line); //skipping lines until the first matrix

	stringstream ss(line);
	string dummy; //will use in checking the input
	for (int m = 0; m < matrices; m++) //reads the matrices and stores in pool
	{
		stringstream ss(line);
		for (int j = 0; line != ""; j++)
		{
			if (j >= rows) //if the number of rows exceeds the declared value, the file is invalid
			{
				if (input.eof()) //the only exception is having finished reading the file
					break;
				cout << "Invalid file" << endl;
				return 0;
			}
			stringstream ss(line);
			for (int i = 0; i < cols; i++)
			{
				ss >> pool[m][j][i];
			}
			getline(input, line);
			if (ss >> dummy) //if the stream can still write after the vector is full, the file is invalid
			{
				cout << "Invalid file" << endl;
				return 0;
			}
		}
		getline(input, line);	
	}

	cout << "Enter k: ";
	cin >> k;

	vector<vector<int>> combination = combinator(pool, k); //listing all combinations according to k in a 2D vector

	vector<vector<vector<int>>> summation; //3D vector for storing all combinations of sums

	for (int i = 0; i < combination.size(); i++) //calculating all combinations and storing in summation
		summation.push_back(calculator(combination[i], pool));

	bool found = false; //is there any match found?

	for (int i = 0; i < summation.size(); i++) //comparing every combination with every matrix in pool
	{
		for (int j = 0; j < pool.size(); j++)
		{
			if (isMatch(summation[i], pool[j]))
			{
				cout << "Matrix " << j << " can be written as the sum of matrices";
				found = true;
				for (int c = 0; c < combination[0].size(); c++)
				{
					cout << " " << combination[i][c];
				}
			}
		}
	}
	if(!found)
		cout << "There are no valid combinations in the given input file";
	
	return 0;
}