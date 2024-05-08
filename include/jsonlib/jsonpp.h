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
auto try_parse_number(std::string_view num) -> std::optional<T> {
    T value;
    auto res = std::from_chars(num.data(), num.data() + num.size(), value);
    if (res.ec == std::errc() && res.ptr == num.data() + num.size()) {
        return value;
    }
    return std::nullopt;
}

class JSONObject {
    // to walkaround incomplete type for unordered_map in c++20
    using SelfType = std::remove_pointer<std::unique_ptr<JSONObject>::pointer>;

public:
    constexpr JSONObject() : inner_(std::monostate{}) {}
    template <typename Arg>
    constexpr JSONObject(Arg arg) : inner_(arg){};

    static auto from_string(std::string_view const sv) -> JSONObject;
    static auto from_bytestream(std::vector<std::byte> const& bs) -> JSONObject;

    // static auto to_string() -> std::string;
    // static auto to_bytestream() -> std::vector<std::byte>;

    std::variant<std::monostate, bool, int, double, std::string,
                 std::vector<JSONObject>,
                 std::unordered_map<std::string, SelfType>>
        inner_;
};

inline auto JSONObject::from_string(std::string_view const sv) -> JSONObject {
    if (sv.empty()) {
        fmt::println(stderr, "empty state");
        return JSONObject{std::monostate{}};
    } else if (std::isdigit(sv[0]) || sv[0] == '+' || sv[0] == '-') {
        std::regex pattern(R"([-+]?(\d+(\.\d*)?|\.\d+)([eE][-+]?\d+)?)");
        std::cmatch match;
        if (!std::regex_search(sv.data(), sv.data() + sv.size(), match,
                               pattern) ||
            match.str().length() != sv.length())
            return JSONObject{std::monostate{}};
        if (match.str().find(".") == match.str().npos) {
            if (auto num = try_parse_number<int>(match.str()); num.has_value())
                return JSONObject{num.value()};
            else
                return JSONObject{std::monostate{}};
        } else {
            if (auto num = try_parse_number<double>(match.str());
                num.has_value())
                return JSONObject{num.value()};
            else
                return JSONObject{std::monostate{}};
        }
    }
    return JSONObject{std::monostate{}};
}

inline auto JSONObject::from_bytestream(std::vector<std::byte> const& bs)
    -> JSONObject {
    return JSONObject::from_string(
        std::string((char const*)(bs.data()), bs.size()));
}

} // namespace mono

#endif