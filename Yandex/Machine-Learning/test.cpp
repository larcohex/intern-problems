#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


using namespace std;


double string_to_double( const std::string& s )
{
	istringstream i(s);
	double x;
	if (!(i >> x))
		return 0;
	return x;
}

int main()
{
	fstream test ("input.txt");
	ofstream output ("output2.txt");
	while (!test.eof())
	{
		string temp;
		getline (test, temp);
		string n1, n2;
		double temp1, temp2;
		bool t = false;
		for (int i = 0; i < temp.size(); ++i)
		{
			if (!t)
			{
				if (temp[i] != ' ')
					n1.push_back (temp[i]);
				else
					t = true;
			}
			else
				n2.push_back (temp[i]);
		}
		temp1 = string_to_double (n1);
		temp2 = string_to_double (n2);
		temp.clear();
		output << temp1 << " + " << temp2 << " = " << temp1 + temp2 << endl;
		output << temp1 << " - " << temp2 << " = " << temp1 - temp2 << endl;
		output << temp1 << " * " << temp2 << " = " << temp1 * temp2 << endl;
		output << temp1 << " / " << temp2 << " = " << temp1 / temp2 << endl;
		output << temp1 << " > " << temp2 << " = " << int (temp1 > temp2) << endl;
		output << temp1 << " < " << temp2 << " = " << int (temp1 < temp2) << endl;
		output << temp1 << " >= " << temp2 << " = " << int (temp1 >= temp2) << endl;
		output << temp1 << " <= " << temp2 << " = " << int (temp1 <= temp2) << endl;
		output << temp1 << " == " << temp2 << " = " << int (temp1 == temp2) << endl;
		output << temp1 << " != " << temp2 << " = " << int (temp1 != temp2) << endl;
		output << "\n\n\n\n\n" << endl;
	}
	test.close();
	output.close();
}
