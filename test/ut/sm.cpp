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
