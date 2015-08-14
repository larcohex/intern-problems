#include <iostream>
#include <fstream>
#include "query.h"


using namespace std;


int main()
{
	Query test;
	fstream input ("test.txt");
	ofstream output ("result.txt");
	while (!input.eof())
	{
		string line;
		getline (input, line);
		cout << line << endl;
		test.setQuery (line);
		output << test << endl;
	}
	input.close();
	output.close();
	return 0;
}
