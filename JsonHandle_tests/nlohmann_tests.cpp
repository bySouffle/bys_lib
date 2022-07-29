//
// Created by bys on 2022/7/15.
//

#include "nlohmann/json.hpp"
#include "gtest/gtest.h"

using json = nlohmann::json;

//! 生成json
TEST(json_gen, test) {
  {
    // create a JSON object
    json j =
        {
            {"pi", 3.141},
            {"happy", true},
            {"name", "Niels"},
            {"nothing", nullptr},
            {
                "answer", {
                {"everything", 42}
            }
            },
            {"list", {1, 0, 2}},
            {
                "object", {
                {"currency", "USD"},
                {"value", 42.99}
            }
            }
        };

    // add new values
    j["new"]["key"]["value"] = {"another", "list"};

    // count elements
    auto s = j.size();
    j["size"] = s;

    // pretty print with indent of 4 spaces
    std::cout << std::setw(4) << j << '\n';
  }
}

namespace ns {
enum TaskState {
  TS_STOPPED,
  TS_RUNNING,
  TS_COMPLETED,
  TS_INVALID = -1
};

NLOHMANN_JSON_SERIALIZE_ENUM(TaskState,
                             {
                               { TS_INVALID, nullptr },
                               { TS_STOPPED, "stopped" },
                               { TS_RUNNING, "running" },
                               { TS_COMPLETED, "completed" }
                             })

enum class Color {
  red, green, blue, unknown
};

NLOHMANN_JSON_SERIALIZE_ENUM(Color,
                             {
                               { Color::unknown, "unknown" }, { Color::red, "red" },
                               { Color::green, "green" }, { Color::blue, "blue" }
                             })
} // namespace ns


TEST(json_serialize_enum, test) {
  // serialization
  json j_stopped = ns::TS_STOPPED;
  json j_red = ns::Color::red;
  std::cout << "ns::TS_STOPPED -> " << j_stopped
            << ", ns::Color::red -> " << j_red << std::endl;

  // deserialization
  json j_running = "running";
  json j_blue = "blue";
  auto running = j_running.get<ns::TaskState>();
  auto blue = j_blue.get<ns::Color>();
  std::cout << j_running << " -> " << running
            << ", " << j_blue << " -> " << static_cast<int>(blue) << std::endl;

  // deserializing undefined JSON value to enum
  // (where the first map entry above is the default)
  json j_pi = 3.14;
  auto invalid = j_pi.get<ns::TaskState>();
  auto unknown = j_pi.get<ns::Color>();
  std::cout << j_pi << " -> " << invalid << ", "
            << j_pi << " -> " << static_cast<int>(unknown) << std::endl;
}

TEST(json_serialize_enum2, test) {
  // serialization
  json j_red = ns::Color::red;
  std::cout << static_cast<int>(ns::Color::red) << " -> " << j_red << std::endl;

  // deserialization
  json j_rot = "rot";
  auto rot = j_rot.get<ns::Color>();
  auto red = j_red.get<ns::Color>();
  std::cout << j_rot << " -> " << static_cast<int>(rot) << std::endl;
  std::cout << j_red << " -> " << static_cast<int>(red) << std::endl;
}

TEST(valid_json, test) {
// a valid JSON text
  auto valid_text = R"(
    {
        "numbers": [1, 2, 3]
    }
    )";

// an invalid JSON text
  auto invalid_text = R"(
    {
        "strings": ["extra", "comma", ]
    }
    )";

  std::cout << std::boolalpha
            << json::accept(valid_text) << ' '
            << json::accept(invalid_text) << '\n';
}

//! std::vector<json> & json::array_t
TEST(array, cp_vector_array_t) {
  std::cout << std::boolalpha << std::is_same<std::vector<json>, json::array_t>::value << std::endl;
}

//! array init
TEST(array, array_init) {
  // create JSON arrays
  json j_no_init_list = json::array();
  json j_empty_init_list = json::array({});
  json j_nonempty_init_list = json::array({1, 2, 3, 4});
  json j_list_of_pairs = json::array({{"one", 1}, {"two", 2}});

  // serialize the JSON arrays
  std::cout << j_no_init_list << '\n';
  std::cout << j_empty_init_list << '\n';
  std::cout << j_nonempty_init_list << '\n';
  std::cout << j_list_of_pairs << '\n';
}

//! at 使用方法以及越界异常捕获
TEST(at, at_const) {
  // create JSON object
  const json object =
      {
          {"the good", "il buono"},
          {"the bad", "il cattivo"},
          {"the ugly", "il brutto"}
      };

  // output element with key "the ugly".5.5
  std::cout << object.at("the ugly") << '\n';


  // exception type_error.304
  try {
    // use at() on a non-object type
    const json str = "I am a string";
    std::cout << str.at("the good") << '\n';
  }
  catch (json::type_error &e) {
    std::cout << e.what() << '\n';
  }

  // exception out_of_range.401
  try {
    // try to read from a nonexisting key
    std::cout << object.at("the fast") << '\n';
  }
  catch (json::out_of_range) {
    std::cout << "out of range" << '\n';
  }
}

//! at 非const使用
TEST(at, at_) {
  // create JSON object
  json object =
      {
          {"the good", "il buono"},
          {"the bad", "il cattivo"},
          {"the ugly", "il brutto"}
      };

  // output element with key "the ugly"
  std::cout << object.at("the ugly") << '\n';

  // change element with key "the bad"
  object.at("the bad") = "il";

  // output changed array
  std::cout << object << '\n';


  // exception type_error.304
  try {
    // use at() on a non-object type
    json str = "I am a string";
    str.at("the good") = "Another string";
  }
  catch (json::type_error &e) {
    std::cout << e.what() << '\n';
  }

  // exception out_of_range.401
  try {
    // try to write at a nonexisting key
    object.at("the fast") = "il rapido";
  }
  catch (json::out_of_range &e) {
    std::cout << e.what() << '\n';
  }
}

//! const at index 使用
TEST(at, const_at_index) {
  // create JSON array
  const json array = {"first", "2nd", "third", "fourth"};

  // output element at index 2 (third element)
  std::cout << array.at(2) << '\n';


  // exception type_error.304
  try {
    // use at() on a non-array type
    const json str = "I am a string";
    std::cout << str.at(0) << '\n';
  }
  catch (json::type_error &e) {
    std::cout << e.what() << '\n';
  }

  // exception out_of_range.401
  try {
    // try to read beyond the array limit
    std::cout << array.at(5) << '\n';
  }
  catch (json::out_of_range &e) {
    std::cout << e.what() << '\n';
  }
}

TEST(at, at_index) {
  // create JSON array
  json array = {"first", "2nd", "third", "fourth"};

  // output element at index 2 (third element)
  std::cout << array.at(2) << '\n';

  // change element at index 1 (second element) to "second"
  array.at(1) = "second";

  // output changed array
  std::cout << array << '\n';


  // exception type_error.304
  try {
    // use at() on a non-array type
    json str = "I am a string";
    str.at(0) = "Another string";
  }
  catch (json::type_error &e) {
    std::cout << e.what() << '\n';
  }

  // exception out_of_range.401
  try {
    // try to write beyond the array limit
    array.at(5) = "sixth";
  }
  catch (json::out_of_range &e) {
    std::cout << e.what() << '\n';
  }
}

TEST(at, const_pointer_at) {
  // create a JSON value
  const json j =
      {
          {"number", 1}, {"string", "foo"}, {"array", {1, 2}}
      };

  // read-only access

  // output element with JSON pointer "/number"
  std::cout << j.at("/number"_json_pointer) << '\n';
  // output element with JSON pointer "/string"
  std::cout << j.at("/string"_json_pointer) << '\n';
  // output element with JSON pointer "/array"
  std::cout << j.at("/array"_json_pointer) << '\n';
  // output element with JSON pointer "/array/1"
  std::cout << j.at("/array/1"_json_pointer) << '\n';

  // out_of_range.109
  try {
    // try to use an array index that is not a number
    json::const_reference ref = j.at("/array/one"_json_pointer);
  }
  catch (json::parse_error &e) {
    std::cout << e.what() << '\n';
  }

  // out_of_range.401
  try {
    // try to use an invalid array index
    json::const_reference ref = j.at("/array/4"_json_pointer);
  }
  catch (json::out_of_range &e) {
    std::cout << e.what() << '\n';
  }

  // out_of_range.402
  try {
    // try to use the array index '-'
    json::const_reference ref = j.at("/array/-"_json_pointer);
  }
  catch (json::out_of_range &e) {
    std::cout << e.what() << '\n';
  }

  // out_of_range.403
  try {
    // try to use a JSON pointer to a nonexistent object key
    json::const_reference ref = j.at("/foo"_json_pointer);
  }
  catch (json::out_of_range &e) {
    std::cout << e.what() << '\n';
  }

  // out_of_range.404
  try {
    // try to use a JSON pointer that cannot be resolved
    json::const_reference ref = j.at("/number/foo"_json_pointer);
  }
  catch (json::out_of_range &e) {
    std::cout << e.what() << '\n';
  }
}

//! back 返回最后一个元素
TEST(back, test) {
  // create JSON values
  json j_boolean = true;
  json j_number_integer = 17;
  json j_number_float = 23.42;
  json j_object = {{"one", 1}, {"two", 2}};
  json j_object_empty(json::value_t::object);
  json j_array = {1, 2, 4, 8, 16};
  json j_array_empty(json::value_t::array);
  json j_string = "Hello, world";

  // call back()
  std::cout << j_boolean.back() << '\n';
  std::cout << j_number_integer.back() << '\n';
  std::cout << j_number_float.back() << '\n';
  std::cout << j_object.back() << '\n';
  //std::cout << j_object_empty.back() << '\n';  // undefined behavior
  std::cout << j_array.back() << '\n';
  //std::cout << j_array_empty.back() << '\n';   // undefined behavior
  std::cout << j_string.back() << '\n';

  // back() called on a null value
  try {
    json j_null;
    j_null.back();
  }
  catch (json::invalid_iterator &e) {
    std::cout << e.what() << '\n';
  }

}

/////////////////////////////////// basic   json    //////////////////////////////////////
TEST(basic_json, copy_construct) {
  // create a JSON array
  json j1 = {"one", "two", 3, 4.5, false};

  // create a copy
  json j2(j1);

  // serialize the JSON array
  std::cout << j1 << " = " << j2 << '\n';
  std::cout << std::boolalpha << (j1 == j2) << '\n';
}

//! 对象类型    直接初始化   map初始化
TEST(basic_json, object_type) {
  // create an object from an object_t value
  json::object_t object_value = {{"one", 1}, {"two", 2}};
  json j_object_t(object_value);
  std::cout << j_object_t << '\n';  //  {"one":1,"two":2}

  // create an object from std::map
  std::map<std::string, int> c_map
      {
          {"one", 1}, {"two", 2}, {"three", 3}
      };
  json j_map(c_map);
  std::cout << j_map << '\n';  //  {"one":1,"three":3,"two":2}

  // create an object from std::unordered_map
  std::unordered_map<const char *, double> c_umap
      {
          {"one", 1.2}, {"two", 2.3}, {"three", 3.4}
      };
  json j_umap(c_umap);
  std::cout << j_umap << '\n';  //  {"one":1.2,"three":3.4,"two":2.3}


  // create an object from std::multimap
  std::multimap<std::string, bool> c_mmap
      {
          {"one", true}, {"two", true}, {"three", false}, {"three", true}
      };
  json j_mmap(c_mmap); // only one entry for key "three" is used
  std::cout << j_mmap << '\n';  //  {"one":true,"three":false,"two":true}


  // create an object from std::unordered_multimap
  std::unordered_multimap<std::string, bool> c_ummap
      {
          {"one", true}, {"two", true}, {"three", false}, {"three", true}
      };
  json j_ummap(c_ummap); // only one entry for key "three" is used
  std::cout << j_ummap << "\n\n"; //  {"one":true,"three":false,"two":true}

}

#include <deque>
#include <list>
#include <unordered_set>
#include <deque>
#include <deque>
#include <deque>
#include <deque>
#include <deque>
//!  数组类型  直接初始化 vector  valarray  deque list  forward_list  array  set
TEST(basic_json, array_type) {
  // create an array from an array_t value
  json::array_t array_value = {"one", "two", 3, 4.5, false};
  json j_array_t(array_value);
  std::cout << j_array_t << "\n";   //  ["one","two",3,4.5,false]

  // create an array from std::vector
  std::vector<int> c_vector{1, 2, 3, 4};
  json j_vec(c_vector);
  std::cout << j_vec << "\n";   //  [1,2,3,4]

  // create an array from std::valarray
  std::valarray<short> c_valarray{10, 9, 8, 7};
  json j_valarray(c_valarray);
  std::cout << j_valarray << "\n";    //  [10,9,8,7]

  // create an array from std::deque
  std::deque<double> c_deque{1.2, 2.3, 3.4, 5.6};
  json j_deque(c_deque);
  std::cout << j_deque << "\n";   //  [1.2,2.3,3.4,5.6]

  // create an array from std::list
  std::list<bool> c_list{true, true, false, true};
  json j_list(c_list);
  std::cout << j_list << "\n";    //  [true,true,false,true]

  // create an array from std::forward_list
  std::forward_list<std::int64_t> c_flist{12345678909876, 23456789098765, 34567890987654, 45678909876543};
  json j_flist(c_flist);
  std::cout << j_flist << "\n";   //  [12345678909876,23456789098765,34567890987654,45678909876543]

  // create an array from std::array
  std::array<unsigned long, 4> c_array{{1, 2, 3, 4}};
  json j_array(c_array);
  std::cout << j_array << "\n";   //  [1,2,3,4]

  // create an array from std::set
  std::set<std::string> c_set{"one", "two", "three", "four", "one"};
  json j_set(c_set); // only one entry for "one" is used
  std::cout << j_set << "\n";   //  ["four","one","three","two"]

  // create an array from std::unordered_set
  std::unordered_set<std::string> c_uset{"one", "two", "three", "four", "one"};
  json j_uset(c_uset); // only one entry for "one" is used
  std::cout << j_uset << "\n";    //  ["four","three","two","one"]

  // create an array from std::multiset
  std::multiset<std::string> c_mset{"one", "two", "one", "four"};
  json j_mset(c_mset); // both entries for "one" are used
  std::cout << j_mset << "\n";    //  ["four","one","one","two"]

  // create an array from std::unordered_multiset
  std::unordered_multiset<std::string> c_umset{"one", "two", "one", "four"};
  json j_umset(c_umset); // both entries for "one" are used
  std::cout << j_umset << "\n";   //  ["four","two","one","one"]

}

//!  string 类型
TEST(base_json, string) {
  // create string from a string_t value
  json::string_t string_value = "The quick brown fox jumps over the lazy dog.";
  json j_string_t(string_value);

  // create a JSON string directly from a string literal
  json j_string_literal("The quick brown fox jumps over the lazy dog.");

  // create string from std::string
  std::string s_stdstring = "The quick brown fox jumps over the lazy dog.";
  json j_stdstring(s_stdstring);

  // serialize the JSON strings
  std::cout << j_string_t << '\n';    //  "The quick brown fox jumps over the lazy dog."
  std::cout << j_string_literal << '\n';    //  "The quick brown fox jumps over the lazy dog."
  std::cout << j_stdstring << "\n\n";   //    std::cout << j_stdstring << "\n\n";
}

//! number 类型
TEST(base_json, number) {
  // create a JSON number from number_integer_t
  json::number_integer_t value_integer_t = -42;
  json j_integer_t(value_integer_t);
  std::cout << j_integer_t << '\n';   //  -42

  // create a JSON number from number_unsigned_t
  json::number_integer_t value_unsigned_t = 17;
  json j_unsigned_t(value_unsigned_t);
  std::cout << j_unsigned_t << '\n';    //  17

  // create a JSON number from an anonymous enum
  enum { enum_value = 17 };
  json j_enum(enum_value);
  std::cout << j_enum << '\n';    //  17

  // create values of different integer types
  short n_short = 42;
  int n_int = -23;
  long n_long = 1024;
  int_least32_t n_int_least32_t = -17;
  uint8_t n_uint8_t = 8;

  // create (integer) JSON numbers
  json j_short(n_short);
  json j_int(n_int);
  json j_long(n_long);
  json j_int_least32_t(n_int_least32_t);
  json j_uint8_t(n_uint8_t);
  std::cout << j_short << '\n';   //  42
  std::cout << j_int << '\n';   //  -23
  std::cout << j_long << '\n';    //  1024
  std::cout << j_int_least32_t << '\n';   //  -17
  std::cout << j_uint8_t << '\n';   //  8

  // create values of different floating-point types
  json::number_float_t v_ok = 3.141592653589793;
  json::number_float_t v_nan = NAN;
  json::number_float_t v_infinity = INFINITY;

  // create values of different floating-point types
  float n_float = 42.23;
  float n_float_nan = 1.0f / 0.0f;
  double n_double = 23.42;

  // create (floating point) JSON numbers
  json j_ok(v_ok);    //  3.141592653589793
  json j_nan(v_nan);  //  null
  json j_infinity(v_infinity);  //  null
  json j_float(n_float);    //  42.22999954223633
  json j_float_nan(n_float_nan);  //  null
  json j_double(n_double);  //  23.42
  std::cout << j_ok << '\n';
  std::cout << j_nan << '\n';
  std::cout << j_infinity << '\n';
  std::cout << j_float << '\n';
  std::cout << j_float_nan << '\n';
  std::cout << j_double << '\n';
}

//!  boolean 类型
TEST(basic_type, booleans) {
  // create boolean values
  json j_truth = true;
  json j_falsity = false;

  // serialize the JSON booleans
  std::cout << j_truth << '\n';     //  true
  std::cout << j_falsity << '\n';   //  false
}

//! basic copy
TEST(basic_type, copyassignment) {
  // create JSON values
  json a = 23;
  json b = 42;

  // copy-assign a to b
  b = a;

  // serialize the JSON arrays
  std::cout << a << '\n'; //  23
  std::cout << b << '\n'; //  23
}

//! basic input iter
TEST(basic_type, iter) {
  // create JSON values
  json j_array = {"alpha", "bravo", "charly", "delta", "easy"};
  json j_number = 42;
  json j_object = {{"one", "eins"}, {"two", "zwei"}};

  // create copies using iterators
  json j_array_range(j_array.begin() + 1, j_array.end() - 2);
  json j_number_range(j_number.begin(), j_number.end());
  json j_object_range(j_object.begin(), j_object.find("two"));

  // serialize the values
  std::cout << j_array_range << '\n';   //  ["bravo","charly"]
  std::cout << j_number_range << '\n';  //  42
  std::cout << j_object_range << '\n';  //  {"one":"eins"}

  // example for an exception
  try {
    json j_invalid(j_number.begin() + 1, j_number.end());
  }
  catch (json::invalid_iterator &e) {
    std::cout << e.what() << '\n';  //  [json.exception.invalid_iterator.204] iterators out of range
  }
}

//! init list
TEST(basic_type, init_list) {
  // create JSON values
  json j_empty_init_list = json({});  //  {}
  json j_object = {{"one", 1}, {"two", 2}}; //  {"one":1,"two":2}
  json j_array = {1, 2, 3, 4};  //  [1,2,3,4]
  json j_nested_object = {{"one", {1}}, {"two", {1, 2}}}; //  {"one":[1],"two":[1,2]}
  json j_nested_array = {{{1}, "one"}, {{1, 2}, "two"}};  //  [[[1],"one"],[[1,2],"two"]]

  // serialize the JSON value
  std::cout << j_empty_init_list << '\n';
  std::cout << j_object << '\n';
  std::cout << j_array << '\n';
  std::cout << j_nested_object << '\n';
  std::cout << j_nested_array << '\n';
}

//! move constructor
TEST(basic_type, move_constructor) {
  // create a JSON value
  json a = 23;  //  null

  // move contents of a to b
  json b(std::move(a));   //  23

  // serialize the JSON arrays
  std::cout << a << '\n';
  std::cout << b << '\n';
}

//! nullptr
TEST(basic_json, nullptr_) {
  // implicitly create a JSON null value
  json j1;

  // explicitly create a JSON null value
  json j2(nullptr);

  // serialize the JSON null value
  std::cout << j1 << '\n' << j2 << '\n';
}

//! 创建重复数组
TEST(basic_json, copy_json_value) {
  // create an array by creating copies of a JSON value
  json value = "Hello";
  json array_0 = json(0, value);
  json array_1 = json(1, value);
  json array_5 = json(5, value);

  // serialize the JSON arrays
  std::cout << array_0 << '\n';
  std::cout << array_1 << '\n';
  std::cout << array_5 << '\n';
}

//! json value ptr
TEST(basic_json, val_ptr) {
  // create a JSON object with different entry types
  json j =
      {
          {"integer", 1},
          {"floating", 42.23},
          {"string", "hello world"},
          {"boolean", true},
          {"object", {{"key1", 1}, {"key2", 2}}},
          {"array", {1, 2, 3}}
      };

  // access existing values
  int v_integer = j.value("/integer"_json_pointer, 0);
  double v_floating = j.value("/floating"_json_pointer, 47.11);

  // access nonexisting values and rely on default value
  std::string v_string = j.value("/nonexisting"_json_pointer, "oops");
  bool v_boolean = j.value("/nonexisting"_json_pointer, false);

  // output values
  std::cout << std::boolalpha << v_integer << " " << v_floating
            << " " << v_string << " " << v_boolean << "\n";

  //  all out
  std::cout << j.dump(4) << "\n";
}

//! json value template
TEST(basic_json, json_value_t) {
  // create the different JSON values with default values
  json j_null(json::value_t::null);
  json j_boolean(json::value_t::boolean);
  json j_number_integer(json::value_t::number_integer);
  json j_number_float(json::value_t::number_float);
  json j_object(json::value_t::object);
  json j_array(json::value_t::array);
  json j_string(json::value_t::string);

  // serialize the JSON values
  std::cout << j_null << '\n';
  std::cout << j_boolean << '\n';
  std::cout << j_number_integer << '\n';
  std::cout << j_number_float << '\n';
  std::cout << j_object << '\n';
  std::cout << j_array << '\n';
  std::cout << j_string << '\n';
}

//! json value
TEST(basic_json, json_value) {
  // create a JSON object with different entry types
  json j =
      {
          {"integer", 1},
          {"floating", 42.23},
          {"string", "hello world"},
          {"boolean", true},
          {"object", {{"key1", 1}, {"key2", 2}}},
          {"array", {1, 2, 3}}
      };

  // access existing values
  int v_integer = j.value("integer", 0);  //  1
  double v_floating = j.value("floating", 47.11);   //  42.23

  // access nonexisting values and rely on default value
  std::string v_string = j.value("nonexisting", "oops");    //  oops
  bool v_boolean = j.value("nonexisting", false);   //  false

  // output values
  std::cout << std::boolalpha << v_integer << " " << v_floating
            << " " << v_string << " " << v_boolean << "\n";
}

//! iter begin
TEST(iter, begin) {
  // create an array value
  json array = {1, 2, 3, 4, 5};

  // get an iterator to the first element
  json::iterator it = array.begin();  //  1

  // serialize the element that the iterator points to
  std::cout << *it << '\n';
}

//! binary_t
TEST(binary_t, test) {
  std::cout << std::boolalpha <<
            std::is_same<nlohmann::byte_container_with_subtype<std::vector<std::uint8_t>>, json::binary_t>::value
            << std::endl;   //  true
}

//! binary
TEST(binary, test) {
  // create a binary vector
  std::vector<std::uint8_t> vec = {0xCA, 0xFE, 0xBA, 0xBE};

  // create a binary JSON value with subtype 42
  json j = json::binary(vec, 42);   //  {"bytes":[202,254,186,190],"subtype":42}

  // output type and subtype
  //  type: binary, subtype: 42
  std::cout << "type: " << j.type_name() << ", subtype: " << j.get_binary().subtype() << std::endl;
  std::cout << j << "\n";
}

//! boolean_t
TEST(boolean_t, test) {
  std::cout << std::boolalpha <<
            std::is_same<bool, json::boolean_t>::value << std::endl;
}

//! byte_container_with_subtype init
TEST(byte_container_with_subtype, byte_container_with_subtype) {
  using byte_container_with_subtype = nlohmann::byte_container_with_subtype<std::vector<std::uint8_t>>;

// (1) create empty container
  auto c1 = byte_container_with_subtype();  //  {"bytes":[],"subtype":null}

  std::vector<std::uint8_t> bytes = {{0xca, 0xfe, 0xba, 0xbe}};
// (2) create container
  auto c2 = byte_container_with_subtype(bytes); //  {"bytes":[202,254,186,190],"subtype":null}

// (3) create container with subtype
  auto c3 = byte_container_with_subtype(bytes, 42);   //  {"bytes":[202,254,186,190],"subtype":42}

  std::cout << json(c1) << "\n" << json(c2) << "\n" << json(c3) << std::endl;
}

//! byte_container_with_subtype clear_subtype
TEST(byte_container_with_subtype, clear_subtype) {
  using byte_container_with_subtype = nlohmann::byte_container_with_subtype<std::vector<std::uint8_t>>;

  std::vector<std::uint8_t> bytes = {{0xca, 0xfe, 0xba, 0xbe}};

  // create container with subtype
  auto c1 = byte_container_with_subtype(bytes, 42);

  std::cout << "before calling clear_subtype(): " << json(c1) << '\n';
  //  before calling clear_subtype(): {"bytes":[202,254,186,190],"subtype":42}

  c1.clear_subtype();

  std::cout << "after calling clear_subtype(): " << json(c1) << '\n';
  //  after calling clear_subtype(): {"bytes":[202,254,186,190],"subtype":null}

}

//! byte_container_with_subtype has_subtype
TEST(byte_container_with_subtype, has_subtype) {
  using byte_container_with_subtype = nlohmann::byte_container_with_subtype<std::vector<std::uint8_t>>;

  std::vector<std::uint8_t> bytes = {{0xca, 0xfe, 0xba, 0xbe}};

  // create container   c1.has_subtype() = false
  auto c1 = byte_container_with_subtype(bytes);

  // create container with subtype    c2.has_subtype() = true
  auto c2 = byte_container_with_subtype(bytes, 42);

  std::cout << std::boolalpha << "c1.has_subtype() = " << c1.has_subtype()
            << "\nc2.has_subtype() = " << c2.has_subtype() << std::endl;
}

//! byte_container_with_subtype set_subtype
TEST(byte_container_with_subtype, set_subtype) {
  using byte_container_with_subtype = nlohmann::byte_container_with_subtype<std::vector<std::uint8_t>>;

  std::vector<std::uint8_t> bytes = {{0xca, 0xfe, 0xba, 0xbe}};

  // create container without subtype
  auto c = byte_container_with_subtype(bytes);

  std::cout << "before calling set_subtype(42): " << json(c) << '\n';
  //  before calling set_subtype(42): {"bytes":[202,254,186,190],"subtype":null}
  // set the subtype
  c.set_subtype(42);

  std::cout << "after calling set_subtype(42): " << json(c) << '\n';
  //  after calling set_subtype(42): {"bytes":[202,254,186,190],"subtype":42}
}

//! byte_container_with_subtype subtype
TEST(byte_container_with_subtype, subtype) {
  using byte_container_with_subtype = nlohmann::byte_container_with_subtype<std::vector<std::uint8_t>>;

  std::vector<std::uint8_t> bytes = {{0xca, 0xfe, 0xba, 0xbe}};

  // create container
  auto c1 = byte_container_with_subtype(bytes);   //  c1.subtype() = 18446744073709551615

  // create container with subtype
  auto c2 = byte_container_with_subtype(bytes, 42);   //  c2.subtype() = 42

  std::cout << "c1.subtype() = " << c1.subtype()
            << "\nc2.subtype() = " << c2.subtype() << std::endl;

  // in case no subtype is set, return special value
  assert(c1.subtype() == static_cast<byte_container_with_subtype::subtype_type>(-1));
}

//! const iter begin
TEST(cbegin, test) {
  // create an array value
  const json array = {1, 2, 3, 4, 5};

  // get an iterator to the first element
  json::const_iterator it = array.cbegin();

  // serialize the element that the iterator points to
  std::cout << *it << '\n'; //  1
}

//! cbor_tag_handler
TEST(cbor_tag_handler, test) {
  // tagged byte string
  std::vector<std::uint8_t> vec = {{0xd8, 0x42, 0x44, 0xcA, 0xfe, 0xba, 0xbe}};

  // cbor_tag_handler_t::error throws
  try {
    auto b_throw_on_tag = json::from_cbor(vec, true, true, json::cbor_tag_handler_t::error);
  }
  catch (json::parse_error &e) {
    std::cout << e.what() << std::endl;
  }

  // cbor_tag_handler_t::ignore ignores the tag
  auto b_ignore_tag = json::from_cbor(vec, true, true, json::cbor_tag_handler_t::ignore);
  std::cout << b_ignore_tag << std::endl; //  {"bytes":[202,254,186,190],"subtype":null}

  // cbor_tag_handler_t::store stores the tag as binary subtype
  auto b_store_tag = json::from_cbor(vec, true, true, json::cbor_tag_handler_t::store);
  std::cout << b_store_tag << std::endl;  //  {"bytes":[202,254,186,190],"subtype":66}
}

//  const iter end
TEST(const_iter_end, test) {
  // create an array value
  json array = {1, 2, 3, 4, 5};

  // get an iterator to one past the last element
  json::const_iterator it = array.cend();

  // decrement the iterator to point to the last element
  --it;

  // serialize the element that the iterator points to
  std::cout << *it << '\n';   //  5
}

//  clear
TEST(clear, test) {
  // create JSON values
  json j_null;  //  null
  json j_boolean = true;  //  false
  json j_number_integer = 17;   //  0
  json j_number_float = 23.42;    //  0.0
  json j_object = {{"one", 1}, {"two", 2}};   //  {}
  json j_array = {1, 2, 4, 8, 16};    //  []
  json j_string = "Hello, world";   //  ""

  // call clear()
  j_null.clear();
  j_boolean.clear();
  j_number_integer.clear();
  j_number_float.clear();
  j_object.clear();
  j_array.clear();
  j_string.clear();

  // serialize the cleared values()
  std::cout << j_null << '\n';
  std::cout << j_boolean << '\n';
  std::cout << j_number_integer << '\n';
  std::cout << j_number_float << '\n';
  std::cout << j_object << '\n';
  std::cout << j_array << '\n';
  std::cout << j_string << '\n';
}

//!
TEST(contains, json_pointer) {
  // create a JSON value
  json j =
      {
          {"number", 1}, {"string", "foo"}, {"array", {1, 2}}
      };

  std::cout << j.dump(4) << "\n";

  std::cout << std::boolalpha
            << j.contains("/number"_json_pointer) << '\n'   //  true
            << j.contains("/string"_json_pointer) << '\n'   //  true
            << j.contains("/array"_json_pointer) << '\n'    //  true
            << j.contains("/array/1"_json_pointer) << '\n'  //  true
            << j.contains("/array/-"_json_pointer) << '\n'  //  false
            << j.contains("/array/4"_json_pointer) << '\n'  //  false
            << j.contains("/baz"_json_pointer) << std::endl;//  false

  std::cout << j.contains("/array/0"_json_pointer) << "\n"; //  true

  try {
    // try to use an array index with leading '0'
    j.contains("/array/01"_json_pointer);
  }
  catch (json::parse_error &e) {
    std::cout << e.what() << '\n';
  }

  try {
    // try to use an array index that is not a number
    j.contains("/array/one"_json_pointer);
  }
  catch (json::parse_error &e) {
    std::cout << e.what() << '\n';
  }
}

//! contains    obj arr
TEST(contains, contains) {
  // create some JSON values
  json j_object = R"( {"key": "value"} )"_json;
  json j_array = R"( [1, 2, 3] )"_json;

  // call contains
  std::cout << std::boolalpha <<
            "j_object contains 'key': " << j_object.contains("key") << '\n' <<
            "j_object contains 'another': " << j_object.contains("another") << '\n' <<
            "j_array contains 'key': " << j_array.contains("key") << std::endl;
}

//! count
TEST(count, test) {
  // create a JSON object
  json j_object = {{"one", 1}, {"two", 2}};

  // call count()
  auto count_two = j_object.count("two");
  auto count_three = j_object.count("three");

  // print values
  std::cout << "number of elements with key \"two\": " << count_two << '\n';
  std::cout << "number of elements with key \"three\": " << count_three << '\n';
}

//! const reverse begin
TEST(iter, crbegin) {
  // create an array value
  json array = {1, 2, 3, 4, 5};

  // get an iterator to the reverse-beginning
  json::const_reverse_iterator it = array.crbegin();

  // serialize the element that the iterator points to
  std::cout << *it << '\n';   //  5
}

//! const reverse end
TEST(iter, crend) {
  // create an array value
  json array = {1, 2, 3, 4, 5};

  // get an iterator to the reverse-end
  json::const_reverse_iterator it = array.crend();

  // increment the iterator to point to the first element
  --it;

  // serialize the element that the iterator points to
  std::cout << *it << '\n';   //  1
}

//! compare obj
TEST(compare, default_object_comparator) {
  std::cout << std::boolalpha
            << "one < two : " << json::default_object_comparator_t{}("one", "two") << "\n"
            << "three < four : " << json::default_object_comparator_t{}("three", "four") << std::endl;
}

//! diagnostics_extended
TEST(diagnostics, diagnostics_extended){
  json j;
  j["address"]["street"] = "Fake Street";
  j["address"]["housenumber"] = "12";

  try
  {
    int housenumber = j["address"]["housenumber"];  //  err
  }
  catch (json::exception& e)
  {
    std::cout << e.what() << '\n';
  }
}

//! diagnostics_standard
TEST(diagnostics, diagnostics_standard){
  json j;
  j["address"]["street"] = "Fake Street";
  j["address"]["housenumber"] = "12";

  try
  {
    int housenumber = j["address"]["housenumber"];
  }
  catch (json::exception& e)
  {
    std::cout << e.what() << '\n';
  }
}

//! diff
TEST(diff, test){
  // the source document
  json source = R"(
        {
            "baz": "qux",
            "foo": "bar"
        }
    )"_json;

  // the target document
  json target = R"(
        {
            "baz": "boo",
            "hello": [
                "world"
            ]
        }
    )"_json;

  // create the patch
  json patch = json::diff(source, target);

  // roundtrip
  json patched_source = source.patch(patch);

  // output patch and roundtrip result
  std::cout << std::setw(4) << patch << "\n\n"
            << std::setw(4) << patched_source << std::endl;
}
