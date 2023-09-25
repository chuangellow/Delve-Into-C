#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

int main(int argc, char* argv[]) {
	
	char *filename = argv[1];

	vector<double> numbers;
	double mean = 0;
	fstream fin(filename, ios::in);
	double d;
	while (fin >> d) {
		// In C++, the array size is increased automatically.
		numbers.push_back(d);
		mean = (numbers.size() == 1)? d: mean + (d - mean) / numbers.size();
	}
	cout << "Mean: " << mean << endl;

	return 0;
}
