#include <cstdio>
#include <iostream>
#include <exception>
#include <conio.h>
#include <cstring>
#include <utility>

using namespace std;



int get_key()
{
	int key = _getch();

	if (key == 0 || key == 224)
	{
		int i = 0;
		while (!_kbhit())
		{
			if (i < 1000)
				i++;
			else
				return key;
		}
		key = 1000 + _getch();
	}
	//cout << "[" << key << "]";
	return key;
}

class set
{
private:
	int* list;
	int size;

	void memory_redistribution_up()
	{
		int* c = new int[size];

		for (int i = 0; i < size; i++)
			c[i] = list[i];

		delete[] list;
		list = new int[size + 1];

		for (int i = 0; i < size; i++)
			list[i] = c[i];

		delete[] c;
	}
	void memory_redistribution_down()
	{
		int* c = new int[size - 1];

		for (int i = 0; i < size - 1; i++)
			c[i] = list[i];

		delete[] list;
		list = new int[size - 1];

		for (int i = 0; i < size - 1; i++)
			list[i] = c[i];

		delete[] c;
	}
	void swap(set& rhs)
	{
		std::swap(list, rhs.list);
		std::swap(size, rhs.size);
	}

public:
	set() : size(0), list(NULL)
	{
	}
	set(const int n) : size(n)
	{
		list = new int[size];
	}
	set(const set& s) : size(s.size)
	{
		list = new int[size];
		for (int i = 0; i < size; i++)
			list[i] = s.list[i];
	}
	~set()
	{
		size = 0;
		delete[] list;
		list = NULL;
	}

	int operator [] (const int i) const
	{
		if (i < 0 || i >= size)
			throw "Invalid index";
		return list[i];
	}
	int operator () (const int k) const
	{
		for (int i = 0; i < size; i++)
			if (list[i] == k)
				return i;
		return -1;
	}
	set& operator = (const set& s)
	{
		if (this == &s)
			return *this;

		set rhs = s;
		swap(rhs);
		return *this;
	}

	friend set operator + (const set& s1, const set& s2)
	{
		if (s1.size == 0)
			return s2;
		if (s2.size == 0)
			return s1;

		set result;
		result = s1;
		for (int i = 0; i < s2.size; i++)
			result += s2.list[i];
		return result;
	}
	friend set operator - (const set& s1, const set& s2)
	{
		if (s1.size == 0 || s2.size == 0)
			return s1;

		set result;
		result = s1;
		for (int i = 0; i < s2.size; i++)
			result -= s2.list[i];
		return result;
	}
	friend set operator * (const set& s1, const set& s2)
	{
		return (s1 + s2) - ((s1 - s2) + (s2 - s1));
	}
	friend bool operator == (const set& s1, const set& s2)
	{
		if (s1.size != s2.size)
			return false;

		for (int i = 0; i < s1.size; i++)
			if (s1(s2.list[i]) == -1)
				return false;
		return true;
	}

	set& operator += (const int k)
	{
		if ((*this)(k) != -1)
			return *this;

		memory_redistribution_up();
		list[size] = k;
		size++;
		return *this;
	}
	set& operator += (const set& s)
	{
		for (int i = 0; i < s.size; i++)
			*this += s.list[i];
		return *this;
	}
	set& operator -= (const int k)
	{
		int i = (*this)(k);
		if (i != -1)
		{
			for (int j = i; j < size - 1; j++)
				list[j] = list[j + 1];

			memory_redistribution_down();
			size--;
			return *this;
		}
		return *this;
	}
	set& operator -= (const set& s)
	{
		for (int i = 0; i < s.size; i++)
			*this -= s.list[i];
		return *this;
	}

	friend istream& operator >> (istream& is, set& s)
	{
		char buffer[501] = "\0";
		bool flag = true;
		cout << "Enter numbers: ";

		while (flag)
		{
			int key = get_key();

			switch ((char)key)
			{
			case 13: // enter
				if (strlen(buffer) != 0 && buffer[strlen(buffer) - 1] != '-')
				{
					cout << endl;
					flag = false;
				}
				break;

			case 8: //backspace
				if (strlen(buffer) != 0)
				{
					cout << "\b \b";
					buffer[strlen(buffer) - 1] = '\0';
				}
				break;

			case 32: //whitespace
				if (strlen(buffer) != 0 && buffer[strlen(buffer) - 1] != ' ' && buffer[strlen(buffer) - 1] != '-')
				{
					cout << (char)key;
					strcat_s(buffer, " ");
				}
				break;

			case 45: //minus
				if (buffer[strlen(buffer) - 1] == ' ' || strlen(buffer) == 0)
				{
					cout << (char)key;
					strcat_s(buffer, "-");
				}

			default: // '0' = 48, ..., '9' = 57
				if (strlen(buffer) == 1 && buffer[0] == '0' ||
					buffer[strlen(buffer) - 2] == ' ' && buffer[strlen(buffer) - 1] == '0')
					break;
				else if (key == 48 && buffer[strlen(buffer) - 1] == '-')
					break;
				else if (48 <= key && key <= 57)
				{
					cout << (char)key;
					char str[2] = { (char)key };
					strcat_s(buffer, str);
				}
				break;
			}
		}

		char* context = NULL;
		char* number = strtok_s(buffer, " ", &context);
		while (number != NULL)
		{
			s += atoi(number);
			number = strtok_s(NULL, " ", &context);
		}
		return is;
	}
	friend ostream& operator << (ostream& os, const set& s)
	{
		os << "(";
		for (int i = 0; i < s.size; i++)
		{
			os << s[i];
			if (i != s.size - 1)
				os << "; ";
		}
		os << ")";
		return os;
	}
};



int main()
{
	set s1, s2, s3, s4, s5, s6;

	cout << endl;
	cout << "  "; cin >> s1;
	cout << "  "; cin >> s2;

	cout << endl;
	cout << "  s1 = " << s1 << endl;
	cout << "  s2 = " << s2 << endl;
	cout << endl;

	s3 = s1 + s2;
	cout << "  union of s1 & s2 = " << s3 << endl;

	s4 = s1 - s2;
	s5 = s2 - s1;
	cout << "  difference of s1 & s2 = " << s4 << endl;
	cout << "  difference of s2 & s1 = " << s5 << endl;

	s6 = s1 * s2;
	cout << "  intersection of s1 & s2 = " << s6 << endl;

	if (s1 == s2)
		cout << "  s1 == s2" << endl;
	else
		cout << "  s1 != s2" << endl;

	cout << endl << endl << endl;
	return 0;
}