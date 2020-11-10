#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <cmath>
#include <map>
#include <algorithm>
#include <numeric>
using namespace std;

void intrain(float* train[]);
void inValidation(float* validation[]);
void outTocsv(float* dis[], int validationCase, int trainCase);
void knn(float* distance[], int k, int validationCase, int trainCase);
void Euclidean(float* train[], float* validation[], int trainCase, int validationCase, int WAP, float* distance[]);
void errTocsv_weight(float error[40], int validationCase, int k);
void errTocsv(float error[40], int validationCase, int k);
void weight(float* distance[], int near[][5], int k);
void deviation(int near[][5], float* distance[], int k);
void deviation_weight(int near[][5], float* distance[], int k, float weight[][5]);

int main()
{
	int trainCase = 2709;
	int validationCase = 40;
	int WAP = 520;
	int col = WAP + 2;


	float **train;
	train = new float*[trainCase];

	for (int i = 0; i < trainCase; i++)
		train[i] = new float[col];
/*----------------------------------------------------------*/
	float **validation;
	validation = new float*[validationCase];

	for (int i = 0; i < validationCase; i++)
		validation[i] = new float[col];
/*---------------------------------------------------------*/
	float **distance;
	distance = new float*[validationCase];
	for (int i = 0; i < validationCase; i++)
		distance[i] = new float[trainCase];


	intrain(train);
	inValidation(validation);

	Euclidean(train, validation, trainCase, validationCase, WAP, distance);
	

	

	for (int i = 0; i < trainCase; i++)	
		delete [] train[i];	
	delete [] train;

	for (int i = 0; i < validationCase; i++)
		delete [] validation[i];
	delete [] validation;

	for (int i = 0; i < validationCase; i++)
		delete[] distance[i];
	delete[] distance;

	system("pause");
}

void intrain(float* train[])
{
	fstream infile("trainData.csv", ios::in | ios::out);
	if (!infile)
	{
		cerr << "can't open trainData";
		system("pause");
	}
	string s;
	//int train[10][12];
	int x = 0;
	while (getline(infile, s))
	{
		string buf;
		stringstream ss(s);
		int y = 0;
		while (getline(ss, buf, ','))
		{
			train[x][y] = stof(buf.c_str());
			y++;
		}
		x++;
	}
	infile.close();
}

void inValidation(float* validation[])
{
	fstream infile("validationData.csv", ios::in | ios::out);

	if (!infile)
	{
		cerr << "can't open test.csv";
		system("pause");
	}
	string s;
	//int test[1][12];
	int x = 0;
	while (getline(infile, s))
	{
		string buf;
		stringstream ss(s);
		int y = 0;
		while (getline(ss, buf, ','))
		{
			validation[x][y] = atof(buf.c_str());
			y++;
		}
		x++;
	}
}

void outTocsv(float* dis[], int validationCase, int trainCase)
{
	ofstream outfile("Euclidean.csv", ios::out);
	if (!outfile)
	{
		cerr << "can't open Euclidean.csv";
		system("pause");
	}

	for (int i = 0; i < validationCase; i++)
	{
		for (int j = 0; j < trainCase; j++)
			outfile << dis[i][j] << ',';
		outfile << "\n";
	}
}

void knn(float* distance[], int k, int validationCase, int trainCase)
{
	float **temp;
	temp = new float*[validationCase];
	for (int i = 0; i < validationCase; i++)
		temp[i] = new float[trainCase];

	for (int i = 0; i < validationCase; i++)
	{
		for (int j = 0; j < trainCase; j++)
			temp[i][j] = distance[i][j];
		sort(temp[i], temp[i] + trainCase);
	}

	int **key;
	key = new int*[validationCase];
	for (int i = 0; i < validationCase; i++)
		key[i] = new int[k];


	for (int i = 0; i < validationCase; i++)
	{
		int flag = 0;
		for (int j = 0; j < trainCase; j++)
		{
			if (temp[i][flag] == distance[i][j])
			{
				key[i][flag] = j;
				flag++;
				j = 0;
			}
			if (flag == k)
				break;
		}
	}

	for (int i = 0; i < validationCase; i++)
		delete[] temp[i];
	delete[] temp;

	if (k == 1)
	{
		int near1[40][5];
		for (int i = 0; i < validationCase; i++)
		{
			for (int j = 0; j < k; j++)
				near1[i][j] = key[i][j];
		}
		deviation(near1, distance, k);
	}
	else if (k == 3)
	{
		int near3[40][5];
		for (int i = 0; i < validationCase; i++)
		{
			for (int j = 0; j < k; j++)
				near3[i][j] = key[i][j];
		}
		weight(distance, near3, k);
		deviation(near3, distance, k);
	}
	else if (k == 5)
	{
		int near5[40][5];
		for (int i = 0; i < validationCase; i++)
		{
			for (int j = 0; j < k; j++)
				near5[i][j] = key[i][j];
		}
		weight(distance, near5, k);
		deviation(near5, distance, k);
	}

	for (int i = 0; i < validationCase; i++)
		delete[] key[i];
	delete[] key;
}

void Euclidean(float* train[], float* validation[], int trainCase, int validationCase, int WAP, float* distance[])
{
	int sum = 0;

	for (int i = 0; i < validationCase; i++)
	{
		for (int j = 0; j < trainCase; j++)
		{
			for (int w = 0; w < WAP; w++)
			{
				sum += pow((validation[i][w] - train[j][w]), 2);
			}
			distance[i][j] = (float)sqrt(sum);
			sum = 0;
		}
	}

	knn(distance, 1, validationCase, trainCase);
	knn(distance, 3, validationCase, trainCase);
	knn(distance, 5, validationCase, trainCase);
}

void deviation(int near[][5], float* distance[], int k)
{
	float **train;
	train = new float*[2709];
	for (int i = 0; i < 2709; i++)
		train[i] = new float[522];

	float **validation;
	validation = new float*[40];
	for (int i = 0; i < 40; i++)
		validation[i] = new float[522];

	int trainCase = 2709;
	int validationCase = 40;

	intrain(train);
	inValidation(validation);

	
	if (k == 1)
	{
		float longitude[40];
		float latitude[40];
		float error[40];
		for (int i = 0; i < validationCase; i++)
		{
			longitude[i] = train[near[i][0]][520];
			latitude[i] = train[near[i][0]][521];

			error[i] = sqrt(pow(validation[i][520] - longitude[i], 2) + pow(validation[i][521] - latitude[i], 2));
		}
		errTocsv(error, validationCase, k);
	}
	else
	{
		float longitude[40] = { 0.0 };
		float latitude[40] = { 0.0 };
		float error[40] = { 0.0 };
		for (int i = 0; i < validationCase; i++)
		{
			for (int j = 0; j < k; j++)
			{
				longitude[i] += train[near[i][j]][520];
				latitude[i] += train[near[i][j]][521];
			}
			longitude[i] /= k;
			latitude[i] /= k;

			error[i] = sqrt((pow(validation[i][520] - longitude[i], 2) + pow(validation[i][521] - latitude[i], 2)));
		}

		errTocsv(error, validationCase, k);
	}

	for (int i = 0; i < validationCase; i++)
		delete[] train[i];
	delete[] train;

	for (int i = 0; i < validationCase; i++)
		delete[] validation[i];
	delete[] validation;
}

void errTocsv(float error[40], int validationCase, int k)
{
	if (k == 1)
	{
		ofstream outfile("error1.csv", ios::out);
		if (!outfile)
		{
			cerr << "can't open 1NN.csv";
			system("pause");
		}

		for (int i = 0; i < validationCase; i++)
		{
			outfile << error[i] << "\n";
		}
	}
	else if (k == 3)
	{
		ofstream outfile("error3.csv", ios::out);
		if (!outfile)
		{
			cerr << "can't open 1NN.csv";
			system("pause");
		}

		for (int i = 0; i < validationCase; i++)
		{
			outfile << error[i] << "\n";
		}
	}
	else if (k == 5)
	{
		ofstream outfile("error5.csv", ios::out);
		if (!outfile)
		{
			cerr << "can't open 1NN.csv";
			system("pause");
		}

		for (int i = 0; i < validationCase; i++)
		{
			outfile << error[i] << "\n";
		}
	}
	
}

void weight(float* distance[], int near[][5], int k)
{
	if (k == 3)
	{
		float wei[40][5];

		for (int i = 0; i < 40; i++)
		{
			float temp[40][3];
			float sum = 0;
			for (int j = 0; j < 3; j++)			
				temp[i][j] = 1 / distance[i][near[i][j]];

			for (int a = 0; a < 3; a++)
			{
				wei[i][a] = temp[i][a] / accumulate(temp[i], temp[i] + 3, sum);
			}
		}
		deviation_weight(near, distance, k, wei);
		
	}
	if (k == 5)
	{
		float wei[40][5];

		for (int i = 0; i < 40; i++)
		{
			float temp[40][5];
			float sum = 0;
			for (int j = 0; j < 5; j++)
				temp[i][j] = 1 / distance[i][near[i][j]];

			for (int a = 0; a < 5; a++)
				wei[i][a] = temp[i][a] / accumulate(temp[i], temp[i] + 5, sum);
		}
		deviation_weight(near, distance, k, wei);
	}
}

void deviation_weight(int near[][5], float* distance[], int k, float weight[][5])
{
	float **train;
	train = new float*[2709];
	for (int i = 0; i < 2709; i++)
		train[i] = new float[522];

	float **validation;
	validation = new float*[40];
	for (int i = 0; i < 40; i++)
		validation[i] = new float[522];

	int trainCase = 2709;
	int validationCase = 40;

	intrain(train);
	inValidation(validation);

	float longitude[40] = { 0.0 };
	float latitude[40] = { 0.0 };
	float error[40] = { 0.0 };
	for (int i = 0; i < validationCase; i++)
	{
		for (int j = 0; j < k; j++)
		{
			longitude[i] += train[near[i][j]][520] * weight[i][j];
			latitude[i] += train[near[i][j]][521] * weight[i][j];

		}
		error[i] = sqrt((pow(validation[i][520] - longitude[i], 2) + pow(validation[i][521] - latitude[i], 2)));
	}

	errTocsv_weight(error, validationCase, k);

	for (int i = 0; i < validationCase; i++)
		delete[] train[i];
	delete[] train;

	for (int j = 0; j < validationCase; j++)
		delete[] validation[j];
	delete[] validation;
}

void errTocsv_weight(float error[40], int validationCase, int k)
{
	if (k == 3)
	{
		ofstream outfile("error3_weight.csv", ios::out);
		if (!outfile)
		{
			cerr << "can't open 1NN.csv";
			system("pause");
		}

		for (int i = 0; i < validationCase; i++)
		{
			outfile << error[i] << "\n";
		}
	}
	else if (k == 5)
	{
		ofstream outfile("error5_weight.csv", ios::out);
		if (!outfile)
		{
			cerr << "can't open 1NN.csv";
			system("pause");
		}

		for (int i = 0; i < validationCase; i++)
		{
			outfile << error[i] << "\n";
		}
	}
}