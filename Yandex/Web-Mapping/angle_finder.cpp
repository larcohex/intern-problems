/*

								DOCUMENTATION

	The program takes input when you pass additional parameters to the program in the command line. I have commented some tests in main that you can use. There is also a possibility to input in the process of executing. I have implemented in OOP style - created a class and added methods. Of course, I haven't shown all tricks of OOP like polymorphism, inheritance, exception handling, etc. but you can ask them from me in an interview. I won't also give an explanation to formulas that I've used to compute angle - you can also ask them in interview.
	The class "AngleFinder" constructs class from input and automatically computes angle. It cannot be constructed without parameters to constructor. If input is missing or incorrect, class is constructed showing errors in input. Class with errors cannot be changed, so if you want class that properly computes angle - give proper input.
	Angle is measured properly in the following cases even if the input is not in standard format
	1. spaces, tab or endline characters are ignored
	2. '.' instead of ':' as a separator
	3. missing separator
	There are two clock types - "mechanical" and "quartz". The input must explicitly write it.
	Further explanation is in the code. Hope it'll be useful.

*/	


/**********************************************************************************************************************/


/*
								Methods

	AngleFinder (const char *t, const char *tp, const char *cl) - constructor that dircetly computes angle from the input
	AngleFinder (const AngleFinder &a) - copy constructor
	string getTime() - returns time
	string getAngle() - returns angle
	string getType() - returns angle format
	string getClock() - returns clock type
	void setTime (const char *t) - sets time if it is in the correct format and all other inputs are correct, otherwise prints error message
	void setType (const char *tp) - sets angle format if it is in the correct format and all other inputs are correct, otherwise prints error message
	void setClock (const char *cl) // sets clock type if it is in the correct format and all other inputs are correct, otherwise prints error message
	Overloaded equality and inequality operators check for time equality.
	Assignment operator is overloaded.
	Overloaded output stream operator prints an angle.

*/


#include <sstream>
#include <iostream>
#include <string>
#include <cmath>


using namespace std;


class AngleFinder
{


	private:


		// 						Variables

		string time; // time - time from which angle is computed; "incorrect" if the format was incorrect during construction
		string angle; // angle - computed angle; "undefined" if the input was invalid
		string clock; // clock - clock type: either mechanical or quartz // "unknown" if the format was incorrect during construction
		int type; // type - angle type: 0 - deg, 1 - rad, 2 - dms // -1 if the format was incorrect during construction


		//						Helper functions

		bool correct (string &t) // checks if the time format in t is correct
		{
			string time = t; // copied to erase spaces, tab and endline characters

			// Erasing spaces, tab and endline characters
			for (int i = 0; i < time.size(); ++i)
			{
				if (time[i] == ' ' || time[i] == '\t' || time[i] == '\n')
					time.erase (i, 1);
			}

			// Checking for validity
			if (time.size() < 4 || time.size() > 7)
				return false;
			if (time.size() == 4)
			{
				if (time[0] < 48 || time[0] > 50)
					return false;
				if (time[1] < 48 || time[1] > 57)
					return false;
				if (time[0] == '2' && time[1] > 51)
					return false;
				if (time[2] < 48 || time[2] > 53)
					return false;
				if (time[3] < 48 || time[3] > 57)
					return false;
			}
			else if (time.size() == 5)
			{
				if (time[2] != ':' && time[2] != '.')
					return false;
				if (time[0] < 48 || time[0] > 50)
					return false;
				if (time[1] < 48 || time[1] > 57)
					return false;
				if (time[0] == '2' && time[1] > 51)
					return false;
				if (time[3] < 48 || time[3] > 53)
					return false;
				if (time[4] < 48 || time[4] > 57)
					return false;
			}
			else if (time.size() == 6)
			{
				if (time[4] != 'A' && time[4] != 'a' && time[4] != 'P' && time[4] != 'p')
					return false;
				if (time[5] != 'M' && time[5] != 'm')
					return false;
				if (time[0] < 48 || time[0] > 49)
					return false;
				if (time[1] < 48 || time[1] > 57)
					return false;
				if (time[0] == '1' && time[1] > 49)
					return false;
				if (time[2] < 48 || time[2] > 53)
					return false;
				if (time[3] < 48 || time[3] > 57)
					return false;
			}
			else if (time.size() == 7)
			{
				if (time[5] != 'A' && time[5] != 'a' && time[5] != 'P' && time[5] != 'p')
					return false;
				if (time[6] != 'M' && time[6] != 'm')
					return false;
				if (time[2] != ':' && time[2] != '.')
					return false;
				if (time[0] < 48 || time[0] > 49)
					return false;
				if (time[1] < 48 || time[1] > 57)
					return false;
				if (time[0] == '1' && time[1] > 49)
					return false;
				if (time[3] < 48 || time[3] > 53)
					return false;
				if (time[4] < 48 || time[4] > 57)
					return false;
			}
			return true;
		}


		void degMech() // computes angle in degrees if the clock is mechanical
		{
			int h = ((this->time[0] - 48) * 10 + (this->time[1] - 48)) % 12; // h - hours
			int m; // m - minutes
			if (this->time[2] == ':' || this->time[2] == '.')
				m = (this->time[3] - 48) * 10 + (this->time[4] - 48);
			else
				m = (this->time[2] - 48) * 10 + (this->time[3] - 48);

			// Computing angle
			double a = fabs ((60.0 * h - 11.0 * m) / 2.0); // a - angle
			int temp = (int)(a);
			a -= temp;
			temp %= 180;
			a += temp;

			// Converting to string
			stringstream ss;
			ss << a;
			this->angle = ss.str();
		}


		void radMech() // computes angle in radians if the clock is mechanical
		{
			int h = ((this->time[0] - 48) * 10 + (this->time[1] - 48)) % 12; // h - hours
			int m; // m - minutes
			if (this->time[2] == ':' || this->time[2] == '.')
				m = (this->time[3] - 48) * 10 + (this->time[4] - 48);
			else
				m = (this->time[2] - 48) * 10 + (this->time[3] - 48);

			// Computing angle
			double a = fabs ((60.0 * h - 11.0 * m) / 2.0); // a - angle
			int temp = (int)(a);
			a -= temp;
			temp %= 180;
			a += temp;
			a = 3.14159265359 * a / 180;

			// Converting to string
			stringstream ss;
			ss << a;
			this->angle = ss.str();
		}


		void dmsMech() // computes angle in dms format if the clock is mechanical
		{
			int h = ((this->time[0] - 48) * 10 + (this->time[1] - 48)) % 12; // h - hours
			int m; // m - minutes
			if (this->time[2] == ':' || this->time[2] == '.')
				m = (this->time[3] - 48) * 10 + (this->time[4] - 48);
			else
				m = (this->time[2] - 48) * 10 + (this->time[3] - 48);

			// Computing angle
			double a = fabs ((60.0 * h - 11.0 * m) / 2.0); // a - angle
			int deg = (int)(a); // deg - degrees
			a -= deg;
			deg %= 180;
			a *= 60;
			int min = (int)(a); // min - minutes (of degree)
			a = (a - min) * 60;
			int sec = (int)(a); // sec - seconds

			// Converting to string
			stringstream ss;
			ss << deg;
			this->angle = ss.str();
			this->angle.push_back ('.');
			ss.str(string());
			ss << min;
			this->angle.insert (this->angle.size(), ss.str());
			this->angle.push_back ('\'');
			ss.str(string());
			ss << sec;
			this->angle.insert (this->angle.size(), ss.str());
			this->angle.push_back ('"');
		}


		void degQuartz() // computes angle in degrees if the clock is quartz
		{
			int h = ((this->time[0] - 48) * 10 + (this->time[1] - 48)) % 12; // h - hours
			int m; // m - minutes
			if (this->time[2] == ':' || this->time[2] == '.')
				m = (this->time[3] - 48) * 10 + (this->time[4] - 48);
			else
				m = (this->time[2] - 48) * 10 + (this->time[3] - 48);

			// Computing angle
			double a = fabs (30.0 * h - 6.0 * m); // a - angle
			int temp = (int)(a);
			a -= temp;
			temp %= 180;
			a += temp;

			// Converting to string
			stringstream ss;
			ss << a;
			this->angle = ss.str();
		}


		void radQuartz() // computes angle in radians if the clock is quartz
		{
			int h = ((this->time[0] - 48) * 10 + (this->time[1] - 48)) % 12; // h - hours
			int m; // m - minutes
			if (this->time[2] == ':' || this->time[2] == '.')
				m = (this->time[3] - 48) * 10 + (this->time[4] - 48);
			else
				m = (this->time[2] - 48) * 10 + (this->time[3] - 48);

			// Computing angle
			double a = fabs (30.0 * h - 6.0 * m); // a - angle
			int temp = (int)(a);
			a -= temp;
			temp %= 180;
			a += temp;
			a = 3.14159265359 * a / 180;

			// Converting to string
			stringstream ss;
			ss << a;
			this->angle = ss.str();
		}


		void dmsQuartz() // computes angle in dms format if the clock is quartz
		{
			int h = ((this->time[0] - 48) * 10 + (this->time[1] - 48)) % 12; // h - hours
			int m; // m - minutes
			if (this->time[2] == ':' || this->time[2] == '.')
				m = (this->time[3] - 48) * 10 + (this->time[4] - 48);
			else
				m = (this->time[2] - 48) * 10 + (this->time[3] - 48);

			// Computing angle
			double a = fabs ((60.0 * h - 6.0 * m) / 2.0); // a - angle
			int deg = (int)(a); // deg - degrees
			a -= deg;
			deg %= 180;
			a *= 60;
			int min = (int)(a); // min - minutes (of degree)
			a = (a - min) * 60;
			int sec = (int)(a); // sec - seconds

			// Converting to string
			stringstream ss;
			ss << deg;
			this->angle = ss.str();
			this->angle.push_back ('.');
			ss.str(string());
			ss << min;
			this->angle.insert (this->angle.size(), ss.str());
			this->angle.push_back ('\'');
			ss.str(string());
			ss << sec;
			this->angle.insert (this->angle.size(), ss.str());
			this->angle.push_back ('"');
		}


		bool areEqual (string &t1, string &t2) // checks if two times are equal
		{
			if (correct (t1) && correct (t2))
			{
				for (int i = 0; i < t1.size(); ++i)
				{
					if (t1[i] == ' ' || t1[i] == '\t' || t1[i] == '\n')
						time.erase (i, 1);
				}
				for (int i = 0; i < t2.size(); ++i)
				{
					if (t2[i] == ' ' || t2[i] == '\t' || t2[i] == '\n')
						time.erase (i, 1);
				}
				if (t1 == t2)
					return true;
				else if (t1.size() == 4 && t2.size() == 5)
				{
					if (t1[0] == t2[0] && t1[1] == t2[1] && t1[2] == t2[3] && t1[3] == t2[4])
						return true;
					else
						return false;
				}
				else if (t1.size() == 5 && t2.size() == 4)
				{
					if (t1[0] == t2[0] && t1[1] == t2[1] && t1[3] == t2[2] && t1[4] == t2[3])
						return true;
					else
						return false;
				}
				else if (t1.size() == 4 && t2.size() == 6)
				{
					if (t1[2] == t2[2] && t1[3] == t2[3])
					{
						int h1 = (t1[0] - 48) * 10 + (t1[1] - 48);
						int h2 = (t2[0] - 48) * 10 + (t2[1] - 48);
						if (t2[4] == 'P')
							h2 += 12;
						if (h1 == h2)
							return true;
						else
							return false;
					}
					else
						return false;
				}
				else if (t1.size() == 6 && t2.size() == 4)
				{
					if (t1[2] == t2[2] && t1[3] == t2[3])
					{
						int h1 = (t1[0] - 48) * 10 + (t1[1] - 48);
						if (t1[4] == 'P')
							h1 += 12;
						int h2 = (t2[0] - 48) * 10 + (t2[1] - 48);
						if (h1 == h2)
							return true;
						else
							return false;
					}
					else
						return false;
				}
				else if (t1.size() == 4 && t2.size() == 7)
				{
					if (t1[2] == t2[3] && t1[3] == t2[4])
					{
						int h1 = (t1[0] - 48) * 10 + (t1[1] - 48);
						int h2 = (t2[0] - 48) * 10 + (t2[1] - 48);
						if (t2[5] == 'P')
							h2 += 12;
						if (h1 == h2)
							return true;
						else
							return false;
					}
					else
						return false;
				}
				else if (t1.size() == 7 && t2.size() == 4)
				{
					if (t1[3] == t2[2] && t1[4] == t2[3])
					{
						int h1 = (t1[0] - 48) * 10 + (t1[1] - 48);
						if (t1[5] == 'P')
							h1 += 12;
						int h2 = (t2[0] - 48) * 10 + (t2[1] - 48);
						if (h1 == h2)
							return true;
						else
							return false;
					}
					else
						return false;
				}
				else if (t1.size() == 5 && t2.size() == 6)
				{
					if (t1[3] == t2[2] && t1[4] == t2[3])
					{
						int h1 = (t1[0] - 48) * 10 + (t1[1] - 48);
						int h2 = (t2[0] - 48) * 10 + (t2[1] - 48);
						if (t2[4] == 'P')
							h2 += 12;
						if (h1 == h2)
							return true;
						else
							return false;
					}
					else
						return false;
				}
				else if (t1.size() == 6 && t2.size() == 5)
				{
					if (t1[2] == t2[3] && t1[3] == t2[4])
					{
						int h1 = (t1[0] - 48) * 10 + (t1[1] - 48);
						if (t1[4] == 'P')
							h1 += 12;
						int h2 = (t2[0] - 48) * 10 + (t2[1] - 48);
						if (h1 == h2)
							return true;
						else
							return false;
					}
					else
						return false;
				}
				else if (t1.size() == 5 && t2.size() == 7)
				{
					if (t1[3] == t2[3] && t1[4] == t2[4])
					{
						int h1 = (t1[0] - 48) * 10 + (t1[1] - 48);
						int h2 = (t2[0] - 48) * 10 + (t2[1] - 48);
						if (t2[5] == 'P')
							h2 += 12;
						if (h1 == h2)
							return true;
						else
							return false;
					}
					else
						return false;
				}
				else if (t1.size() == 7 && t2.size() == 5)
				{
					if (t1[3] == t2[3] && t1[4] == t2[4])
					{
						int h1 = (t1[0] - 48) * 10 + (t1[1] - 48);
						if (t1[5] == 'P')
							h1 += 12;
						int h2 = (t2[0] - 48) * 10 + (t2[1] - 48);
						if (h1 == h2)
							return true;
						else
							return false;
					}
					else
						return false;
				}
				else if (t1.size() == 6 && t2.size() == 7)
				{
					if (t1[0] == t2[0] && t1[1] == t2[1] && t1[2] == t2[3] && t1[3] == t2[4] && t1[4] == t2[5])
						return true;
					else
						return false;
				}
				else if (t1.size() == 7 && t2.size() == 6)
				{
					if (t1[0] == t2[0] && t1[1] == t2[1] && t1[3] == t2[2] && t1[4] == t2[3] && t1[5] == t2[4])
						return true;
					else
						return false;
				}
			}
			else
				return false;
		}


/**********************************************************************************************************************/


	public:

		//					Big-Three

		AngleFinder (const char *t, const char *tp, const char *cl) // constructor
		{
			if (t && tp && cl)
			{
				string time (t), type (tp), clock (cl);
				if (correct (time))
				{
					this->time = time;
					if (type == "deg")
					{
						if (clock == "mechanical")
						{
							this->clock = "mechanical";
							this->degMech();
							this->type = 0;
						}
						else if (clock == "quartz")
						{
							this->clock = "quartz";
							this->degQuartz();
							this->type = 0;
						}
						else
						{
							this->clock = "unknown";
							this->angle = "undefined";
							this->type = -1;
						}
					}
					else if (type == "rad")
					{
						if (clock == "mechanical")
						{
							this->clock = "mechanical";
							this->radMech();
							this->type = 1;
						}
						else if (clock == "quartz")
						{
							this->clock = "quartz";
							this->radQuartz();
							this->type = 1;
						}
						else
						{
							this->clock = "unknown";
							this->angle = "undefined";
							this->type = -1;
						}
					}
					else if (type == "dms")
					{
						if (clock == "mechanical")
						{
							this->clock = "mechanical";
							this->dmsMech();
							this->type = 2;
						}
						else if (clock == "quartz")
						{
							this->clock = "quartz";
							this->dmsQuartz();
							this->type = 2;
						}
						else
						{
							this->clock = "unknown";
							this->angle = "undefined";
							this->type = -1;
						}
					}
					else
					{
						this->type = -1;
						this->angle = "undefined";
						this->clock = "unknown";
					}
				}
				else
				{
					this->time = "incorrect";
					this->angle = "undefined";
					this->type = -1;
					this->clock = "unknown";
				}
			}
			else
			{
					this->time = "incorrect";
					this->angle = "undefined";
					this->type = -1;
					this->clock = "unknown";
			}
		}


		~AngleFinder() // destructor (since I don't allocate any memory, it will handle everything for itself)
		{
		}


		AngleFinder (const AngleFinder &a) // copy constructot (note that it won't compute anything in this case since everything was computed in the class from which it copy-constructs
		{
			this->time = a.time;
			this->angle = a.angle;
			this->type = a.type;
			this->clock = a.clock;
		}


		//					Accessors

		string getTime() // returns time
		{
			return this->time;
		}


		string getAngle() // returns angle
		{
			return this->angle;
		}


		string getType() // returns angle format
		{
			string result;
			if (!this->type)
			{
				result = "deg";
				return result;
			}
			else if (this->type == 1)
			{
				result = "rad";
				return result;
			}
			else if (this->type == 2)
			{
				result = "dms";
				return result;
			}
			else
			{
				result = "improper";
				return result;
			}
		}


		string getClock() // returns clock type
		{
			return this->clock;
		}


		//					Mutators

		void setTime (const char *t) // sets time if it is in the correct format and all other inputs are correct, otherwise prints error message
		{
			string time (t);
			if (this->correct (time))
			{
				this->time = time;
				if (!this->type)
				{
					if (this->clock == "mechanical")
						this->degMech();
					else if (this->clock == "quartz")
						this->degQuartz();
					else
						cout << "unknown clock type format" << endl;
				}
				else if (this->type == 1)
					if (this->clock == "mechanical")
						this->radMech();
					else if (this->clock == "quartz")
						this->radQuartz();
					else
						cout << "unknown clock type format" << endl;
				else if (this->type == 2)
					if (this->clock == "mechanical")
						this->dmsMech();
					else if (this->clock == "quartz")
						this->dmsQuartz();
					else
						cout << "unknown clock type" << endl;
				else
					cout << "undefined angle format" << endl;
			}
			else
				cout << "incorrect time" << endl;
		}


		void setType (const char *tp) // sets angle format if it is in the correct format and all other inputs are correct, otherwise prints error message
		{
			string type (tp);
			if (type == "deg")
			{
				if (this->correct (this->time))
				{
					this->type = 0;
					if (this->clock == "mechanical")
						this->degMech();
					else if (this->clock == "quartz")
						this->degQuartz();
					else
						cout << "unknown clock type" << endl;
				}
				else
					cout << "incorrect or not set time" << endl;
			}
			else if (type == "rad")
			{
				if (this->correct (this->time))
				{
					this->type = 1;
					if (this->clock == "mechanical")
						this->radMech();
					else if (this->clock == "quartz")
						this->radQuartz();
					else
						cout << "unknown clock type" << endl;
				}
				else
					cout << "incorrect or not set time" << endl;
			}
			else if (type == "dms")
			{
				if (this->correct (this->time))
				{
					this->type = 2;
					if (this->clock == "mechanical")
						this->dmsMech();
					else if (this->clock == "quartz")
						this->dmsQuartz();
					else
						cout << "unknown clock type" << endl;
				}
				else
					cout << "incorrect or not set time" << endl;
			}
			else
			{
				cout << "incorrect angle format" << endl;
			}
		}


		void setClock (const char *cl) // sets clock type if it is in the correct format and all other inputs are correct, otherwise prints error message
		{
			string clk (cl);
			if (clk == "mechanical")
			{
				if (this->correct (this->time))
				{
					this->clock = clk;
					if (!this->type)
					{
						this->degMech();
					}
					else if (this->type == 1)
					{
						this->radMech();
					}
					else if (this->type == 2)
					{
						this->dmsMech();
					}
					else
						cout << "undefined angle format" << endl;
				}
				else
					cout << "incorrect or not set time" << endl;
			}
			else if (clk == "quartz")
			{
				if (this->correct (this->time))
				{
					this->clock = clk;
					if (!this->type)
					{
						this->degQuartz();
					}
					else if (this->type == 1)
					{
						this->radQuartz();
					}
					else if (this->type == 2)
					{
						this->dmsQuartz();
					}
					else
						cout << "undefined angle format" << endl;
				}
				else
					cout << "incorrect or not set time" << endl;
			}
			else
				cout << "incorrect clock type" << endl;
		}


		//				Overloaded operators

		bool operator== (const AngleFinder &rhs) // overloaded equality operator (note that the equality of time and proper angle is are the only parameters)
		{
			string temp = rhs.time;
			return this->areEqual (this->time, temp) && this->type != -1 && rhs.type != -1;
		}


		bool operator!= (const AngleFinder &rhs) // overloaded inequality operator
		{
			string temp = rhs.time;
			return !areEqual (this->time, temp) || this->type == -1 || rhs.type == -1;
		}


		AngleFinder &operator= (const AngleFinder &rhs) // overloaded assignment operator
		{
			this->time = rhs.time;
			this->angle = rhs.angle;
			this->type = rhs.type;
		}


		friend ostream &operator<< (ostream &o, const AngleFinder &a); // overloaded output stream operator - prints angle (see implementation below)


};


ostream &operator<< (ostream &o, const AngleFinder &a)
{
	o << a.angle;
	return o;
}



/**********************************************************************************************************************/


int main (int argc, char **argv)
{
	AngleFinder test (argv[1], argv[2], argv[3]), test2 ("04:13 PM", "rad", "quartz"), test3 (test);
	cout << test << endl;
/*
	AngleFinder test4 ("23:12 AM", "rad", "mechanical"), test5 ("11:11", "de", "mechanical"), test6 ("12:14", "rad", "cyanide");
	cout << test.getTime() << " " << test.getAngle() << " " << test.getType() << " " << test.getClock() << endl;
	cout << test2.getTime() << " " << test2.getAngle() << " " << test2.getType() << " " << test2.getClock() << endl;
	cout << test3.getTime() << " " << test3.getAngle() << " " << test3.getType() << " " << test3.getClock() << endl;
	cout << test4.getTime() << " " << test4.getAngle() << " " << test4.getType() << " " << test4.getClock() << endl;
	cout << test5.getTime() << " " << test5.getAngle() << " " << test5.getType() << " " << test5.getClock() << endl;
	cout << test6.getTime() << " " << test6.getAngle() << " " << test6.getType() << " " << test6.getClock() << endl;
	test.setTime ("02:20");
	cout << test.getTime() << " " << test.getAngle() << " " << test.getType() << " " << test.getClock() << endl;
	test.setTime ("0213");
	cout << test.getTime() << " " << test.getAngle() << " " << test.getType() << " " << test.getClock() << endl;
	test.setTime ("02.13");
	cout << test.getTime() << " " << test.getAngle() << " " << test.getType() << " " << test.getClock() << endl;
	test.setTime ("021");
	cout << test.getTime() << " " << test.getAngle() << " " << test.getType() << " " << test.getClock() << endl;
	test.setTime ("24:00");
	cout << test.getTime() << " " << test.getAngle() << " " << test.getType() << " " << test.getClock() << endl;
	test.setTime ("clock");
	cout << test.getTime() << " " << test.getAngle() << " " << test.getType() << " " << test.getClock() << endl;
	test.setType ("deg");
	cout << test.getTime() << " " << test.getAngle() << " " << test.getType() << " " << test.getClock() << endl;
	test.setType ("rad");
	cout << test.getTime() << " " << test.getAngle() << " " << test.getType() << " " << test.getClock() << endl;
	test.setType ("dms");
	cout << test.getTime() << " " << test.getAngle() << " " << test.getType() << " " << test.getClock() << endl;
	test.setType ("clock");
	cout << test.getTime() << " " << test.getAngle() << " " << test.getType() << " " << test.getClock() << endl;
	test.setClock ("quartz");
	cout << test.getTime() << " " << test.getAngle() << " " << test.getType() << " " << test.getClock() << endl;
	test.setType ("deg");
	cout << test.getTime() << " " << test.getAngle() << " " << test.getType() << " " << test.getClock() << endl;
	test.setClock ("mechanical");
	cout << test.getTime() << " " << test.getAngle() << " " << test.getType() << " " << test.getClock() << endl;
	test.setClock ("clock");
	cout << test.getTime() << " " << test.getAngle() << " " << test.getType() << " " << test.getClock() << endl;
	if (test == test2)
		cout << "test and test2 are equal" << endl;
	if (test != test2)
		cout << "test and test2 are not equal" << endl;
	test2 = test;
	if (test == test2)
		cout << "test and test2 are equal" << endl;
	if (test != test2)
		cout << "test and test2 are not equal" << endl;
	if (test == test3)
		cout << "test and test3 are equal" << endl;
	if (test != test3)
		cout << "test and test3 are not equal" << endl;
	string t, tp, cl;
	cin >> t >> tp >> cl;
	AngleFinder final (t.c_str(), tp.c_str(), cl.c_str());
	cout << final << endl;
*/
	return 0;
}
