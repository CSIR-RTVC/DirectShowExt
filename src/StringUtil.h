#pragma once
#include <cassert>
#include <string>

static std::string wideStringToString(const wchar_t* wszString)
{
  const unsigned max_len = 255;
  char szBuffer[max_len];
  size_t uiConverted(0);
  errno_t err = wcstombs_s(&uiConverted, szBuffer, max_len, wszString, _TRUNCATE);
  return (err == 0) ? std::string(szBuffer, uiConverted - 1) : ""; // subtract 1 for null terminator
}

static std::wstring stringToWideString(const std::string& sString)
{
  std::wstring wideString(sString.size() + 1, 0);
  size_t converted = 0;
  errno_t err = mbstowcs_s(&converted, &wideString[0], sString.length() + 1, sString.c_str(), _TRUNCATE);
  assert(err == 0);
  return std::wstring(wideString.c_str()); // removes null terminator inserted by mbstowcs_s
}

template <class T>
T convert(const std::string& sStringValue, bool& bSuccess)
{
  T val;
  std::istringstream istr(sStringValue);
  istr >> val;
  bSuccess = !istr.fail() && istr.eof();
  return val;
}
