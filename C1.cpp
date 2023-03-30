#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

const string INPUTKEYWORDS[4] = {"name", "openingTime", "closingTime", "rank"};
const char INPUT_DELIMITER = ',';
const char TIME_DELIMITER = ':';
const int INITIAL_TIME = 480;

enum HOUR
{
	QUARTER = 15,
	HALF = 30,
	HOUR = 60
};

enum KEYWORDINDEX
{
	NAME,
	OPENINGTIME,
	CLOSINGTIME,
	RANK
};

struct locations
{
	int number;
	string name;
	int opening_time;
	int closing_time;
	int rank;
};

string getfirstline(string file_name)
{
	ifstream file(file_name);
	string temp_str;
	getline(file, temp_str);
	return temp_str;
}

vector<string> split_firstline(string firstline)
{
	vector<string> titles;
	string token;
	stringstream S(firstline);
	while (getline(S, token, INPUT_DELIMITER))
		titles.push_back(token);
	return titles;
}

int searcher(vector<string> vec, string element)
{
	for (int i = 0; i < vec.size(); i++)
	{
		if (vec[i] == element)
			return i;
	}
	return -1;
}

vector<int> arrangment_function(vector<string> titles)
{
	int name_index = searcher(titles, INPUTKEYWORDS[NAME]);
	int opentime_index = searcher(titles, INPUTKEYWORDS[OPENINGTIME]);
	int closetime_index = searcher(titles, INPUTKEYWORDS[CLOSINGTIME]);
	int rank_index = searcher(titles, INPUTKEYWORDS[RANK]);
	vector<int> title_arrangment = {name_index, opentime_index,
									closetime_index, rank_index};
	return title_arrangment;
}

vector<string> read_locs_data(string file_name)
{
	vector<string> input_strings;
	ifstream file(file_name);
	string temp_str;
	string temp_str2;
	getline(file, temp_str2);
	while (getline(file, temp_str))
		input_strings.push_back(temp_str);

	return input_strings;
}

vector<vector<string>> split_input(vector<string> input_strings)
{
	vector<vector<string>> input_table;
	int location_number = input_strings.size();
	string token;
	vector<string> temp_vec;
	for (int i = 0; i < location_number; i++)
	{

		stringstream S(input_strings[i]);
		while (getline(S, token, INPUT_DELIMITER))
		{
			temp_vec.push_back(token);
		}
		input_table.push_back(temp_vec);
		temp_vec.clear();
	}
	return input_table;
}

vector<int> create_opentime_vector(vector<vector<string>> input_table, int opentime_index)
{
	vector<int> open_times;
	string token;
	vector<int> temp;
	int location_number = input_table.size();
	for (int i = 0; i < location_number; i++)
	{
		stringstream S(input_table[i][opentime_index]);
		while (getline(S, token, TIME_DELIMITER))
		{
			stringstream ss(token);
			int temp_clock = 0;
			ss >> temp_clock;
			temp.push_back(temp_clock);
		}
		int close_time = temp[0] * HOUR + temp[1];
		temp.clear();
		open_times.push_back(close_time);
	}
	return open_times;
}
vector<int> create_closetime_vector(vector<vector<string>> input_table, int closetime_index)
{
	vector<int> close_times;
	string token;
	vector<int> temp;
	int location_number = input_table.size();
	for (int i = 0; i < location_number; i++)
	{
		stringstream S(input_table[i][closetime_index]);
		while (getline(S, token, TIME_DELIMITER))
		{
			stringstream ss(token);
			int temp_clock = 0;
			ss >> temp_clock;
			temp.push_back(temp_clock);
		}
		int close_time = temp[0] * HOUR + temp[1];
		temp.clear();
		close_times.push_back(close_time);
	}
	return close_times;
}

vector<locations> put_input_to_struct(vector<vector<string>> input_table, vector<int> &open_times, vector<int>& close_times, vector<int> title_arrangment)
{
	vector<locations> input_structs;
	int string_num = input_table.size();
	for (int i = 0; i < string_num; i++)
	{
		vector<locations> input_vector;
		input_structs.push_back({i + 1, input_table[i][title_arrangment[0]], open_times[i], close_times[i], stoi(input_table[i][title_arrangment[RANK]])});
	}
	return input_structs;
}

int find_min(vector<int> vec)
{
	vector<string> nearest_times;
	int length = vec.size();
	if (length > 1)
	{
		int tail = vec[length - 1];
		vec.pop_back();
		if (tail <= find_min(vec))
		{
			return tail;
		}
		else
			return find_min(vec);
	}
	else
		return vec[0];
}
int int_finder(vector<int> vec, int element)
{
	for (int i = 0; i < vec.size(); i++)
	{
		if (vec[i] == element)
			return 1;
	}
	return 0;
}
void find_suitable_indexs(vector<locations> input, int nearest_time, vector<int> location_check,
							 vector<int> &suitable_indexs, vector<int> unsuitable_indexs)
{
	int size = input.size();
	for (int i = 0; i < size; i++)
	{
		if (input[i].opening_time == nearest_time)
		{
			int existence_checker = int_finder(location_check, (input[i].number - 1));
			int unsuitable_checker = int_finder(unsuitable_indexs, i);
			if (existence_checker == 0 && unsuitable_checker == 0)
				suitable_indexs.push_back(i);
		}
	}
}

int match_num_rank(vector<locations> input, int rank)
{
	for (int i = 0; i < input.size(); i++)
	{
		if (input[i].rank == rank)
			return i;
	}
	return -1;
}
int find_best(vector<int> suitable_indexs, vector<locations> input)
{
	vector<int> ranks;
	int index;
	int size = suitable_indexs.size();
	for (int i = 0; i < size; i++)
	{
		index = suitable_indexs[i];
		ranks.push_back(input[index].rank);
	}
	int best = find_min(ranks);
	return best;
}

int calculate(int previous_time, int duration)
{
	if (duration >= HOUR)
		duration = HOUR;
	return previous_time + duration + HALF;
}
int calculate_endtime(int previous_time, int duration)
{
	if (duration >= HOUR)
		duration = HOUR;
	return previous_time + duration;
}

vector<int> find_open_locs(vector<int>& open_times, int current_time, vector<int> location_check, vector<int> unsuitable_indexs)
{
	vector<int> suitable_indexs;
	for (int i = 0; i < open_times.size(); i++)
	{
		int existence_checker = int_finder(location_check, i);
		if (open_times[i] <= current_time && existence_checker == 0)
		{
			int unsuitable_check = int_finder(unsuitable_indexs, i);
			if (unsuitable_check == 0)
				suitable_indexs.push_back(i);
		}
	}
	return suitable_indexs;
}
int find_nearest(int current_time, vector<int> opentimes)
{
	vector<int> late_opentimes;
	for (int i = 0; i < opentimes.size(); i++)
	{
		if (opentimes[i] > current_time)
		{
			late_opentimes.push_back(opentimes[i]);
		}
	}
	int min = find_min(late_opentimes);
	return min;
}

int find_next_destination_index(int &current_time, vector<int> &open_times, vector<locations> input, 
								vector<int> location_check, vector<int> unsuitable_indexs)
{
	vector<int> suitable_indexs;
	int rank, index;
	int counter = 0;
	int len = open_times.size();
	while (true)
	{
		suitable_indexs.clear();
		if (current_time >= find_min(open_times) && counter < len)
		{
			suitable_indexs = find_open_locs(open_times, current_time, location_check, unsuitable_indexs);
			counter += 1;
		}
		else
		{
			vector<int> late_opentimes;
			for (int i = 0; i < open_times.size(); i++)
			{
				if (open_times[i] > current_time)
					late_opentimes.push_back(open_times[i]);
			}
			int nearest_time = find_min(late_opentimes);
			find_suitable_indexs(input, nearest_time, location_check, suitable_indexs, unsuitable_indexs);
			current_time = nearest_time;
		}
		if (suitable_indexs.size() > 0)
		{
			rank = find_best(suitable_indexs, input);
			index = match_num_rank(input, rank);
			break;
		}
	}

	return index;
}

int check_destination_wellness(vector<locations> input, int current_time, int index)
{
	int duration = input[index].closing_time - current_time;
	int duration_checker = duration > QUARTER;
	if (duration_checker == 1)
		return duration;
	else
		return -1;
}

int find_max(vector<int> closing_time)
{
	int size = closing_time.size();
	if (size == 1)
		return closing_time[0];
	else
	{
		int tail = closing_time[size - 1];
		closing_time.pop_back();
		if (tail >= find_max(closing_time))
			return tail;
		else
			return find_max(closing_time);
	}
}

int existence_check(vector<int> location_check, int index)
{
	for (int i = 0; i < location_check.size(); i++)
	{
		if (location_check[i] == index)
			return i;
	}
	return -1;
}
void find_next_destenation(vector<locations> input, vector<int> &location_check, vector<int> &start,
						vector<int> &durations, vector<int> &open_times, vector<int> &close_times)
{
	int size = input.size();
	vector<int> not_suitables;
	int counter = 0;
	int current_time = INITIAL_TIME;
	while (current_time < find_max(close_times) && counter < size)
	{
		int index = find_next_destination_index(current_time, open_times, input, location_check, not_suitables);
		int existence_checker = existence_check(location_check, index);
		int duration_check = check_destination_wellness(input, current_time, index);
		if (existence_checker == (-1) && duration_check != (-1))
		{
			location_check.push_back(index);
			start.push_back(current_time);
			durations.push_back(duration_check);
			current_time = calculate(current_time, duration_check);
		}
		else
			not_suitables.push_back(index);
		counter++;
	}
}
string convert_int_to_clockform(int time)
{
	int hour = time / HOUR;
	int min = time - (hour * HOUR);
	stringstream ss;
	ss << hour;
	string hour_str = ss.str();
	stringstream sss;
	sss << min;
	string min_str = sss.str();
	if (hour < 10 && min < 10)
		return "0" + hour_str + TIME_DELIMITER + "0" + min_str;
	else if (hour < 10 && min >= 10)
		return "0" + hour_str + TIME_DELIMITER + min_str;
	else if (hour >= 10 && min < 10)
		return hour_str + TIME_DELIMITER + "0" + min_str;
	else
		return hour_str + TIME_DELIMITER + min_str;
}

vector<vector<string>> make_vector_ready_for_print(vector<locations> input, vector<int> &close_times,
										vector<int> &location_check, vector<int> &start, vector<int> &durations)
{
	vector<vector<string>> temp_vector;
	for (int i = 0; i < location_check.size(); i++)
	{
		int end = calculate_endtime(start[i], durations[i]);
		string standard_start = convert_int_to_clockform(start[i]);
		string standard_end = convert_int_to_clockform(end);
		vector<string> temp = {input[location_check[i]].name, standard_start, standard_end};
		temp_vector.push_back(temp);
	}
	return temp_vector;
}

void print_output(vector<vector<string>> temp_vector)
{
	for (int i = 0; i < temp_vector.size(); i++)
	{
		cout << "Location " << temp_vector[i][0] << endl
			 << "Visit from " << temp_vector[i][1] << " until " << temp_vector[i][2] << endl
			 << "---" << endl;
	}
}

vector<locations> read_from_file(string file_name, vector<int> &open_times, vector<int> &close_times)
{
	ifstream file(file_name);
	string temp_str;
	getline(file, temp_str);
	vector<string> splitted_firstline;
	string token;
	stringstream S(temp_str);
	while (getline(S, token, INPUT_DELIMITER))
		splitted_firstline.push_back(token);
	vector<int> arrangment = arrangment_function(splitted_firstline);
	vector<string> primitive_get = read_locs_data(file_name);
	vector<vector<string>> splitted_input = split_input(primitive_get);
	open_times = create_opentime_vector(splitted_input, arrangment[1]);
	close_times = create_closetime_vector(splitted_input, arrangment[2]);
	vector<locations> location_data = put_input_to_struct(splitted_input, open_times, close_times, arrangment);
	return location_data;
}

int main(int argc, char *argv[])
{

	vector<int> gone_location;
	vector<int> start_times;
	vector<int> durations;
	vector<int> open_times;
	vector<int> close_times;
	vector<locations> location_data = read_from_file(argv[1] + 2, open_times, close_times);
	find_next_destenation(location_data, gone_location, start_times, durations, open_times, close_times);
	vector<vector<string>> ready_to_print = make_vector_ready_for_print(location_data, close_times,
																		gone_location, start_times, durations);
	print_output(ready_to_print);
}
