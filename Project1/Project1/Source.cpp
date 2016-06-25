#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <conio.h>
#include <thread>
#include <vector>

/*
	Application takes two arguments
	- name of file with data
	- time in which it should find most peaks
	- 'width' of 2D array
	- 'height' of 2D array
*/

using namespace std;

#define NUM_THREADS 10

struct peak {
	double value;
	int x;
	int y;
};

vector<peak*> peaks;

void seek(vector<vector<double>> matrix, int chunk) {

	peak * temp;

	int col = chunk * 1000;
	vector<double> curCol;
	double curVal;
	for (int i = 0; i < 1000; i++) {
		for (int j = 0; j < 1000; j++) {
			//curVal = ;
			if (col + i > 0 && col + i < 9999 && j > 0 && j < 999) {
				if (
					matrix[col + i][j] > matrix[col + i - 1][j - 1] &&
					matrix[col + i][j] > matrix[col + i][j - 1] &&
					matrix[col + i][j] > matrix[col + i + 1][j - 1] &&
					matrix[col + i][j] > matrix[col + i - 1][j] &&
					matrix[col + i][j] > matrix[col + i + 1][j] &&
					matrix[col + i][j] > matrix[col + i - 1][j + 1] &&
					matrix[col + i][j] > matrix[col + i][j + 1] &&
					matrix[col + i][j] > matrix[col + i + 1][j + 1]
					) {
					/*temp = new peak;
					temp->value = matrix[col + i][j];
					temp->x = col + i;
					temp->y = j;
					peaks.push_back(temp);*/
					cout << "Found peak at " << col + i << ":" << j << " with value " << matrix[col + i][j] << endl;
				}
			}
		}
	}

}

int main(int argc, char* argv[]) {

	string fileName = argv[1];
	int seekTime = atoi(argv[2]);
	int width = atoi(argv[3]);
	int height = atoi(argv[4]);
	int w = 0;
	int h = 0;
	time_t start;

	char c;
	float temp;
	string tempString;

	vector < vector <double> > matrix;
	matrix.resize(height, vector<double>(width, 0.0));

	fstream fs;
	fs.open(fileName, fstream::in | fstream::out | fstream::app);

	cout << "Opening file..." << endl;
	start = time(NULL);
	int i = 0;
	while (fs.get(c)) {
		if (c == '\t' || c == '\n') {
			matrix[h][w] = atof(tempString.c_str());
			tempString = "";
			if (w == width - 1) {
				w = 0;
				h++;
			}
			else {
				w++;
			}
			if (h == height - 1) {
				i++;
				h = 0;
			}
		}
		else {
			tempString += c;
		}

	}

	matrix[h][w] = atof(tempString.c_str());

	cout << "File was opened in " << time(NULL) - start << " seconds" << endl;
	cout << "Starting seeking peaks" << endl;

	thread * threads = new thread[10];
	
	for (int i = 0; i < 10; i++) {
		threads[i] = thread([=]() {seek(matrix, i); });
	}

	_getch();
	return 0;

}