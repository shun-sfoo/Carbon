#pragma once
#include <cctype>
#include <string>
#include <unordered_map>

constexpr size_t MaxCodeLength{4};
const std::string NotADigit{"*"};

class Soundex {
public:
  std::string encode(const std::string &word) const {
    return zeroPad(upperFont(head(word)) + encodedDigits(tail(word)));
  }

  std::string encodeDigit(char letter) const {
    const std::unordered_map<char, std::string> encodings{
        {'b', "1"}, {'f', "1"}, {'p', "1"}, {'v', "1"}, //
        {'c', "2"}, {'g', "2"}, {'j', "2"}, {'k', "2"}, //
        {'q', "2"}, {'s', "2"}, {'x', "2"}, {'z', "2"}, //
        {'d', "3"}, {'t', "3"},                         //
        {'l', "4"}, {'m', "5"},                         //
        {'n', "5"}, {'r', "6"}                          //
    };
    auto it = encodings.find(lower(letter));

    return it == encodings.end() ? NotADigit : it->second;
  }

private:
  char lower(char c) const { return std::tolower(static_cast<unsigned char>(c)); }

  std::string upperFont(const std::string &string) const {
    return std::string(1, std::toupper(static_cast<unsigned char>(string.front())));
  }

  std::string head(const std::string &word) const { return word.substr(0, 1); }

  std::string encodedDigits(const std::string &word) const {
    std::string encoding;
    for (auto letter : word) {
      if (isComplete(encoding)) {
        break;
      }
      auto digit = encodeDigit(letter);
      if (digit != NotADigit && digit != lastDigit(encoding)) {
        encoding += encodeDigit(letter);
      }
    }
    return encoding;
  }

  std::string lastDigit(const std::string &encoding) const {
    if (encoding.empty()) {
      return NotADigit;
    }
    return std::string(1, encoding.back());
  }

  std::string tail(const std::string &word) const { return word.substr(1); }

  bool isComplete(const std::string &encoding) const { return encoding.length() == MaxCodeLength - 1; }

  std::string zeroPad(const std::string &word) const {
    auto zerosNeeded = MaxCodeLength - word.length();
    return word + std::string(zerosNeeded, '0');
  }
};
