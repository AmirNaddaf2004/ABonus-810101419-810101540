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
	vector<locations> locations_data;
	locations adding_loc;
	int location_number = locations_info_table.size();
	for (int i = 0; i < location_number; i++)
	{
		adding_loc.number = i + 1;
		adding_loc.name = locations_info_table[i][title_arrangment[NAME]];
		adding_loc.opening_time = open_times[i];
		adding_loc.closing_time	= close_times[i];
		adding_loc.rank = stoi(locations_info_table[i][title_arrangment[RANK]]);
		locations_data.push_back(adding_loc);
	}
	return locations_data;
}

int find_min(vector<int> numbers_list)
{
	int length = numbers_list.size();
	if (length > 1)
	{
		int tail = numbers_list[length - 1];
		numbers_list.pop_back();
		int remainings_min = find_min(numbers_list);
		if (tail <= remainings_min)
			return tail;
		else
			return remainings_min;
	}
	else
		return numbers_list[0];
}

bool existance_checker(vector<int> resource, int element)
{
	for (int i = 0; i < resource.size(); i++)
		if (resource[i] == element)
			return true;
	return false;
}

vector<int> find_suitable_indexs(vector<locations> locations_data, int nearest_time, vector<int> location_check,
						 vector<int> unsuitable_loc_indexes)
{
	vector<int> suitable_loc_indexes;
	int size = locations_data.size();
	for (int i = 0; i < size; i++)
		if (locations_data[i].opening_time == nearest_time && !existance_checker(unsuitable_loc_indexes, i))
			suitable_loc_indexes.push_back(i);
	return suitable_loc_indexes;
}

int match_num_rank(vector<locations> locations_data, int rank)
{
	for (int i = 0; i < locations_data.size(); i++)
		if (locations_data[i].rank == rank)
			return i;
	return NOT_FOUND;
}

int find_best(vector<int> suitable_indexs, vector<locations> input)
{
	vector<int> ranks;
	int location_index, best;
	int size = suitable_indexs.size();
	for (int i = 0; i < size; i++)
	{
		location_index = suitable_indexs[i];
		ranks.push_back(input[location_index].rank);
	}
	best = find_min(ranks);
	return best;
}

int update_current_time(int previous_time, int duration)
{
	int current_time = previous_time + duration + HALF;
	return current_time;
}

int calculate_end_time(int previous_time, int duration)
{
	int end_time = previous_time + duration;
	return end_time;
}

vector<int> find_open_locs(vector<int> open_times, int current_time, vector<int> location_check,
							 vector<int> unsuitable_indexs)
{
	vector<int> suitable_indexs;
	for (int i = 0; i < open_times.size(); i++)
	{
		if (open_times[i] <= current_time && !existance_checker(location_check, i) &&
			!existance_checker(unsuitable_indexs, i))
				suitable_indexs.push_back(i);
	}
	return suitable_indexs;
}

vector<int> find_open_times_after_now(vector<int> all_opening_times, int current_time)
{
	vector<int> late_open_times;
	for (int i = 0; i < all_opening_times.size(); i++)
		if (all_opening_times[i] > current_time)
			late_open_times.push_back(all_opening_times[i]);
	return late_open_times;
}
int find_next_destination_index(int &current_time, vector<int> &open_times, vector<locations> input, 
							vector<int> location_check, vector<int> unsuitable_loc_indexes)
{
	vector<int> suitable_loc_indexes;
	int rank, next_destination_index = NOT_FOUND;
	int counter = 0;
	int len = open_times.size();
	while (true)
	{
		suitable_loc_indexes.clear();
		if (current_time >= find_min(open_times) && counter < len)
		{
			suitable_loc_indexes = find_open_locs(open_times, current_time, location_check, unsuitable_loc_indexes);
			counter++;
		}
		else
		{
			vector<int> late_open_times = find_open_times_after_now(open_times, current_time);
			int nearest_time = find_min(late_open_times);
			suitable_loc_indexes = find_suitable_indexs(input, nearest_time, location_check, unsuitable_loc_indexes);
			current_time = nearest_time;
		}
		if (suitable_loc_indexes.size() > 0)
		{
			rank = find_best(suitable_loc_indexes, input);
			next_destination_index = match_num_rank(input, rank);
			break;
		}
	}
	return next_destination_index;
}

int find_duration(locations location, int current_time)
{
	int duration = location.closing_time - current_time;
	if (duration >= HOUR)
		duration = HOUR;
	if (duration > QUARTER)
		return duration;
	else
		return NOT_FOUND;
}

int find_max(vector<int> closing_time)
{
	int size = closing_time.size();
	if (size <= 1)
		return closing_time[0];
	else
	{
		int tail = closing_time[size - 1];
		closing_time.pop_back();
		int remainings_max = find_max(closing_time);
		if (tail >= remainings_max)
			return tail;
		else
			return remainings_max;
	}
}

vector<int> final_next_destination(vector<locations> locations_data, vector<int> &start_times,
							 vector<int> &durations, vector<int> open_times, vector<int> close_times)
{
	vector<int> gone_locations_indexes;
	int location_number = locations_data.size();
	vector<int> unsuitable_loc_indexes;
	int number_of_checked_locs = 0;
	int current_time = INITIAL_TIME;
	int end_time = find_max(close_times);
	while (current_time < end_time && number_of_checked_locs < location_number)
	{
		int location_index = find_next_destination_index(current_time, open_times, locations_data,
														 gone_locations_indexes, unsuitable_loc_indexes);
		int duration = find_duration(locations_data[location_index], current_time);
		if (location_index != NOT_FOUND && duration != NOT_FOUND)
		{
			gone_locations_indexes.push_back(location_index);
			start_times.push_back(current_time);
			durations.push_back(duration);
			current_time = update_current_time(current_time, duration);
		}
		else
			unsuitable_loc_indexes.push_back(location_index);
		number_of_checked_locs++;
	}
	return gone_locations_indexes;
}

string convert_int_to_clockform(int time)
{
	int hour = time / HOUR;
	int min = time - (hour * HOUR);
	stringstream hour_stream;
	hour_stream << hour;
	string hour_str = hour_stream.str();
	stringstream min_stream;
	min_stream << min;
	string min_str = min_stream.str();
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
	vector<vector<string>> final_schedjule;
	for (int i = 0; i < location_check.size(); i++)
	{
		int end = calculate_end_time(start[i], durations[i]);
		string standard_start = convert_int_to_clockform(start[i]);
		string standard_end = convert_int_to_clockform(end);
		vector<string> temp_schedjule = {input[location_check[i]].name, standard_start, standard_end};
		final_schedjule.push_back(temp_schedjule);
	}
	return final_schedjule;
}

void print_output(vector<vector<string>> final_schedjule)
{
	for (int i = 0; i < final_schedjule.size(); i++)
		cout << "Location " << final_schedjule[i][NAME] << endl
			 << "Visit from " << final_schedjule[i][OPENING_TIME] << " until " <<
			 	final_schedjule[i][CLOSING_TIME] << endl << "---" << endl;
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
	vector<int> start_times;
	vector<int> durations;
	vector<int> open_times;
	vector<int> close_times;
	vector<locations> locations_data = read_from_file(argv[1] + 2, open_times, close_times);
	vector<int> gone_location = final_next_destination(locations_data, start_times, durations, open_times, close_times);
	vector<vector<string>> ready_to_print = make_vector_ready_for_print(locations_data, close_times,
																		gone_location, start_times, durations);
	print_output(ready_to_print);
}
