#include "Soundex.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

using Catch::Matchers::StartsWith;

class SoundexTestsFixture {
protected:
  SoundexTestsFixture() : m_soundex() {}

  int getID() { return ++uniqueID; }

protected:
  Soundex m_soundex;

private:
  static int uniqueID;
};

int SoundexTestsFixture::uniqueID = 0;

TEST_CASE_METHOD(SoundexTestsFixture, "SoundexEncoding", "[RetainsSoleLetterOfOneLetterWord]") {
  REQUIRE(m_soundex.encode("Ab") == "A100");
}

TEST_CASE_METHOD(SoundexTestsFixture, "SoundexEncoding", "[ReplacesConsonantsWithAppropriateDigits]") {
  REQUIRE(m_soundex.encode("Ax") == "A200");
}

TEST_CASE_METHOD(SoundexTestsFixture, "SoundexEncoding", "[PadsWithZerosToEnsureThreeDigits]") {
  REQUIRE(m_soundex.encode("I") == "I000");
}

TEST_CASE_METHOD(SoundexTestsFixture, "SoundexEncoding", "[IgnoreNonAlphabetics]") {
  REQUIRE(m_soundex.encode("A#") == "A000");
}

TEST_CASE_METHOD(SoundexTestsFixture, "SoundexEncoding", "[ReplacesMultipleConsonantsWithDigits]") {
  REQUIRE(m_soundex.encode("Acdl") == "A234");
}

TEST_CASE_METHOD(SoundexTestsFixture, "SoundexEncoding", "[LimitsLengthToFourCharacters]") {
  REQUIRE(m_soundex.encode("Dcdlb").length() == 4u);
}

TEST_CASE_METHOD(SoundexTestsFixture, "SoundexEncoding", "[IgnoreVowelLikeLetters]") {
  REQUIRE(m_soundex.encode("Baeiouhycdl") == "B234");
}

TEST_CASE_METHOD(SoundexTestsFixture, "SoundexEncoding", "[CombinesDuplicateEncoding]") {
  REQUIRE(m_soundex.encodeDigit('b') == m_soundex.encodeDigit('f'));
  REQUIRE(m_soundex.encodeDigit('c') == m_soundex.encodeDigit('g'));
  REQUIRE(m_soundex.encodeDigit('d') == m_soundex.encodeDigit('t'));

  REQUIRE(m_soundex.encode("Abfcgdt") == "A123");
}

TEST_CASE_METHOD(SoundexTestsFixture, "SoundexEncoding", "[UppercasesFirstLetter]") {
  REQUIRE_THAT(m_soundex.encode("abcd"), StartsWith("A"));
}

TEST_CASE_METHOD(SoundexTestsFixture, "SoundexEncoding", "[IgnoreCaseWhenEncodingConsonants]") {
  REQUIRE(m_soundex.encode("BCDL") == m_soundex.encode("Bcdl"));
}
