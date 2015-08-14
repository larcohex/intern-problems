/*

			DOCUMENTATION

	I have decided to implement this program in OOP style.
	Query is the class where basically query is processed.
	I tried to make helpful comments inside code. Hope it
	will be understandable.
	"Query" class automatically processes query once query
	is set. Then the result can be outputted in an output
	stream (like cout).
	There are multiple ways in which you can retrieve ori-
	ginal query and result. Also there are multiple ways to
	set query.
	Note that original query does not change in the process
	for further manipulations with it. Also note that query
	cannot take negative values but can produce them.
	Query can contain any number of spaces, tab and newline
	characters.
	Allowed numerical values are integers and floating-point
	numbers (reading both '.' and ',' as a point and allowing
	.001 like numbers where the characteristic of this
	expression is assumed to be 0).
	Numerical values can be of any length (note that increa-
	sing length will result in slower performance).
	Comparison operators return numerical values instead of
	"true" and "false", so you can use their result in fur-
	ther arithmetic operations.
	My algorithm works in the following way:
	1. Checking for validity
	If query has alphabetic characters - result won't be
	yilded.
	If query has only a number without arithmetic operators
	result won't be yilded.
	2. Converting expression into RNP.
	If in the process it finds invalid expressions, result
	won't be yilded.
	3. Solving RNP.
	Tribute to Donald Knuth for his algorithms on big num-
	bers.

*/


/**********************************************************************************************************************/


/*

				Methods

	Query() - default constructor that waits a query from a user
	Query (string &str) - constructor that takes a query and di-
	rectly processes it
	Query (const char *str) - constructor that takes a query and
	directly processes it
	Query (const Query &q) - copy constructor
	string getQuery() - returns original query
	string getResult() - returns result of query processing
	bool isValid() - checks if query is valid
	void setQuery (string &str) - sets query and processes it
	void setQuery (const char *str) - sets query and processes it
	Assignment operator is overloaded.
	Output stream operator is overloaded. It outputs result of
	query processing.

*/



#ifndef QUERY_H
#define QUERY_H
#include <string>
#include <list>


using namespace std;


class Query
{


	private:

		/*		Variables		*/

		string query; /* query - query that is to be processed */
		string result; /* result - result of processing */


		/*		Helper functions		*/

		bool isValue (string &str) /* checks if str is a numerical value */
		{
			bool fl_point = false; /* assures that str has only one floating point */
			for (int i = 0; i < str.size(); ++i)
			{
				if (!fl_point && (str[i] == '.' || str[i] == ','))
					fl_point = true;
				else if (fl_point && (str[i] == '.' || str[i] == ','))
					return false;
				else if (!(str[i] > 47 && str[i] < 58))
					return false;
			}
			return true;
		}


		bool isNotValidExpr (string &str) /* checks if str is not a valid expression */
		{
			if (isValue (str))
				return false;
			else if (str == "==" || str == "!=" || str == "<" || str == ">" || str == "<=" || str == ">=" || str == "+" || str == "-" || str == "*" || str == "/" || str == "%" || str == "^" || str == "(" || str == ")")
				return false;
			return true;
		}


		int precedence (string &str) /* assigns precedence value to an operator in str */
		{
			if (str == "(")
				return 0;
			else if (str == "==" || str == "!=")
				return 1;
			else if (str == "<" || str == ">" || str == "<=" || str == ">=")
				return 2;
			else if (str == "+" || str == "-")
				return 3;
			else if (str == "*" || str == "/" || str == "%")
				return 4;
			else if (str == "^")
				return 5;
		}


		void delWhitespaces (string &str) /* deletes whitespaces, tab and newline characters from str */
		{
			for (int i = 0; i < str.size(); ++i)
			{
				if (str[i] == ' ' || str[i] == '\t' || str[i] == '\n')
				{
					str.erase (i, 1);
					--i;
				}
			}
		}


		list <string> partition (string &str) /* divides string into list of numerical values and operators */
		{
			delWhitespaces (str); /* delete whitespaces, tab and newline characters first */
			list <string> result; /* will be returned in the end */
			string temp; /* keeps numerical values and operators one by one */
			bool op = false; /* true when it reads operator, false otherwise */
			for (int i = 0; i < str.size(); ++i)
			{
				if (str[i] == 40 || str[i] == 41) /* if it reads brackets */
				{
					if (!temp.empty())
					{
						result.push_back (temp);
						temp.clear();
					}
					temp.push_back (str[i]);
					result.push_back (temp);
					temp.clear();
				}
				else if (!op) /* if it is reading numerical value */
				{
					if ((str[i] > 47 && str[i] < 58) || str[i] == 46 || str[i] == 44)
					{
						temp.push_back (str[i]);
					}
					else
					{
						op = true;
						if (!temp.empty())
						{
							result.push_back (temp);
							temp.clear();
						}
						temp.push_back (str[i]);
					}
				}
				else /* if it is reading an operator */
				{
					if (!((str[i] > 47 && str[i] < 58) || str[i] == 46 || str[i] == 44))
					{
						temp.push_back (str[i]);
					}
					else
					{
						op = false;
						if (!temp.empty())
						{
							result.push_back (temp);
							temp.clear();
						}
						temp.push_back (str[i]);
					}
				}
			}
			if (!temp.empty())
				result.push_back (temp);
			return result;
		}


		list <string> shuntingYard (string &str) /* performs shunting-yard algorithm on an expression in str; returns empty list if it finds invalid expressions */
		{
			list <string> expr = partition (str); /* divide string into parts */
			list <string> stack; /* stack for further manipulations */
			list <string> output; /* will be returned in the end */
			while (!expr.empty())
			{
				if (isNotValidExpr (expr.front())) /* will return empty list if expression it reads is invalid */
				{
					list <string> error;
					return error;
				}		
				else if (isValue (expr.front())) /* if is numerical value it will be directly pushed into the output */
					output.push_back (expr.front());
				else if (expr.front() == "(" || expr.front() == "^" || precedence (expr.front()) > precedence (stack.front())) /* left bracket and exponentiation will always be pushed into the stack */
					stack.push_front (expr.front());
				else if (expr.front() == ")") /* pops until left bracket is found */
				{
					bool found = false;
					while (!stack.empty() && !found)
					{
						if (stack.front() == "(")
						{
							found = true;
							stack.pop_front();
							break;
						}
						output.push_back (stack.front());
						stack.pop_front();
					}
					if (!found)
					{
						list <string> error;
						return error;
					}
				}
				else if (precedence (expr.front()) <= precedence (stack.front())) /* pops until its precedence is less than or equal to the precedence of the one on top of the stack */
				{
					while (precedence (expr.front()) <= precedence (stack.front()))
					{
						output.push_back (stack.front());
						stack.pop_front();
					}
					stack.push_front (expr.front());
				}
				expr.pop_front();
			}
			while (!stack.empty()) /* if there are still items in the stack */
			{
				if (stack.front() == "(") /* returns error since there is unpaired left bracket left */
				{
					list <string> error;
					return error;
				}
				output.push_back (stack.front());
				stack.pop_front();
			}
			return output;
		}


		string addHelper (string &n1, string &n2) /* helper function for an addition */
		{

			/*
			Algorithm equates sizes of n1 and n2 to make summation easier.
			For example, for 123 + 42, it will make
			 0123
			+0042
			-----
			 0165
			165 is the result. We keep at least one zero for the following condition.
			 0918
			+0097
			-----
			 1015
			*/

			if (n1 == "0") /* will directly return without further operations */
				return n2;
			else if (n2 == "0") /* will directly return without further operations */
				return n1;
			else
			{
				bool f1 = (int)(n1.find_first_of ('.')) == -1 && (int)(n1.find_first_of (',')) == -1;
				bool f2 = (int)(n2.find_first_of ('.')) == -1 && (int)(n2.find_first_of (',')) == -1;
				if (f1 && f2) /* if both of them are integers */
				{
					while (n1.size() < n2.size()) /* inserting zeros at the beginning, will help in the future */
					{
						n1.insert (n1.begin(), '0');
					}
					while (n2.size() < n1.size()) /* inserting zeros at the beginning, will help in the future */
					{
						n2.insert (n2.begin(), '0');
					}
					n1.insert (n1.begin(), '0'); /* another one if the sum has one more digit */
					n2.insert (n2.begin(), '0'); /* another one if the sum has one more digit */
					string result; /* will be returned in the end */
					int s = n1.size();
					for (int i = 0; i < s; ++i) /* pushing zeros to change them in the summation */
					{
						result.push_back ('0');
					}
					for (int i = s - 1; i > 0; --i)
					{
						int temp1 = result[i] + n1[i] + n2[i] - 144; /* since we are dealing with characters, we need to subtract 144 */
						int temp2 = temp1 / 10; /* if the sum of digits exceed 10 */
						result[i] = (temp1 % 10) + 48;
						result[i - 1] += temp2;
					}
					while (result[0] == '0' && result[1] != '.') /* if there are zeros left at the beginning */
					{
						result.erase (result.begin());
					}
					return result;
				}
				else if (f1) /* if the first one is an integer while the second is a floating-point number */
				{
					int c2 = 0; /* keeps size of characteristic of the second number */
					for (int i = 0; n2[i] != '.' && n2[i] != ','; ++i) /* counting c2 */
					{
						++c2;
					}
					int m = n2.size() - c2 - 1; /* keeps the size of mantissa */
					int c1 = n1.size();
					n1.push_back ('.');
					for (int i = 0; i < m; ++i) /* zeros are added after floating point, will help in summation */
					{
						n1.push_back ('0');
					}
					while (c1 > c2) /* inserting zeros at the beginning, will help in the future */
					{
						n2.insert (n2.begin(), '0'); 
						++c2;
					}
					while (c2 > c1) /* inserting zeros at the beginning, will help in the future */
					{
						n1.insert (n1.begin(), '0');
						++c1;
					}
					n1.insert (n1.begin(), '0'); /* another one if the sum has one more digit */
					++c1;
					n2.insert (n2.begin(), '0'); /* another one if the sum has one more digit */
					++c2;
					string result; /* will be returned in the end */
					for (int i = 0; i < c1; ++i) /* pushing zeros to change them in the summation */
					{
						result.push_back ('0');
					}
					result.push_back ('.');
					for (int i = 0; i < m; ++i)
					{
						result.push_back ('0');
					}
					for (int i = n1.size() - 1; i > c1; --i) /* summing mantissas */
					{
						int temp1 = result[i] + n1[i] + n2[i] - 144; /* since we are dealing with characters, we need to subtract 144 */
						int temp2 = temp1 / 10; /* if the sum of digits exceed 10 */
						result[i] = (temp1 % 10) + 48;
						result[i - 1] += temp2;
					}
					result[c1 - 1] += result[c1] - '.';
					result[c1] = '.';
					for (int i = c1 - 1; i > 0; --i) /* summing characteristics */
					{
						int temp1 = result[i] + n1[i] + n2[i] - 144; /* since we are dealing with characters, we need to subtract 144 */
						int temp2 = temp1 / 10;
						result[i] = (temp1 % 10) + 48; /* if the sum of digits exceed 10 */
						result[i - 1] += temp2;
					}
					while (result[0] == '0' && result[1] != '.') /* if there are zeros left at the beginning */
					{
						result.erase (result.begin());
					}
					return result;
				}
				else if (f2) /* if the first one is a floating-point number while the second is an integer */
				{
					int c1 = 0; /* keeps size of characteristic of the first number */
					for (int i = 0; n1[i] != '.' && n1[i] != ','; ++i) /* counting c1 */
					{
						++c1;
					}
					int m = n1.size() - c1 - 1; /* keeps the size of mantissa */
					int c2 = n2.size();
					n2.push_back ('.');
					for (int i = 0; i < m; ++i) /* zeros are added after floating point, will help in summation */
					{
						n2.push_back ('0');
					}
					while (c1 > c2) /* inserting zeros at the beginning, will help in the future */
					{
						n2.insert (n2.begin(), '0');
						++c2;
					}
					while (c2 > c1) /* inserting zeros at the beginning, will help in the future */
					{
						n1.insert (n1.begin(), '0');
						++c1;
					}
					n1.insert (n1.begin(), '0'); /* another one if the sum has one more digit */
					++c1;
					n2.insert (n2.begin(), '0'); /* another one if the sum has one more digit */
					++c2;
					string result; /* will be returned in the end */
					for (int i = 0; i < c1; ++i) /* pushing zeros to change them in the summation */
					{
						result.push_back ('0');
					}
					result.push_back ('.');
					for (int i = 0; i < m; ++i)
					{
						result.push_back ('0');
					}
					for (int i = n1.size() - 1; i > c1; --i) /* summing mantissas */
					{
						int temp1 = result[i] + n1[i] + n2[i] - 144; /* since we are dealing with characters, we need to subtract 144 */
						int temp2 = temp1 / 10; /* if the sum of digits exceed 10 */
						result[i] = (temp1 % 10) + 48;
						result[i - 1] += temp2;
					}
					result[c1 - 1] += result[c1] - '.';
					result[c1] = '.';
					for (int i = c1 - 1; i > 0; --i) /* summing characteristics */
					{
						int temp1 = result[i] + n1[i] + n2[i] - 144; /* since we are dealing with characters, we need to subtract 144 */
						int temp2 = temp1 / 10;
						result[i] = (temp1 % 10) + 48; /* if the sum of digits exceed 10 */
						result[i - 1] += temp2;
					}
					while (result[0] == '0' && result[1] != '.') /* if there are zeros left at the beginning */
					{
						result.erase (result.begin());
					}
					return result;
				}
				else /* if both of them are floating-point numbers */
				{
					int c1 = 0, c2 = 0, m1 = 0, m2 = 0; /* sizes characteristics and mantissas */
					bool fl = false; /* false if it is reading characteristic, true when it reads mantissa */
					for (int i = 0; i < n1.size(); ++i) /* counting c1 and m1 */
					{
						if (!fl)
						{
							if (n1[i] != '.' && n1[i] != ',')
								++c1;
							else
								fl = true;
						}
						else
							++m1;
					}
					fl = false;
					for (int i = 0; i < n2.size(); ++i) /* counting c2 and m2 */
					{
						if (!fl)
						{
							if (n2[i] != '.' && n2[i] != ',')
								++c2;
							else
								fl = true;
						}
						else
							++m2;
					}
					while (c1 > c2) /* inserting zeros at the beginning, will help in the future */
					{
						n2.insert (n2.begin(), '0');
						++c2;
					}
					while (c2 > c1) /* inserting zeros at the beginning, will help in the future */
					{
						n1.insert (n1.begin(), '0');
						++c1;
					}
					while (m1 > m2) /* inserting zeros at the end, will help in the future */
					{
						n2.push_back ('0');
						++m2;
					}
					while (m2 > m1) /* inserting zeros at the end, will help in the future */
					{
						n1.push_back ('0');
						++m1;
					}
					n1.insert (n1.begin(), '0'); /* another one if the sum has one more digit */
					++c1;
					n2.insert (n2.begin(), '0'); /* another one if the sum has one more digit */
					++c2;
					string result; /* will be returned in the end */
					for (int i = 0; i < c1; ++i) /* pushing zeros to change them in the summation */
					{
						result.push_back ('0');
					}
					result.push_back ('.');
					for (int i = 0; i < m1; ++i)
					{
						result.push_back ('0');
					}
					for (int i = n1.size() - 1; i > c1; --i) /* summing mantissas */
					{
						int temp1 = result[i] + n1[i] + n2[i] - 144; /* since we are dealing with characters, we need to subtract 144 */
						int temp2 = temp1 / 10;
						result[i] = (temp1 % 10) + 48; /* if the sum of digits exceed 10 */
						result[i - 1] += temp2;
					}
					result[c1 - 1] += result[c1] - '.';
					result[c1] = '.';
					for (int i = c1 - 1; i > 0; --i) /* summing characteristics */
					{
						int temp1 = result[i] + n1[i] + n2[i] - 144; /* since we are dealing with characters, we need to subtract 144 */
						int temp2 = temp1 / 10;
						result[i] = (temp1 % 10) + 48; /* if the sum of digits exceed 10 */
						result[i - 1] += temp2;
					}
					while (result[result.size() - 1] == '0' || result[result.size() - 1] == '.') /* if there are zeros left at the end in mantissa */
					{
						if (result[result.size() - 1] == '.')
						{
							result.erase (result.end() - 1);
							break;
						}
						else
							result.erase (result.end() - 1);
					}
					while (result[0] == '0' && result[1] != '.') /* if there are zeros left at the beginning */
					{
						result.erase (result.begin());
					}
					return result;
				}
			}
		}


		string subtrHelper (string &n1, string &n2) /* helper function for a subtraction */
		{

			/*
			Algorithm equates sizes of n1 and n2 to make subtraction easier.
			For example, for 123 - 42, it will make
			 123
			-042
			----
			 081
			81 is the result.
			It does borrowing after subtraction.
			Note that it assumes that n1 is greater than n2.
			Other conditions are dealt in subtraction function
			*/

			if (n2 == "0") /* will directly return without further operations */
				return n1;
			else
			{
				bool f1 = ((int)(n1.find_first_of ('.')) == -1 && (int)(n1.find_first_of (',')) == -1);
				bool f2 = ((int)(n2.find_first_of ('.')) == -1 && (int)(n2.find_first_of (',')) == -1);
				if (f1 && f2) /* if both of them are integers */
				{		
					string result; /* will be returned in the end */
					int s1 = n1.size();
					int s2 = n2.size();
					for (int i = 0; i < s1; ++i) /* pushing zeros to change them in the summation */
					{
						result.push_back ('0');
					}
					for (int i = 0; i < s2; ++i) /* performing subtraction without borrowing */
					{
						int temp = n1[s1 - 1 - i] - n2[s2 - 1 - i];
						result[s1 - 1 - i] += temp;
					}
					for (int i = 0; i < s1 - s2; ++i) /* taking the rest from n1 */
					{
						result[i] = n1[i];
					}
					for (int i = s1 - 1; i > 0; --i) /* borrowing if it is necessary */
					{
						if (result[i] < 48)
						{
							int j = i;
							while (result[j] <= 48)
							{
								--j;
							}
							while (j < i)
							{
								result[j + 1] += 10;
								--result[j];
								++j;
							}
						}
					}
					while (result[0] == '0') /* if there are zeros left at the beginning */
					{
						result.erase (result.begin());
					}
					return result;
				}
				else if (f1) /* if the first one is an integer while the second is a floating-point number */
				{
					int c2 = 0; /* keeps size of characteristic of the second number */
					for (int i = 0; n2[i] != '.' && n2[i] != ','; ++i) /* counting c2 */
					{
						++c2;
					}
					int m = n2.size() - c2 - 1; /* keeps the size of mantissa */
					int c1 = n1.size();
					n1.push_back ('.');
					for (int i = 0; i < m; ++i) /* zeros are added after floating point, will help in subtraction */
					{
						n1.push_back ('0');
					}
					while (c1 > c2) /* inserting zeros at the beginning, will help in the future */
					{
						n2.insert (n2.begin(), '0');
						++c2;
					}
					string result; /* will be returned in the end */
					for (int i = 0; i < c1; ++i) /* pushing zeros to change them in the subtraction */
					{
						result.push_back ('0');
					}
					result.push_back ('.');
					for (int i = 0; i < m; ++i)
					{
						result.push_back ('0');
					}
					for (int i = c1 + m; i >= 0; --i) /* performing subtraction without borrowing */
					{
						int temp = n1[i] - n2[i];
						result[i] += temp;
					}
					for (int i = result.size() - 1; i > 0; --i) /* borrowing if it is necessary */
					{
						if (result[i] < 48 && i != c1)
						{
							int j = i;
							while (result[j] <= 48)
							{
								--j;
							}
							while (j < i)
							{
								result[j + 1] += 10;
								--result[j];
								++j;
							}
						}
					}
					result[c1] = '.';
					while (result[0] == '0' && result[1] != '.') /* if there are zeros left at the beginning */
					{
						result.erase (result.begin());
					}
					return result;
				}
				else if (f2) /* if the first one is a floating-point number while the second is an integer */
				{
					int c1 = 0; /* keeps size of characteristic of the first number */
					for (int i = 0; n1[i] != '.' && n1[i] != ','; ++i) /* counting c1 */
					{
						++c1;
					}
					int m = n1.size() - c1 - 1; /* keeps the size of mantissa */
					int c2 = n2.size();
					n2.push_back ('.');
					for (int i = 0; i < m; ++i) /* zeros are added after floating point, will help in subtraction */
					{
						n2.push_back ('0');
					}
					while (c1 > c2) /* inserting zeros at the beginning, will help in the future */
					{
						n2.insert (n2.begin(), '0');
						++c2;
					}
					string result; /* will be returned in the end */
					for (int i = 0; i < c1; ++i)
					{
						result.push_back ('0');
					}
					result.push_back ('.'); /* pushing zeros to change them in the summation */
					for (int i = 0; i < m; ++i)
					{
						result.push_back ('0');
					}
					for (int i = c1 + m; i >= 0; --i) /* performing subtraction without borrowing */
					{
						int temp = n1[i] - n2[i];
						result[i] += temp;
					}
					for (int i = result.size() - 1; i > 0; --i) /* borrowing if it is necessary */
					{
						if (result[i] < 48 && i != c1)
						{
							int j = i;
							while (result[j] <= 48)
							{
								--j;
							}
							while (j < i)
							{
								result[j + 1] += 10;
								--result[j];
								++j;
							}
						}
					}
					result[c1] = '.';
					while (result[0] == '0' && result[1] != '.') /* if there are zeros left at the beginning */
					{
						result.erase (result.begin());
					}
					return result;
				}
				else /* if both of them are floating-point numbers */
				{
					int c1 = 0, c2 = 0, m1 = 0, m2 = 0; /* sizes of characteristics and mantissas */
					bool fl = false; /* false if it is reading characteristic, true when it reads mantissa */
					for (int i = 0; i < n1.size(); ++i) /* counting c1 and m1 */
					{
						if (!fl)
						{
							if (n1[i] != '.' && n1[i] != ',')
								++c1;
							else
								fl = true;
						}
						else
							++m1;
					}
					fl = false;
					for (int i = 0; i < n2.size(); ++i) /* counting c2 and m2 */
					{
						if (!fl)
						{
							if (n2[i] != '.' && n2[i] != ',')
								++c2;
							else
								fl = true;
						}
						else
							++m2;
					}
					while (c1 > c2) /* inserting zeros at the beginning, will help in the future */
					{
						n2.insert (n2.begin(), '0');
						++c2;
					}
					while (m1 > m2) /* inserting zeros at the end, will help in the future */
					{
						n2.push_back ('0');
						++m2;
					}
					while (m2 > m1) /* inserting zeros at the end, will help in the future */
					{
						n1.push_back ('0');
						++m1;
					}
					string result; /* will be returned in the end */
					for (int i = 0; i < c1; ++i) /* pushing zeros to change them in the subtraction */
					{
						result.push_back ('0');
					}
					result.push_back ('.');
					for (int i = 0; i < m1; ++i)
					{
						result.push_back ('0');
					}
					for (int i = c1 + m1; i >= 0; --i) /* performing subtraction without borrowing */
					{
						int temp = n1[i] - n2[i];
						result[i] += temp;
					}
					for (int i = result.size() - 1; i > 0; --i) /* borrowing if it is necessary */
					{
						if (result[i] < 48 && i != c1)
						{
							int j = i;
							while (result[j] <= 48)
							{
								--j;
							}
							while (j < i)
							{
								result[j + 1] += 10;
								--result[j];
								++j;
							}
						}
					}
					result[c1] = '.';
					while (result[result.size() - 1] == '0' || result[result.size() - 1] == '.') /* if there are zeros left at the end in mantissa */
					{
						if (result[result.size() - 1] == '.')
						{
							result.erase (result.end() - 1);
							break;
						}
						else
							result.erase (result.end() - 1);
					}
					while (result[0] == '0' && result[1] != '.') /* if there are zeros left at the beginning */
					{
						result.erase (result.begin());
					}
					return result;
				}
			}
		}


		string multHelper (string &n1, string &n2) /* helper function for a multiplication */
		{
			/*
			Floating point is inserted to the result in the end.
			In any case it takes away floating points from n1 and n2 to make multiplication easier.
			For example, for 12.3 * 42, it will make
			 123
			 *42
			 ---
			 246
			492
			----
			5166
			516.6 is the result.
			*/
			int c1 = 0, c2 = 0, m1 = 0, m2 = 0; /* sizes of characteristics and mantissas */
			bool fl = false; /* false if it is reading characteristic, true when it reads mantissa */
			for (int i = 0; i < n1.size(); ++i) /* counting c1 and m1 */
			{
				if (!fl)
				{
					if (n1[i] != '.' && n1[i] != ',')
						++c1;
					else
						fl = true;
				}
				else
					++m1;
			}
			fl = false;
			for (int i = 0; i < n2.size(); ++i) /* counting c2 and m2 */
			{
				if (!fl)
				{
					if (n2[i] != '.' && n2[i] != ',')
						++c2;
					else
						fl = true;
				}
				else
					++m2;
			}
			n1.erase (c1, 1); /* erasing floating point */
			n2.erase (c2, 1); /* erasing floating point */
			int s1 = n1.size();
			int s2 = n2.size();
			if (s2 == 1) /* if size of n2 is one */
			{
				bool inserted = false; /* true if floating point was inserted to the result, false otherwise */
				string result; /* will be returned in the end */
				for (int i = 0; i < s1 + 1; ++i) /* pushing zeros to change them in the multiplication */
				{
					result.push_back ('0');
				}
				for (int i = 0; i < s1; ++i) /* performing multiplication */
				{
					int temp1 = (result[s1 - i] - 48) + (n1[s1 - 1 - i] - 48) * (n2[0] - 48);
					int temp2 = temp1 / 10;
					temp1 %= 10;
					result[s1 - i] = temp1 + 48;
					result[s1 - 1 - i] += temp2;
				}
				while (result[0] == '0') /* if there are zeros left at the beginning */
				{
					result.erase (result.begin());
				}
				if (m1 + m2 > result.size()) /* inserting floating point and zeros */
				{
					while (result.size() < m1 + m2)
					{
						result.insert (result.begin(), '0');
					}
					result.insert (result.begin(), '.');
					result.insert (result.begin(), '0');
					inserted = true;
				}
				else /* inserting floating point only */
				{
					if (m1 + m2)
						result.insert (result.size() - m1 - m2, 1, '.');
				}
				while (inserted && (result[result.size() - 1] == '0' || result[result.size() - 1] == '.')) /* if there are zeros left at the end in mantissa */
				{
					if (result[result.size() - 1] == '.')
					{
						result.erase (result.end() - 1);
						break;
					}
					else
						result.erase (result.end() - 1);
				}
				return result;
			}
			else if (s1 == 1) /* if size of n1 is one */
			{
				bool inserted = false; /* true if floating point was inserted to the result, false otherwise */
				string result; /* will be returned in the end */
				for (int i = 0; i < s2 + 1; ++i) /* pushing zeros to change them in the multiplication */
				{
					result.push_back ('0');
				}
				for (int i = 0; i < s2; ++i) /* performing multiplication */
				{
					int temp1 = (result[s2 - i] - 48) + (n2[s2 - 1 - i] - 48) * (n1[0] - 48);
					int temp2 = temp1 / 10;
					temp1 %= 10;
					result[s2 - i] = temp1 + 48;
					result[s2 - 1 - i] += temp2;
				}
				while (result[0] == '0') /* if there are zeros left at the beginning */
				{
					result.erase (result.begin());
				}
				if (m1 + m2 > result.size()) /* inserting floating point and zeros */
				{
					while (result.size() < m1 + m2)
					{
						result.insert (result.begin(), '0');
					}
					result.insert (result.begin(), '.');
					result.insert (result.begin(), '0');
					inserted = true;
				}
				else /* inserting floating point only */
				{
					if (m1 + m2)
						result.insert (result.size() - m1 - m2, 1, '.');
				}
				while (inserted && (result[result.size() - 1] == '0' || result[result.size() - 1] == '.')) /* if there are zeros left at the end in mantissa */
				{
					if (result[result.size() - 1] == '.')
					{
						result.erase (result.end() - 1);
						break;
					}
					else
						result.erase (result.end() - 1);
				}
				return result;
			}
			else
			{
				string result[s2]; /* array of results of multiplication of n1 and digits in n2 */
				for (int i = 0; i < s2; ++i) /* performing multiplication */
				{
					string temp; /* result of multiplication of n1 and ith digit */
					for (int j = 0; j < s1 + s2; ++j) /* pushing zeros to change them in the multiplication */
					{
						temp.push_back ('0');
					}
					result[i] = temp;
					for (int j = 0; j < s1; ++j)
					{
						int temp1 = (result[i][s2 + s1 - 1 - j - i] - 48) + (n1[s1 - 1 - j] - 48) * (n2[s2 - 1 - i] - 48);
						int temp2 = temp1 / 10;
						temp1 %= 10;
						result[i][s2 + s1 - 1 - j - i] = temp1 + 48;
						result[i][s2 + s1 - 2 - j - i] += temp2;
					}
				}
				string compResult; /* will be returned in the end */
				for (int i = 0; i < s1 + s2; ++i) /* pushing zeros to change them in the multiplication */
				{
					compResult.push_back ('0');
				}
				for (int i = 0; i < s2; ++i) /* performing summation */
				{
					for (int j = 0; j < s1 + s2; ++j)
					{
						compResult[s1 + s2 - 1 - j] += result[i][s1 + s2 - 1 - j] - 48;
						if (j != s1 + s2 - 1)
						{
							compResult[s1 + s2 - 2 - j] += (compResult[s1 + s2 - 1 - j] - 48) / 10;
						}
						compResult [s1 + s2 - 1 - j] = 48 + (compResult[s1 + s2 - 1 - j] - 48) % 10;
					}
				}
				while (compResult[0] == '0') /* if there are zeros left at the beginning */
				{
					compResult.erase (compResult.begin());
				}
				bool inserted = false; /* true if floating point was inserted to the result, false otherwise */
				if (m1 + m2 > compResult.size()) /* inserting floating point and zeros */
				{
					while (compResult.size() < m1 + m2)
					{
						compResult.insert (compResult.begin(), '0');
					}
					compResult.insert (compResult.begin(), '.');
					compResult.insert (compResult.begin(), '0');
					inserted = true;
				}
				else /* inserting floating point only */
				{
					if (m1 + m2)
						compResult.insert (compResult.size() - m1 - m2, 1, '.');
				}
				while (inserted && (compResult[compResult.size() - 1] == '0' || compResult[compResult.size() - 1] == '.')) /* if there are zeros left at the end in mantissa */
				{
					if (compResult[compResult.size() - 1] == '.')
					{
						compResult.erase (compResult.end() - 1);
						break;
					}
					else
						compResult.erase (compResult.end() - 1);
				}
				return compResult;
			}
		}


		string divHelper (string &n1, string &n2) /* helper function for a division */
		{
			/*
			Algorithm uses long division logic to divide first number to second.
			It multiplies both numbers by 10 until both of them will become integers.
			It uses binary search for guessing.
			Result is computed until 8 digits of mantissa that don't start with 0 are found.
			For example, for 12.3 / 42, it will make
			1230|420
			 840|-------
			----|0.292...
			 3900
			 3780
			 ----
			  1200
			   840
			  ----
			   360
			   ...

			0.292... is the result.
			*/
			string quotient; /* will be returned in the end */
			int c1 = 0, c2 = 0, m1 = 0, m2 = 0; /* sizes of characteristics and mantissas */
			bool fl = false; /* false if it is reading characteristic, true when it reads mantissa */
			for (int i = 0; i < n1.size(); ++i) /* counting c1 and m1 */
			{
				if (!fl)
				{
					if (n1[i] != '.' && n1[i] != ',')
						++c1;
					else
						fl = true;
				}
				else
					++m1;
			}
			fl = false;
			for (int i = 0; i < n2.size(); ++i) /* counting c2 and m2 */
			{
				if (!fl)
				{
					if (n2[i] != '.' && n2[i] != ',')
						++c2;
					else
						fl = true;
				}
				else
					++m2;
			}
			int mancounter = 0; /* counts how many digits in mantissa it has found */
			n1.erase (c1, 1); /* erasing floating point */
			n2.erase (c2, 1); /* erasing floating point */
			while (n1[0] == '0') /* erasing zeros at the beginning */
			{
				n1.erase (n1.begin());
			}
			while (n2[0] == '0') /* erasing zeros at the beginning */
			{
				n2.erase (n2.begin());
			}
			while (m1 < m2) /* adding zeros at the end */
			{
				n1.push_back ('0');
				++m1;
			}
			while (m2 < m1) /* adding zeros at the end */
			{
				n2.push_back ('0');
				++m2;
			}
			int zeros = n2.size() - n1.size(); /* number of zeros to be inserted if n2 > n1 */
			bool inserted = false; /* true if floating point was inserted to the result, false otherwise */
			while (mancounter != 8 && !n1.empty() && n1[0] != '0') /* performing division */
			{
				if (!inserted) /* if floating point was not inserted yet */
				{
					string temp; /* part of n1 that will be subtracted */
					if (n1.size() >= n2.size()) /* if partitioning is possible */
					{
						temp = n1.substr (0, n2.size());
						if (lessThan (temp, n2) == "1") /* if partitioning needs another digit */
						{
							if (n1.size() > n2.size()) /* if it is possible to add another integer */
								temp.push_back (n1[n2.size()]);
							else
							{
								if (quotient.empty())
									quotient.push_back ('0');
								quotient.push_back ('.');
								inserted = true;
								continue;
							}
						}
					}
					else /* if n1 is smaller */
					{
						if (quotient.empty())
							quotient.push_back ('0');
						quotient.push_back ('.');
						inserted = true;
						continue;
					}
					if (zeros >= 0 && !quotient.empty()) /* if there is a need to add zeros to the quotient */
					{
						for (int i = 0; i < temp.size() - zeros - 1; ++i)
						{
							quotient.push_back ('0');
						}
					}
					string div = "5"; /* guessing 5 */
					string temp1 = n2;
					div = multHelper (temp1, div);
					temp1 = temp;
					div = subtraction (temp1, div);
					if (div[0] == '-') /* if 5 is too big */
					{
						div = "2"; /* guessing 2 */
						temp1 = n2;
						div = multHelper (temp1, div);
						temp1 = temp;
						div = subtraction (temp1, div);
						if (div[0] == '-') /* if 2 is too big */
						{
							div = "1"; /* it is 1 for sure */
							quotient.push_back ('1');
							temp1 = n2;
							div = multHelper (temp1, div);
							temp1 = temp;
							div = subtraction (temp1, div);
							n1.erase (0, temp.size());
							if (div != "0")
							{
								n1.insert (0, div);
								zeros = div.size();
							}
							else
								zeros = 0;
						}
						else if (div == "0") /* if 2 makes subtraction zero */
						{
							n1.erase (0, temp.size());
							quotient.push_back ('2');
							zeros = 0;
						}
						else if (grThOrEqTo (div, n2) == "1") /* if 2 is too small */
						{
							div = "3"; /* guessing 3 */
							temp1 = n2;
							div = multHelper (temp1, div);
							temp1 = temp;
							div = subtraction (temp1, div);
							if (div == "0") /* if 3 makes subtraction zero */
							{
								n1.erase (0, temp.size());
								quotient.push_back ('3');
								zeros = 0;
							}
							else if (grThOrEqTo (div, n2) == "1") /* if 3 is too small */
							{
								string div = "4"; /* it is 4 for sure */
								quotient.push_back ('4');
								temp1 = n2;
								div = multHelper (temp1, div);
								temp1 = temp;
								div = subtraction (temp1, div);
								n1.erase (0, temp.size());
								if (div != "0")
								{
									n1.insert (0, div);
									zeros = div.size();
								}
								else
									zeros = 0;
							}
							else /* it is 3 for sure */
							{
								quotient.push_back ('3');
								n1.erase (0, temp.size());
								n1.insert (0, div);
								zeros = div.size();
							}
						}
						else /* it is 2 for sure */
						{
							quotient.push_back ('2');
							n1.erase (0, temp.size());
							n1.insert (0, div);
							zeros = div.size();
						}
					}
					else if (div == "0") /* if 5 makes subtraction zero */
					{
						n1.erase (0, temp.size());
						quotient.push_back ('5');
						zeros = 0;
					}
					else if (grThOrEqTo (div, n2) == "1") /* if 5 is too small */
					{
						div = "8"; /* guessing 8 */
						temp1 = n2;
						div = multHelper (temp1, div);
						temp1 = temp;
						div = subtraction (temp1, div);
						if (div[0] == '-') /* if 8 is too big */
						{
							div = "7"; /* guessing 7 */
							temp1 = n2;
							div = multHelper (temp1, div);
							temp1 = temp;
							div = subtraction (temp1, div);
							if (div == "0") /* if 7 makes subtraction zero */
							{
								n1.erase (0, temp.size());
								quotient.push_back ('7');
								zeros = 0;
							}
							else if (div[0] == '-') /* if 7 is too big */
							{
								string div = "6"; /* it is 6 for sure */
								quotient.push_back ('6');
								temp1 = n2;
								div = multHelper (temp1, div);
								temp1 = temp;
								div = subtraction (temp1, div);
								n1.erase (0, temp.size());
								if (div != "0")
								{
									n1.insert (0, div);
									zeros = div.size();
								}
								else
									zeros = 0;
							}
							else /* it is 7 for sure */
							{
								quotient.push_back ('7');
								n1.erase (0, temp.size());
								n1.insert (0, div);
								zeros = div.size();
							}
						}
						else if (div == "0") /* if 8 makes subtraction zero */
						{
							n1.erase (0, temp.size());
							quotient.push_back ('8');
							zeros = 0;
						}
						else if (grThOrEqTo (div, n2) == "1") /* if 8 is too small */
						{
							string div = "9"; /* it is 9 for sure */
							quotient.push_back ('9');
							temp1 = n2;
							div = multHelper (n2, div);
							temp1 = temp;
							div = subtraction (temp, div);
							n1.erase (0, temp.size());
							if (div != "0")
							{
								n1.insert (0, div);
								zeros = div.size();
							}
							else
								zeros = 0;
						}
						else /* it is 8 for sure */
						{
							quotient.push_back ('8');
							n1.erase (0, temp.size());
							n1.insert (0, div);
							zeros = div.size();
						}
					}
					else /* it is 5 for sure */
					{
						quotient.push_back ('5');
						n1.erase (0, temp.size());
						n1.insert (0, div);
						zeros = div.size();
					}
				}
				else
				{
					while (n1.size() <= n2.size()) /* push 0's to n1 to equate sizes */
					{
						n1.push_back ('0');
					}
					++mancounter;
					string temp = n1.substr (0, n2.size());
					if (lessThan (temp, n2) == "1") /* if partitioning needs another digit */
					{
						temp.push_back (n1[n2.size()]);
						/* if there is a need to add zeros to the quotient */
						if (quotient == "0.")
						{
							for (int i = 0; i < zeros; ++i)
							{
								quotient.push_back ('0');
							}
						}
						else
						{
							for (int i = 0; i < temp.size() - zeros - 1; ++i)
							{
								quotient.push_back ('0');
							}
						}
					}
					else /* if there is a need to add zeros to the quotient */
					{
						if (quotient == "0.")
						{
							for (int i = 0; i < zeros - 1; ++i)
							{
								quotient.push_back ('0');
							}
						}			
						else
						{
							for (int i = 0; i < temp.size() - zeros - 1; ++i)
							{
								quotient.push_back ('0');
							}
						}
					}
					string div = "5"; /* guessing 5 */
					string temp1 = n2;
					div = multHelper (temp1, div);
					temp1 = temp;
					div = subtraction (temp1, div);
					if (div[0] == '-') /* if 5 is too big */
					{
						div = "2"; /* guessing 2 */
						temp1 = n2;
						div = multHelper (temp1, div);
						temp1 = temp;
						div = subtraction (temp1, div);
						if (div[0] == '-') /* if 2 is too big */
						{
							div = "1"; /* it is 1 for sure */
							quotient.push_back ('1');
							temp1 = n2;
							div = multHelper (temp1, div);
							temp1 = temp;
							div = subtraction (temp1, div);
							n1.erase (0, temp.size());
							if (div != "0")
							{
								n1.insert (0, div);
								zeros = div.size();
							}
							else
								zeros = 0;
						}
						else if (div == "0") /* if 2 makes subtraction zero */
						{
							n1.erase (0, temp.size());
							quotient.push_back ('5');
							zeros = 0;
						}
						else if (grThOrEqTo (div, n2) == "1") /* if 2 is too small */
						{
							div = "3"; /* guessing 3 */
							temp1 = n2;
							div = multHelper (temp1, div);
							temp1 = temp;
							div = subtraction (temp1, div);
							if (div == "0") /* if 3 makes subtraction zero */
							{
								n1.erase (0, temp.size());
								quotient.push_back ('5');
								zeros = 0;
							}
							else if (grThOrEqTo (div, n2) == "1") /* if 3 is too small */
							{
								string div = "4"; /* it is 4 for sure */
								quotient.push_back ('4');
								temp1 = n2;
								div = multHelper (temp1, div);
								temp1 = temp;
								div = subtraction (temp1, div);
								n1.erase (0, temp.size());
								if (div != "0")
								{
									n1.insert (0, div);
									zeros = div.size();
								}
								else
									zeros = 0;
								}
							else /* it is 3 for sure */
							{
								quotient.push_back ('3');
								n1.erase (0, temp.size());
								n1.insert (0, div);
								zeros = div.size();
							}
						}
						else /* it is 2 for sure */
						{
							quotient.push_back ('2');
							n1.erase (0, temp.size());
							n1.insert (0, div);
							zeros = div.size();
						}
					}
					else if (div == "0") /* if 5 makes subtraction zero */
					{
						n1.erase (0, temp.size());
						quotient.push_back ('5');
						zeros = 0;
					}
					else if (grThOrEqTo (div, n2) == "1") /* if 5 is too small */
					{
						div = "8"; /* guessing 8 */
						temp1 = n2;
						div = multHelper (temp1, div);
						temp1 = temp;
						div = subtraction (temp1, div);
						if (div[0] == '-') /* if 8 is too big */
						{
							div = "7"; /* guessing 7 */
							temp1 = n2;
							div = multHelper (temp1, div);
							temp1 = temp;
							div = subtraction (temp1, div);
							if (div == "0") /* if 7 makes subtraction zero */
							{
								n1.erase (0, temp.size());
								quotient.push_back ('7');
								zeros = 0;
							}
							else if (div[0] == '-') /* if 7 is too big */
							{
								string div = "6"; /* it is 6 for sure */
								quotient.push_back ('6');
								temp1 = n2;
								div = multHelper (temp1, div);
								temp1 = temp;
								div = subtraction (temp1, div);
								n1.erase (0, temp.size());
								if (div != "0")
								{
									n1.insert (0, div);
									zeros = div.size();
								}
								else
									zeros = 0;
							}
							else /* it is 7 for sure */
							{
								quotient.push_back ('7');
								n1.erase (0, temp.size());
								n1.insert (0, div);
								zeros = div.size();
							}
						}
						else if (div == "0") /* if 8 makes subtraction zero */
						{
							n1.erase (0, temp.size());
							quotient.push_back ('8');
							zeros = 0;
						}
						else if (grThOrEqTo (div, n2) == "1") /* if 8 is too small */
						{
							string div = "9"; /* it is 9 for sure */
							quotient.push_back ('9');
							temp1 = n2;
							div = multHelper (n2, div);
							temp1 = temp;
							div = subtraction (temp, div);
							n1.erase (0, temp.size());
							if (div != "0")
							{
								n1.insert (0, div);
								zeros = div.size();
							}
							else
								zeros = 0;
						}
						else /* it is 8 for sure */
						{
							quotient.push_back ('8');
							n1.erase (0, temp.size());
							n1.insert (0, div);
							zeros = div.size();
						}
					}
					else /* it is 5 for sure */
					{
						quotient.push_back ('5');
						n1.erase (0, temp.size());
						n1.insert (0, div);
						zeros = div.size();
					}
				}
			}
			bool z = true; /* true if there are zeros left in n1, false otherwise */
			for (int i = 0; i < n1.size(); ++i)
			{
				if (n1[i] != '0')
				{
					z = false;
					break;
				}
			}
			if (z && !inserted) /* inserting zeros that left after division */
				quotient.insert (quotient.end(), n1.size(), '0');
			return quotient;
		}


		string modHelper (string &n1, string &n2) /* helper function for a modulo */
		{

			/*
			Algorithm uses long division logic to find the remainder.
			It assumes that both n1 and n2 are integers where n1 > n2 > 0. Other conditions are dealt in modulo function.
			It uses binary search for guessing.
			For example, for 123 / 42, it will make
			123|42
			 84|-------
			---|2
			 39

			39 is the result.
			*/
			bool z; /* true if there are only zeros left at the end of n1, false otherwise */
			while (n1.size() >= n2.size()) /* performing division */
			{
				z = true;
				for (int i = 0; i < n1.size(); ++i) /* checking if only zeros are left in n1 */
				{
					if (n1[i] != '0')
					{
						z = false;
						break;
					}
				}
				if (!z)
				{
					string temp = n1.substr (0, n2.size()); /* part of n1 that will be subtracted */
					if (lessThan (temp, n2) == "1") /* if partitioning needs another digit */
					{
						if (n1.size() > n2.size()) /* if it is possible to add another integer */
							temp.push_back (n1[n2.size()]);
						else /* remainder is found in n1 */
							return n1;
					}
					string div = "5"; /* guessing 5 */
					string temp1 = n2;
					div = multHelper (temp1, div);
					temp1 = temp;
					div = subtraction (temp1, div); /* if 5 is too big */
					if (div[0] == '-')
					{
						div = "2"; /* guessing 2 */
						temp1 = n2;
						div = multHelper (temp1, div);
						temp1 = temp;
						div = subtraction (temp1, div);
						if (div[0] == '-') /* if 2 is too big */
						{
							div = "1"; /* it is 1 for sure */
							temp1 = n2;
							div = multHelper (temp1, div);
							temp1 = temp;
							div = subtraction (temp1, div);
							n1.erase (0, temp.size());
							if (div != "0")
								n1.insert (0, div);
						}
						else if (div == "0") /* if 2 makes subtraction zero */
							n1.erase (0, temp.size());
						else if (grThOrEqTo (div, n2) == "1") /* if 2 is too small */
						{
							div = "3";
							temp1 = n2;
							div = multHelper (temp1, div);
							temp1 = temp;
							div = subtraction (temp1, div);
							if (div == "0") /* if 3 makes subtraction zero */
								n1.erase (0, temp.size());
							else if (grThOrEqTo (div, n2) == "1") /* if 3 is too small */
							{
								string div = "4"; /* it is 4 for sure */
								temp1 = n2;
								div = multHelper (temp1, div);
								temp1 = temp;
								div = subtraction (temp1, div);
								n1.erase (0, temp.size());
								if (n1 != "0")
									n1.insert (0, div);
							}
							else /* it is 3 for sure */
							{
								n1.erase (0, temp.size());
								n1.insert (0, div);
							}
						}
						else /* it is 2 for sure */
						{
							n1.erase (0, temp.size());
							n1.insert (0, div);
						}
					}
					else if (div == "0") /* if 5 makes subtraction zero */
						n1.erase (0, temp.size());
					else if (grThOrEqTo (div, n2) == "1") /* if 5 is too small */
					{
						div = "8"; /* guessing 8 */
						temp1 = n2;
						div = multHelper (temp1, div);
						temp1 = temp;
						div = subtraction (temp1, div);
						if (div[0] == '-') /* if 8 is too big */
						{
							div = "7"; /* guessing 7 */
							temp1 = n2;
							div = multHelper (temp1, div);
							temp1 = temp;
							div = subtraction (temp1, div);
							if (div == "0") /* if 7 makes subtraction zero */
								n1.erase (0, temp.size());
							else if (div[0] == '-') /* if 7 is too big */
							{
								string div = "6"; /* it is 6 for sure */
								temp1 = n2;
								div = multHelper (temp1, div);
								temp1 = temp;
								div = subtraction (temp1, div);
								n1.erase (0, temp.size());
								if (div != "0")
									n1.insert (0, div);
							}
							else /* it is 7 for sure */
							{
								n1.erase (0, temp.size());
								n1.insert (0, div);
							}
						}
						else if (div == "0") /* if 8 makes subtraction zero */
							n1.erase (0, temp.size());
						else if (grThOrEqTo (div, n2) == "1") /* if 8 is too small */
						{
							string div = "9"; /* it is 9 for sure */
							temp1 = n2;
							div = multHelper (n2, div);
							temp1 = temp;
							div = subtraction (temp, div);
							n1.erase (0, temp.size());
							if (div != "0")
								n1.insert (0, div);
						}
						else /* it is 8 for sure */
						{
							n1.erase (0, temp.size());
							n1.insert (0, div);
						}
					}
					else /* it is 5 for sure */
					{
						n1.erase (0, temp.size());
						n1.insert (0, div);
					}
				}
				else
					break;
			}
			if (n1.empty() || z) /* if n1 is divisible by n2 */
				n1 = "0";
			return n1;
		}


		string addition (string &n1, string &n2) /* returns n1 + n2; changes n1 and n2 in the process */
		{

			/* Deleting basic typos in input */

			if ((int)(n1.find_first_of ('.')) != -1 || (int)(n1.find_first_of (',')) != -1)
			{
				while (n1 != "0" && (n1[n1.size() - 1] == '0' || n1[n1.size() - 1] == '.' || n1[n1.size() - 1] == ','))
				{
					if (n1[n1.size() - 1] == '.' || n1[n1.size() - 1] == ',')
					{
						n1.erase (n1.end() - 1);
						break;
					}
					else
						n1.erase (n1.end() - 1);
				}
			}
			if ((int)(n2.find_first_of ('.')) != -1 || (int)(n2.find_first_of (',')) != -1)
			{
				while (n2[n2.size() - 1] == '0' || n2[n2.size() - 1] == '.' || n2[n2.size() - 1] == ',')
				{
					if (n2[n2.size() - 1] == '.' || n2[n2.size() - 1] == ',')
					{
						n2.erase (n2.end() - 1);
						break;
					}
					else
						n2.erase (n2.end() - 1);
				}
			}
			if (n1[0] != '-')
			{
				while (n1 != "0" && n1[0] == '0' && n1[1] != '.' && n1[1] != ',')
				{
					n1.erase (n1.begin());
				}
				while (n2 != "0" && n2[0] == '0' && n2[1] != '.' && n2[1] != ',')
				{
					n2.erase (n2.begin());
				}
			}
			else
			{
				while (n1 != "-0" && n1[1] == '0' && n1[2] != '.' && n1[2] != ',')
				{
					n1.erase (n1.begin() + 1);
				}
				while (n2 != "-0" && n2[1] == '0' && n2[2] != '.' && n2[2] != ',')
				{
					n2.erase (n2.begin() + 1);
				}
			}
			if (n1 == "-0")
				n1 = "0";
			if (n2 == "-0")
				n2 = "0";

			if (n1[0] == '-' && n2[0] == '-') /* if both n1 and n2 are negative */
			{
				n1.erase (n1.begin()); /* erase minus sign from n1 */
				n2.erase (n2.begin()); /* erase minus sign from n2 */
				string result = addHelper (n1, n2); /* perform addition on their absolute values */
				result.insert (result.begin(), '-'); /* add minus sign at the beginning */
				return result;
			}
			else if (n1[0] == '-') /* if only n1 is negative */
			{
				n1.erase (n1.begin()); /* erase minus sign from n1 */
				return subtraction (n2, n1); /* subtract n1 from n2 */
			}
			else if (n2[0] == '-') /* if only n2 is negative */
			{
				n2.erase (n2.begin()); /* erase minus sign from n2 */
				return subtraction (n1, n2); /* subtract n2 from n1 */
			}
			else /* if both of them are positive */
				return addHelper (n1, n2); /* perform addition */
		}


		string subtraction (string &n1, string &n2) /* returns n1 - n2; changes n1 and n2 in the process */
		{

			/* Deleting basic typos in input */

			if ((int)(n1.find_first_of ('.')) != -1 || (int)(n1.find_first_of (',')) != -1)
			{
				while (n1 != "0" && (n1[n1.size() - 1] == '0' || n1[n1.size() - 1] == '.' || n1[n1.size() - 1] == ','))
				{
					if (n1[n1.size() - 1] == '.' || n1[n1.size() - 1] == ',')
					{
						n1.erase (n1.end() - 1);
						break;
					}
					else
						n1.erase (n1.end() - 1);
				}
			}
			if ((int)(n2.find_first_of ('.')) != -1 || (int)(n2.find_first_of (',')) != -1)
			{
				while (n2[n2.size() - 1] == '0' || n2[n2.size() - 1] == '.' || n2[n2.size() - 1] == ',')
				{
					if (n2[n2.size() - 1] == '.' || n2[n2.size() - 1] == ',')
					{
						n2.erase (n2.end() - 1);
						break;
					}
					else
						n2.erase (n2.end() - 1);
				}
			}
			if (n1[0] != '-')
			{
				while (n1 != "0" && n1[0] == '0' && n1[1] != '.' && n1[1] != ',')
				{
					n1.erase (n1.begin());
				}
				while (n2 != "0" && n2[0] == '0' && n2[1] != '.' && n2[1] != ',')
				{
					n2.erase (n2.begin());
				}
			}
			else
			{
				while (n1 != "-0" && n1[1] == '0' && n1[2] != '.' && n1[2] != ',')
				{
					n1.erase (n1.begin() + 1);
				}
				while (n2 != "-0" && n2[1] == '0' && n2[2] != '.' && n2[2] != ',')
				{
					n2.erase (n2.begin() + 1);
				}
			}
			if (n1 == "-0")
				n1 = "0";
			if (n2 == "-0")
				n2 = "0";

			if (n1 == n2) /* return zero if both of them are equal */
			{
				string result = "0";
				return result;
			}
			else if (n1[0] == '-' && n2[0] == '-') /* if both of them are negative */
			{
				n1.erase (n1.begin()); /* erase minus sign from n1 */
				n2.erase (n2.begin()); /* erase minus sign from n2 */
				if (greaterThan (n2, n1) == "1") /* if n2 > n1 */
					return subtrHelper (n2, n1); /* subtract n1 from n2 */
				else
				{
					string result = subtrHelper (n1, n2); /* subtract n1 from n2 */
					result.insert (result.begin(), '-'); /* insert minus sign at the beginning */
					return result;
				}
			}
			else if (n1[0] == '-') /* if only n1 is negative */
			{
				n2.insert (n2.begin(), '-'); /* insert minus sign to n2 */
				return addition (n1, n2); /* perform addition */
			}
			else if (n2[0] == '-') /* if only n2 is negative */
			{
				n2.erase (n2.begin()); /* erase minus sign from n2 */
				return addHelper (n1, n2); /* perform addition */
			}
			else /* if both of them are positive */
			{
				if (greaterThan (n1, n2) == "1") /* if n1 is greater than n2 */
					return subtrHelper (n1, n2); /* subtract n2 from n1 */
				else
				{
					string result = subtrHelper (n2, n1); /* subtract n1 from n2 */
					result.insert (result.begin(), '-'); /* insert minus sign at the beginning */
					return result;
				}
			}
		}


		string multiplication (string &n1, string &n2) /* returns n1 * n2; changes n1 and n2 in the process */
		{
			/* Deleting basic typos in input */

			if ((int)(n1.find_first_of ('.')) != -1 || (int)(n1.find_first_of (',')) != -1)
			{
				while (n1 != "0" && (n1[n1.size() - 1] == '0' || n1[n1.size() - 1] == '.' || n1[n1.size() - 1] == ','))
				{
					if (n1[n1.size() - 1] == '.' || n1[n1.size() - 1] == ',')
					{
						n1.erase (n1.end() - 1);
						break;
					}
					else
						n1.erase (n1.end() - 1);
				}
			}
			if ((int)(n2.find_first_of ('.')) != -1 || (int)(n2.find_first_of (',')) != -1)
			{
				while (n2[n2.size() - 1] == '0' || n2[n2.size() - 1] == '.' || n2[n2.size() - 1] == ',')
				{
					if (n2[n2.size() - 1] == '.' || n2[n2.size() - 1] == ',')
					{
						n2.erase (n2.end() - 1);
						break;
					}
					else
						n2.erase (n2.end() - 1);
				}
			}
			if (n1[0] != '-')
			{
				while (n1 != "0" && n1[0] == '0' && n1[1] != '.' && n1[1] != ',')
				{
					n1.erase (n1.begin());
				}
				while (n2 != "0" && n2[0] == '0' && n2[1] != '.' && n2[1] != ',')
				{
					n2.erase (n2.begin());
				}
			}
			else
			{
				while (n1 != "-0" && n1[1] == '0' && n1[2] != '.' && n1[2] != ',')
				{
					n1.erase (n1.begin() + 1);
				}
				while (n2 != "-0" && n2[1] == '0' && n2[2] != '.' && n2[2] != ',')
				{
					n2.erase (n2.begin() + 1);
				}
			}
			if (n1 == "-0")
				n1 = "0";
			if (n2 == "-0")
				n2 = "0";

			if (n1 == "0" || n2 == "1") /* if n1 is zero or n2 is one, n1 is a result */
				return n1;
			else if (n1 == "1" || n2 == "0") /* if n1 is one or n2 is zero, n2 is a result */
				return n2;
			else if (n1[0] == '-' && n2[0] == '-') /* if both n1 and n2 are negative */
			{
				n1.erase (n1.begin()); /* erase minus sign from n1 */
				n2.erase (n2.begin()); /* erase minus sign from n2 */
				return multHelper (n1, n2); /* perform multiplication */
			}
			else if (n1[0] == '-') /* if only n1 is negative */
			{
				n1.erase (n1.begin()); /* erase minus sign from n1 */
				string result = multHelper (n1, n2); /* perform multiplication */
				result.insert (result.begin(), '-'); /* insert minus sign at the beginning */
				return result;
			}
			else if (n2[0] == '-') /* if only n2 is negative */
			{
				n2.erase (n2.begin()); /* erase minus sign from n2 */
				string result = multHelper (n1, n2); /* perform multiplication */
				result.insert (result.begin(), '-'); /* insert minus sign at the beginning */
				return result;
			}
			else
				return multHelper (n1, n2); /* perform multiplication */
		}


		string division (string &n1, string &n2) /* returns n1 / n2; changes n1 and n2 in the process */
		{

			/* Deleting basic typos in input */

			if ((int)(n1.find_first_of ('.')) != -1 || (int)(n1.find_first_of (',')) != -1)
			{
				while (n1 != "0" && (n1[n1.size() - 1] == '0' || n1[n1.size() - 1] == '.' || n1[n1.size() - 1] == ','))
				{
					if (n1[n1.size() - 1] == '.' || n1[n1.size() - 1] == ',')
					{
						n1.erase (n1.end() - 1);
						break;
					}
					else
						n1.erase (n1.end() - 1);
				}
			}
			if ((int)(n2.find_first_of ('.')) != -1 || (int)(n2.find_first_of (',')) != -1)
			{
				while (n2[n2.size() - 1] == '0' || n2[n2.size() - 1] == '.' || n2[n2.size() - 1] == ',')
				{
					if (n2[n2.size() - 1] == '.' || n2[n2.size() - 1] == ',')
					{
						n2.erase (n2.end() - 1);
						break;
					}
					else
						n2.erase (n2.end() - 1);
				}
			}
			if (n1[0] != '-')
			{
				while (n1 != "0" && n1[0] == '0' && n1[1] != '.' && n1[1] != ',')
				{
					n1.erase (n1.begin());
				}
				while (n2 != "0" && n2[0] == '0' && n2[1] != '.' && n2[1] != ',')
				{
					n2.erase (n2.begin());
				}
			}
			else
			{
				while (n1 != "-0" && n1[1] == '0' && n1[2] != '.' && n1[2] != ',')
				{
					n1.erase (n1.begin() + 1);
				}
				while (n2 != "-0" && n2[1] == '0' && n2[2] != '.' && n2[2] != ',')
				{
					n2.erase (n2.begin() + 1);
				}
			}
			if (n1 == "-0")
				n1 = "0";
			if (n2 == "-0")
				n2 = "0";

			if (n2 == "0") /* division is undefined if n2 is zero */
			{
				string result = "u";
				return result;
			}
			else if (n1 == "0" || n2 == "1") /* if n1 is zero or n2 is one, n1 is the result */
				return n1;
			else if (n1[0] == '-' && n2[0] == '-') /* if both n1 and n2 are negative */
			{
				n1.erase (n1.begin()); /* erase minus sign from n1 */
				n2.erase (n2.begin()); /* erase minus sign from n2 */
				return divHelper (n1, n2); /* perform division */
			}
			else if (n1[0] == '-') /* if only n1 is negative */
			{
				n1.erase (n1.begin()); /* erase minus sign from n1 */
				string result = divHelper (n1, n2); /* perform division */
				result.insert (result.begin(), '-'); /* insert minus sign at the beginning */
				return result;
			}
			else if (n2[0] == '-') /* if only n2 is negative */
			{
				n2.erase (n2.begin()); /* erase minus sign from n2 */
				string result = divHelper (n1, n2); /* perform division */
				result.insert (result.begin(), '-'); /* insert minus sign at the beginning */
				return result;
			}
			else
				return divHelper (n1, n2); /* perform division */
		}


		string modulo (string &n1, string &n2) /* returns n1 % n2; changes n1 and n2 in the process */
		{

			/* Deleting basic typos in input */

			if ((int)(n1.find_first_of ('.')) != -1 || (int)(n1.find_first_of (',')) != -1)
			{
				while (n1 != "0" && (n1[n1.size() - 1] == '0' || n1[n1.size() - 1] == '.' || n1[n1.size() - 1] == ','))
				{
					if (n1[n1.size() - 1] == '.' || n1[n1.size() - 1] == ',')
					{
						n1.erase (n1.end() - 1);
						break;
					}
					else
						n1.erase (n1.end() - 1);
				}
			}
			if ((int)(n2.find_first_of ('.')) != -1 || (int)(n2.find_first_of (',')) != -1)
			{
				while (n2[n2.size() - 1] == '0' || n2[n2.size() - 1] == '.' || n2[n2.size() - 1] == ',')
				{
					if (n2[n2.size() - 1] == '.' || n2[n2.size() - 1] == ',')
					{
						n2.erase (n2.end() - 1);
						break;
					}
					else
						n2.erase (n2.end() - 1);
				}
			}
			if (n1[0] != '-')
			{
				while (n1 != "0" && n1[0] == '0' && n1[1] != '.' && n1[1] != ',')
				{
					n1.erase (n1.begin());
				}
				while (n2 != "0" && n2[0] == '0' && n2[1] != '.' && n2[1] != ',')
				{
					n2.erase (n2.begin());
				}
			}
			else
			{
				while (n1 != "-0" && n1[1] == '0' && n1[2] != '.' && n1[2] != ',')
				{
					n1.erase (n1.begin() + 1);
				}
				while (n2 != "-0" && n2[1] == '0' && n2[2] != '.' && n2[2] != ',')
				{
					n2.erase (n2.begin() + 1);
				}
			}
			if (n1 == "-0")
				n1 = "0";
			if (n2 == "-0")
				n2 = "0";

			if ((int)(n1.find_first_of ('.')) == -1 && (int)(n1.find_first_of (',')) == -1 && (int)(n2.find_first_of ('.')) == -1 && (int)(n2.find_first_of (',')) == -1 && n1[0] != '-' && n2[0] != '-' && n2 != "0") /* if both of them are positive integers */
			{
				if (lessThan (n1, n2) == "1") /* n1 is a remainder if it is less than n2 */
					return n1;
				else
					return modHelper (n1, n2); /* perform modulo */
			}
			else /* result is undefined */
			{
				string result = "u";
				return result;
			}
		}


		string power (string &n1, string &n2) /* returns n1 ^ n2; changes n1 and n2 in the process */
		{

			/*
			Result will be undefined if n2 is a floating point number.
			Be careful with big n2.
			*/

			/* Deleting basic typos in input */

			if ((int)(n1.find_first_of ('.')) != -1 || (int)(n1.find_first_of (',')) != -1)
			{
				while (n1 != "0" && (n1[n1.size() - 1] == '0' || n1[n1.size() - 1] == '.' || n1[n1.size() - 1] == ','))
				{
					if (n1[n1.size() - 1] == '.' || n1[n1.size() - 1] == ',')
					{
						n1.erase (n1.end() - 1);
						break;
					}
					else
						n1.erase (n1.end() - 1);
				}
			}
			if ((int)(n2.find_first_of ('.')) != -1 || (int)(n2.find_first_of (',')) != -1)
			{
				while (n2[n2.size() - 1] == '0' || n2[n2.size() - 1] == '.' || n2[n2.size() - 1] == ',')
				{
					if (n2[n2.size() - 1] == '.' || n2[n2.size() - 1] == ',')
					{
						n2.erase (n2.end() - 1);
						break;
					}
					else
						n2.erase (n2.end() - 1);
				}
			}
			if (n1[0] != '-')
			{
				while (n1 != "0" && n1[0] == '0' && n1[1] != '.' && n1[1] != ',')
				{
					n1.erase (n1.begin());
				}
				while (n2 != "0" && n2[0] == '0' && n2[1] != '.' && n2[1] != ',')
				{
					n2.erase (n2.begin());
				}
			}
			else
			{
				while (n1 != "-0" && n1[1] == '0' && n1[2] != '.' && n1[2] != ',')
				{
					n1.erase (n1.begin() + 1);
				}
				while (n2 != "-0" && n2[1] == '0' && n2[2] != '.' && n2[2] != ',')
				{
					n2.erase (n2.begin() + 1);
				}
			}
			if (n1 == "-0")
				n1 = "0";
			if (n2 == "-0")
				n2 = "0";

			if ((int)(n2.find_first_of ('.')) == -1 && (int)(n2.find_first_of (',')) == -1) /* if n2 is an integer */
			{
				if (n2 == "0") /* zero exponentiation always leads to one */
				{
					string result = "1";
					return result;
				}
				if (n1 == "1") /* n1 stays the same if it is one */
					return n1;
				if (n1 == "-1") /* sign of n1 depends on n2 */
				{
					string temp = "2";
					if (n2[0] == '-') /* it is not important if n2 is negative or not */
						n2.erase (n2.begin());
					if (modulo (n2, temp) == "0") /* if n2 is even, result is positive */
					{
						temp = "1";
						return temp;
					}
					else
						return n1;
				}
				if (n2[0] == '-') /* if power is negative */
				{
					string temp = "1";
					n1 = division (temp, n1); /* taking reciprocal of n1 */
					n2.erase (n2.begin());
				}
				string result = "1"; /* will be returned in the end */
				while (n2 != "0") /* performing exponentiation */
				{
					string temp1 = "1";
					string temp2 = n1;
					n2 = subtraction (n2, temp1);
					result = multiplication (result, temp2);
				}
				return result;
			}
			else /* result is undefined */
			{
				string result = "u";
				return result;
			}
		}


		string compare (string &n1, string &n2) /* compares two strings; 1 - n1 is greater, 0 - n1 is less, 2 - n1 is equal to n2 */
		{
			string result;
			if (n1.size() > n2.size())
				result = "1";
			else if (n1.size() < n2.size())
				result = "0";
			else
			{
				if (n1 > n2)
					result = "1";
				else if (n1 < n2)
					result = "0";
				else
					result = "2";
			}
			return result;
		}


		string greaterThan (string &n1, string &n2) /* checks if n1 is greater than n2; returns 1 if it is true and 0 otherwise */
		{

			/* Deleting basic typos in input */

			if ((int)(n1.find_first_of ('.')) != -1 || (int)(n1.find_first_of (',')) != -1)
			{
				while (n1 != "0" && (n1[n1.size() - 1] == '0' || n1[n1.size() - 1] == '.' || n1[n1.size() - 1] == ','))
				{
					if (n1[n1.size() - 1] == '.' || n1[n1.size() - 1] == ',')
					{
						n1.erase (n1.end() - 1);
						break;
					}
					else
						n1.erase (n1.end() - 1);
				}
			}
			if ((int)(n2.find_first_of ('.')) != -1 || (int)(n2.find_first_of (',')) != -1)
			{
				while (n2[n2.size() - 1] == '0' || n2[n2.size() - 1] == '.' || n2[n2.size() - 1] == ',')
				{
					if (n2[n2.size() - 1] == '.' || n2[n2.size() - 1] == ',')
					{
						n2.erase (n2.end() - 1);
						break;
					}
					else
						n2.erase (n2.end() - 1);
				}
			}
			if (n1[0] != '-')
			{
				while (n1 != "0" && n1[0] == '0' && n1[1] != '.' && n1[1] != ',')
				{
					n1.erase (n1.begin());
				}
				while (n2 != "0" && n2[0] == '0' && n2[1] != '.' && n2[1] != ',')
				{
					n2.erase (n2.begin());
				}
			}
			else
			{
				while (n1 != "-0" && n1[1] == '0' && n1[2] != '.' && n1[2] != ',')
				{
					n1.erase (n1.begin() + 1);
				}
				while (n2 != "-0" && n2[1] == '0' && n2[2] != '.' && n2[2] != ',')
				{
					n2.erase (n2.begin() + 1);
				}
			}
			if (n1 == "-0")
				n1 = "0";
			if (n2 == "-0")
				n2 = "0";

			if (n1[0] == '-' && n2[0] == '-') /* if both of them are negative */
			{
				n1.erase (n1.begin()); /* erasing minus sign from n1 */
				n2.erase (n2.begin()); /* erasing minus sign from n2 */
				return lessThan (n1, n2); /* "greater than" changes to "less than" */
			}
			else if (n1[0] == '-') /* if only n1 is negative, it is not greater than n2 for sure */
			{
				string result = "0";
				return result;
			}
			else if (n2[0] == '-') /* if only n2 is negative, n1 is greater than it for sure */
			{
				string result = "1";
				return result;
			}
			else /* if both of them are positive */
			{
				string result; /* will be returned in the end */
				if (n1 == n2) /* if they are equal, n1 is not greater */
				{
					result = "0";
					return result;
				}
				else
				{
					int c1 = 0, c2 = 0; /* sizes of characteristics of n1 and n2 */
					for (int i = 0; n1[i] != '.' && n1[i] != ',' && i < n1.size(); ++i) /* counting c1 */
					{
						++c1;
					}
					for (int i = 0; n2[i] != '.' && n2[i] != ',' && i < n2.size(); ++i) /* counting c2 */
					{
						++c2;
					}
					if (c1 > c2) /* if size of characteristic part of n1 is greater than that of n2, n1 is greater */
					{
						result = "1";
						return result;
					}
					else if (c1 < c2) /* otherwise it is not greater */
					{
						result = "0";
						return result;
					}
					else /* if both sizes are equal */
					{
						string chstr1 (n1, 0, c1);
						string chstr2 (n2, 0, c2);
						string temp = compare (chstr1, chstr2); /* compairing characteristics */
						if (temp != "2")
							return temp;
						else /* compairing mantissas */
						{
							if (n1.size() != chstr1.size())
								chstr1 = n1.substr (c1 + 1, n1.size() - c1 - 1);
							else
								chstr1.clear();
							if (n2.size() != chstr2.size())
								chstr2 = n2.substr (c2 + 1, n2.size() - c2 - 1);
							else
								chstr2.clear();
							while (chstr1.size() < chstr2.size())
							{
								chstr1.push_back ('0');
							}
							while (chstr2.size() < chstr1.size())
							{
								chstr2.push_back ('0');
							}
							return compare (chstr1, chstr2);
						}
					}
				}
			}
		}


		string lessThan (string &n1, string &n2) /* checks if n1 is less than n2; returns 1 if it is true and 0 otherwise */
		{

			/* Deleting basic typos in input */

			if ((int)(n1.find_first_of ('.')) != -1 || (int)(n1.find_first_of (',')) != -1)
			{
				while (n1 != "0" && (n1[n1.size() - 1] == '0' || n1[n1.size() - 1] == '.' || n1[n1.size() - 1] == ','))
				{
					if (n1[n1.size() - 1] == '.' || n1[n1.size() - 1] == ',')
					{
						n1.erase (n1.end() - 1);
						break;
					}
					else
						n1.erase (n1.end() - 1);
				}
			}
			if ((int)(n2.find_first_of ('.')) != -1 || (int)(n2.find_first_of (',')) != -1)
			{
				while (n2[n2.size() - 1] == '0' || n2[n2.size() - 1] == '.' || n2[n2.size() - 1] == ',')
				{
					if (n2[n2.size() - 1] == '.' || n2[n2.size() - 1] == ',')
					{
						n2.erase (n2.end() - 1);
						break;
					}
					else
						n2.erase (n2.end() - 1);
				}
			}
			if (n1[0] != '-')
			{
				while (n1 != "0" && n1[0] == '0' && n1[1] != '.' && n1[1] != ',')
				{
					n1.erase (n1.begin());
				}
				while (n2 != "0" && n2[0] == '0' && n2[1] != '.' && n2[1] != ',')
				{
					n2.erase (n2.begin());
				}
			}
			else
			{
				while (n1 != "-0" && n1[1] == '0' && n1[2] != '.' && n1[2] != ',')
				{
					n1.erase (n1.begin() + 1);
				}
				while (n2 != "-0" && n2[1] == '0' && n2[2] != '.' && n2[2] != ',')
				{
					n2.erase (n2.begin() + 1);
				}
			}
			if (n1 == "-0")
				n1 = "0";
			if (n2 == "-0")
				n2 = "0";

			if (n1[0] == '-' && n2[0] == '-') /* if both of them are negative */
			{
				n1.erase (n1.begin()); /* erasing minus sign from n1 */
				n2.erase (n2.begin()); /* erasing minus sign from n2 */
				return greaterThan (n1, n2); /* "less than" changes to "greater than" */
			}
			else if (n1[0] == '-') /* if only n1 is negative, it is less than n2 for sure */
			{
				string result = "1";
				return result;
			}
			else if (n2[0] == '-') /* if only n2 is negative, n1 is not less than it for sure */
			{
				string result = "0";
				return result;
			}
			else /* if both of them are positive */
			{
				string result; /* will be returned in the end */
				if (n1 == n2) /* if they are equal, n1 is not greater */
				{
					result = "0";
					return result;
				}
				else
				{
					int c1 = 0, c2 = 0; /* sizes of characteristics of n1 and n2 */
					for (int i = 0; n1[i] != '.' && n1[i] != ',' && i < n1.size(); ++i) /* counting c1 */
					{
						++c1;
					}
					for (int i = 0; n2[i] != '.' && n2[i] != ',' && i < n2.size(); ++i) /* counting c2 */
					{
						++c2;
					}
					if (c1 > c2) /* if size of characteristic part of n1 is greater than that of n2, n1 is not less */
					{
						result = "0";
						return result;
					}
					else if (c1 < c2) /* otherwise it is not less */
					{
						result = "1";
						return result;
					}
					else /* if both sizes are equal */
					{
						string chstr1 (n1, 0, c1);
						string chstr2 (n2, 0, c2);
						string temp = compare (chstr1, chstr2); /* compairing characteristics */
						if (temp == "0")
						{
							result = "1";
							return result;
						}
						else if (temp == "1")
						{
							result = "0";
							return result;
						}
						else /* compairing mantissas */
						{
							if (n1.size() != chstr1.size())
								chstr1 = n1.substr (c1 + 1, n1.size() - c1 - 1);
							else
								chstr1.clear();
							if (n2.size() != chstr2.size())
								chstr2 = n2.substr (c2 + 1, n2.size() - c2 - 1);
							else
								chstr2.clear();
							while (chstr1.size() < chstr2.size())
							{
								chstr1.push_back ('0');
							}
							while (chstr2.size() < chstr1.size())
							{
								chstr2.push_back ('0');
							}
							result = compare (chstr1, chstr2);
							if (result == "0")
							{
								result = "1";
								return result;
							}
							else
							{
								result = "0";
								return result;
							}
						}
					}
				}
			}
		}


		string grThOrEqTo (string &n1, string &n2) /* checks if n1 is greater than or equal to n2; returns 1 if it is true and 0 otherwise */
		{

			/* Deleting basic typos in input */

			if ((int)(n1.find_first_of ('.')) != -1 || (int)(n1.find_first_of (',')) != -1)
			{
				while (n1 != "0" && (n1[n1.size() - 1] == '0' || n1[n1.size() - 1] == '.' || n1[n1.size() - 1] == ','))
				{
					if (n1[n1.size() - 1] == '.' || n1[n1.size() - 1] == ',')
					{
						n1.erase (n1.end() - 1);
						break;
					}
					else
						n1.erase (n1.end() - 1);
				}
			}
			if ((int)(n2.find_first_of ('.')) != -1 || (int)(n2.find_first_of (',')) != -1)
			{
				while (n2[n2.size() - 1] == '0' || n2[n2.size() - 1] == '.' || n2[n2.size() - 1] == ',')
				{
					if (n2[n2.size() - 1] == '.' || n2[n2.size() - 1] == ',')
					{
						n2.erase (n2.end() - 1);
						break;
					}
					else
						n2.erase (n2.end() - 1);
				}
			}
			if (n1[0] != '-')
			{
				while (n1 != "0" && n1[0] == '0' && n1[1] != '.' && n1[1] != ',')
				{
					n1.erase (n1.begin());
				}
				while (n2 != "0" && n2[0] == '0' && n2[1] != '.' && n2[1] != ',')
				{
					n2.erase (n2.begin());
				}
			}
			else
			{
				while (n1 != "-0" && n1[1] == '0' && n1[2] != '.' && n1[2] != ',')
				{
					n1.erase (n1.begin() + 1);
				}
				while (n2 != "-0" && n2[1] == '0' && n2[2] != '.' && n2[2] != ',')
				{
					n2.erase (n2.begin() + 1);
				}
			}
			if (n1 == "-0")
				n1 = "0";
			if (n2 == "-0")
				n2 = "0";

			if (n1[0] == '-' && n2[0] == '-') /* if both of them are negative */
			{
				n1.erase (n1.begin()); /* erasing minus sign from n1 */
				n2.erase (n2.begin()); /* erasing minus sign from n2 */
				return lessThOrEqTo (n1, n2); /* "greater than or equal to" changes to "less than or equal to" */
			}
			else if (n1[0] == '-') /* if only n1 is negative, it is not greater than or equal to n2 for sure */
			{
				string result = "0";
				return result;
			}
			else if (n2[0] == '-') /* if only n2 is negative, n1 is greater than or equal to it for sure */
			{
				string result = "1";
				return result;
			}
			else /* if both of them are positive */
			{
				string result; /* will be returned in the end */
				if (n1 == n2) /* if they are equal, result is true */
				{
					result = "1";
					return result;
				}
				else
				{
					int c1 = 0, c2 = 0; /* sizes of characteristics of n1 and n2 */
					for (int i = 0; n1[i] != '.' && n1[i] != ',' && i < n1.size(); ++i) /* counting c1 */
					{
						++c1;
					}
					for (int i = 0; n2[i] != '.' && n2[i] != ',' && i < n2.size(); ++i) /* counting c2 */
					{
						++c2;
					}
					if (c1 > c2) /* if size of characteristic part of n1 is greater than that of n2, result is true */
					{
						result = "1";
						return result;
					}
					else if (c1 < c2) /* otherwise it is not false */
					{
						result = "0";
						return result;
					}
					else /* if both sizes are equal */
					{
						string chstr1 (n1, 0, c1);
						string chstr2 (n2, 0, c2);
						string temp = compare (chstr1, chstr2); /* compairing characteristics */
						if (temp != "2")
							return temp;
						else /* compairing mantissas */
						{
							if (n1.size() != chstr1.size())
								chstr1 = n1.substr (c1 + 1, n1.size() - c1 - 1);
							else
								chstr1.clear();
							if (n2.size() != chstr2.size())
								chstr2 = n2.substr (c2 + 1, n2.size() - c2 - 1);
							else
								chstr2.clear();
							while (chstr1.size() < chstr2.size())
							{
								chstr1.push_back ('0');
							}
							while (chstr2.size() < chstr1.size())
							{
								chstr2.push_back ('0');
							}
							return compare (chstr1, chstr2);
						}
					}
				}
			}
		}


		string lessThOrEqTo (string &n1, string &n2) /* checks if n1 is less than or equal to n2; returns 1 if it is true and 0 otherwise */
		{

			/* Deleting basic typos in input */

			if ((int)(n1.find_first_of ('.')) != -1 || (int)(n1.find_first_of (',')) != -1)
			{
				while (n1 != "0" && (n1[n1.size() - 1] == '0' || n1[n1.size() - 1] == '.' || n1[n1.size() - 1] == ','))
				{
					if (n1[n1.size() - 1] == '.' || n1[n1.size() - 1] == ',')
					{
						n1.erase (n1.end() - 1);
						break;
					}
					else
						n1.erase (n1.end() - 1);
				}
			}
			if ((int)(n2.find_first_of ('.')) != -1 || (int)(n2.find_first_of (',')) != -1)
			{
				while (n2[n2.size() - 1] == '0' || n2[n2.size() - 1] == '.' || n2[n2.size() - 1] == ',')
				{
					if (n2[n2.size() - 1] == '.' || n2[n2.size() - 1] == ',')
					{
						n2.erase (n2.end() - 1);
						break;
					}
					else
						n2.erase (n2.end() - 1);
				}
			}
			if (n1[0] != '-')
			{
				while (n1 != "0" && n1[0] == '0' && n1[1] != '.' && n1[1] != ',')
				{
					n1.erase (n1.begin());
				}
				while (n2 != "0" && n2[0] == '0' && n2[1] != '.' && n2[1] != ',')
				{
					n2.erase (n2.begin());
				}
			}
			else
			{
				while (n1 != "-0" && n1[1] == '0' && n1[2] != '.' && n1[2] != ',')
				{
					n1.erase (n1.begin() + 1);
				}
				while (n2 != "-0" && n2[1] == '0' && n2[2] != '.' && n2[2] != ',')
				{
					n2.erase (n2.begin() + 1);
				}
			}
			if (n1 == "-0")
				n1 = "0";
			if (n2 == "-0")
				n2 = "0";

			if (n1[0] == '-' && n2[0] == '-') /* if both of them are negative */
			{
				n1.erase (n1.begin()); /* erasing minus sign from n1 */
				n2.erase (n2.begin()); /* erasing minus sign from n2 */
				return grThOrEqTo (n1, n2); /* "less than or equal to" changes to "less than or equal to" */
			}
			else if (n1[0] == '-') /* if only n1 is negative, it is less than or equal to n2 for sure */
			{
				string result = "1";
				return result;
			}
			else if (n2[0] == '-') /* if only n2 is negative, n1 is not less than or equal to it for sure */
			{
				string result = "0";
				return result;
			}
			else /* if both of them are positive */
			{
				string result; /* will be returned in the end */
				if (n1 == n2) /* if they are equal, result is true */
				{
					result = "1";
					return result;
				}
				else
				{
					int c1 = 0, c2 = 0; /* sizes of characteristics of n1 and n2 */
					for (int i = 0; n1[i] != '.' && n1[i] != ',' && i < n1.size(); ++i) /* counting c1 */
					{
						++c1;
					}
					for (int i = 0; n2[i] != '.' && n2[i] != ',' && i < n2.size(); ++i) /* counting c2 */
					{
						++c2;
					}
					if (c1 > c2) /* if size of characteristic part of n1 is greater than that of n2, n1 is greater */
					{
						result = "0";
						return result;
					}
					else if (c1 < c2) /* otherwise it is not greater */
					{
						result = "1";
						return result;
					}
					else /* if both sizes are equal */
					{
						string chstr1 (n1, 0, c1);
						string chstr2 (n2, 0, c2);
						string temp = compare (chstr1, chstr2); /* compairing characteristics */
						if (temp == "0")
						{
							result = "1";
							return result;
						}
						else if (temp == "1")
						{
							result = "0";
							return result;
						}
						else /* compairing mantissas */
						{
							if (n1.size() != chstr1.size())
								chstr1 = n1.substr (c1 + 1, n1.size() - c1 - 1);
							else
								chstr1.clear();
							if (n2.size() != chstr2.size())
								chstr2 = n2.substr (c2 + 1, n2.size() - c2 - 1);
							else
								chstr2.clear();
							while (chstr1.size() < chstr2.size())
							{
								chstr1.push_back ('0');
							}
							while (chstr2.size() < chstr1.size())
							{
								chstr2.push_back ('0');
							}
							result = compare (chstr1, chstr2);
							if (result == "0")
							{
								result = "1";
								return result;
							}
							else
							{
								result = "0";
								return result;
							}
						}
					}
				}
			}
		}


		string isEqualTo (string &n1, string &n2) /* checks if n1 is equal to n2; returns 1 if it is true and 0 otherwise */
		{

			/* Deleting basic typos in input */

			if ((int)(n1.find_first_of ('.')) != -1 || (int)(n1.find_first_of (',')) != -1)
			{
				while (n1 != "0" && (n1[n1.size() - 1] == '0' || n1[n1.size() - 1] == '.' || n1[n1.size() - 1] == ','))
				{
					if (n1[n1.size() - 1] == '.' || n1[n1.size() - 1] == ',')
					{
						n1.erase (n1.end() - 1);
						break;
					}
					else
						n1.erase (n1.end() - 1);
				}
			}
			if ((int)(n2.find_first_of ('.')) != -1 || (int)(n2.find_first_of (',')) != -1)
			{
				while (n2[n2.size() - 1] == '0' || n2[n2.size() - 1] == '.' || n2[n2.size() - 1] == ',')
				{
					if (n2[n2.size() - 1] == '.' || n2[n2.size() - 1] == ',')
					{
						n2.erase (n2.end() - 1);
						break;
					}
					else
						n2.erase (n2.end() - 1);
				}
			}
			if (n1[0] != '-')
			{
				while (n1 != "0" && n1[0] == '0' && n1[1] != '.' && n1[1] != ',')
				{
					n1.erase (n1.begin());
				}
				while (n2 != "0" && n2[0] == '0' && n2[1] != '.' && n2[1] != ',')
				{
					n2.erase (n2.begin());
				}
			}
			else
			{
				while (n1 != "-0" && n1[1] == '0' && n1[2] != '.' && n1[2] != ',')
				{
					n1.erase (n1.begin() + 1);
				}
				while (n2 != "-0" && n2[1] == '0' && n2[2] != '.' && n2[2] != ',')
				{
					n2.erase (n2.begin() + 1);
				}
			}
			if (n1 == "-0")
				n1 = "0";
			if (n2 == "-0")
				n2 = "0";

			if (n1[0] == '-' && n2[0] == '-') /* if both of them are negative */
			{
				n1.erase (n1.begin()); /* erasing minus sign from n1 */
				n2.erase (n2.begin()); /* erasing minus sign from n2 */
			}
			else if (n1[0] == '-' || n2[0] == '-') /* ig one of them is negative, result is false */
			{
				string result = "0";
				return result;
			}
			string result; /* will be returned in the end */
			if (n1 == n2)
				result = "1";
			else
				result = "0";
			return result;
		}


		string isNotEqualTo (string &n1, string &n2) /* checks if n1 is not equal to n2; returns 1 if it is true and 0 otherwise */
		{

			/* Deleting basic typos in input */

			if ((int)(n1.find_first_of ('.')) != -1 || (int)(n1.find_first_of (',')) != -1)
			{
				while (n1 != "0" && (n1[n1.size() - 1] == '0' || n1[n1.size() - 1] == '.' || n1[n1.size() - 1] == ','))
				{
					if (n1[n1.size() - 1] == '.' || n1[n1.size() - 1] == ',')
					{
						n1.erase (n1.end() - 1);
						break;
					}
					else
						n1.erase (n1.end() - 1);
				}
			}
			if ((int)(n2.find_first_of ('.')) != -1 || (int)(n2.find_first_of (',')) != -1)
			{
				while (n2[n2.size() - 1] == '0' || n2[n2.size() - 1] == '.' || n2[n2.size() - 1] == ',')
				{
					if (n2[n2.size() - 1] == '.' || n2[n2.size() - 1] == ',')
					{
						n2.erase (n2.end() - 1);
						break;
					}
					else
						n2.erase (n2.end() - 1);
				}
			}
			if (n1[0] != '-')
			{
				while (n1 != "0" && n1[0] == '0' && n1[1] != '.' && n1[1] != ',')
				{
					n1.erase (n1.begin());
				}
				while (n2 != "0" && n2[0] == '0' && n2[1] != '.' && n2[1] != ',')
				{
					n2.erase (n2.begin());
				}
			}
			else
			{
				while (n1 != "-0" && n1[1] == '0' && n1[2] != '.' && n1[2] != ',')
				{
					n1.erase (n1.begin() + 1);
				}
				while (n2 != "-0" && n2[1] == '0' && n2[2] != '.' && n2[2] != ',')
				{
					n2.erase (n2.begin() + 1);
				}
			}
			if (n1 == "-0")
				n1 = "0";
			if (n2 == "-0")
				n2 = "0";

			if (n1[0] == '-' && n2[0] == '-') /* if both of them are negative */
			{
				n1.erase (n1.begin()); /* erasing minus sign from n1 */
				n2.erase (n2.begin()); /* erasing minus sign from n2 */
			}
			else if (n1[0] == '-' || n2[0] == '-') /* if one of them is negative, result is true */
			{
				string result = "1";
				return result;
			}
			string result; /* will be returned in the end */
			if (n1 == n2)
				result = "0";
			else
				result = "1";
			return result;
		}




		bool hasOperator() /* checks if query has operator in it */
		{
			if ((int)(query.find_first_of ('+')) != -1)
				return true;
			if ((int)(query.find_first_of ('-')) != -1)
				return true;
			if ((int)(query.find_first_of ('*')) != -1)
				return true;
			if ((int)(query.find_first_of ('/')) != -1)
				return true;
			if ((int)(query.find_first_of ('%')) != -1)
				return true;
			if ((int)(query.find_first_of ('^')) != -1)
				return true;
			if ((int)(query.find_first_of ('<')) != -1)
				return true;
			if ((int)(query.find_first_of ('>')) != -1)
				return true;
			if ((int)(query.find (">=")) != -1)
				return true;
			if ((int)(query.find ("<=")) != -1)
				return true;
			if ((int)(query.find ("==")) != -1)
				return true;
			if ((int)(query.find ("!=")) != -1)
				return true;
			return false;
		}


		bool hasInvalid() /* checks if query has invalid characters */
		{
			for (int i = 0; i < this->query.size(); ++i)
			{
				if ((this->query[i] < 48 || this->query[i] > 57) && this->query[i] != 42 && this->query[i] != 43 && this->query[i] != 45 && this->query[i] != 47 && this->query[i] != 44 && this->query[i] != 46 && this->query[i] != 60 && this->query[i] != 62 && this->query[i] != ' ' && this->query[i] != '\t' && this->query[i] != '\n' && this->query[i] != 40 && this->query[i] != 41 && this->query[i] != '^' && this->query[i] != 61 && this->query[i] != 33)
					return true;
			}
			return false;
		}


		bool isOperator (string &op) /* checks if op is an operator */
		{
			if (op == "+" || op == "-" || op == "*" || op == "/" || op == "^" || op == ">" || op == "<" || op == ">=" || op == "<=" || op == "==" || op == "!=")
				return true;
			return false;
		}


		string performOperation (string &n1, string &n2, string &op) /* performs operation on n1 and n2 based on op */
		{
			if (n1[0] == '.' || n1[0] == ',')
				n1.insert (n1.begin(), '0');
			if (n2[0] == '.' || n2[0] == ',')
				n2.insert (n2.begin(), '0');
			if (op == "+")
				return addition (n1, n2);
			else if (op == "-")
				return subtraction (n1, n2);
			else if (op == "*")
				return multiplication (n1, n2);
			else if (op == "/")
				return division (n1, n2);
			else if (op == "^")
				return power (n1, n2);
			else if (op == ">")
				return greaterThan (n1, n2);
			else if (op == "<")
				return lessThan (n1, n2);
			else if (op == ">=")
				return grThOrEqTo (n1, n2);
			else if (op == "<=")
				return lessThOrEqTo (n1, n2);
			else if (op == "==")
				return isEqualTo (n1, n2);
			else if (op == "!=")
				return isNotEqualTo (n1, n2);
			else
			{
				string result = "u";
				return result;
			}
		}


/*****************************************************************************************************/


	public:

		/*		Big-Three		*/

		Query() /* default constructor */
		{
		}


		Query (string &str) /* constructor directly initializing query */
		{
			this->query = str;
			this->process ();
		}


		Query (const char *str)
		{
			this->query = str;
			this->process();
		}


		~Query() /* destructor */
		{
		}


		Query (const Query &q) /* copy constructor */
		{
			this->query = q.query;
			this->result = q.result;
		}


		/*		Accessors		*/

		string getQuery() /* returns actual query */
		{
			return this->query;
		}


		string getResult() /* returns result of the query */
		{
			return this->result;
		}


		/*		Checker			*/

		bool isValid() /* checks if query is valid */
		{
			return this->hasOperator() && !this->hasInvalid();
		}


		/*		Mutators		*/

		void setQuery (string &str) /* setQuery - sets query and processes it */
		{
			this->query = str;
			this->process();
		}


		void process() /* processes input; result is "-" in case of error */
		{
			if (this->isValid ()) /* if the query is valid */
			{
				bool error = false; /* true if error is found, false */
				string str = this->query; /* to keep original query unchanged */
				list <string> stack; /* stack for further operations */
				list <string> temp = shuntingYard (str); /* converts original query into RNP */
				while (!temp.empty())
				{
					if (isValue (temp.front())) /* numerical values are pushed into the stack */
					{
						stack.push_front (temp.front());
						temp.pop_front();
					}
					else if (isOperator (temp.front())) /* operator takes two items from the stack and performs operation on them */
					{
						if (stack.size() < 2) /* error - operator was alone */
						{
							error = true;
							break;
						}
						else
						{
							string n1 = stack.front();
							stack.pop_front();
							string n2 = stack.front();
							stack.pop_front();
							string result = this->performOperation (n2, n1, temp.front());
							if (result == "u") /* in case of undefined result */
							{
								error = true;
								break;
							}
							stack.push_front (result);
							temp.pop_front();
						}
					}
				}
				if (stack.size() == 1 && !error) /* if there is only one item in the stack which is a result and error flag is false */
					this->result = stack.front();
				else
					this->result = "-";
			}
			else
				this->result = "-";
		}


		/*		Overloaded operators		*/

		Query &operator= (const Query &rhs) /* assigns query using assignment operator */
		{
			this->query = rhs.query;
			this->result = rhs.result;
		}


		friend ostream &operator<< (ostream &o, const Query &q); /* outputs result - see implementation below */


};


ostream &operator<< (ostream &o, const Query &q)
{
	o << q.result;
	return o;
}


#endif
