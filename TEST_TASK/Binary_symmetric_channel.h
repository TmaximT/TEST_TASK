#pragma once
#include <iostream>
#include "Codec.h"
#include <random>
#include <string>

#define END_PROBABILITY 1.0
#define STEP_OF_PROBABILITY 0.01
#define COUNT_OF_TESTS 100
#define LENGTH 100

using namespace std;


class Binary_symmetric_channel
{
private:

	double probability;//вероятность ошибки в двоичном симметричном канале

	char Reverse_bit(const char& bit) const;

	int Error_counting(const string& first_msg, const string& second_msg) const;

public:

	Binary_symmetric_channel(const double& probability);

	string Message_transmission(const string& code_msg) const;

	string Send_message_by_channel(Codec& codec, string& msg) const;

	double ErrorProbabilityForBit(Codec& codec, string& msg) const;

};