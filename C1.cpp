#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

const string INPUT_KEYWORDS[4] = {"name", "openingTime", "closingTime", "rank"};
const char INPUT_DELIMITER = ',';
const char TIME_DELIMITER = ':';
const int INITIAL_TIME = 480;
const int NOT_FOUND = -1;

enum HOUR
{
	QUARTER = 15,
	HALF = 30,
	HOUR = 60
};

enum KEYWORD_INDEX
{
	NAME,
	OPENING_TIME,
	CLOSING_TIME,
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

int search_element(vector<string> titles, string element)
{
	for (int i = 0; i < titles.size(); i++)
		if (titles[i] == element)
			return i;
	return NOT_FOUND;
}

vector<string> read_titles(string file_name)
{
	ifstream file(file_name);
	string titles_line;
	getline(file, titles_line);
	vector<string> all_titles;
	string title;
	stringstream titles_line_stream(titles_line);
	while (getline(titles_line_stream, title, INPUT_DELIMITER))
		all_titles.push_back(title);
	return all_titles;
}

vector<int> find_title_order(string file_name)
{
	vector<string> titles = read_titles(file_name);
	int name_index = search_element(titles, INPUT_KEYWORDS[NAME]);
	int opentime_index = search_element(titles, INPUT_KEYWORDS[OPENING_TIME]);
	int closetime_index = search_element(titles, INPUT_KEYWORDS[CLOSING_TIME]);
	int rank_index = search_element(titles, INPUT_KEYWORDS[RANK]);
	vector<int> title_arrangment = {name_index, opentime_index,
									closetime_index, rank_index};
	return title_arrangment;
}

vector<vector<string>> split_data(vector<string> lines_of_locations_info)
{
	vector<vector<string>> locations_info_table;
	int location_number = lines_of_locations_info.size();
	string new_loc;
	vector<string> location_info;
	for (int i = 0; i < location_number; i++)
	{
		stringstream stream_of_loc_info(lines_of_locations_info[i]);
		while (getline(stream_of_loc_info, new_loc, INPUT_DELIMITER))
		{
			location_info.push_back(new_loc);
		}
		locations_info_table.push_back(location_info);
		location_info.clear();
	}
	return locations_info_table;
}

vector<vector<string>> read_locs_data(string file_name)
{
	vector<vector<string>> locations_info_table;
	vector<string> lines_of_locations_info;
	ifstream file(file_name);
	string location_info, arrangment;
	getline(file, arrangment);
	while (getline(file, location_info))
		lines_of_locations_info.push_back(location_info);
	locations_info_table = split_data(lines_of_locations_info);
	return locations_info_table;
}

int time_to_minute(string time)
{
	int hour, minute, time_in_minute;
	string new_time;
	stringstream time_stream(time);
	for(int i = 0; i < 2; i++)
	{
		getline(time_stream, new_time, TIME_DELIMITER);
		stringstream time_stream(new_time);
		if(i == 0)
			time_stream >> hour;
		else
			time_stream >> minute;
	}
	time_in_minute = hour * HOUR + minute;
	return time_in_minute;	
}

vector<int> organize_open_times(vector<vector<string>> locations_info_table, int open_time_index)
{
	vector<int> open_times;
	int location_number = locations_info_table.size();
	for (int i = 0; i < location_number; i++)
	{
		int open_time_in_minute = time_to_minute(locations_info_table[i][open_time_index]);
		open_times.push_back(open_time_in_minute);
	}
	return open_times;
}

vector<int> organize_close_times(vector<vector<string>> locations_info_table, int close_time_index)
{
	vector<int> close_times;
	int location_number = locations_info_table.size();
	for (int i = 0; i < location_number; i++)
	{
		int close_time_in_minute = time_to_minute(locations_info_table[i][close_time_index]);
		close_times.push_back(close_time_in_minute);
	}
	return close_times;
}

vector<locations> organize_locations_data(vector<vector<string>> locations_info_table, vector<int> open_times,
										 vector<int> close_times, vector<int> title_arrangment)
{
	vector<locations> input_structs;
	locations adding_loc;
	int location_number = locations_info_table.size();
	for (int i = 0; i < location_number; i++)
	{
		adding_loc.number = i + 1;
		adding_loc.name = locations_info_table[i][title_arrangment[NAME]];
		adding_loc.opening_time = open_times[i];
		adding_loc.closing_time	= close_times[i];
		adding_loc.rank = stoi(locations_info_table[i][title_arrangment[RANK]]);
		input_structs.push_back(adding_loc);
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
			return tail;
		else
			return find_min(vec);
	}
	else
		return vec[0];
}

int is_num_in_vector(vector<int> vec, int element)
{
	for (int i = 0; i < vec.size(); i++)
		if (vec[i] == element)
			return 1;
	return 0;
}

void find_suitable_indexs(vector<locations> input, int nearest_time, vector<int> location_check,
						  vector<int> &suitable_indexs, vector<int> unsuitable_indexs)
{
	int size = input.size();
	for (int i = 0; i < size; i++)
		if (input[i].opening_time == nearest_time)
		{
			int existence_checker = is_num_in_vector(location_check, (input[i].number - 1));
			int unsuitable_checker = is_num_in_vector(unsuitable_indexs, i);
			if (existence_checker == 0 && unsuitable_checker == 0)
				suitable_indexs.push_back(i);
		}
}

int match_num_rank(vector<locations> input, int rank)
{
	for (int i = 0; i < input.size(); i++)
		if (input[i].rank == rank)
			return i;
	return NOT_FOUND;
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

vector<int> find_open_locs(vector<int> &open_times, int current_time, vector<int> location_check, vector<int> unsuitable_indexs)
{
	vector<int> suitable_indexs;
	for (int i = 0; i < open_times.size(); i++)
	{
		int existence_checker = is_num_in_vector(location_check, i);
		if (open_times[i] <= current_time && existence_checker == 0)
		{
			int unsuitable_check = is_num_in_vector(unsuitable_indexs, i);
			if (unsuitable_check == 0)
				suitable_indexs.push_back(i);
		}
	}
	return suitable_indexs;
}

int find_next_destination_index(int &current_time, vector<int> &open_times, vector<locations> input, vector<int> location_check, vector<int> unsuitable_indexs)
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
			counter++;
		}
		else
		{
			vector<int> late_opentimes;
			for (int i = 0; i < open_times.size(); i++)
				if (open_times[i] > current_time)
					late_opentimes.push_back(open_times[i]);
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
		return NOT_FOUND;
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
		if (location_check[i] == index)
			return i;
	return NOT_FOUND;
}

void find_next_destenation(vector<locations> input, vector<int> &location_check, vector<int> &start, vector<int> &durations,
						   vector<int> &open_times, vector<int> &close_times)
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
		if (existence_checker == (NOT_FOUND) && duration_check != (NOT_FOUND))
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

vector<vector<string>> make_vector_ready_for_print(vector<locations> input, vector<int> &close_times, vector<int> &location_check, vector<int> &start, vector<int> &durations)
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
		cout << "Location " << temp_vector[i][NAME] << endl
			 << "Visit from " << temp_vector[i][OPENING_TIME] << " until " << 
			temp_vector[i][CLOSING_TIME] << endl << "---" << endl;
}

vector<locations> read_from_file(string file_name, vector<int> &open_times, vector<int> &close_times)
{
	vector<int> arrangment = find_title_order(file_name);
	vector<vector<string>> locations_info_table = read_locs_data(file_name);
	open_times = organize_open_times(locations_info_table, arrangment[OPENING_TIME]);
	close_times = organize_close_times(locations_info_table, arrangment[CLOSING_TIME]);
	vector<locations> locations_data = organize_locations_data(locations_info_table, open_times, close_times, arrangment);
	return locations_data;
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
