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

struct fake_sm {
  auto process_event(const auto& event, auto&&...) {
    pe_calls.push_back(event);
  }
  std::vector<std::any> pe_calls{};
};

test process_event = [] {
  struct {} event{};
  fake_sm sm{};
  struct {
    sml::aux::tuple<> args{};
    sml::aux::pool<> deps{};
  } deps;
  struct {} subs;

  sml::front::actions::process{}([]{return 42; })(event, sm, deps, subs);

  expect(1 == std::size(sm.pe_calls));
  expect(42 == std::any_cast<int>(sm.pe_calls[0]));
};

test process_optional = [] {
  struct {} event{};
  fake_sm sm{};
  struct {
    sml::aux::tuple<> args{};
    sml::aux::pool<> deps{};
  } deps;
  struct {} subs;

  sml::front::actions::process{}([]{return std::optional{42}; })(event, sm, deps, subs);

  expect(1 == std::size(sm.pe_calls));
  expect(42 == std::any_cast<int>(sm.pe_calls[0]));
};

test process_optional_disengaged = [] {
  struct {} event{};
  fake_sm sm{};
  struct {
    sml::aux::tuple<> args{};
    sml::aux::pool<> deps{};
  } deps;
  struct {} subs;

  sml::front::actions::process{}([]{return std::optional<int>{}; })(event, sm, deps, subs);

  expect(0 == std::size(sm.pe_calls));
};

test process_tuple = [] {
  struct {} event{};
  fake_sm sm{};
  struct {
    sml::aux::tuple<> args{};
    sml::aux::pool<> deps{};
  } deps;
  struct {} subs;

  sml::front::actions::process{}([]{return std::tuple<int>{42}; })(event, sm, deps, subs);

  expect(1 == std::size(sm.pe_calls));
  expect(42 == std::any_cast<int>(sm.pe_calls[0]));
};


test process_range_of_tuples = [] {
  struct {} event{};
  fake_sm sm{};
  struct {
    sml::aux::tuple<> args{};
    sml::aux::pool<> deps{};
  } deps;
  struct {} subs;

  sml::front::actions::process{}([]{return std::vector{std::tuple{1, 2}}; })(event, sm, deps, subs);

  expect(2 == std::size(sm.pe_calls));
  expect(1 == std::any_cast<int>(sm.pe_calls[0]));
  expect(2 == std::any_cast<int>(sm.pe_calls[1]));
};

test process_lazy = [] {
  struct {} event{};
  fake_sm sm{};
  struct {
    sml::aux::tuple<> args{};
    sml::aux::pool<> deps{};
  } deps;
  struct {} subs;

  sml::front::actions::process{}([]{return [](auto& process) { process(42); };})(event, sm, deps, subs);

  expect(1 == std::size(sm.pe_calls));
  expect(42 == std::any_cast<int>(sm.pe_calls[0]));
};
