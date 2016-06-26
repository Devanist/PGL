#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <conio.h>
#include <thread>
#include <vector>

/*
	Application takes four arguments
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

double highest = 0;
vector<peak*> peaks;
vector < vector <double> > matrix;
thread * threads = new thread[10];

void seek(int chunk) {

	int col = chunk * 1000;
	for (int i = 0; i < 1000; i++) {
		for (int j = 0; j < 10000; j++) {
			//if (col + i > 0 && col + i < 9999 && j > 0 && j < 9999) {
			//	if (
			//		matrix[col + i][j] > matrix[col + i - 1][j - 1] &&
			//		matrix[col + i][j] > matrix[col + i][j - 1] &&
			//		matrix[col + i][j] > matrix[col + i + 1][j - 1] &&
			//		matrix[col + i][j] > matrix[col + i - 1][j] &&
			//		matrix[col + i][j] > matrix[col + i + 1][j] &&
			//		matrix[col + i][j] > matrix[col + i - 1][j + 1] &&
			//		matrix[col + i][j] > matrix[col + i][j + 1] &&
			//		matrix[col + i][j] > matrix[col + i + 1][j + 1]
			//		) {
			//		//cout << "Found peak at " << col + i << ":" << j << " with value " << matrix[col + i][j] << endl;
			//		if (highest < matrix[col + i][j]) {
			//			highest = matrix[col + i][j];
			//			//cout << "New highest peak! " << matrix[col + i][j];
			//		}
			//	}
			//}
			if (highest < matrix[col + i][j]) {
				highest = matrix[col + i][j];
				//cout << "New highest peak! " << matrix[col + i][j];
			}
		}
	}

}

void checkTime(time_t start, int timeFor) {
	if (time(NULL) - start >= timeFor) {
		cout << "End of time!" << endl;
		cout << "Highest found value is " << highest << endl;
		for (int i = 0; i < 10; i++) {
			threads[i].~thread();
		}
	}
	else {
		this_thread::sleep_for(chrono::milliseconds(1000));
		checkTime(start, timeFor);
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
	string tempString;

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
	
	start = time(NULL);
	for (int i = 0; i < 10; i++) {
		threads[i] = thread(seek, i);
		//cout << "starting thread no. " << i << endl;
	}

	checkTime(start, seekTime);

	for (int i = 0; i < 10; i++) {
		threads[i].join();
	}

	cout << "Seeking peaks ended in " << time(NULL) - start << " seconds." << endl;
	cout << "Highest found peak is " << highest << endl;

	_getch();
	return 0;

}