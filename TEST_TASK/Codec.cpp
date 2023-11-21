#include "Codec.h"



int highest_bit_index(const vector<int>& polynoms)
{
	int max = *max_element(polynoms.begin(), polynoms.end());

	int k = 0;

	if (max == 0)
	{
		throw "Invalid data";
	}

	while (max != 0)
	{
		max = max >> 1;
		k++;
	}

	return k - 1;
}

int Hamming_Distance(const string& first, const string& second)
{
	int distance = 0;

	for (int i = 0; i < first.size(); i++)
	{
		distance += (first[i] != second[i]);
	}

	return distance;
}


Codec::Codec(const vector<int>& polynoms)
{
	for (int i = 0; i < polynoms.size(); i++)
	{
		if (polynoms[i] <= 0)
		{
			throw exception("The value of the polynomial cannot be non-positive");
		}
	}

	this->polynoms = polynoms;

	this->shift_register_length = highest_bit_index(polynoms) + 1;

	FillOutPuts();
}

string Codec::Encode(const string& msg)
{
	for (int i = 0; i < msg.size(); i++)
	{
		if ((msg[i] != '0') && (msg[i] != '1'))
		{
			throw exception("A bit message cannot contain characters other than 0 or 1");
		}
	}

	string code;

	int state_node = 0;

	for (int i = 0; i < msg.size(); i++)
	{
		bool input_reg = msg[i] - '0';

		code += Output_for_node(state_node, input_reg);

		state_node = Next_state_node(state_node, input_reg);

	}

	return code;
}

void Codec::UpdateGrid(vector<vector<int>>& Grid, const string& word, const int& state)
{
	vector<int> Grid_Column;

	const int mask = (1 << (shift_register_length - 1)) - 1;

	for (int i = 0; i < output_variants.size(); i++)
	{
		int next_state = i;

		int current_state1 = (next_state << (shift_register_length - 2)) & mask;
		int current_state2 = ((next_state << (shift_register_length - 2)) | 1) & mask;

		int input = next_state >> (shift_register_length - 2) & 1;

		int dist1 = Hamming_Distance(Output_for_node(current_state1, input), word);
		int dist2 = Hamming_Distance(Output_for_node(current_state2, input), word);

		if (dist1 < dist2)
		{
			Grid_Column.push_back(current_state1);
		}
		else
		{
			Grid_Column.push_back(current_state2);
		}
	}

	Grid.push_back(Grid_Column);
}

void Codec::UpdateMetrics(vector<vector<int>>& Metrics, vector<int>& Metrics_Column ,const string& word)
{

	const int mask = (1 << (shift_register_length - 1)) - 1;

	for (int i = 0; i < output_variants.size(); i++)
	{
		int next_state = i;

		int current_state1 = (next_state << (shift_register_length - 2)) & mask;
		int current_state2 = ((next_state << (shift_register_length - 2)) | 1) & mask;

		int input = next_state >> (shift_register_length - 2) & 1;

		int dist1 = Hamming_Distance(Output_for_node(current_state1, input), word);
		int dist2 = Hamming_Distance(Output_for_node(current_state2, input), word);

		if (dist1 < dist2)
		{
			Metrics_Column[i] = dist1;
		}
		else
		{
			Metrics_Column[i] = dist2;
		}
	}

	Metrics.push_back(Metrics_Column);
}

string Codec::Decode(string& bits)
{
	vector<vector<int>> Grid;

	vector<vector<int>> Metrics;

	for (int i = 0; i < bits.size(); i++)
	{
		if ((bits[i] != '0') && (bits[i] != '1'))
		{
			throw exception("A bit message cannot contain characters other than 0 or 1");
		}
	}

	int delta = bits.size() % polynoms.size();

	for (int i = 0; i < delta; i++)//Добавляем нули в конец входного сообщения для декодирования, в случае когда длина входного сообщения не делится нацело на количество выходов кодера
	{
		bits.push_back(0);
	}

	vector<int> brackets(output_variants.size(), numeric_limits<int>::max());

	vector<int> Metrics_Column = brackets;

	int state = 0;

	int word_length = polynoms.size();

	for (int i = 0; i < bits.size(); i += word_length)
	{
		string word(bits, i, word_length);

		UpdateGrid(Grid, word, state);

		UpdateMetrics(Metrics, Metrics_Column, word);

		state++;
	}

	vector<string> reverse_decodes(output_variants.size());

	vector<int> weights(output_variants.size());

	for (int k = 0; k < output_variants.size(); k++)
	{
		int i = k;
		int j = Grid.size() - 1;

		int sum = 0;;

		while (j >= 0)
		{

			int input = (i >> (shift_register_length - 2));

			reverse_decodes[k].push_back((char)(input + 48));

			sum += Metrics[j][i];

			i = Grid[j][i];

			j--;
		}

		weights[k] = sum;
	}

	int min = weights[0];

	int count = 0;

	for (int i = 0; i < weights.size(); i++)
	{
		if (min >= weights[i])
		{
			min = weights[i];

			count = i;
		}
	}

	reverse(reverse_decodes[count].begin(), reverse_decodes[count].end());

	return reverse_decodes[count];

}

string& Codec::Output_for_node(const int& state_node, const bool& input_reg)
{
	int index = state_node;

	if (input_reg)
	{
		return output_variants[index].second;
	}
	else
	{
		return output_variants[index].first;
	}
}

int Codec::Next_state_node(const int& current_state_node, const bool& input_reg) const
{
	return ((current_state_node >> 1) | input_reg << (shift_register_length - 2));
}

void Codec::FillOutPuts()
{
	int max_index = shift_register_length - 1;

	output_variants.resize(1 << (max_index)); // количество всех возможных комбинаций регистра сдвига

	for (int i = 0; i < output_variants.size(); i++)
	{
		for (int j = 0; j < polynoms.size(); j++) // количество генераторных полиномов определяет размерность выходных слов (количество выходов)
		{
			int state_reg = i;

			int summator = 0;

			int k = 1;

			vector<int> spolynoms = polynoms;

			while (k < max_index + 1)
			{
				summator ^= (state_reg & 1) & (spolynoms[j] & 1);

				state_reg = state_reg >> 1;

				spolynoms[j] = spolynoms[j] >> 1;

				k = k << 1;
			}

			output_variants[i].first += to_string(summator);

			summator ^= (spolynoms[j] & 1);

			output_variants[i].second += to_string(summator);

		}
	}
}