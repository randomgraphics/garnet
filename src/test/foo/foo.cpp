#define FMT_HEADER_ONLY
#include <fmt/format.h>
#include <fmt/printf.h>
#include <fmt/xchar.h>
#include <stdio.h>

template<typename T>
struct Foo {
    template<typename... Args, std::enable_if_t<(std::is_convertible<T, char>::value), bool> = true>
    static auto foo(fmt::format_string<Args...> fmt, Args &&... args_) {
        return fmt::format(fmt, args_...);
    }

    template<typename... Args, std::enable_if_t<(std::is_convertible<T, wchar_t>::value), bool> = true>
    static auto foo(fmt::wformat_string<Args...> fmt, Args &&... args_) {
        return fmt::format(fmt, args_...);
    }
};

int main() {
    printf("%s", Foo<char>::foo("Hello, World!\n").c_str());
    printf("%S", Foo<wchar_t>::foo(L"Hello, World!\n").c_str());
    return 0;
}