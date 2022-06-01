//
// Copyright (c) 2016-2017 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/sml.hpp>

struct e final {};

test events = [] {
  auto events = [] {
    using namespace boost::sml::dsl;
    return transition_table{
     * "idle"_s + event<e> = X
    };
  };

  boost::sml::sm<decltype(events)> sm{events};
  using namespace boost::sml::dsl;
  expect(sm.is("idle"_s));
  sm.process_event(e{});
  expect(sm.is(X));
};

struct non_movable {
  constexpr non_movable(int value) : value{value} {}
  non_movable(non_movable&&) = delete;
  non_movable(const non_movable&) = delete;
  int value{};
};

test deps = [] {
  auto ref_deps = [] {
    using namespace boost::sml::dsl;
    return transition_table{
     * "idle"_s + event<e> / ([](const non_movable& i) { expect(i.value == 42); }, [](non_movable& i) {expect(i.value == 42);}) = X
    };
  };

  non_movable i{42};
  boost::sml::sm<decltype(ref_deps)> sm{i};
  sm.process_event(e{});
  using namespace boost::sml::dsl;
  expect(sm.is(X));
};
