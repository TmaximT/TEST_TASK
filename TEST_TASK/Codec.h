#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <limits>

using namespace std;

int highest_bit_index(const vector<int>& polynoms);

int Hamming_Distance(const string& first, const string& second);


class Codec
{
public:

	Codec(const vector<int>& polynoms);

	string Encode(const string& msg);

	string Decode(string& bits);

private:

	vector<int> polynoms;

	int shift_register_length;

	vector<pair<string, string>> output_variants;//all possible variants of the state register (For example: for a shift register with a length of 3 bits, we get 00, 01, 10, 11)

	string& Output_for_node(const int& state_node, const bool& input_reg);

	int Next_state_node(const int& current_state_node, const bool& input_reg) const;

	void FillOutPuts();

	void UpdateMetrics(vector<vector<int>>& Metrics, vector<int>& Metrics_Column, const string& word);

	void UpdateGrid(vector<vector<int>>& Grid, const string& word, const int& state);

};
