#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "Binary_symmetric_channel.h"
#include <chrono>
#include <thread>
#include <map>


using namespace std;

string Random_message(const int& length);

void FillFile(const string& file, Codec& codec);

void CheckPolynomial(const int& polynomial);

void CheckWordLength(const int& word_length);

void CheckBitMsg(const string& bit_msg);


int main()
{
	cout << "This program allows you to encode and decode convolutional codes using the Viterbi algorithm, .\n";
	cout << "as well as test a binary symmetric communication channel model with a given error probability (The testing information will be in the file TestBSC.txt)\n";
	cout << "Enter the generator polynomials in integer format. They will be converted to the binary format.\n";
	cout << "\tExamples:\n";
	cout << "\t7 ---> 111\n";
	cout << "\t5 ---> 101\n";
	cout << "\t13 ---> 1101\n";
	cout << "The number of generator polynomials is equal to the number of bits into which the input bit will be encoded.\n\n";;

		while (true)
		{
			cout << "Enter:\n";
			cout << "\t1) encode - if you want to encode a message\n";
			cout << "\t2) decode - if you want to decode a message\n";
			cout << "\t3) channel - if you want to write binary symmetric channel testing to a file TestBSC.txt\n";
			cout << "\t4) exit - if you want to get out\n\n";

			string key;

			cin >> key;

			if (key != "encode" && key != "decode" && key != "channel" && key != "exit")
			{
				cout << "Incorrect input\n";
				cout << "Please, try again\n";

				continue;
			}

			else if (key == "exit")
			{
				break;
			}

			cout << "Enter the number of generator polynomials: ";
			
			string word_length;

			while (true)
			{
				try
				{
					cin >> word_length;

					CheckWordLength(stoi(word_length));

					break;
				}
				catch (const exception& ex)
				{
					cout << ex.what() << endl;
					cout << "Please, try again" << endl;
				}
			}


			cout << "Enter the generator polynomials: ";
			vector<int> polynoms;

			while (true)
			{
				try
				{
					for (int i = 0; i < stoi(word_length); i++)
					{
						string polynom;

						cin >> polynom;

						CheckPolynomial(stoi(polynom));

						polynoms.push_back(stoi(polynom));

					}

					break;
				}
				catch (const exception& ex)
				{
					cout << ex.what() << endl;
					cout << "Please, try again" << endl;
				}
			}

			Codec codec(polynoms);

			if (key == "encode")
			{
				try
				{
					string bit_message;

					cout << "Enter a bit message\n";

					cin >> bit_message;

					CheckBitMsg(bit_message);

					cout << codec.Encode(bit_message) << endl;

				}
				catch (const exception& ex)
				{
					cout << ex.what() << endl;
					cout << "Please, try again" << endl;
				}
			}
			else if (key == "decode")
			{
				try
				{
					string bit_message;

					cout << "Enter a bit message\n";

					cin >> bit_message;

					CheckBitMsg(bit_message);

					cout << codec.Decode(bit_message) << endl;

				}
				catch (const exception& ex)
				{
					cout << ex.what() << endl;
					cout << "Please, try again" << endl;
				}
			}
			else if (key == "channel")
			{
				try
				{
					cout << "Please wait, the process of writing to the file is underway" << endl;

					string file_name = "TestBSC.txt";

					auto start = chrono::high_resolution_clock::now();

					FillFile(file_name, codec);

					auto end = chrono::high_resolution_clock::now();

					chrono::duration<float> duration = end - start;

					cout << "Time: " << duration.count() << "seconds" << endl;

					cout << "The data is written to a file" << endl;
				}
				catch (const exception& ex)
				{
					cout << ex.what() << endl;
					cout << "Please, try again" << endl;
				}
				
			}
			
		}

	return 0;
}


string Random_message(const int& length)
{
	string msg;

	random_device dvc;
	uniform_int_distribution<int> dist(0, 1);

	for (int i = 0; i < length; i++)
	{
		dist(dvc) ? msg.push_back('1') : msg.push_back('0');
	}

	return msg;
}

void FillFile(const string& file, Codec& codec)
{
	vector<double> mp;
	mp.resize((END_PROBABILITY / STEP_OF_PROBABILITY) + 1);

	const int SIZE = 4;

	thread th1([&]()
		{
			for (int i = 0; i < (END_PROBABILITY / STEP_OF_PROBABILITY) / SIZE; i++)
			{
				double probability = static_cast<double>(i * STEP_OF_PROBABILITY);
				Binary_symmetric_channel BSC(probability);

				double ErrorProbabilityPerBit = 0;

				for (int i = 0; i < COUNT_OF_TESTS; i++)
				{
					string r_m = Random_message(LENGTH);
					ErrorProbabilityPerBit += BSC.ErrorProbabilityForBit(codec, r_m);
				}

				ErrorProbabilityPerBit /= COUNT_OF_TESTS;

				cout << probability << "\t" << ErrorProbabilityPerBit << endl;

				mp[i] = ErrorProbabilityPerBit;

			}
		});

	thread th2([&]()
		{
			

			for (int i =  ((END_PROBABILITY / STEP_OF_PROBABILITY) / SIZE); i < 2 * ((END_PROBABILITY / STEP_OF_PROBABILITY) / SIZE); i++)
			{
				double probability = static_cast<double>(i * STEP_OF_PROBABILITY);

				Binary_symmetric_channel BSC(probability);

				double ErrorProbabilityPerBit = 0;

				for (int i = 0; i < COUNT_OF_TESTS; i++)
				{
					string r_m = Random_message(LENGTH);
					ErrorProbabilityPerBit += BSC.ErrorProbabilityForBit(codec, r_m);
				}

				ErrorProbabilityPerBit /= COUNT_OF_TESTS;

				cout << probability << "\t" << ErrorProbabilityPerBit << endl;

				mp[i] = ErrorProbabilityPerBit;

			}
		});

	thread th3([&]()
		{
			

			for (int i = 2 * ((END_PROBABILITY / STEP_OF_PROBABILITY) / SIZE); i < 3 * ((END_PROBABILITY / STEP_OF_PROBABILITY) / SIZE); i++)
			{
				double probability = static_cast<double>(i * STEP_OF_PROBABILITY);

				Binary_symmetric_channel BSC(probability);

				double ErrorProbabilityPerBit = 0;

				for (int i = 0; i < COUNT_OF_TESTS; i++)
				{
					string r_m = Random_message(LENGTH);
					ErrorProbabilityPerBit += BSC.ErrorProbabilityForBit(codec, r_m);
				}

				ErrorProbabilityPerBit /= COUNT_OF_TESTS;

				cout << probability << "\t" << ErrorProbabilityPerBit << endl;

				mp[i] = ErrorProbabilityPerBit;

			}
		});

	thread th4([&]()
		{
			

			for (int i = 3 * ((END_PROBABILITY / STEP_OF_PROBABILITY) / SIZE); i < 4 * ((END_PROBABILITY / STEP_OF_PROBABILITY) / SIZE); i++)
			{
				double probability = static_cast<double>(i * STEP_OF_PROBABILITY);

				Binary_symmetric_channel BSC(probability);

				double ErrorProbabilityPerBit = 0;

				for (int i = 0; i < COUNT_OF_TESTS; i++)
				{
					string r_m = Random_message(LENGTH);
					ErrorProbabilityPerBit += BSC.ErrorProbabilityForBit(codec, r_m);
				}

				ErrorProbabilityPerBit /= COUNT_OF_TESTS;

				cout << probability << "\t" << ErrorProbabilityPerBit << endl;

				mp[i] = ErrorProbabilityPerBit;

			}
		});

	//cout << th1.get_id() << endl;
	//cout << th2.get_id() << endl;
	//cout << th3.get_id() << endl;
	//cout << th4.get_id() << endl;



	/*for (double probability = 0.0; probability < END_PROBABILITY; probability += STEP_OF_PROBABILITY)
	{
		Binary_symmetric_channel BSC(probability);

		double ErrorProbabilityPerBit = 0;

		for (int i = 0; i < COUNT_OF_TESTS; i++)
		{
			string r_m = Random_message(LENGTH);
			ErrorProbabilityPerBit += BSC.ErrorProbabilityForBit(codec, r_m);
		}

		ErrorProbabilityPerBit /= COUNT_OF_TESTS;

		cout << probability << "\t" << ErrorProbabilityPerBit << endl;

		mp[probability] = ErrorProbabilityPerBit;

	}*/


	double probability = static_cast<double>((END_PROBABILITY / STEP_OF_PROBABILITY) * STEP_OF_PROBABILITY);

	Binary_symmetric_channel BSC(probability);

	double ErrorProbabilityPerBit = 0;

	for (int i = 0; i < COUNT_OF_TESTS; i++)
	{
		string r_m = Random_message(LENGTH);
		ErrorProbabilityPerBit += BSC.ErrorProbabilityForBit(codec, r_m);
	}

	ErrorProbabilityPerBit /= COUNT_OF_TESTS;

	cout << probability << endl;

	mp[END_PROBABILITY / STEP_OF_PROBABILITY] = ErrorProbabilityPerBit;

	/*for (double probability = 0.0; probability < END_PROBABILITY; probability += STEP_OF_PROBABILITY)
	{
		Binary_symmetric_channel BSC(probability);

		double ErrorProbabilityPerBit = 0;

		for (int i = 0; i < COUNT_OF_TESTS; i++)
		{
			string r_m = Random_message(LENGTH);
			ErrorProbabilityPerBit += BSC.ErrorProbabilityForBit(codec, r_m);
		}

		ErrorProbabilityPerBit /= COUNT_OF_TESTS;

		cout << probability << endl;

		fs << probability << "\t" << ErrorProbabilityPerBit << "\n";

	}*/

	th1.join();
	th2.join();
	th3.join();
	th4.join();



	fstream fs;

	fs.open(file, fstream::out);

	for (int i = 0; i < (END_PROBABILITY / STEP_OF_PROBABILITY) + 1; i++)
	{
		fs << i * STEP_OF_PROBABILITY << "\t" << mp[i] << endl;
	}

	fs.close();
}

void CheckPolynomial(const int& polynomial)
{
	if (polynomial <= 0)
	{
		throw exception("The polynomial cannot be non-positive");
	}
}

void CheckWordLength(const int& word_length)
{
	if (word_length <= 0)
	{
		throw exception("The length of the output message cannot be non-positive");
	}
}

void CheckBitMsg(const string& bit_msg)
{
	for (int i = 0; i < bit_msg.size(); i++)
	{
		if (bit_msg[i] != '0' && bit_msg[i] != '1')
		{
			throw exception("The message should contain only 0 or 1");
		}
	}
}
