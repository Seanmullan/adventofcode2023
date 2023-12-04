
#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <optional>
#include <vector>

using Numbers = std::vector<int>;

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

void part1(const std::vector<Numbers> &winning_nums_list,
           const std::vector<Numbers> &lottery_nums_list) {
  const auto num_cards = winning_nums_list.size() - 1;

  auto sum = 0;
  for (auto i = 0; i <= num_cards; i++) {
    const auto &winning_nums = winning_nums_list[i];
    const auto &lottery_nums = lottery_nums_list[i];
    Numbers intersection;
    std::set_intersection(winning_nums.begin(), winning_nums.end(),
                          lottery_nums.begin(), lottery_nums.end(),
                          std::back_inserter(intersection));

    if (!intersection.empty()) {
      sum += std::pow(2.0, intersection.size() - 1);
    }
  }
  std::cout << "Part one sum: " << sum << std::endl;
}

void part2(const std::vector<Numbers> &winning_nums_list,
           const std::vector<Numbers> &lottery_nums_list) {
  std::map<int, int> num_matches;
  std::map<int, int> num_copies;
  for (auto i = 1; i <= winning_nums_list.size(); i++) {
    const auto &winning_nums = winning_nums_list[i - 1];
    const auto &lottery_nums = lottery_nums_list[i - 1];
    Numbers intersection;
    std::set_intersection(winning_nums.begin(), winning_nums.end(),
                          lottery_nums.begin(), lottery_nums.end(),
                          std::back_inserter(intersection));
    num_matches[i] = intersection.size();
    num_copies[i] = 1;
  }

  for (auto &[card, num] : num_copies) {
    const auto matches = num_matches[card];
    for (auto i = card + 1; i <= card + matches; i++) {
      num_copies[i] += num;
    }
  }
  auto total_cards = 0;
  for (auto &[_, num] : num_copies) {
    total_cards += num;
  }

  std::cout << "Part two sum: " << total_cards << std::endl;
}

int main() {
  std::vector<Numbers> winning_nums_list;
  std::vector<Numbers> lottery_nums_list;
  for (std::string line; std::getline(std::cin, line);) {
    Numbers winning_nums;
    Numbers lottery_nums;
    const auto all_nums_str = splitString(line, ':')[1];
    const auto split_nums_str = splitString(all_nums_str, '|');
    const auto &winning_nums_str = split_nums_str[0];
    const auto &lottery_nums_str = split_nums_str[1];
    for (const auto &n : splitString(winning_nums_str, ' ')) {
      winning_nums.push_back(std::stoi(n));
    }
    for (const auto &n : splitString(lottery_nums_str, ' ')) {
      lottery_nums.push_back(std::stoi(n));
    }
    std::sort(winning_nums.begin(), winning_nums.end());
    std::sort(lottery_nums.begin(), lottery_nums.end());
    winning_nums_list.push_back(winning_nums);
    lottery_nums_list.push_back(lottery_nums);
  }

  part1(winning_nums_list, lottery_nums_list);
  part2(winning_nums_list, lottery_nums_list);

  return 0;
}