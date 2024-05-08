#include <extlibs/ranges_ext.h>
#include <fmt/format.h>
#include <jsonlib/jsonpp.h>
#include <monolibs/utility.h>
#include <vector>

auto main() -> int {
    mono::JSONObject::from_string("23.");
    mono::JSONObject::from_string("23");
    mono::JSONObject::from_string("-23ae2");
    mono::JSONObject::from_string("+2.3e2");
    mono::JSONObject::from_string("+2.3.3");
}