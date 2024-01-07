#include "Soundex.h"
#include <catch2/catch_test_macros.hpp>

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
