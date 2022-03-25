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

test fwd = [] {
  fake_handler handler{};

  sml::front::fwd(42, handler);

  expect(1 == std::size(handler.on_calls));
  expect(42 == std::any_cast<int>(handler.on_calls[0]));
};
