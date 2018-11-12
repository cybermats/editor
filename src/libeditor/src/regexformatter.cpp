//
// Created by mats on 11/11/18.
//

#include "formatter/regexformatter.h"
#include "style/stylefactory.h"

void RegexFormatter::format_line(StyleList &styleList, unsigned long, const char *str) {
  auto len = strlen(str);
  for (auto regex_item: m_regexs) {
    auto regex = regex_item.second;
    auto name = regex_item.first;
    auto begin = std::cregex_iterator(str, str + len, regex);
    auto end = std::cregex_iterator();

    for (auto it = begin; it != end; ++it) {
      std::cmatch match = *it;
      auto start = match.position();
      auto finish = start + match.length();
      m_styleMap.populate(styleList, name, start, finish);
    }
  }
}

RegexFormatter::RegexFormatter() : Formatter() {

  std::regex datatypes = from_keywords("ATOM BOOL BOOLEAN BYTE CHAR COLORREF DWORD DWORDLONG DWORD_PTR "
                                       "DWORD32 DWORD64 FLOAT HACCEL HALF_PTR HANDLE HBITMAP HBRUSH "
                                       "HCOLORSPACE HCONV HCONVLIST HCURSOR HDC HDDEDATA HDESK HDROP HDWP "
                                       "HENHMETAFILE HFILE HFONT HGDIOBJ HGLOBAL HHOOK HICON HINSTANCE HKEY "
                                       "HKL HLOCAL HMENU HMETAFILE HMODULE HMONITOR HPALETTE HPEN HRESULT "
                                       "HRGN HRSRC HSZ HWINSTA HWND INT INT_PTR INT32 INT64 LANGID LCID LCTYPE "
                                       "LGRPID LONG LONGLONG LONG_PTR LONG32 LONG64 LPARAM LPBOOL LPBYTE LPCOLORREF "
                                       "LPCSTR LPCTSTR LPCVOID LPCWSTR LPDWORD LPHANDLE LPINT LPLONG LPSTR LPTSTR "
                                       "LPVOID LPWORD LPWSTR LRESULT PBOOL PBOOLEAN PBYTE PCHAR PCSTR PCTSTR PCWSTR "
                                       "PDWORDLONG PDWORD_PTR PDWORD32 PDWORD64 PFLOAT PHALF_PTR PHANDLE PHKEY PINT "
                                       "PINT_PTR PINT32 PINT64 PLCID PLONG PLONGLONG PLONG_PTR PLONG32 PLONG64 POINTER_32 "
                                       "POINTER_64 PSHORT PSIZE_T PSSIZE_T PSTR PTBYTE PTCHAR PTSTR PUCHAR PUHALF_PTR "
                                       "PUINT PUINT_PTR PUINT32 PUINT64 PULONG PULONGLONG PULONG_PTR PULONG32 PULONG64 "
                                       "PUSHORT PVOID PWCHAR PWORD PWSTR SC_HANDLE SC_LOCK SERVICE_STATUS_HANDLE SHORT "
                                       "SIZE_T SSIZE_T TBYTE TCHAR UCHAR UHALF_PTR UINT UINT_PTR UINT32 UINT64 ULONG "
                                       "ULONGLONG ULONG_PTR ULONG32 ULONG64 USHORT USN VOID WCHAR WORD WPARAM WPARAM WPARAM "
                                       "char char16_t char32_t bool short int __int32 __int64 __int8 __int16 long float double __wchar_t "
                                       "clock_t _complex _dev_t _diskfree_t div_t ldiv_t _exception _EXCEPTION_POINTERS "
                                       "FILE _finddata_t _finddatai64_t _wfinddata_t _wfinddatai64_t __finddata64_t "
                                       "__wfinddata64_t _FPIEEE_RECORD fpos_t _HEAPINFO _HFILE lconv intptr_t "
                                       "jmp_buf mbstate_t _off_t _onexit_t _PNH ptrdiff_t _purecall_handler "
                                       "sig_atomic_t size_t _stat __stat64 _stati64 terminate_function "
                                       "time_t __time64_t _timeb __timeb64 tm uintptr_t _utimbuf "
                                       "va_list wchar_t wctrans_t wctype_t wint_t signed");

  std::regex keywords = from_keywords(
          "alignas alignof auto break case catch class const constexpr decltype __finally __exception __try "
          "const_cast continue private public protected __declspec "
          "default delete deprecated dllexport dllimport do dynamic_cast "
          "else enum explicit extern if for friend goto inline "
          "mutable naked namespace new noinline noreturn nothrow noexcept nullptr "
          "register reinterpret_cast return selectany "
          "sizeof static static_cast static_assert struct switch template this "
          "thread thread_local throw true false try typedef typeid typename union "
          "using uuid virtual void volatile whcar_t while");


  m_regexs.emplace_back("comments", std::regex("\\/\\/.*$", std::regex_constants::ECMAScript));
  m_regexs.emplace_back("string", std::regex(R"("([^\\"\n]|\\.)*")", std::regex_constants::ECMAScript));
  m_regexs.emplace_back("string", std::regex(R"('([^\\'\n]|\\.)*')", std::regex_constants::ECMAScript));
  m_regexs.emplace_back("keyword", keywords);
  m_regexs.emplace_back("preprocessor", std::regex("^ *#.*", std::regex_constants::ECMAScript));
  m_regexs.emplace_back("datatypes", datatypes);
//  m_regexs.push_back(std::make_pair("variable", std::regex("")));
//  m_regexs.push_back(std::make_pair("value", std::regex("")));
//  m_regexs.push_back(std::make_pair("functions", std::regex("")));


  m_styleMap.add("plain",
                 StyleFactory::build().addForeground(0, 130, 0));
  m_styleMap.add("comments",
                 StyleFactory::build().addForeground(0, 130, 0));
  m_styleMap.add("string",
                 StyleFactory::build().addForeground(0, 0, 255));
  m_styleMap.add("keyword",
                 StyleFactory::build().addForeground(0, 102, 153));
  m_styleMap.add("preprocessor",
                 StyleFactory::build().addForeground(128, 128, 128));
  m_styleMap.add("datatypes",
                 StyleFactory::build().addForeground(128, 128, 128).addBold());
  m_styleMap.add("variable",
                 StyleFactory::build().addForeground(170, 119, 0));
  m_styleMap.add("value",
                 StyleFactory::build().addForeground(0, 153, 0));
  m_styleMap.add("functions",
                 StyleFactory::build().addForeground(255, 20, 147));
}
