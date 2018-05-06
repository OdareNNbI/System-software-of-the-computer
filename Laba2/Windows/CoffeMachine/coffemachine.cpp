#ifdef __linux__
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

#elif _WIN32 
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#endif

#include<iostream>
#include<fstream>
#include<vector>
#include<cstring>
#include<stdio.h>
#include<cstdlib>

using namespace std;
class Coffee
{
private:
	char name[256];
	int count;
	int price;
public:
	Coffee(fstream& file)
	{
		file >> name;
		file >> price;
		file >> count;

	}

	void WriteToFile(fstream& file, int i)
	{
		file << name << " ";
		file << price << " ";
		if (i != 1)
			file << count << " ";
		else file << count;
	}

	int CheckCoffee(char* name, int price)
	{
		if (strcmp(name, this->name) == 0)
		{
			if (price >= this->price)
			{
				count--;
				if (count <= 0)
					return 0;

				return 1;
			}
			return -1;
		}

		return -2;
	}
};


int main(int argc, char* argv[])
{
	fstream file;
	char fileName[20] = "Availability.txt";
	file.open(fileName, ios::out | ios::in);
	if (!file)
	{
		return 3; //File not open;
	}
	char str[256];
	char coffee_name[256];
	vector<Coffee> mas = vector<Coffee>();

	while (!file.eof())
	{
		mas.push_back(Coffee(file));
	}


	int check;
	for (int i = 0; i < mas.size(); i++)
	{

		check = mas[i].CheckCoffee(argv[2], atoi(argv[1]));
		if (check == 0)
		{
			mas.erase(mas.begin() + i);
			break;
		}
		if (check != -2)
			break;
	}
	file.close();

	if (check >= 0)
	{
		fstream newFile;
		newFile.open(fileName, ios::in | ios::out | ios::trunc);
		for (int i = 0; i < mas.size(); i++)
		{
			mas[i].WriteToFile(newFile, mas.size() - i);
		}
		newFile.close();
	}


	if (check >= 0) return 0;
	else return check + 3;
}

