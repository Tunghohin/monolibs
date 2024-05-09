#ifndef _JSONPP_H
#define _JSONPP_H

#include <charconv>
#include <fmt/format.h>
#include <numeric>
#include <optional>
#include <regex>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace mono {

template <typename T>
    requires std::is_arithmetic_v<T>
inline constexpr auto try_parse_number(std::string_view const num) -> std::optional<T> {
    T value;
    auto res = std::from_chars(num.data(), num.data() + num.size(), value);
    if (res.ec == std::errc() && res.ptr == num.data() + num.size()) {
        return value;
    }
    return std::nullopt;
}

inline constexpr auto unescaped_char(char c) -> char {
    switch (c) {
    case 'n': return '\n';
    case 'r': return '\r';
    case '0': return '\0';
    case 't': return '\t';
    case 'v': return '\v';
    case 'f': return '\f';
    case 'b': return '\b';
    case 'a': return '\a';
    default: return c;
    }
}

inline constexpr auto try_parse_string_with_escape(std::string_view const str) -> std::optional<std::string> {
    enum {Raw, Escaped} status = Raw;
    std::string res{};
    for (auto i = 1; i < str.size(); i++) {
        char c = str[i];
        if (status == Raw) {
            if (c == '\\') {
                status = Escaped;
                continue;
            } else if (c == '\"') {
                return std::move(res);
            } else {
                res += c;
            }
        } else {
            res += unescaped_char(c);
            status = Raw;
        }
    }
    return std::nullopt; 
} 

class JSONObject {
    // to walkaround incomplete type for unordered_map in c++20
    using SelfType = std::remove_pointer<std::unique_ptr<JSONObject>::pointer>;

public:
    // constexpr JSONObject() : inner_(std::monostate{}) {}
    // template <typename Arg>
    // constexpr JSONObject(Arg&& arg) : inner_(std::forward<Arg>(arg)){};

    static auto from_string(std::string_view const sv) -> JSONObject;
    static auto from_bytestream(std::vector<std::byte> const& bs) -> JSONObject;
    static auto try_from_string(std::string_view const sv) -> std::optional<JSONObject>;
    static auto try_from_bytestream(std::vector<std::byte> const& bs) -> std::optional<JSONObject>;

    // static auto to_string() -> std::string;
    // static auto to_bytestream() -> std::vector<std::byte>;

    std::variant<std::monostate, bool, int64_t, double_t, std::string,
                 std::vector<JSONObject>,
                 std::unordered_map<std::string, SelfType>>
        inner_;
};

inline auto json_parser(std::string_view const sv) -> std::pair<JSONObject, size_t> {
    if (sv.empty()) {
        fmt::println(stderr, "empty state");
        return {JSONObject{std::monostate{}}, 0};
    } else if (std::isdigit(sv[0]) || sv[0] == '+' || sv[0] == '-') {
        std::regex pattern(R"([-+]?(\d+(\.\d*)?|\.\d+)?([eE][-+]?\d+)?)");
        std::cmatch match;

        // invalid pattern
        if (!std::regex_search(sv.data(), sv.data() + sv.size(), match,
                               pattern))
            return {JSONObject{std::monostate{}}, 0};

        if (std::none_of(match.str().data(), 
        match.str().data() + match.str().size(), 
        [](char const& c) {return c == 'e' || c == '.' || c =='E';})) {
            // integer
            if (auto num = try_parse_number<int64_t>(match.str()); num.has_value()) {
                return {JSONObject{num.value()}, match.str().length()}; 
            }
            else {
                return {JSONObject{std::monostate{}}, 0}; 
            }
        } else {
            // float
            if (auto num = try_parse_number<double_t>(match.str()); num.has_value()) {
                return {JSONObject{num.value()}, match.str().length()}; 
            }
            else {
                return {JSONObject{std::monostate{}}, 0}; 
            }
        }
    } else if (*sv.begin() == '"') {
        if (auto res = try_parse_string_with_escape(sv); res.has_value()) {
            return {JSONObject{res.value()}, res.value().length() + 2}; // +2 for quotes
        } 
        return {JSONObject{std::monostate{}}, 0};
    } else if (*sv.begin() == '[') {
        std::vector<JSONObject> res;

        size_t i;
        for (i = 1; i < sv.size();) {
            if (sv[i] == ']') {
                i += 1;
                break;
            }
            auto [obj, consumed] = json_parser(sv.substr(i));
            if (consumed == 0) {
                if (sv[i] != ']') {
                    return {JSONObject{std::monostate{}}, 0};
                }
                i = 0;
                break;
            }
            res.push_back(std::move(obj));
            i += consumed;
            if (sv[i] == ',') {
                while (sv[i + 1] == ' ') {
                    i++;
                }
                i++;
            }
        }

        return {JSONObject{std::move(res)}, i};
    }
    return {JSONObject{std::monostate{}}, 0};
}

inline auto JSONObject::from_string(std::string_view const sv) -> JSONObject {
    return sgimg::json_parser(sv).first;
}

inline auto JSONObject::from_bytestream(std::vector<std::byte> const& bs)
    -> JSONObject {
    return JSONObject::from_string(
        std::string((char const*)(bs.data()), bs.size()));
}

} // namespace mono

#endif
