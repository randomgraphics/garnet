// Copyright (c) 2005, Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Author: Sanjay Ghemawat

#ifndef _PCRE_REGEXP_H
#define _PCRE_REGEXP_H

// C++ interface to the pcre regular-expression library.  RE supports
// Perl-style regular expressions (with extensions like \d, \w, \s,
// ...).
//
// -----------------------------------------------------------------------
// REGEXP SYNTAX:
//
// This module is part of the pcre library and hence supports its syntax
// for regular expressions.
//
// The syntax is pretty similar to Perl's.  For those not familiar
// with Perl's regular expressions, here are some examples of the most
// commonly used extensions:
//
//   "hello (\\w+) world"  -- \w matches a "word" character
//   "version (\\d+)"      -- \d matches a digit
//   "hello\\s+world"      -- \s matches any whitespace character
//   "\\b(\\w+)\\b"        -- \b matches empty string at a word boundary
//   "(?i)hello"           -- (?i) turns on case-insensitive matching
//   "/\\*(.*?)\\*/"       -- .*? matches . minimum no. of times possible
//
// -----------------------------------------------------------------------
// MATCHING INTERFACE:
//
// The "FullMatch" operation checks that supplied text matches a
// supplied pattern exactly.
//
// Example: successful match
//    pcrecpp::RE re("h.*o");
//    re.FullMatch("hello");
//
// Example: unsuccessful match (requires full match):
//    pcrecpp::RE re("e");
//    !re.FullMatch("hello");
//
// Example: creating a temporary RE object:
//    pcrecpp::RE("h.*o").FullMatch("hello");
//
// You can pass in a "const char*" or a "string" for "text".  The
// examples below tend to use a const char*.
//
// You can, as in the different examples above, store the RE object
// explicitly in a variable or use a temporary RE object.  The
// examples below use one mode or the other arbitrarily.  Either
// could correctly be used for any of these examples.
//
// -----------------------------------------------------------------------
// MATCHING WITH SUB-STRING EXTRACTION:
//
// You can supply extra pointer arguments to extract matched subpieces.
//
// Example: extracts "ruby" into "s" and 1234 into "i"
//    int i;
//    string s;
//    pcrecpp::RE re("(\\w+):(\\d+)");
//    re.FullMatch("ruby:1234", &s, &i);
//
// Example: does not try to extract any extra sub-patterns
//    re.FullMatch("ruby:1234", &s);
//
// Example: does not try to extract into NULL
//    re.FullMatch("ruby:1234", NULL, &i);
//
// Example: integer overflow causes failure
//    !re.FullMatch("ruby:1234567891234", NULL, &i);
//
// Example: fails because there aren't enough sub-patterns:
//    !pcrecpp::RE("\\w+:\\d+").FullMatch("ruby:1234", &s);
//
// Example: fails because string cannot be stored in integer
//    !pcrecpp::RE("(.*)").FullMatch("ruby", &i);
//
// The provided pointer arguments can be pointers to any scalar numeric
// type, or one of
//    string        (matched piece is copied to string)
//    StringPiece   (StringPiece is mutated to point to matched piece)
//    T             (where "bool T::ParseFrom(const char*, int)" exists)
//    NULL          (the corresponding matched sub-pattern is not copied)
//
// -----------------------------------------------------------------------
// DO_MATCH
//
// The matching interface supports at most 16 arguments per call.
// If you need more, consider using the more general interface
// pcrecpp::RE::DoMatch().  See pcrecpp.h for the signature for DoMatch.
//
// -----------------------------------------------------------------------
// PARTIAL MATCHES
//
// You can use the "PartialMatch" operation when you want the pattern
// to match any substring of the text.
//
// Example: simple search for a string:
//    pcrecpp::RE("ell").PartialMatch("hello");
//
// Example: find first number in a string:
//    int number;
//    pcrecpp::RE re("(\\d+)");
//    re.PartialMatch("x*100 + 20", &number);
//    assert(number == 100);
//
// -----------------------------------------------------------------------
// UTF-8 AND THE MATCHING INTERFACE:
//
// By default, pattern and text are plain text, one byte per character.
// The UTF8 flag, passed to the constructor, causes both pattern
// and string to be treated as UTF-8 text, still a byte stream but
// potentially multiple bytes per character. In practice, the text
// is likelier to be UTF-8 than the pattern, but the match returned
// may depend on the UTF8 flag, so always use it when matching
// UTF8 text.  E.g., "." will match one byte normally but with UTF8
// set may match up to three bytes of a multi-byte character.
//
// Example:
//    pcrecpp::RE_Options options;
//    options.set_utf8();
//    pcrecpp::RE re(utf8_pattern, options);
//    re.FullMatch(utf8_string);
//
// Example: using the convenience function UTF8():
//    pcrecpp::RE re(utf8_pattern, pcrecpp::UTF8());
//    re.FullMatch(utf8_string);
//
// NOTE: The UTF8 option is ignored if pcre was not configured with the
//       --enable-utf8 flag.
//
// -----------------------------------------------------------------------
// SCANNING TEXT INCREMENTALLY
//
// The "Consume" operation may be useful if you want to repeatedly
// match regular expressions at the front of a string and skip over
// them as they match.  This requires use of the "StringPiece" type,
// which represents a sub-range of a real string.  Like RE, StringPiece
// is defined in the pcrecpp namespace.
//
// Example: read lines of the form "var = value" from a string.
//    string contents = ...;                 // Fill string somehow
//    pcrecpp::StringPiece input(contents);  // Wrap in a StringPiece
//
//    string var;
//    int value;
//    pcrecpp::RE re("(\\w+) = (\\d+)\n");
//    while (re.Consume(&input, &var, &value)) {
//      ...;
//    }
//
// Each successful call to "Consume" will set "var/value", and also
// advance "input" so it points past the matched text.
//
// The "FindAndConsume" operation is similar to "Consume" but does not
// anchor your match at the beginning of the string.  For example, you
// could extract all words from a string by repeatedly calling
//     pcrecpp::RE("(\\w+)").FindAndConsume(&input, &word)
//
// -----------------------------------------------------------------------
// PARSING HEX/OCTAL/C-RADIX NUMBERS
//
// By default, if you pass a pointer to a numeric value, the
// corresponding text is interpreted as a base-10 number.  You can
// instead wrap the pointer with a call to one of the operators Hex(),
// Octal(), or CRadix() to interpret the text in another base.  The
// CRadix operator interprets C-style "0" (base-8) and "0x" (base-16)
// prefixes, but defaults to base-10.
//
// Example:
//   int a, b, c, d;
//   pcrecpp::RE re("(.*) (.*) (.*) (.*)");
//   re.FullMatch("100 40 0100 0x40",
//                pcrecpp::Octal(&a), pcrecpp::Hex(&b),
//                pcrecpp::CRadix(&c), pcrecpp::CRadix(&d));
// will leave 64 in a, b, c, and d.
//
// -----------------------------------------------------------------------
// REPLACING PARTS OF STRINGS
//
// You can replace the first match of "pattern" in "str" with
// "rewrite".  Within "rewrite", backslash-escaped digits (\1 to \9)
// can be used to insert text matching corresponding parenthesized
// group from the pattern.  \0 in "rewrite" refers to the entire
// matching text.  E.g.,
//
//   string s = "yabba dabba doo";
//   pcrecpp::RE("b+").Replace("d", &s);
//
// will leave "s" containing "yada dabba doo".  The result is true if
// the pattern matches and a replacement occurs, or false otherwise.
//
// GlobalReplace() is like Replace(), except that it replaces all
// occurrences of the pattern in the string with the rewrite.
// Replacements are not subject to re-matching.  E.g.,
//
//   string s = "yabba dabba doo";
//   pcrecpp::RE("b+").GlobalReplace("d", &s);
//
// will leave "s" containing "yada dada doo".  It returns the number
// of replacements made.
//
// Extract() is like Replace(), except that if the pattern matches,
// "rewrite" is copied into "out" (an additional argument) with
// substitutions.  The non-matching portions of "text" are ignored.
// Returns true iff a match occurred and the extraction happened
// successfully.  If no match occurs, the string is left unaffected.


#include <string>
// These aren't technically needed here, but we include them
// anyway so folks who include pcrecpp.h don't have to include
// all these other header files as well.
#include <pcre.h>
#include <pcre_stringpiece.h>


#define PCRE_HAS_LONG_LONG 1
#define PCRE_HAS_ULONG_LONG 1

namespace pcrecpp {

// We convert user-passed pointers into special Arg objects
class Arg;
extern Arg no_arg;

/***** Compiling regular expressions: the RE class *****/

// RE_Options allow you to set options to be passed along to pcre,
// along with other options we put on top of pcre.  Only UTF and
// match_limit are supported now.  Setting match_limit
// to a non-zero value will limit the executation of pcre to
// keep it from doing bad things like blowing the stack or taking
// an eternity to return a result.  A value of 5000 is good enough
// to stop stack blowup in a 2MB thread stack.
// Setting match_limit to zero will disable match limiting.
class RE_Options {
 public:
  // constructor
  RE_Options() : match_limit_(0), utf8_(false) {}
  // we're fine with the default destructor, copy constructor, etc.

  // accessors and mutators
  int match_limit() const { return match_limit_; };
  void set_match_limit(int limit) {
    match_limit_ = limit;
  }

  bool utf8() const { return utf8_; }
  void set_utf8(bool u) {
    utf8_ = u;
  }

  // TODO: add other pcre flags

 private:
  int match_limit_;
  bool utf8_;
};

// These functions return some common RE_Options
static inline RE_Options UTF8() {
  RE_Options options;
  options.set_utf8(true);
  return options;
}


// Interface for regular expression matching.  Also corresponds to a
// pre-compiled regular expression.  An "RE" object is safe for
// concurrent use by multiple threads.
class RE {
 public:
  // We provide implicit conversions from strings so that users can
  // pass in a string or a "const char*" wherever an "RE" is expected.
  RE(const char* pat) { Init(pat, NULL); }
  RE(const char *pat, const RE_Options& option) { Init(pat, &option); }
  RE(const string& pat) { Init(pat.c_str(), NULL); }
  RE(const string& pat, const RE_Options& option) { Init(pat.c_str(), &option); }

  ~RE();

  // The string specification for this RE.  E.g.
  //   RE re("ab*c?d+");
  //   re.pattern();    // "ab*c?d+"
  const string& pattern() const { return pattern_; }

  // If RE could not be created properly, returns an error string.
  // Else returns the empty string.
  const string& error() const { return *error_; }

  /***** The useful part: the matching interface *****/

  // This is provided so one can do pattern.ReplaceAll() just as
  // easily as ReplaceAll(pattern-text, ....)

  bool FullMatch(const StringPiece& text,
                 const Arg& ptr1 = no_arg,
                 const Arg& ptr2 = no_arg,
                 const Arg& ptr3 = no_arg,
                 const Arg& ptr4 = no_arg,
                 const Arg& ptr5 = no_arg,
                 const Arg& ptr6 = no_arg,
                 const Arg& ptr7 = no_arg,
                 const Arg& ptr8 = no_arg,
                 const Arg& ptr9 = no_arg,
                 const Arg& ptr10 = no_arg,
                 const Arg& ptr11 = no_arg,
                 const Arg& ptr12 = no_arg,
                 const Arg& ptr13 = no_arg,
                 const Arg& ptr14 = no_arg,
                 const Arg& ptr15 = no_arg,
                 const Arg& ptr16 = no_arg) const;

  bool PartialMatch(const StringPiece& text,
                    const Arg& ptr1 = no_arg,
                    const Arg& ptr2 = no_arg,
                    const Arg& ptr3 = no_arg,
                    const Arg& ptr4 = no_arg,
                    const Arg& ptr5 = no_arg,
                    const Arg& ptr6 = no_arg,
                    const Arg& ptr7 = no_arg,
                    const Arg& ptr8 = no_arg,
                    const Arg& ptr9 = no_arg,
                    const Arg& ptr10 = no_arg,
                    const Arg& ptr11 = no_arg,
                    const Arg& ptr12 = no_arg,
                    const Arg& ptr13 = no_arg,
                    const Arg& ptr14 = no_arg,
                    const Arg& ptr15 = no_arg,
                    const Arg& ptr16 = no_arg) const;

  bool Consume(StringPiece* input,
               const Arg& ptr1 = no_arg,
               const Arg& ptr2 = no_arg,
               const Arg& ptr3 = no_arg,
               const Arg& ptr4 = no_arg,
               const Arg& ptr5 = no_arg,
               const Arg& ptr6 = no_arg,
               const Arg& ptr7 = no_arg,
               const Arg& ptr8 = no_arg,
               const Arg& ptr9 = no_arg,
               const Arg& ptr10 = no_arg,
               const Arg& ptr11 = no_arg,
               const Arg& ptr12 = no_arg,
               const Arg& ptr13 = no_arg,
               const Arg& ptr14 = no_arg,
               const Arg& ptr15 = no_arg,
               const Arg& ptr16 = no_arg) const;

  bool FindAndConsume(StringPiece* input,
                      const Arg& ptr1 = no_arg,
                      const Arg& ptr2 = no_arg,
                      const Arg& ptr3 = no_arg,
                      const Arg& ptr4 = no_arg,
                      const Arg& ptr5 = no_arg,
                      const Arg& ptr6 = no_arg,
                      const Arg& ptr7 = no_arg,
                      const Arg& ptr8 = no_arg,
                      const Arg& ptr9 = no_arg,
                      const Arg& ptr10 = no_arg,
                      const Arg& ptr11 = no_arg,
                      const Arg& ptr12 = no_arg,
                      const Arg& ptr13 = no_arg,
                      const Arg& ptr14 = no_arg,
                      const Arg& ptr15 = no_arg,
                      const Arg& ptr16 = no_arg) const;

  bool Replace(const StringPiece& rewrite,
               string *str) const;

  int GlobalReplace(const StringPiece& rewrite,
                    string *str) const;

  bool Extract(const StringPiece &rewrite,
               const StringPiece &text,
               string *out) const;

  /***** Generic matching interface *****/

  // Type of match (TODO: Should be restructured as part of RE_Options)
  enum Anchor {
    UNANCHORED,         // No anchoring
    ANCHOR_START,       // Anchor at start only
    ANCHOR_BOTH         // Anchor at start and end
  };

  // General matching routine.  Stores the length of the match in
  // "*consumed" if successful.
  bool DoMatch(const StringPiece& text,
               Anchor anchor,
               int* consumed,
               const Arg* const* args, int n) const;

  // Return the number of capturing subpatterns, or -1 if the
  // regexp wasn't valid on construction.
  int NumberOfCapturingGroups();

 private:

  void Init(const char* pattern, const RE_Options* options);

  // Match against "text", filling in "vec" (up to "vecsize" * 2/3) with
  // pairs of integers for the beginning and end positions of matched
  // text.  The first pair corresponds to the entire matched text;
  // subsequent pairs correspond, in order, to parentheses-captured
  // matches.  Returns the number of pairs (one more than the number of
  // the last subpattern with a match) if matching was successful
  // and zero if the match failed.
  // I.e. for RE("(foo)|(bar)|(baz)") it will return 2, 3, and 4 when matching
  // against "foo", "bar", and "baz" respectively.
  // When matching RE("(foo)|hello") against "hello", it will return 1.
  // But the values for all subpattern are filled in into "vec".
  int TryMatch(const StringPiece& text,
               int startpos,
               Anchor anchor,
               int *vec,
               int vecsize) const;

  // Append the "rewrite" string, with backslash subsitutions from "text"
  // and "vec", to string "out".
  bool Rewrite(string *out,
               const StringPiece& rewrite,
               const StringPiece& text,
               int *vec,
               int veclen) const;

  // internal implementation for DoMatch
  bool DoMatchImpl(const StringPiece& text,
                   Anchor anchor,
                   int* consumed,
                   const Arg* const args[],
                   int n,
                   int* vec,
                   int vecsize) const;

  // Compile the regexp for the specified anchoring mode
  pcre* Compile(Anchor anchor);

  string        pattern_;
  RE_Options    options_;
  pcre*         re_full_;       // For full matches
  pcre*         re_partial_;    // For partial matches
  const string* error_;         // Error indicator (or points to empty string)
  int           match_limit_;   // limit on execution resources

  // Don't allow the default copy or assignment constructors --
  // they're expensive and too easy to do by accident.
  RE(const RE&);
  void operator=(const RE&);
};


/***** Implementation details *****/

// Hex/Octal/Binary?

// Special class for parsing into objects that define a ParseFrom() method
template <class T>
class _RE_MatchObject {
 public:
  static inline bool Parse(const char* str, int n, void* dest) {
    T* object = reinterpret_cast<T*>(dest);
    return object->ParseFrom(str, n);
  }
};

class Arg {
 public:
  // Empty constructor so we can declare arrays of Arg
  Arg();

  // Constructor specially designed for NULL arguments
  Arg(void*);

  typedef bool (*Parser)(const char* str, int n, void* dest);

// Type-specific parsers
#define PCRE_MAKE_PARSER(type,name)                             \
  Arg(type* p) : arg_(p), parser_(name) { }                     \
  Arg(type* p, Parser parser) : arg_(p), parser_(parser) { }


  PCRE_MAKE_PARSER(char,               parse_char);
  PCRE_MAKE_PARSER(unsigned char,      parse_uchar);
  PCRE_MAKE_PARSER(short,              parse_short);
  PCRE_MAKE_PARSER(unsigned short,     parse_ushort);
  PCRE_MAKE_PARSER(int,                parse_int);
  PCRE_MAKE_PARSER(unsigned int,       parse_uint);
  PCRE_MAKE_PARSER(long,               parse_long);
  PCRE_MAKE_PARSER(unsigned long,      parse_ulong);
#if PCRE_HAS_LONG_LONG
  PCRE_MAKE_PARSER(long long,          parse_longlong);
#endif
#if PCRE_HAS_ULONG_LONG
  PCRE_MAKE_PARSER(unsigned long long, parse_ulonglong);
#endif
  PCRE_MAKE_PARSER(float,              parse_float);
  PCRE_MAKE_PARSER(double,             parse_double);
  PCRE_MAKE_PARSER(string,             parse_string);
  PCRE_MAKE_PARSER(StringPiece,        parse_stringpiece);

#undef PCRE_MAKE_PARSER

  // Generic constructor
  template <class T> Arg(T*, Parser parser);
  // Generic constructor template
  template <class T> Arg(T* p)
    : arg_(p), parser_(_RE_MatchObject<T>::Parse) {
  }

  // Parse the data
  bool Parse(const char* str, int n) const;

 private:
  void*         arg_;
  Parser        parser_;

  static bool parse_null          (const char* str, int n, void* dest);
  static bool parse_char          (const char* str, int n, void* dest);
  static bool parse_uchar         (const char* str, int n, void* dest);
  static bool parse_float         (const char* str, int n, void* dest);
  static bool parse_double        (const char* str, int n, void* dest);
  static bool parse_string        (const char* str, int n, void* dest);
  static bool parse_stringpiece   (const char* str, int n, void* dest);

#define PCRE_DECLARE_INTEGER_PARSER(name)                                   \
 private:                                                                   \
  static bool parse_ ## name(const char* str, int n, void* dest);           \
  static bool parse_ ## name ## _radix(                                     \
    const char* str, int n, void* dest, int radix);                         \
 public:                                                                    \
  static bool parse_ ## name ## _hex(const char* str, int n, void* dest);   \
  static bool parse_ ## name ## _octal(const char* str, int n, void* dest); \
  static bool parse_ ## name ## _cradix(const char* str, int n, void* dest)

  PCRE_DECLARE_INTEGER_PARSER(short);
  PCRE_DECLARE_INTEGER_PARSER(ushort);
  PCRE_DECLARE_INTEGER_PARSER(int);
  PCRE_DECLARE_INTEGER_PARSER(uint);
  PCRE_DECLARE_INTEGER_PARSER(long);
  PCRE_DECLARE_INTEGER_PARSER(ulong);
  PCRE_DECLARE_INTEGER_PARSER(longlong);
  PCRE_DECLARE_INTEGER_PARSER(ulonglong);

#undef PCRE_DECLARE_INTEGER_PARSER
};

inline Arg::Arg() : arg_(NULL), parser_(parse_null) { }
inline Arg::Arg(void* p) : arg_(p), parser_(parse_null) { }

inline bool Arg::Parse(const char* str, int n) const {
  return (*parser_)(str, n, arg_);
}

// This part of the parser, appropriate only for ints, deals with bases
#define MAKE_INTEGER_PARSER(type, name) \
  inline Arg Hex(type* ptr) { \
    return Arg(ptr, Arg::parse_ ## name ## _hex); } \
  inline Arg Octal(type* ptr) { \
    return Arg(ptr, Arg::parse_ ## name ## _octal); } \
  inline Arg CRadix(type* ptr) { \
    return Arg(ptr, Arg::parse_ ## name ## _cradix); }

MAKE_INTEGER_PARSER(short,              short);
MAKE_INTEGER_PARSER(unsigned short,     ushort);
MAKE_INTEGER_PARSER(int,                int);
MAKE_INTEGER_PARSER(unsigned int,       uint);
MAKE_INTEGER_PARSER(long,               long);
MAKE_INTEGER_PARSER(unsigned long,      ulong);
#if PCRE_HAS_LONG_LONG
MAKE_INTEGER_PARSER(long long,          longlong);
#endif
#if PCRE_HAS_ULONG_LONG
MAKE_INTEGER_PARSER(unsigned long long, ulonglong);
#endif

#undef MAKE_INTEGER_PARSER

}   // namespace pcrecpp

#endif /* _PCRE_REGEXP_H */
