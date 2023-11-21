#include "Binary_symmetric_channel.h"



Binary_symmetric_channel::Binary_symmetric_channel(const double& probability)
{
	this->probability = probability;
}

char Binary_symmetric_channel::Reverse_bit(const char& bit) const
{
	if (bit == '0')
	{
		return '1';
	}
	else if (bit == '1')
	{
		return '0';
	}
	else
	{
		throw exception("Invalid bit value");
	}
}

string Binary_symmetric_channel::Message_transmission(const string& code_msg) const
{
	string noised_msg;

	random_device dvc;
	uniform_real_distribution<double> dist(0.0, 1.0);

	for (int i = 0; i < code_msg.size(); i++)
	{
		if (dist(dvc) < this->probability)
		{
			noised_msg.push_back(Reverse_bit(code_msg[i]));
		}
		else
		{
			noised_msg.push_back(code_msg[i]);
		}
	}

	return noised_msg;
}

string Binary_symmetric_channel::Send_message_by_channel(Codec& codec, string& msg) const
{
	string encode_msg = codec.Encode(msg);

	string transmission_msg = Message_transmission(encode_msg);

	return codec.Decode(transmission_msg);
}

int Binary_symmetric_channel::Error_counting(const string& first_msg, const string& second_msg) const
{
	int error_count = 0;

	for (int i = 0; i < first_msg.size(); i++)
	{
		if (first_msg[i] != second_msg[i])
		{
			error_count++;
		}
	}

	return error_count;
}

double Binary_symmetric_channel::ErrorProbabilityForBit(Codec& codec, string& msg) const
{
	string recieved_msg = Send_message_by_channel(codec, msg);

	int errors_count = Error_counting(msg, recieved_msg);

	return errors_count / static_cast<double>(msg.size());
}