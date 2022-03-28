//
// Copyright (c) 2016-2017 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/sml.hpp>
#include <vector>
#include <any>
#include <string>
#include <tuple>
#include <variant>

namespace sml = boost::sml;

struct fake_handler {
  auto on(const auto& event, auto&&...) {
    on_calls.push_back(event);
  }
  std::vector<std::any> on_calls{};
};

test emit_event = [] {
  struct {} event{};
  struct {} sm{};
  fake_handler handler{};
  sml::aux::tuple<fake_handler&> args{handler};
  struct {
    decltype(args)& args;
    sml::aux::pool<> deps{};
  } deps{{args}};
  struct {} subs;

  sml::front::actions::emit{[]{return 42; }}(event, sm, deps, subs);

  expect(1 == std::size(handler.on_calls));
  expect(42 == std::any_cast<int>(handler.on_calls[0]));
};

test emit_callable = [] {
  struct {} event{};
  struct {} sm{};
  fake_handler handler{};
  sml::aux::tuple<fake_handler&> args{handler};
  struct {
    decltype(args)& args;
    sml::aux::pool<> deps{};
  } deps{{args}};
  struct {} subs;

  sml::front::actions::emit{[]{return []{return 42; }; }}(event, sm, deps, subs);

  expect(1 == std::size(handler.on_calls));
  expect(42 == std::any_cast<int>(handler.on_calls[0]));
};

test emit_optional = [] {
  struct {} event{};
  struct {} sm{};
  fake_handler handler{};
  sml::aux::tuple<fake_handler&> args{handler};
  struct {
    decltype(args)& args;
    sml::aux::pool<> deps{};
  } deps{{args}};
  struct {} subs;

  sml::front::actions::emit{[]{return std::optional<int>(42); }}(event, sm, deps, subs);

  expect(1 == std::size(handler.on_calls));
  expect(42 == std::any_cast<int>(handler.on_calls[0]));
};

test emit_optional_disengaged = [] {
  struct {} event{};
  struct {} sm{};
  fake_handler handler{};
  sml::aux::tuple<fake_handler&> args{handler};
  struct {
    decltype(args)& args;
    sml::aux::pool<> deps{};
  } deps{{args}};
  struct {} subs;

  sml::front::actions::emit{[]{return std::optional<int>(); }}(event, sm, deps, subs);

  expect(0 == std::size(handler.on_calls));
};

test emit_tuple = [] {
  struct {} event{};
  struct {} sm{};
  fake_handler handler{};
  sml::aux::tuple<fake_handler&> args{handler};
  struct {
    decltype(args)& args;
    sml::aux::pool<> deps{};
  } deps{{args}};
  struct {} subs;

  sml::front::actions::emit{[]{return std::tuple<int>{42}; }}(event, sm, deps, subs);

  expect(1 == std::size(handler.on_calls));
  expect(42 == std::any_cast<int>(handler.on_calls[0]));
};

test emit_tuples = [] {
  struct {} event{};
  struct {} sm{};
  fake_handler handler{};
  sml::aux::tuple<fake_handler&> args{handler};
  struct {
    decltype(args)& args;
    sml::aux::pool<> deps{};
  } deps{{args}};
  struct {} subs;

  sml::front::actions::emit{[]{return std::tuple<int, std::string>{42, "str"}; }}(event, sm, deps, subs);

  expect(2 == std::size(handler.on_calls));
  expect(42 == std::any_cast<int>(handler.on_calls[0]));
  expect("str" == std::any_cast<std::string>(handler.on_calls[1]));
};

test emit_variant = [] {
  struct {} event{};
  struct {} sm{};
  fake_handler handler{};
  sml::aux::tuple<fake_handler&> args{handler};
  struct {
    decltype(args)& args;
    sml::aux::pool<> deps{};
  } deps{{args}};
  struct {} subs;

  sml::front::actions::emit{[]{return std::variant<int, std::string>{42}; }}(event, sm, deps, subs);

  expect(1 == std::size(handler.on_calls));
  expect(42 == std::any_cast<int>(handler.on_calls[0]));
};

test emit_variant_not_set = [] {
  struct {} event{};
  struct {} sm{};
  fake_handler handler{};
  sml::aux::tuple<fake_handler&> args{handler};
  struct {
    decltype(args)& args;
    sml::aux::pool<> deps{};
  } deps{{args}};
  struct {} subs;

  sml::front::actions::emit{[]{return std::variant<int, std::string>{}; }}(event, sm, deps, subs);

  expect(1 == std::size(handler.on_calls));
  expect(0 == std::any_cast<int>(handler.on_calls[0]));
};

test emit_range = [] {
  struct {} event{};
  struct {} sm{};
  fake_handler handler{};
  sml::aux::tuple<fake_handler&> args{handler};
  struct {
    decltype(args)& args;
    sml::aux::pool<> deps{};
  } deps{{args}};
  struct {} subs;

  sml::front::actions::emit{[]{return std::vector<int>{1, 2, 3}; }}(event, sm, deps, subs);

  expect(3 == std::size(handler.on_calls));
  expect(1 == std::any_cast<int>(handler.on_calls[0]));
  expect(2 == std::any_cast<int>(handler.on_calls[1]));
  expect(3 == std::any_cast<int>(handler.on_calls[2]));
};

test emit_range_but_not_string = [] {
  struct {} event{};
  struct {} sm{};
  fake_handler handler{};
  sml::aux::tuple<fake_handler&> args{handler};
  struct {
    decltype(args)& args;
    sml::aux::pool<> deps{};
  } deps{{args}};
  struct {} subs;

  sml::front::actions::emit{[]{return std::string{"str"}; }}(event, sm, deps, subs);

  expect(1 == std::size(handler.on_calls));
  expect("str" == std::any_cast<std::string>(handler.on_calls[0]));
};

test emit_tuple_of_optional_ranges = [] {
  struct {} event{};
  struct {} sm{};
  fake_handler handler{};
  sml::aux::tuple<fake_handler&> args{handler};
  struct {
    decltype(args)& args;
    sml::aux::pool<> deps{};
  } deps{{args}};
  struct {} subs;

  sml::front::actions::emit{[]{return std::tuple{std::vector<int>{1, 2}, std::optional<int>{}, std::string{"str"}, std::tuple{1, 2}}; }}(event, sm, deps, subs);

  expect(5 == std::size(handler.on_calls));
  expect(1 == std::any_cast<int>(handler.on_calls[0]));
  expect(2 == std::any_cast<int>(handler.on_calls[1]));
  expect("str" == std::any_cast<std::string>(handler.on_calls[2]));
  expect(1 == std::any_cast<int>(handler.on_calls[3]));
  expect(2 == std::any_cast<int>(handler.on_calls[4]));
};

test emit_range_of_tuples = [] {
  struct {} event{};
  struct {} sm{};
  fake_handler handler{};
  sml::aux::tuple<fake_handler&> args{handler};
  struct {
    decltype(args)& args;
    sml::aux::pool<> deps{};
  } deps{{args}};
  struct {} subs;

  sml::front::actions::emit{[]{return std::vector{std::tuple{1, 2}}; }}(event, sm, deps, subs);

  expect(2 == std::size(handler.on_calls));
  expect(1 == std::any_cast<int>(handler.on_calls[0]));
  expect(2 == std::any_cast<int>(handler.on_calls[1]));
};
