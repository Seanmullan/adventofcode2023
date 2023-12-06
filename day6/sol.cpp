#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <optional>
#include <vector>

struct Race {
  long long int time;
  long long int distance;
};
using Races = std::vector<Race>;

std::vector<std::string> splitString(const std::string &str,
                                     const char separator) {
  std::vector<std::string> result;
  std::size_t start_idx = 0, end_idx = 0;
  for (std::size_t i = 0; i <= str.size(); i++) {
    if (str[i] == separator || i == str.size()) {
      // Handle case when start of string is separator
      if (str[i] == separator && i == 0) {
        start_idx = i + 1;
        continue;
      }
      end_idx = i;
      std::string temp;
      temp.append(str, start_idx, end_idx - start_idx);
      if (!temp.empty()) {
        result.push_back(temp);
      }
      start_idx = end_idx + 1;
    }
  }
  return result;
}

void solve(const Races& races)
{
  int res = 1;
  for (const auto& race : races)
  {
    int num_wins = 0;
    for(auto hold_time = 1; hold_time < race.time; hold_time++)
    {
      const auto remaining_time = race.time - hold_time;
      const auto distance = hold_time * remaining_time;
      if (distance > race.distance)
      {
        num_wins++;
      }
    }
    res *= num_wins;
  }

  std::cout << "Number of ways to win: " << res << std::endl;
}

int main() {

  std::string line;
  std::getline(std::cin, line);
  const auto times = splitString(splitString(line, ':')[1], ' ');
  std::getline(std::cin, line);
  const auto distances = splitString(splitString(line, ':')[1], ' ');

  Races part_1_races;
  for (auto i = 0; i < times.size(); i++)
  {
    part_1_races.push_back(Race({std::stoi(times[i]), std::stoi(distances[i])}));
  }

  solve(part_1_races);

  std::string time_str;
  std::string dist_str;
  for (auto i = 0; i < times.size(); i++)
  {
    time_str.append(times[i]);
    dist_str.append(distances[i]);
  }

  Races part_2_races;
  part_2_races.push_back(Race({std::stoll(time_str), std::stoll(dist_str)}));

  solve(part_2_races);
}