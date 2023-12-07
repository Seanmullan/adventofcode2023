#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <optional>
#include <set>
#include <vector>

std::map<char, int> FACE_CARD_VALUE_MAP = { { 'T', 10 }, { 'J', 11 }, { 'Q', 12 }, { 'K', 13 }, { 'A', 14 } };

struct Hand
{
    std::vector<int> cards;
    int bid;
};
using Hands = std::vector<Hand>;

std::vector<std::string> splitString(const std::string& str, const char separator)
{
    std::vector<std::string> result;
    std::size_t start_idx = 0, end_idx = 0;
    for (std::size_t i = 0; i <= str.size(); i++)
    {
        if (str[i] == separator || i == str.size())
        {
            // Handle case when start of string is separator
            if (str[i] == separator && i == 0)
            {
                start_idx = i + 1;
                continue;
            }
            end_idx = i;
            std::string temp;
            temp.append(str, start_idx, end_idx - start_idx);
            if (!temp.empty())
            {
                result.push_back(temp);
            }
            start_idx = end_idx + 1;
        }
    }
    return result;
}

auto number_of(const std::vector<int> v, const int e)
{
    return std::count(v.begin(), v.end(), e);
}

int resolve_hand_type(std::vector<int> cards)
{
    std::set<int> s(cards.begin(), cards.end());
    if (s.size() == 1)
    {
        return 7;  // five of a kind
    }
    else if (s.size() == 2)
    {
        for (auto v : s)
        {
            if (number_of(cards, v) == 1)
            {
                return 6;  // four of a kind
            }
        }
        return 5;  // full house
    }
    else if (s.size() == 3)
    {
        for (auto v : s)
        {
            if (number_of(cards, v) == 3)
            {
                return 4;  // three of a kind
            }
        }
        return 3;  // two pair
    }
    else if (s.size() == 4)
    {
        return 2;  // one pair
    }
    return 1;  // high card
}

int resolve_hand_type_with_wildcard(std::vector<int> cards)
{
    const auto normal_type = resolve_hand_type(cards);
    const auto num_wildcards = std::count(cards.begin(), cards.end(), 1);
    if (num_wildcards == 0)
    {
        return normal_type;
    }

    if (normal_type == 1)  // high card
    {
        switch (num_wildcards)
        {
            case 1:
                return 2;  // one pair
            case 2:
                return 4;  // three of a kind
            case 3:
                return 6;  // four of a kind
            case 4:
                return 7;  // five of a kind
            default:
                std::cout << "Defualt 1" << std::endl;
                return 0;
        }
    }

    if (normal_type == 2)  // one pair
    {
        switch (num_wildcards)
        {
            case 1:
            case 2:
                return 4;  // three of a kind
            default:
                std::cout << "Defualt 2" << std::endl;
                return 0;
        }
    }

    if (normal_type == 3)  // two pair
    {
        switch (num_wildcards)
        {
            case 1:
                return 5;  // full house
            case 2:
                return 6;  // four of a kind
            default:
                std::cout << "Defualt 3" << std::endl;
                return 0;
        }
    }

    if (normal_type == 4)  // three of a kind
    {
        switch (num_wildcards)
        {
            case 1:
            case 3:
                return 6;  // four of a kind
            default:
                std::cout << "Defualt 4" << std::endl;
                return 0;
        }
    }

    if (normal_type == 5)  // full house
    {
        switch (num_wildcards)
        {
            case 2:
            case 3:
                return 7;  // five of a kind
            default:
                std::cout << "Defualt 5" << std::endl;
                return 0;
        }
    }

    if (normal_type == 6)  // four of a kind
    {
        switch (num_wildcards)
        {
            case 1:
            case 4:
                return 7;  // five of a kind
            default:
                std::cout << "Defualt 5" << std::endl;
                return 0;
        }
    }

    return 7;  // five of a kind
}

void replace_with_wildcard(Hands& hands)
{
    for (auto& h : hands)
    {
        for (auto& c : h.cards)
        {
            if (c == FACE_CARD_VALUE_MAP['J'])
            {
                c = 1;
            }
        }
    }
}

struct less_than_comparator
{
    inline bool operator()(const Hand& hand1, const Hand& hand2)
    {
        const auto v1 = resolve_hand_type(hand1.cards);
        const auto v2 = resolve_hand_type(hand2.cards);
        if (v1 != v2)
        {
            return v1 < v2;
        }
        for (auto i = 0; i < hand1.cards.size(); i++)
        {
            if (hand1.cards[i] != hand2.cards[i])
            {
                return hand1.cards[i] < hand2.cards[i];
            }
        }
        std::cout << "Found equal hands" << std::endl;
        exit(1);
    }
};

struct wildcard_less_than_comparator
{
    inline bool operator()(const Hand& hand1, const Hand& hand2)
    {
        const auto v1 = resolve_hand_type_with_wildcard(hand1.cards);
        const auto v2 = resolve_hand_type_with_wildcard(hand2.cards);
        if (v1 != v2)
        {
            return v1 < v2;
        }
        for (auto i = 0; i < hand1.cards.size(); i++)
        {
            if (hand1.cards[i] != hand2.cards[i])
            {
                return hand1.cards[i] < hand2.cards[i];
            }
        }
        std::cout << "Found equal hands" << std::endl;
        exit(1);
    }
};

void part1(Hands hands)
{
    std::sort(hands.begin(), hands.end(), less_than_comparator());
    long long int res = 0;
    for (auto i = 0; i < hands.size(); i++)
    {
        res += hands[i].bid * (i + 1);
    }
    std::cout << "Part 1: " << res << std::endl;
}

void part2(Hands hands)
{
    std::sort(hands.begin(), hands.end(), wildcard_less_than_comparator());
    long long int res = 0;
    for (auto i = 0; i < hands.size(); i++)
    {
        res += hands[i].bid * (i + 1);
    }
    std::cout << "Part 2: " << res << std::endl;
}

int main()
{
    Hands hands;
    for (std::string line; std::getline(std::cin, line);)
    {
        Hand hand;
        const auto str = splitString(line, ' ');
        for (const auto c : str[0])
        {
            if (c == ':')
            {
                continue;
            }
            else if (std::isdigit(c))
            {
                hand.cards.push_back((int)c - '0');
            }
            else
            {
                hand.cards.push_back(FACE_CARD_VALUE_MAP[c]);
            }
        }
        hand.bid = std::stoi(str[1]);
        hands.push_back(hand);
    }
    part1(hands);
    replace_with_wildcard(hands);
    part2(hands);
}