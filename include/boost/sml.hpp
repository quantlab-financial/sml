//
// Copyright (c) 2016-2017 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once
#include <utility>
#include <vector>
#include <variant>
#include <type_traits>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/mp11/algorithm.hpp>
#include <tuple>

#if (__cplusplus < 201305L && _MSC_VER < 1900)
#error "[Boost].SML requires C++14 support (Clang-3.4+, GCC-5.1+, MSVC-2015+)"
#else
#define BOOST_SML_VERSION 1'1'0
#define BOOST_SML_NAMESPACE_BEGIN \
  namespace boost {               \
  namespace sml {                 \
  inline namespace v1_1_0 {
#define BOOST_SML_NAMESPACE_END \
  }                             \
  }                             \
  }
#if defined(__clang__)
#define __BOOST_SML_UNUSED __attribute__((unused))
#define __BOOST_SML_VT_INIT \
  {}
#if !defined(BOOST_SML_CFG_DISABLE_MIN_SIZE)
#define __BOOST_SML_ZERO_SIZE_ARRAY(...) __VA_ARGS__ _[0]
#else
#define __BOOST_SML_ZERO_SIZE_ARRAY(...)
#endif
#define __BOOST_SML_ZERO_SIZE_ARRAY_CREATE(...)
#define __BOOST_SML_TEMPLATE_KEYWORD template
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-string-literal-operator-template"
#pragma clang diagnostic ignored "-Wzero-length-array"
#pragma clang diagnostic ignored "-Wexpansion-to-defined"
#elif defined(__GNUC__)
#define __has_builtin(...) 0
#define __BOOST_SML_UNUSED __attribute__((unused))
#define __BOOST_SML_VT_INIT \
  {}
#if !defined(BOOST_SML_CFG_DISABLE_MIN_SIZE)
#define __BOOST_SML_ZERO_SIZE_ARRAY(...) __VA_ARGS__ _[0]
#else
#define __BOOST_SML_ZERO_SIZE_ARRAY(...)
#endif
#define __BOOST_SML_ZERO_SIZE_ARRAY_CREATE(...) __VA_ARGS__ ? __VA_ARGS__ : 1
#define __BOOST_SML_TEMPLATE_KEYWORD template
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#elif defined(_MSC_VER)
#define __has_builtin(...) __has_builtin##__VA_ARGS__
#define __has_builtin__make_integer_seq(...) 1
#define __BOOST_SML_UNUSED
#define __BOOST_SML_VT_INIT
#define __BOOST_SML_ZERO_SIZE_ARRAY(...)
#define __BOOST_SML_ZERO_SIZE_ARRAY_CREATE(...) __VA_ARGS__ ? __VA_ARGS__ : 1
#define __BOOST_SML_TEMPLATE_KEYWORD
#pragma warning(disable : 4503)
#pragma warning(disable : 4200)
#endif
BOOST_SML_NAMESPACE_BEGIN
#define __BOOST_SML_REQUIRES(...) typename aux::enable_if<__VA_ARGS__, int>::type = 0
namespace aux {
template <class>
inline constexpr auto is_tuple_impl = false;
template <class... Ts>
inline constexpr auto is_tuple_impl<std::tuple<Ts...>> = true;
template <class T>
concept is_tuple = is_tuple_impl<std::remove_cvref_t<T>>;
template <class TEvent>
auto on(const TEvent &) -> void {}
template <class TEvent, class THandler, class... TArgs>
auto on(const TEvent &event, THandler &handler, TArgs &&... args) -> void {
  if constexpr (requires { handler.on(event, args...); }) {
    handler.on(event, static_cast<TArgs &&>(args)...);
  } else if constexpr (requires { handler(event, args...); }) {
    handler(event, static_cast<TArgs &&>(args)...);
  }
}
using byte = unsigned char;
struct none_type {};
template <class...>
struct type {};
template <class T, T>
struct non_type {};
template <class, class>
struct pair {};
template <class...>
struct type_list {
  using type = type_list;
};
template <bool...>
struct bool_list {
  using type = bool_list;
};
template <class... Ts>
struct inherit : Ts... {
  using type = inherit;
};
template<class>
struct wrapper {};
template<class... Ts>
using wrap_t = aux::type_list<wrapper<Ts>...>;
template <class T>
struct identity {
  using type = T;
};
template <class T>
T &&declval();
template <class T, T V>
struct integral_constant {
  using type = integral_constant;
  static constexpr T value = V;
};
using true_type = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;
template <class... Ts>
using void_t = void;
template <class...>
struct always : true_type {};
template <class...>
struct never : false_type {};
template <bool B, class T, class F>
struct conditional {
  using type = T;
};
template <class T, class F>
struct conditional<false, T, F> {
  using type = F;
};
template <bool B, class T, class F>
using conditional_t = typename conditional<B, T, F>::type;
template <bool B, class T = void>
struct enable_if {};
template <class T>
struct enable_if<true, T> {
  using type = T;
};
template <bool B, class T = void>
using enable_if_t = typename enable_if<B, T>::type;
template <class, class>
struct is_same : false_type {};
template <class T>
struct is_same<T, T> : true_type {};
template <class T, class U>
#if defined(_MSC_VER)
struct is_base_of : integral_constant<bool, __is_base_of(T, U)> {
};
#else
using is_base_of = integral_constant<bool, __is_base_of(T, U)>;
#endif
template <class T, class... TArgs>
decltype(T(declval<TArgs>()...), true_type{}) test_is_constructible(int);
template <class, class...>
false_type test_is_constructible(...);
template <class T, class... TArgs>
#if defined(_MSC_VER)
struct is_constructible : decltype(test_is_constructible<T, TArgs...>(0)) {
};
#else
using is_constructible = decltype(test_is_constructible<T, TArgs...>(0));
#endif
template <class T>
struct remove_reference {
  using type = T;
};
template <class T>
struct remove_reference<T &> {
  using type = T;
};
template <class T>
struct remove_reference<const T &> {
  using type = T;
};
template <class T>
struct remove_reference<T &&> {
  using type = T;
};
template <class T>
using remove_reference_t = typename remove_reference<T>::type;
template<class T>
struct remove_const_ref {
  using type = T;
};

template<class T>
struct remove_const_ref<const T&> {
  using type = T&;
};
template<class T>
using remove_const_ref_t = typename remove_const_ref<T>::type;
template <class>
struct function_traits;
template <class R, class... TArgs>
struct function_traits<R (*)(TArgs...)> {
  using args = type_list<TArgs...>;
};
template <class R, class... TArgs>
struct function_traits<R(TArgs...)> {
  using args = type_list<TArgs...>;
};
template <class R, class T, class... TArgs>
struct function_traits<R (T::*)(TArgs...)> {
  using args = type_list<TArgs...>;
};
template <class R, class T, class... TArgs>
struct function_traits<R (T::*)(TArgs...) const> {
  using args = type_list<TArgs...>;
};
template <class T>
using function_traits_t = typename function_traits<T>::args;
}  // namespace aux
namespace aux {
using swallow = int[];
template <int...>
struct index_sequence {
  using type = index_sequence;
};
#if __has_builtin(__make_integer_seq)
template <class T, T...>
struct integer_sequence;
template <int... Ns>
struct integer_sequence<int, Ns...> {
  using type = index_sequence<Ns...>;
};
template <int N>
struct make_index_sequence_impl {
  using type = typename __make_integer_seq<integer_sequence, int, N>::type;
};
#else
template <class, class>
struct concat;
template <int... I1, int... I2>
struct concat<index_sequence<I1...>, index_sequence<I2...>> : index_sequence<I1..., (sizeof...(I1) + I2)...> {};
template <int N>
struct make_index_sequence_impl
    : concat<typename make_index_sequence_impl<N / 2>::type, typename make_index_sequence_impl<N - N / 2>::type>::type {};
template <>
struct make_index_sequence_impl<0> : index_sequence<> {};
template <>
struct make_index_sequence_impl<1> : index_sequence<0> {};
#endif
template <int N>
using make_index_sequence = typename make_index_sequence_impl<N>::type;
template <class...>
struct join {
  using type = type_list<>;
};
template <class T>
struct join<T> {
  using type = T;
};
template <class... Ts>
struct join<type_list<Ts...>> : type_list<Ts...> {};
template <class... T1s, class... T2s>
struct join<type_list<T1s...>, type_list<T2s...>> : type_list<T1s..., T2s...> {};
template <class... T1s, class... T2s, class... T3s>
struct join<type_list<T1s...>, type_list<T2s...>, type_list<T3s...>> : type_list<T1s..., T2s..., T3s...> {};
template <class... T1s, class... T2s, class... Ts>
struct join<type_list<T1s...>, type_list<T2s...>, Ts...> : join<type_list<T1s..., T2s...>, Ts...> {};
template <class... Ts, class... T1s, class... T2s, class... T3s, class... T4s, class... T5s, class... T6s, class... T7s,
          class... T8s, class... T9s, class... T10s, class... T11s, class... T12s, class... T13s, class... T14s, class... T15s,
          class... T16s, class... Us>
struct join<type_list<Ts...>, type_list<T1s...>, type_list<T2s...>, type_list<T3s...>, type_list<T4s...>, type_list<T5s...>,
            type_list<T6s...>, type_list<T7s...>, type_list<T8s...>, type_list<T9s...>, type_list<T10s...>, type_list<T11s...>,
            type_list<T12s...>, type_list<T13s...>, type_list<T14s...>, type_list<T15s...>, type_list<T16s...>, Us...>
    : join<type_list<Ts..., T1s..., T2s..., T3s..., T4s..., T5s..., T6s..., T7s..., T8s..., T9s..., T10s..., T11s..., T12s...,
                     T13s..., T14s..., T15s..., T16s...>,
           Us...> {};
template <class... TArgs>
using join_t = typename join<TArgs...>::type;
template <class, class...>
struct unique_impl;
template <class T1, class T2, class... Rs, class... Ts>
struct unique_impl<type<T1, Rs...>, T2, Ts...>
    : conditional_t<is_base_of<type<T2>, T1>::value, unique_impl<type<inherit<T1>, Rs...>, Ts...>,
                    unique_impl<type<inherit<T1, type<T2>>, Rs..., T2>, Ts...>> {};
template <class T1, class... Rs>
struct unique_impl<type<T1, Rs...>> : type_list<Rs...> {};
template <class... Ts>
struct unique : unique_impl<type<none_type>, Ts...> {};
template <class T>
struct unique<T> : type_list<T> {};
template <class... Ts>
using unique_t = boost::mp11::mp_unique<aux::type_list<Ts...>>;
template <class, class...>
struct is_unique;
template <class T>
struct is_unique<T> : true_type {};
template <class T1, class T2, class... Ts>
struct is_unique<T1, T2, Ts...>
    : conditional_t<is_base_of<type<T2>, T1>::value, false_type, is_unique<inherit<T1, type<T2>>, Ts...>> {};
template <class... Ts>
using is_unique_t = is_unique<none_type, Ts...>;
template <template <class...> class, class>
struct apply;
template <template <class...> class T, template <class...> class U, class... Ts>
struct apply<T, U<Ts...>> {
  using type = T<Ts...>;
};
template <template <class...> class T, class D>
using apply_t = typename apply<T, D>::type;
template <int, class T>
struct tuple_type {
  constexpr tuple_type(const T &object) : value(object) {}
  T value;
};
template <class, class...>
struct tuple_impl;
template <int... Ns, class... Ts>
struct tuple_impl<index_sequence<Ns...>, Ts...> : tuple_type<Ns, Ts>... {
  constexpr tuple_impl(Ts... ts) : tuple_type<Ns, Ts>(ts)... {}
};
template <>
struct tuple_impl<index_sequence<0>> {
  __BOOST_SML_ZERO_SIZE_ARRAY(byte);
};
template <class... Ts>
using tuple = tuple_impl<make_index_sequence<sizeof...(Ts)>, Ts...>;
template <class T, int N>
constexpr T &get_by_type(tuple_type<N, T> *object) {
  return static_cast<tuple_type<N, T> &>(*object).value;
}
template <int N, class T>
constexpr T &get_by_id(tuple_type<N, T> *object) {
  return static_cast<tuple_type<N, T> &>(*object).value;
}
struct init {};

template <class T, class = void>
struct pool_type_impl {
  template<class U>
  constexpr explicit pool_type_impl(U& value) : value{value} {
  }
  T value;
};

template <class T>
struct pool_type_impl<T&, aux::enable_if_t<!aux::is_constructible<T>::value>> {
  template<class U>
  constexpr explicit pool_type_impl(U& value) : value{value} {
  }
  T& value;
};

template <class T>
struct pool_type_impl<T&, aux::enable_if_t<aux::is_constructible<T>::value>> {
  constexpr pool_type_impl() : value_{}, value{value_} {}
  constexpr explicit pool_type_impl(T& value) : value{value} {
  }
  constexpr pool_type_impl(pool_type_impl &&other) noexcept
    : value_{std::move(other.value)}, value{value_} {}
  template<class TObject>
  constexpr explicit pool_type_impl(const TObject& value) : value_{value}, value{value_} {
  }
  T value_;
  T& value;
};

template <class T>
struct pool_type : pool_type_impl<T> {
  using pool_type_impl<T>::pool_type_impl;
};

template <class T, class TPool>
constexpr T &get(TPool &p) {
  return static_cast<pool_type<T> &>(p).value;
}

template <class T, class TPool>
constexpr const T &cget(TPool &p) {
  return static_cast<const pool_type<T> &>(p).value;
}

template<class T, class... Ts> constexpr auto contains = (aux::is_same<T, Ts>::value or ...);

struct nothing {
  constexpr nothing() = default;
  template<class T> constexpr nothing(T&&) { }
};

template <class... Ts>
struct pool : pool_type<Ts>..., nothing {
  template <auto... Ns, class... TArgs> constexpr pool(TArgs&... args)
    : aux::conditional_t<contains<TArgs, Ts...>, pool_type<TArgs>, aux::conditional_t<contains<TArgs&, Ts...>, pool_type<TArgs&>, std::conditional_t<contains<const TArgs&, Ts...>, pool_type<const TArgs&>, nothing>>>(args)... {}

  template <class TArg> constexpr pool(init, TArg& arg) : pool_type<Ts>(arg)... {}
};
template <>
struct pool<> {
  template <class... Ts>
  constexpr pool(Ts &&...) {}
  __BOOST_SML_ZERO_SIZE_ARRAY(byte);
};
template <class>
constexpr false_type has_type(...);
template <class T>
constexpr true_type has_type(const pool_type<T> *);
template <int, class>
struct type_id_type {};
template <class, class...>
struct type_id_impl;
template <int... Ns, class... Ts>
struct type_id_impl<index_sequence<Ns...>, Ts...> : type_id_type<Ns, Ts>... {};
template <class... Ts>
struct type_id : type_id_impl<make_index_sequence<sizeof...(Ts)>, Ts...> {};
template <class R, class T, int N>
constexpr R get_id(type_id_type<N, T> *) {
  return static_cast<R>(N);
}
template <template <class...> class, class T>
struct is : false_type {};
template <template <class...> class T, class... Ts>
struct is<T, T<Ts...>> : true_type {};
template <class>
struct size;
template <template <class...> class T, class... Ts>
struct size<T<Ts...>> {
  static constexpr auto value = sizeof...(Ts);
};
#if defined(_MSC_VER)
constexpr int max_impl() { return 0; }
constexpr int max_impl(int r) { return r; }
constexpr int max_impl(int r, int i) { return r > i ? r : i; }
constexpr int max_impl(int r, int i, int ints...) { return i > r ? max_impl(i, ints) : max_impl(r, ints); }
template <int... Ts>
constexpr int max() {
  return max_impl(Ts...);
}
#else
template <int... Ts>
constexpr int max() {
  int max = 0;
  (void)swallow{0, (Ts > max ? max = Ts : max)...};
  return max;
}
#endif
template <class TExpr, class = void>
struct zero_wrapper : TExpr {
  constexpr explicit zero_wrapper(const TExpr &expr) : TExpr(expr) {}
  constexpr const TExpr &get() const { return *this; }
};
template <class, class>
struct zero_wrapper_impl;
template <class TExpr, class... TArgs>
struct zero_wrapper_impl<TExpr, type_list<TArgs...>> {
  constexpr zero_wrapper_impl() = default;
  constexpr auto operator()(TArgs... args) const { return reinterpret_cast<const TExpr &>(*this)(args...); }
  __BOOST_SML_ZERO_SIZE_ARRAY(byte);
};
template <class TExpr>
struct zero_wrapper<TExpr, void_t<decltype(+declval<TExpr>())>>
    : zero_wrapper_impl<TExpr, function_traits_t<decltype(&TExpr::operator())>> {
  template <class... Ts>
  constexpr zero_wrapper(Ts &&...) {}
  constexpr const TExpr &get() const { return reinterpret_cast<const TExpr &>(*this); }
};
namespace detail {
template <class, int N, int... Ns>
auto get_type_name(const char *ptr, index_sequence<Ns...>) {
  static const char str[] = {ptr[N + Ns]..., 0};
  return str;
}
}  // namespace detail
template <class T>
const char *get_type_name() {
#if defined(_MSC_VER)
  return detail::get_type_name<T, 34>(__FUNCSIG__, make_index_sequence<sizeof(__FUNCSIG__) - 34 - 8>{});
#elif defined(__clang__)
  return detail::get_type_name<T, 58>(__PRETTY_FUNCTION__, make_index_sequence<sizeof(__PRETTY_FUNCTION__) - 58 - 2>{});
#elif defined(__GNUC__)
  return detail::get_type_name<T, 63>(__PRETTY_FUNCTION__, make_index_sequence<sizeof(__PRETTY_FUNCTION__) - 63 - 2>{});
#endif
}
template <class T, T...>
struct string;
template <char... Chrs>
struct string<char, Chrs...> {
  using type = string;
  static auto c_str() {
    static constexpr char str[] = {Chrs..., 0};
    return str;
  }
};
template <class T>
struct string<T> {
  using type = T;
  static auto c_str() { return c_str_impl((T *)0); }
  template <class U>
  static decltype(U::c_str()) c_str_impl(U *) {
    return U::c_str();
  }
  static auto c_str_impl(...) { return get_type_name<T>(); }
};
}  // namespace aux
namespace back {
struct _ {};
struct initial {};
struct unexpected {};
struct entry_exit {};
struct terminate_state {
  static auto c_str() { return "terminate"; }
};
struct internal_event {
  static auto c_str() { return "internal_event"; }
};
struct anonymous : internal_event {
  static auto c_str() { return "anonymous"; }
};
template <class T, class TEvent = T>
struct on_entry : internal_event, entry_exit {
  static auto c_str() { return "on_entry"; }
  explicit on_entry(const TEvent &event = {}) : event_(event) {}
  const TEvent &event_;
};
template <class T, class TEvent = T>
struct on_exit : internal_event, entry_exit {
  static auto c_str() { return "on_exit"; }
  explicit on_exit(const TEvent &event = {}) : event_(event) {}
  const TEvent &event_;
};
template <class TException>
struct exception : internal_event {
  using type = TException;
  explicit exception(const TException &exception = {}) : exception_(exception) {}
  const TException &exception_;
};
template <class T, class TEvent = T>
struct unexpected_event : internal_event, unexpected {
  explicit unexpected_event(const TEvent &event = {}) : event_(event) {}
  const TEvent &event_;
};
template <class TEvent>
struct event_type {
  using event_t = TEvent;
  using generic_t = TEvent;
  using mapped_t = void;
};
template <class TEvent, class... TEvents>
struct event_type<aux::type_list<TEvent, TEvents...>> {
  using event_t = TEvent;
  using generic_t = TEvent;
  using mapped_t = void;
};
template <class TEvent>
struct event_type<exception<TEvent>> {
  using event_t = TEvent;
  using generic_t = exception<TEvent>;
  using mapped_t = void;
};
template <class TEvent, class T>
struct event_type<unexpected_event<T, TEvent>> {
  using event_t = TEvent;
  using generic_t = unexpected_event<T>;
  using mapped_t = void;
};
template <class TEvent, class T>
struct event_type<on_entry<T, TEvent>> {
  using event_t = TEvent;
  using generic_t = on_entry<T>;
  using mapped_t = on_entry<T, TEvent>;
};
template <class TEvent, class T>
struct event_type<on_exit<T, TEvent>> {
  using event_t = TEvent;
  using generic_t = on_exit<T>;
  using mapped_t = on_exit<T, TEvent>;
};
template <class TEvent>
using get_event_t = typename event_type<TEvent>::event_t;
template <class TEvent>
using get_generic_t = typename event_type<TEvent>::generic_t;
template <class TEvent>
using get_mapped_t = typename event_type<TEvent>::mapped_t;
}  // namespace back
namespace back {
template <class>
class sm;
template <class>
struct sm_impl;
template <class, class...>
struct sm_policy;
template <class TEvent>
using get_event = aux::conditional_t<aux::is_base_of<internal_event, TEvent>::value, aux::type_list<>, aux::type_list<TEvent>>;
template <class, class, class TEvent>
struct get_all_events_impl {
  using type = get_event<TEvent>;
};
template <class TSrc, class TDst, class TEvent>
struct get_all_events_impl<TSrc, TDst, unexpected_event<TEvent>> {
  using type = aux::type_list<TEvent>;
};
template <class TSrc, class TDst, class... TEvents>
struct get_all_events_impl<TSrc, TDst, aux::type_list<TEvents...>> {
  using type = boost::mp11::mp_unique<aux::type_list<TEvents...>>;
};
template <class TSrc, class TDst, class TEvent>
struct get_all_events_impl<sm<TSrc>, TDst, TEvent> {
  using type = boost::mp11::mp_unique<aux::join_t<get_event<TEvent>, typename sm<TSrc>::events>>;
};
template <class TSrc, class TDst, class TEvent>
struct get_all_events_impl<TSrc, sm<TDst>, TEvent> {
  using type = boost::mp11::mp_unique<aux::join_t<get_event<TEvent>, typename sm<TDst>::events>>;
};
template <class TSrc, class TDst, class TEvent>
struct get_all_events_impl<sm<TSrc>, sm<TDst>, TEvent> {
  using type = boost::mp11::mp_unique<aux::join_t<get_event<TEvent>, typename sm<TSrc>::events, typename sm<TDst>::events>>;
};
template <class, class TEvent>
struct get_sub_internal_events_impl {
  using type = aux::conditional_t<aux::is_base_of<internal_event, TEvent>::value, aux::type_list<TEvent>, aux::type_list<>>;
};
template <class T, class TEvent>
struct get_sub_internal_events_impl<sm<T>, TEvent> {
  using type = boost::mp11::mp_unique<aux::join_t<aux::type_list<TEvent>, typename sm_impl<T>::sub_internal_events_t>>;
};
template <class... Ts>
using get_all_events =
    aux::join_t<typename get_all_events_impl<typename Ts::src_state, typename Ts::dst_state, typename Ts::event>::type...>;
template <class... Ts>
using get_sub_internal_events =
    aux::join_t<typename get_sub_internal_events_impl<typename Ts::src_state, typename Ts::event>::type...>;
template <class... Ts>
using get_events = aux::type_list<typename Ts::event...>;
template <class T>
struct get_exception : aux::type_list<> {};
template <class T>
struct get_exception<exception<T>> : aux::type_list<exception<T>> {};
template <class... Ts>
using get_exceptions = aux::join_t<typename get_exception<Ts>::type...>;
template <class... Ts>
using get_states = aux::join_t<aux::type_list<typename Ts::src_state, typename Ts::dst_state>...>;
template <class... Ts>
using get_initial_states =
    aux::join_t<typename aux::conditional<Ts::initial, aux::type_list<typename Ts::src_state>, aux::type_list<>>::type...>;
template <class... Ts>
using get_history_states = aux::join_t<
    typename aux::conditional<!Ts::history && Ts::initial, aux::type_list<typename Ts::src_state>, aux::type_list<>>::type...>;
template <class>
struct get_sub_sm : aux::type_list<> {};
template <class T>
struct get_sub_sm<sm<T>> : aux::join_t<aux::type_list<T>, typename sm<T>::state_machines> {};
template <class... Ts>
using get_sub_sms = aux::join_t<typename get_sub_sm<Ts>::type...>;
template <class... Ts>
using get_sm_t = aux::type_list<typename Ts::sm...>;
template <class... Ts>
using merge_deps = aux::join_t<typename Ts::deps...>;
template <class>
struct sub_sm;
template <class T>
struct sub_sm<sm_impl<sm_policy<T>>> {
  template <class U, class... TPolicies>
  static sm_impl<sm_policy<T, aux::identity<U>, TPolicies...>> &get(
      aux::pool_type<sm_impl<sm_policy<T, aux::identity<U>, TPolicies...>>> *object) {
    return static_cast<aux::pool_type<sm_impl<sm_policy<T, aux::identity<U>, TPolicies...>>> &>(*object).value;
  }
  template <class... TPolicies>
  static sm_impl<sm_policy<T, TPolicies...>> &get(aux::pool_type<sm_impl<sm_policy<T, TPolicies...>>> *object) {
    return static_cast<aux::pool_type<sm_impl<sm_policy<T, TPolicies...>>> &>(*object).value;
  }
  template <class... TPolicies>
  static const sm_impl<sm_policy<T, TPolicies...>> &cget(const aux::pool_type<sm_impl<sm_policy<T, TPolicies...>>> *object) {
    return static_cast<const aux::pool_type<sm_impl<sm_policy<T, TPolicies...>>> &>(*object).value;
  }
};
template <class T, class U>
struct sub_sm<sm_impl<sm_policy<T, aux::identity<U>>>> {
  template <class... TPolicies>
  static sm_impl<sm_policy<T, aux::identity<U>, TPolicies...>> &get(
      aux::pool_type<sm_impl<sm_policy<T, aux::identity<U>, TPolicies...>>> *object) {
    return static_cast<aux::pool_type<sm_impl<sm_policy<T, aux::identity<U>, TPolicies...>>> &>(*object).value;
  }
  template <class... TPolicies>
  static const sm_impl<sm_policy<T, aux::identity<U>, TPolicies...>> &cget(
      const aux::pool_type<sm_impl<sm_policy<T, aux::identity<U>, TPolicies...>>> *object) {
    return static_cast<const aux::pool_type<sm_impl<sm_policy<T, aux::identity<U>, TPolicies...>>> &>(*object).value;
  }
};
template <class T, class... TPolicies>
struct rebind_impl {
  using type = sm_policy<T, TPolicies...>;
};
template <class T, class... TDetails, class... TPolicies>
struct rebind_impl<sm_policy<T, TDetails...>, TPolicies...> {
  using type = sm_policy<T, TDetails..., TPolicies...>;
};
template <class T, class... TDetails, class... TPolicies>
struct rebind_impl<sm<sm_policy<T, TDetails...>>, TPolicies...> {
  using type = sm_policy<T, TDetails..., TPolicies...>;
};
template <class, class>
struct convert_to_sm;
template <class T, class... Ts>
struct convert_to_sm<T, aux::type_list<Ts...>> {
  using type = aux::type_list<sm_impl<T>, sm_impl<typename T::template rebind<Ts>>...>;
};
}  // namespace back
namespace back {
template <class>
class sm;
template <class, class...>
struct sm_policy;
struct thread_safety_policy__ {};
struct defer_queue_policy__ {};
struct logger_policy__ {};
struct testing_policy__ {};
struct dispatch_policy__ {};
struct no_policy {
  using type = no_policy;
  template <class>
  using rebind = no_policy;
  template <class...>
  using defer = no_policy;
  __BOOST_SML_ZERO_SIZE_ARRAY(aux::byte);
};
template <class TDefault, class>
TDefault get_policy(...);
template <class, class T, class TPolicy>
TPolicy get_policy(aux::pair<T, TPolicy> *);
template <class, class...>
struct sm_policy;
template <class SM, class... TPolicies>
struct sm_policy {
  using sm = SM;
  using thread_safety_policy = decltype(get_policy<no_policy, thread_safety_policy__>((aux::inherit<TPolicies...> *)0));
  using defer_queue_policy = decltype(get_policy<no_policy, defer_queue_policy__>((aux::inherit<TPolicies...> *)0));
  using logger_policy = decltype(get_policy<no_policy, logger_policy__>((aux::inherit<TPolicies...> *)0));
  using testing_policy = decltype(get_policy<no_policy, testing_policy__>((aux::inherit<TPolicies...> *)0));
  using dispatch_policy = decltype(get_policy<struct branch_stm, dispatch_policy__>((aux::inherit<TPolicies...> *)0));
  template <class T>
  using rebind = typename rebind_impl<T, TPolicies...>::type;
};
template <class...>
struct state_machine;
template <class>
struct get_state_name;
template <class T>
struct get_state_name<aux::string<T>> {
  using type = aux::string<T>;
};
template <class T, class... Ts>
struct get_state_name<aux::string<sm<sm_policy<T, Ts...>>>> {
  using type = typename get_state_name<aux::string<sm_policy<T, Ts...>>>::type;
};
template <class T>
struct get_state_name<aux::string<sm_policy<T>>> {
  using type = aux::string<T>;
};
template <class T, class TName>
struct get_state_name<aux::string<sm_policy<T, TName>>> {
  using type = aux::string<T(typename TName::type)>;
};
template <class T>
using get_state_name_t = typename get_state_name<T>::type;
template <class, class TDeps, class TEvent>
void log_process_event(const aux::type<no_policy> &, const void *, TDeps &, const TEvent &) {}
template <class SM, class TLogger, class TDeps, class TEvent>
void log_process_event(const aux::type<TLogger> &, const void *sm, TDeps &deps, const TEvent &event) {
  return static_cast<aux::pool_type<TLogger &> &>(deps.deps).value.template log_process_event<SM>(sm, event);
}
template <class, class TDeps, class TSrcState, class TDstState>
void log_state_change(const aux::type<no_policy> &, const void *, TDeps &, const TSrcState &, const TDstState &) {}
template <class SM, class TLogger, class TDeps, class TSrcState, class TDstState>
void log_state_change(const aux::type<TLogger> &, const void *sm, TDeps &deps, const TSrcState &, const TDstState &) {
  return static_cast<aux::pool_type<TLogger &> &>(deps.deps).value.template log_state_change<SM>(sm, get_state_name_t<TSrcState>{},
                                                                                            get_state_name_t<TDstState>{});
}
template <class, class TDeps, class TAction, class TEvent>
void log_call(const aux::type<no_policy> &, const void *, TDeps &, const TAction &, const TEvent &) {}
template <class SM, class TLogger, class TDeps, class TAction, class TEvent>
void log_call(const aux::type<TLogger> &, const void *sm, TDeps &deps, const TAction &action, const TEvent &event) {
  return static_cast<aux::pool_type<TLogger &> &>(deps.deps).value.template log_call<SM>(sm, action, event);
}
template <class SM, class TLogger, class TDeps, class TAction, class TEvent>
void log_call(const aux::type<TLogger> &, const void *sm, TDeps &deps, const aux::zero_wrapper<TAction> &action,
              const TEvent &event) {
  return static_cast<aux::pool_type<TLogger &> &>(deps.deps).value.template log_call<SM>(sm, action.get(), event);
}
template <class, class TDeps, class TGuard, class TEvent, class Q>
void log_call(const aux::type<no_policy> &, const void *, TDeps &, const TGuard &, const TEvent &, Q) {}
template <class SM, class TLogger, class TDeps, class TGuard, class TEvent, class Q>
void log_call(const aux::type<TLogger> &, const void *sm, TDeps &deps, const TGuard &guard, const TEvent &event, Q result) {
  return static_cast<aux::pool_type<TLogger &> &>(deps.deps).value.template log_call<SM>(sm, guard, event, result);
}
template <class SM, class TLogger, class TDeps, class TGuard, class TEvent, class Q>
void log_call(const aux::type<TLogger> &, const void *sm, TDeps &deps, const aux::zero_wrapper<TGuard> &guard,
              const TEvent &event, Q result) {
  return static_cast<aux::pool_type<TLogger &> &>(deps.deps).value.template log_call<SM>(sm, guard.get(), event, result);
}
}  // namespace back
namespace back {
template <class>
class sm;
template <class>
struct sm_impl;
template <class...>
struct transitions;
template <class...>
struct transitions_sub;
template <class T, class... Ts>
struct transitions<T, Ts...> {
  template <class TEvent, class SM, class TDeps, class TSubs>
  constexpr static bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state) {
    if (aux::get<T>(sm.transitions_).execute(event, sm, deps, subs, current_state, typename SM::has_entry_exits{})) {
      return true;
    }
    return transitions<Ts...>::execute(event, sm, deps, subs, current_state);
  }
};
template <class T>
struct transitions<T> {
  template <class TEvent, class SM, class TDeps, class TSubs>
  constexpr static bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state) {
    return execute_impl(event, sm, deps, subs, current_state);
  }
  template <class TEvent, class SM, class TDeps, class TSubs>
  constexpr static bool execute_impl(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs,
                                     typename SM::state_t &current_state) {
    return aux::get<T>(sm.transitions_).execute(event, sm, deps, subs, current_state, typename SM::has_entry_exits{});
  }
  template <class _, class TEvent, class SM, class TDeps, class TSubs>
  constexpr static bool execute_impl(const on_exit<_, TEvent> &event, SM &sm, TDeps &deps, TSubs &subs,
                                     typename SM::state_t &current_state) {
    aux::get<T>(sm.transitions_).execute(event, sm, deps, subs, current_state, typename SM::has_entry_exits{});
    return false;
  }
};
template <>
struct transitions<aux::true_type> {
  template <class TEvent, class SM, class TDeps, class TSubs>
  constexpr static bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state) {
    sm.process_internal_event(unexpected_event<TEvent>{event}, deps, subs, current_state);
    return false;
  }
};
template <>
struct transitions<aux::false_type> {
  template <class TEvent, class SM, class TDeps, class TSubs>
  constexpr static bool execute(const TEvent &, SM &, TDeps &, TSubs &, typename SM::state_t &) {
    return false;
  }
};
template <class TSM, class T, class... Ts>
struct transitions_sub<sm<TSM>, T, Ts...> {
  template <class TEvent, class SM, class TDeps, class TSubs>
  constexpr static bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state) {
    return execute_impl(event, sm, deps, subs, current_state);
  }
  template <class, class SM, class TDeps, class TSubs>
  constexpr static bool execute(const anonymous &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state) {
    if (sub_sm<sm_impl<TSM>>::cget(&subs).is_terminated()) {
      const auto handled = sub_sm<sm_impl<TSM>>::get(&subs).process_event(event, deps, subs);
      return handled ? handled : transitions<T, Ts...>::execute(event, sm, deps, subs, current_state);
    }
    return false;
  }
  template <class TEvent, class SM, class TDeps, class TSubs>
  constexpr static bool execute_impl(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs,
                                     typename SM::state_t &current_state) {
    const auto handled = sub_sm<sm_impl<TSM>>::get(&subs).process_event(event, deps, subs);
    return handled ? handled : transitions<T, Ts...>::execute(event, sm, deps, subs, current_state);
  }
  template <class _, class TEvent, class SM, class TDeps, class TSubs>
  constexpr static bool execute_impl(const back::on_entry<_, TEvent> &event, SM &sm, TDeps &deps, TSubs &subs,
                                     typename SM::state_t &current_state) {
    transitions<T, Ts...>::execute(event, sm, deps, subs, current_state);
    sub_sm<sm_impl<TSM>>::get(&subs).process_event(event, deps, subs);
    return true;
  }
};
template <class TSM>
struct transitions_sub<sm<TSM>> {
  template <class TEvent, class SM, class TDeps, class TSubs>
  constexpr static bool execute(const TEvent &event, SM &, TDeps &deps, TSubs &subs, typename SM::state_t &) {
    sub_sm<sm_impl<TSM>>::get(&subs).template process_event<TEvent>(event, deps, subs);
    return true;
  }
};
}  // namespace back
namespace back {
template <class>
class sm;
template <class>
struct state;
template <class>
struct event;
template <class...>
struct transitions;
template <class...>
struct transitions_sub;
template <class, class>
struct state_mappings;
template <class S, class... Ts>
struct state_mappings<S, aux::type_list<Ts...>> {
  using element_type = state<S>;
  using types = aux::type_list<Ts...>;
};
template <class, class>
struct event_mappings;
template <class E, class... Ts>
struct event_mappings<E, aux::inherit<Ts...>> {
  using element_type = event<E>;
  using types = aux::type_list<Ts...>;
};
template <class E, class... Es, class... Ts>
struct event_mappings<aux::type_list<E, Es...>, aux::inherit<Ts...>> {
  using element_type = event<E>;
  using types = aux::type_list<Ts...>;
};
template <class...>
struct unique_mappings;
template <class, class...>
struct unique_mappings_impl;
template <class... Ts>
using unique_mappings_t = typename unique_mappings<Ts...>::type;
template <class, class, class, class R>
struct get_mapping : aux::type_list<R> {};
template <class E, class T, class R>
struct get_mapping<event<E>, event<E>, T, R>
    : aux::type_list<event_mappings<E, aux::apply_t<unique_mappings_t, aux::join_t<typename R::types, typename T::types>>>> {};
template <class S, class T, class R>
struct get_mapping<state<S>, state<S>, T, R>
    : aux::type_list<state_mappings<S, aux::join_t<typename R::types, typename T::types>>> {};
template <class T, class... Ts>
struct extend_mapping : aux::join_t<typename get_mapping<typename T::element_type, typename Ts::element_type, T, Ts>::type...> {
};
template <class T, class... Ts>
using extend_mapping_t = aux::apply_t<aux::inherit, typename extend_mapping<T, Ts...>::type>;
template <bool, class, class...>
struct conditional_mapping;
template <class T1, class T2, class... Rs, class... Ts>
struct conditional_mapping<true, aux::type<T1, aux::inherit<Rs...>>, T2, Ts...> {
  using type = unique_mappings_impl<aux::type<aux::inherit<T1>, extend_mapping_t<T2, Rs...>>, Ts...>;
};
template <class T1, class T2, class... Rs, class... Ts>
struct conditional_mapping<false, aux::type<T1, aux::inherit<Rs...>>, T2, Ts...> {
  using type =
      unique_mappings_impl<aux::type<aux::inherit<T1, aux::type<typename T2::element_type>>, aux::inherit<T2, Rs...>>, Ts...>;
};
template <class T1, class T2, class... Rs, class... Ts>
struct unique_mappings_impl<aux::type<T1, aux::inherit<Rs...>>, T2, Ts...>
    : conditional_mapping<aux::is_base_of<aux::type<typename T2::element_type>, T1>::value, aux::type<T1, aux::inherit<Rs...>>,
                          T2, Ts...>::type {};
template <class T1, class Rs>
struct unique_mappings_impl<aux::type<T1, Rs>> : aux::apply_t<aux::inherit, Rs> {};
template <class... Ts>
struct unique_mappings : unique_mappings_impl<aux::type<aux::none_type, aux::inherit<>>, Ts...> {};
template <class T>
struct unique_mappings<T> : aux::inherit<T> {};
template <class, class...>
struct mappings;
template<class T, class TEvent>
struct event_mappings_impl {
  using type = aux::type_list<
    event_mappings<TEvent, aux::inherit<state_mappings<typename T::src_state, aux::type_list<T>>>>
  >;
};
template<class T, class... TEvents>
struct event_mappings_impl<T, aux::type_list<TEvents...>> {
  using type = aux::type_list<
    event_mappings<TEvents, aux::inherit<state_mappings<typename T::src_state, aux::type_list<T>>>>...
  >;
};
template <template<class...> class T, class... Ts>
struct mappings<T<Ts...>>
  : aux::apply_t<
      unique_mappings_t,
      aux::join_t<
        typename event_mappings_impl<Ts, typename Ts::event>::type...
      >
    >
{};
template <class T>
using mappings_t = typename mappings<T>::type;
template <class, class TUnexpected>
transitions<TUnexpected> get_state_mapping_impl(...);
template <class T, class, class... Ts>
transitions<Ts...> get_state_mapping_impl(state_mappings<T, aux::type_list<Ts...>> *);
template <class T, class TMappings, class TUnexpected>
struct get_state_mapping {
  using type = decltype(get_state_mapping_impl<T, TUnexpected>((TMappings *)0));
};
#if 0
template <class S>
transitions_sub<S> get_sub_state_mapping_impl(...);
template <class T, class... Ts>
transitions_sub<T, Ts...> get_sub_state_mapping_impl(state_mappings<T, aux::type_list<Ts...>> *);
template <class T, class TMappings, class TUnexpected>
struct get_state_mapping<sm<T>, TMappings, TUnexpected> {
  using type = decltype(get_sub_state_mapping_impl<sm<T>>((TMappings *)0));
};
#endif
template <class T, class TMappings, class TUnexpected>
using get_state_mapping_t = typename get_state_mapping<T, TMappings, TUnexpected>::type;
template <class>
transitions<aux::true_type> get_event_mapping_impl(...);

template <class>
transitions<aux::true_type> get_event_mapping_impl(...);
template <class T, class TMappings>
TMappings get_event_mapping_impl(event_mappings<T, TMappings> *);
template <class T, class... T1Mappings, class... T2Mappings>
unique_mappings_t<T1Mappings..., T2Mappings...> get_event_mapping_impl(event_mappings<T, aux::inherit<T1Mappings...>> *,
                                                                       event_mappings<_, aux::inherit<T2Mappings...>> *);
template <class T, class TMappings>
struct get_event_mapping_impl_helper
    : aux::conditional<aux::is_same<transitions<aux::true_type>, decltype(get_event_mapping_impl<_>((TMappings *)0))>::value,
                       decltype(get_event_mapping_impl<T>((TMappings *)0)),
                       decltype(get_event_mapping_impl<T>((TMappings *)0, (TMappings *)0))>::type {};
template <class T, class TMappings>
struct get_event_mapping_impl_helper<exception<T>, TMappings> : decltype(get_event_mapping_impl<exception<T>>((TMappings *)0)) {
};
template <class T1, class T2, class TMappings>
struct get_event_mapping_impl_helper<unexpected_event<T1, T2>, TMappings>
    : decltype(get_event_mapping_impl<unexpected_event<T1, T2>>((TMappings *)0)) {};
template <class T1, class T2, class TMappings>
struct get_event_mapping_impl_helper<on_entry<T1, T2>, TMappings>
    : decltype(get_event_mapping_impl<on_entry<T1, T2>>((TMappings *)0)) {};
template <class T1, class T2, class TMappings>
struct get_event_mapping_impl_helper<on_exit<T1, T2>, TMappings>
    : decltype(get_event_mapping_impl<on_exit<T1, T2>>((TMappings *)0)) {};
template <class T, class TMappings>
using get_event_mapping_t = get_event_mapping_impl_helper<T, TMappings>;

}  // namespace back
namespace concepts {
struct callable_fallback {
  constexpr void operator()();
};
template <class T>
constexpr aux::false_type test_callable(aux::non_type<void (callable_fallback::*)(), &T::operator()> *);
template <class>
constexpr aux::true_type test_callable(...);

template <bool, class T>
struct cimpl : decltype(test_callable<aux::inherit<T, callable_fallback>>(0)) {};
template <class T>
struct cimpl<false, T> : aux::false_type {};
template <class, class T>
struct callable : cimpl<__is_class(T) && !__is_final(T), T> {};
}  // namespace concepts
#if !defined(BOOST_SML_DISABLE_EXCEPTIONS)
#define BOOST_SML_DISABLE_EXCEPTIONS !(defined(__cpp_exceptions) || defined(__EXCEPTIONS))
#endif
namespace back {
struct jump_table {
  template <int, class TMappings, class sm_impl, class State, class TEvent, class TDeps, class TSubs>
  [[gnu::always_inline]] static bool dispatch(sm_impl &, State &, const TEvent &, TDeps &, TSubs &, const aux::type_list<> &) {
    return false;
  }
  template <int, class TMappings, class sm_impl, class State, class TEvent, class TDeps, class TSubs, class... TStates>
  [[gnu::always_inline]] static bool dispatch(sm_impl &self, State &current_state, const TEvent &event, TDeps &deps,
                                              TSubs &subs, const aux::type_list<TStates...> &) {
    using dispatch_table_t = bool (*)(const TEvent &, sm_impl &, TDeps &, TSubs &, State &);
    constexpr static dispatch_table_t dispatch_table[__BOOST_SML_ZERO_SIZE_ARRAY_CREATE(sizeof...(TStates))] = {
        &get_state_mapping_t<TStates, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl,
                                                                                                            TDeps, TSubs>...};
    return dispatch_table[current_state](event, self, deps, subs, current_state);
  }
};

struct branch_stm {
  template <int, class TMappings, class sm_impl, class State, class TEvent, class TDeps, class TSubs>
  [[gnu::always_inline]] static bool dispatch(sm_impl &, State &, const TEvent &, TDeps &, TSubs &, const aux::type_list<> &) {
    return false;
  }
  template <int N, class TMappings, class sm_impl, class State, class TEvent, class TDeps, class TSubs, class TState,
            class... TStates>
  [[gnu::always_inline]] static bool dispatch(sm_impl &self, State &current_state, const TEvent &event, TDeps &deps,
                                              TSubs &subs, const aux::type_list<TState, TStates...> &) {
    return current_state == N
               ? get_state_mapping_t<TState, TMappings, typename sm_impl::has_unexpected_events>::template execute<
                     TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state)
               : dispatch<N + 1, TMappings>(self, current_state, event, deps, subs, aux::type_list<TStates...>{});
    return false;
  }
};

struct switch_stm {
  template <int, class TMappings, class sm_impl, class State, class TEvent, class TDeps, class TSubs>
  [[gnu::always_inline]] static bool dispatch(sm_impl &, State &, const TEvent &, TDeps &, TSubs &, const aux::type_list<> &) {
    return false;
  }
  template <int N, class TMappings, class sm_impl, class State, class TEvent, class TDeps, class TSubs, class TState,
            class... TStates>
  [[gnu::always_inline]] static bool dispatch(sm_impl &self, State &current_state, const TEvent &event, TDeps &deps,
                                              TSubs &subs, const aux::type_list<TState, TStates...> &) {
    switch (current_state) {
      case N:
        return get_state_mapping_t<TState, TMappings, typename sm_impl::has_unexpected_events>::template execute<
            TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
      default:
        return dispatch<N + 1, TMappings>(self, current_state, event, deps, subs, aux::type_list<TStates...>{});
    }
    return false;
  }
};

struct fold_expr {
  template <class TMappings, auto... Ns, class sm_impl, class State, class TEvent, class TDeps, class TSubs, class... TStates>
  [[gnu::always_inline]] static bool dispatch_impl(sm_impl &self, State &current_state, aux::index_sequence<Ns...>,
                                                   const TEvent &event, TDeps &deps, TSubs &subs,
                                                   const aux::type_list<TStates...> &) {
    return ((current_state == Ns
                 ? get_state_mapping_t<TStates, TMappings, typename sm_impl::has_unexpected_events>::template execute<
                       TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state)
                 : false) ||
            ...);
  }

  template <int N, class TMappings, class sm_impl, class State, class TEvent, class TDeps, class TSubs, class... TStates>
  [[gnu::always_inline]] static bool dispatch(sm_impl &self, State &current_state, const TEvent &event, TDeps &deps,
                                              TSubs &subs, const aux::type_list<TStates...> &states) {
    return dispatch_impl<TMappings>(self, current_state, aux::make_index_sequence<sizeof...(TStates)>{}, event, deps, subs,
                                    states);
  }
};

struct goto_stm {
  template <int N, class TMappings, class sm_impl, class State, class TEvent, class TDeps, class TSubs, class TState1, class TState2>
  [[gnu::always_inline]] static bool dispatch(sm_impl &self, State &current_state, const TEvent &event, TDeps &deps, TSubs &subs,
                       const aux::type_list<TState1, TState2> &) {
    static constexpr const void* dispatch_table[] = { &&state1, &&state2 };
    goto *dispatch_table[current_state];
    state1: return back::get_state_mapping_t<TState1, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state2: return back::get_state_mapping_t<TState2, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
  }
  template <int N, class TMappings, class sm_impl, class State, class TEvent, class TDeps, class TSubs, class TState1, class TState2, class TState3>
  [[gnu::always_inline]] static bool dispatch(sm_impl &self, State &current_state, const TEvent &event, TDeps &deps, TSubs &subs,
                       const aux::type_list<TState1, TState2, TState3> &) {
    static constexpr const void* dispatch_table[] = { &&state1, &&state2, &&state3 };
    goto *dispatch_table[current_state];
    state1: return back::get_state_mapping_t<TState1, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state2: return back::get_state_mapping_t<TState2, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state3: return back::get_state_mapping_t<TState3, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
  }
  template <int N, class TMappings, class sm_impl, class State, class TEvent, class TDeps, class TSubs, class TState1, class TState2, class TState3, class TState4>
  [[gnu::always_inline]] static bool dispatch(sm_impl &self, State &current_state, const TEvent &event, TDeps &deps, TSubs &subs,
                       const aux::type_list<TState1, TState2, TState3, TState4> &) {
    static constexpr const void* dispatch_table[] = { &&state1, &&state2, &&state3, &&state4 };
    goto *dispatch_table[current_state];
    state1: return back::get_state_mapping_t<TState1, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state2: return back::get_state_mapping_t<TState2, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state3: return back::get_state_mapping_t<TState3, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state4: return back::get_state_mapping_t<TState4, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
  }
  template <int N, class TMappings, class sm_impl, class State, class TEvent, class TDeps, class TSubs, class TState1, class TState2, class TState3, class TState4, class TState5>
  [[gnu::always_inline]] static bool dispatch(sm_impl &self, State &current_state, const TEvent &event, TDeps &deps, TSubs &subs,
                       const aux::type_list<TState1, TState2, TState3, TState4, TState5> &) {
    static constexpr const void* dispatch_table[] = { &&state1, &&state2, &&state3, &&state4, &&state5 };
    goto *dispatch_table[current_state];
    state1: return back::get_state_mapping_t<TState1, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state2: return back::get_state_mapping_t<TState2, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state3: return back::get_state_mapping_t<TState3, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state4: return back::get_state_mapping_t<TState4, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state5: return back::get_state_mapping_t<TState5, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
  }
  template <int N, class TMappings, class sm_impl, class State, class TEvent, class TDeps, class TSubs, class TState1, class TState2, class TState3, class TState4, class TState5, class TState6>
  [[gnu::always_inline]] static bool dispatch(sm_impl &self, State &current_state, const TEvent &event, TDeps &deps, TSubs &subs,
                       const aux::type_list<TState1, TState2, TState3, TState4, TState5, TState6> &) {
    static constexpr const void* dispatch_table[] = { &&state1, &&state2, &&state3, &&state4, &&state5, &&state6 };
    goto *dispatch_table[current_state];
    state1: return back::get_state_mapping_t<TState1, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state2: return back::get_state_mapping_t<TState2, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state3: return back::get_state_mapping_t<TState3, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state4: return back::get_state_mapping_t<TState4, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state5: return back::get_state_mapping_t<TState5, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state6: return back::get_state_mapping_t<TState6, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
  }
  template <int N, class TMappings, class sm_impl, class State, class TEvent, class TDeps, class TSubs, class TState1, class TState2, class TState3, class TState4, class TState5, class TState6, class TState7>
  [[gnu::always_inline]] static bool dispatch(sm_impl &self, State &current_state, const TEvent &event, TDeps &deps, TSubs &subs,
                       const aux::type_list<TState1, TState2, TState3, TState4, TState5, TState6, TState7> &) {
    static constexpr const void* dispatch_table[] = { &&state1, &&state2, &&state3, &&state4, &&state5, &&state6, &&state7 };
    goto *dispatch_table[current_state];
    state1: return back::get_state_mapping_t<TState1, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state2: return back::get_state_mapping_t<TState2, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state3: return back::get_state_mapping_t<TState3, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state4: return back::get_state_mapping_t<TState4, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state5: return back::get_state_mapping_t<TState5, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state6: return back::get_state_mapping_t<TState6, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state7: return back::get_state_mapping_t<TState7, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
  }
  template <int N, class TMappings, class sm_impl, class State, class TEvent, class TDeps, class TSubs, class TState1, class TState2, class TState3, class TState4, class TState5, class TState6, class TState7, class TState8>
  [[gnu::always_inline]] static bool dispatch(sm_impl &self, State &current_state, const TEvent &event, TDeps &deps, TSubs &subs,
                       const aux::type_list<TState1, TState2, TState3, TState4, TState5, TState6, TState7, TState8> &) {
    static constexpr const void* dispatch_table[] = { &&state1, &&state2, &&state3, &&state4, &&state5, &&state6, &&state7, &&state8 };
    goto *dispatch_table[current_state];
    state1: return back::get_state_mapping_t<TState1, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state2: return back::get_state_mapping_t<TState2, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state3: return back::get_state_mapping_t<TState3, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state4: return back::get_state_mapping_t<TState4, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state5: return back::get_state_mapping_t<TState5, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state6: return back::get_state_mapping_t<TState6, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state7: return back::get_state_mapping_t<TState7, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state8: return back::get_state_mapping_t<TState8, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
  }
  template <int N, class TMappings, class sm_impl, class State, class TEvent, class TDeps, class TSubs, class TState1, class TState2, class TState3, class TState4, class TState5, class TState6, class TState7, class TState8, class TState9>
  [[gnu::always_inline]] static bool dispatch(sm_impl &self, State &current_state, const TEvent &event, TDeps &deps, TSubs &subs,
                       const aux::type_list<TState1, TState2, TState3, TState4, TState5, TState6, TState7, TState8, TState9> &) {
    static constexpr const void* dispatch_table[] = { &&state1, &&state2, &&state3, &&state4, &&state5, &&state6, &&state7, &&state8, &&state9};
    goto *dispatch_table[current_state];
    state1: return back::get_state_mapping_t<TState1, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state2: return back::get_state_mapping_t<TState2, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state3: return back::get_state_mapping_t<TState3, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state4: return back::get_state_mapping_t<TState4, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state5: return back::get_state_mapping_t<TState5, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state6: return back::get_state_mapping_t<TState6, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state7: return back::get_state_mapping_t<TState7, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state8: return back::get_state_mapping_t<TState8, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state9: return back::get_state_mapping_t<TState9, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
  }
  template <int N, class TMappings, class sm_impl, class State, class TEvent, class TDeps, class TSubs, class TState1, class TState2, class TState3, class TState4, class TState5, class TState6, class TState7, class TState8, class TState9, class TState10>
  [[gnu::always_inline]] static bool dispatch(sm_impl &self, State &current_state, const TEvent &event, TDeps &deps, TSubs &subs,
                       const aux::type_list<TState1, TState2, TState3, TState4, TState5, TState6, TState7, TState8, TState9, TState10> &) {
    static constexpr const void* dispatch_table[] = { &&state1, &&state2, &&state3, &&state4, &&state5, &&state6, &&state7, &&state8, &&state9, &&state10 };
    goto *dispatch_table[current_state];
    state1: return back::get_state_mapping_t<TState1, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state2: return back::get_state_mapping_t<TState2, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state3: return back::get_state_mapping_t<TState3, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state4: return back::get_state_mapping_t<TState4, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state5: return back::get_state_mapping_t<TState5, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state6: return back::get_state_mapping_t<TState6, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state7: return back::get_state_mapping_t<TState7, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state8: return back::get_state_mapping_t<TState8, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state9: return back::get_state_mapping_t<TState9, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state10: return back::get_state_mapping_t<TState10, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
  }
  template <int N, class TMappings, class sm_impl, class State, class TEvent, class TDeps, class TSubs, class TState1, class TState2, class TState3, class TState4, class TState5, class TState6, class TState7, class TState8, class TState9, class TState10, class TState11>
  [[gnu::always_inline]] static bool dispatch(sm_impl &self, State &current_state, const TEvent &event, TDeps &deps, TSubs &subs,
                       const aux::type_list<TState1, TState2, TState3, TState4, TState5, TState6, TState7, TState8, TState9, TState10, TState11> &) {
    static constexpr const void* dispatch_table[] = { &&state1, &&state2, &&state3, &&state4, &&state5, &&state6, &&state7, &&state8, &&state9, &&state10, &&state11 };
    goto *dispatch_table[current_state];
    state1: return back::get_state_mapping_t<TState1, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state2: return back::get_state_mapping_t<TState2, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state3: return back::get_state_mapping_t<TState3, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state4: return back::get_state_mapping_t<TState4, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state5: return back::get_state_mapping_t<TState5, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state6: return back::get_state_mapping_t<TState6, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state7: return back::get_state_mapping_t<TState7, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state8: return back::get_state_mapping_t<TState8, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state9: return back::get_state_mapping_t<TState9, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state10: return back::get_state_mapping_t<TState10, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state11: return back::get_state_mapping_t<TState11, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
  }
  template <int N, class TMappings, class sm_impl, class State, class TEvent, class TDeps, class TSubs, class TState1, class TState2, class TState3, class TState4, class TState5, class TState6, class TState7, class TState8, class TState9, class TState10, class TState11, class TState12>
  [[gnu::always_inline]] static bool dispatch(sm_impl &self, State &current_state, const TEvent &event, TDeps &deps, TSubs &subs,
                       const aux::type_list<TState1, TState2, TState3, TState4, TState5, TState6, TState7, TState8, TState9, TState10, TState11, TState12> &) {
    static constexpr const void* dispatch_table[] = { &&state1, &&state2, &&state3, &&state4, &&state5, &&state6, &&state7, &&state8, &&state9, &&state10, &&state11, &&state12 };
    goto *dispatch_table[current_state];
    state1: return back::get_state_mapping_t<TState1, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state2: return back::get_state_mapping_t<TState2, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state3: return back::get_state_mapping_t<TState3, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state4: return back::get_state_mapping_t<TState4, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state5: return back::get_state_mapping_t<TState5, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state6: return back::get_state_mapping_t<TState6, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state7: return back::get_state_mapping_t<TState7, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state8: return back::get_state_mapping_t<TState8, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state9: return back::get_state_mapping_t<TState9, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state10: return back::get_state_mapping_t<TState10, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state11: return back::get_state_mapping_t<TState11, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state12: return back::get_state_mapping_t<TState12, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
  }
  template <int N, class TMappings, class sm_impl, class State, class TEvent, class TDeps, class TSubs, class TState1, class TState2, class TState3, class TState4, class TState5, class TState6, class TState7, class TState8, class TState9, class TState10, class TState11, class TState12, class TState13>
  [[gnu::always_inline]] static bool dispatch(sm_impl &self, State &current_state, const TEvent &event, TDeps &deps, TSubs &subs,
                       const aux::type_list<TState1, TState2, TState3, TState4, TState5, TState6, TState7, TState8, TState9, TState10, TState11, TState12, TState13> &) {
    static constexpr const void* dispatch_table[] = { &&state1, &&state2, &&state3, &&state4, &&state5, &&state6, &&state7, &&state8, &&state9, &&state10, &&state11, &&state12, &&state13 };
    goto *dispatch_table[current_state];
    state1: return back::get_state_mapping_t<TState1, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state2: return back::get_state_mapping_t<TState2, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state3: return back::get_state_mapping_t<TState3, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state4: return back::get_state_mapping_t<TState4, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state5: return back::get_state_mapping_t<TState5, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state6: return back::get_state_mapping_t<TState6, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state7: return back::get_state_mapping_t<TState7, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state8: return back::get_state_mapping_t<TState8, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state9: return back::get_state_mapping_t<TState9, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state10: return back::get_state_mapping_t<TState10, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state11: return back::get_state_mapping_t<TState11, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state12: return back::get_state_mapping_t<TState12, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state13: return back::get_state_mapping_t<TState13, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
  }
  template <int N, class TMappings, class sm_impl, class State, class TEvent, class TDeps, class TSubs, class TState1, class TState2, class TState3, class TState4, class TState5, class TState6, class TState7, class TState8, class TState9, class TState10, class TState11, class TState12, class TState13, class TState14>
  [[gnu::always_inline]] static bool dispatch(sm_impl &self, State &current_state, const TEvent &event, TDeps &deps, TSubs &subs,
                       const aux::type_list<TState1, TState2, TState3, TState4, TState5, TState6, TState7, TState8, TState9, TState10, TState11, TState12, TState13, TState14> &) {
    static constexpr const void* dispatch_table[] = { &&state1, &&state2, &&state3, &&state4, &&state5, &&state6, &&state7, &&state8, &&state9, &&state10, &&state11, &&state12, &&state13, &&state14 };
    goto *dispatch_table[current_state];
    state1: return back::get_state_mapping_t<TState1, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state2: return back::get_state_mapping_t<TState2, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state3: return back::get_state_mapping_t<TState3, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state4: return back::get_state_mapping_t<TState4, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state5: return back::get_state_mapping_t<TState5, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state6: return back::get_state_mapping_t<TState6, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state7: return back::get_state_mapping_t<TState7, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state8: return back::get_state_mapping_t<TState8, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state9: return back::get_state_mapping_t<TState9, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state10: return back::get_state_mapping_t<TState10, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state11: return back::get_state_mapping_t<TState11, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state12: return back::get_state_mapping_t<TState12, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state13: return back::get_state_mapping_t<TState13, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state14: return back::get_state_mapping_t<TState14, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
  }
  template <int N, class TMappings, class sm_impl, class State, class TEvent, class TDeps, class TSubs, class TState1, class TState2, class TState3, class TState4, class TState5, class TState6, class TState7, class TState8, class TState9, class TState10, class TState11, class TState12, class TState13, class TState14, class TState15>
  [[gnu::always_inline]] static bool dispatch(sm_impl &self, State &current_state, const TEvent &event, TDeps &deps, TSubs &subs,
                       const aux::type_list<TState1, TState2, TState3, TState4, TState5, TState6, TState7, TState8, TState9, TState10, TState11, TState12, TState13, TState14, TState15> &) {
    static constexpr const void* dispatch_table[] = { &&state1, &&state2, &&state3, &&state4, &&state5, &&state6, &&state7, &&state8, &&state9, &&state10, &&state11, &&state12, &&state13, &&state14, &&state15 };
    goto *dispatch_table[current_state];
    state1: return back::get_state_mapping_t<TState1, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state2: return back::get_state_mapping_t<TState2, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state3: return back::get_state_mapping_t<TState3, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state4: return back::get_state_mapping_t<TState4, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state5: return back::get_state_mapping_t<TState5, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state6: return back::get_state_mapping_t<TState6, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state7: return back::get_state_mapping_t<TState7, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state8: return back::get_state_mapping_t<TState8, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state9: return back::get_state_mapping_t<TState9, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state10: return back::get_state_mapping_t<TState10, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state11: return back::get_state_mapping_t<TState11, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state12: return back::get_state_mapping_t<TState12, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state13: return back::get_state_mapping_t<TState13, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state14: return back::get_state_mapping_t<TState14, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state15: return back::get_state_mapping_t<TState15, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
  }
  template <int N, class TMappings, class sm_impl, class State, class TEvent, class TDeps, class TSubs, class TState1, class TState2, class TState3, class TState4, class TState5, class TState6, class TState7, class TState8, class TState9, class TState10, class TState11, class TState12, class TState13, class TState14, class TState15, class TState16>
  [[gnu::always_inline]] static bool dispatch(sm_impl &self, State &current_state, const TEvent &event, TDeps &deps, TSubs &subs,
                       const aux::type_list<TState1, TState2, TState3, TState4, TState5, TState6, TState7, TState8, TState9, TState10, TState11, TState12, TState13, TState14, TState15, TState16> &) {
    static constexpr const void* dispatch_table[] = { &&state1, &&state2, &&state3, &&state4, &&state5, &&state6, &&state7, &&state8, &&state9, &&state10, &&state11, &&state12, &&state13, &&state14, &&state15, &&state16 };
    goto *dispatch_table[current_state];
    state1: return back::get_state_mapping_t<TState1, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state2: return back::get_state_mapping_t<TState2, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state3: return back::get_state_mapping_t<TState3, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state4: return back::get_state_mapping_t<TState4, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state5: return back::get_state_mapping_t<TState5, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state6: return back::get_state_mapping_t<TState6, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state7: return back::get_state_mapping_t<TState7, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state8: return back::get_state_mapping_t<TState8, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state9: return back::get_state_mapping_t<TState9, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state10: return back::get_state_mapping_t<TState10, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state11: return back::get_state_mapping_t<TState11, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state12: return back::get_state_mapping_t<TState12, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state13: return back::get_state_mapping_t<TState13, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state14: return back::get_state_mapping_t<TState14, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state15: return back::get_state_mapping_t<TState15, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
    state16: return back::get_state_mapping_t<TState16, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
  }
};

template <class TSM>
struct sm_impl {
  using sm_t = typename TSM::sm;
  using thread_safety_t = typename TSM::thread_safety_policy::type;
  using defer_policy_t = typename TSM::defer_queue_policy;
  template <class T>
  using defer_queue_t = typename defer_policy_t::template rebind<T>;
  template <class... Ts>
  using defer_event_t = typename defer_policy_t::template defer<Ts...>;
  using logger_t = typename TSM::logger_policy::type;
  using dispatch_t = typename TSM::dispatch_policy;
  using transitions_t = decltype(aux::declval<sm_t>().operator()());
  using states_t = aux::apply_t<aux::unique_t, aux::apply_t<get_states, transitions_t>>;
  using states_ids_t = aux::apply_t<aux::type_id, states_t>;
  using initial_states_t = aux::apply_t<aux::unique_t, aux::apply_t<get_initial_states, transitions_t>>;
  using initial_states_ids_t = aux::apply_t<aux::type_id, initial_states_t>;
  using history_states_t = aux::apply_t<get_history_states, transitions_t>;
  using has_history_states =
      aux::integral_constant<bool, aux::size<initial_states_t>::value != aux::size<history_states_t>::value>;
  using sub_internal_events_t = aux::apply_t<get_sub_internal_events, transitions_t>;
  using events_t = aux::apply_t<aux::unique_t, aux::join_t<sub_internal_events_t, aux::apply_t<get_events, transitions_t>>>;
  using events_ids_t = aux::apply_t<aux::inherit, aux::apply_t<aux::wrap_t, events_t>>;
  using has_unexpected_events = typename aux::is_base_of<unexpected, aux::apply_t<aux::inherit, events_t>>::type;
  using has_entry_exits = typename aux::is_base_of<entry_exit, aux::apply_t<aux::inherit, events_t>>::type;
  using defer_t = defer_queue_t<aux::apply_t<defer_event_t, events_t>>;
  using deps = aux::apply_t<merge_deps, transitions_t>;
  using state_t = aux::conditional_t<(aux::size<states_t>::value > 0xFF), unsigned short, aux::byte>;
  static constexpr auto regions = aux::size<initial_states_t>::value;
  static_assert(regions > 0 || aux::is_same<aux::type_list<void>, states_t>::value, "At least one initial state is required");
#if !BOOST_SML_DISABLE_EXCEPTIONS
  using exceptions = aux::apply_t<aux::unique_t, aux::apply_t<get_exceptions, events_t>>;
  using has_exceptions = aux::integral_constant<bool, (aux::size<exceptions>::value > 0)>;
#endif
  struct mappings : mappings_t<transitions_t> {};

  template<class T>
  sm_impl(T &sm) : transitions_{sm()} {
    initialize(typename sm_impl<TSM>::initial_states_t{});
  }
  sm_impl(sm_impl &&) = default;
  sm_impl(const sm_impl &) = delete;
  sm_impl &operator=(const sm_impl &) = delete;
  template <class TEvent, class TDeps, class TSubs>
  bool process_event(const TEvent &event, TDeps &deps, TSubs &subs) {
    log_process_event<sm_t>(aux::type<logger_t>{}, this, deps, event);
#if BOOST_SML_DISABLE_EXCEPTIONS
    const auto handled = process_event_impl<get_event_mapping_t<get_generic_t<TEvent>, mappings>>(
        event, deps, subs, states_t{}, aux::make_index_sequence<regions>{});
#else
    const auto handled =
        process_event_noexcept<get_event_mapping_t<get_generic_t<TEvent>, mappings>>(event, deps, subs, has_exceptions{});
#endif
    process_internal_events(anonymous{}, deps, subs);
    process_defer_events(deps, subs, handled, aux::type<defer_queue_t<TEvent>>{}, events_t{});
    return handled;
  }
  void initialize(const aux::type_list<> &) {}
  template <class TState>
  void initialize(const aux::type_list<TState> &) {
    current_state_[0] = aux::get_id<state_t, TState>((states_ids_t *)0);
  }
  template <class... TStates>
  void initialize(const aux::type_list<TStates...> &) {
    auto region = 0, i = region;
    (void)aux::swallow{0, (region = i, current_state_[region] = aux::get_id<state_t, TStates>((states_ids_t *)0), ++i, 0)...};
  }
  template <class TDeps, class TSubs>
  void start(TDeps &deps, TSubs &subs) {
    process_internal_events(on_entry<_, initial>{}, deps, subs);
    process_internal_events(anonymous{}, deps, subs);
  }
  template <class TEvent, class TDeps, class TSubs, class... Ts,
            __BOOST_SML_REQUIRES(!aux::is_base_of<aux::wrapper<get_generic_t<TEvent>>, events_ids_t>::value &&
                                 !aux::is_base_of<aux::wrapper<get_mapped_t<TEvent>>, events_ids_t>::value)>
  bool process_internal_events(const TEvent &, TDeps &, TSubs &, Ts &&...) {
    return false;
  }
  template <class TEvent, class TDeps, class TSubs,
            __BOOST_SML_REQUIRES(aux::is_base_of<aux::wrapper<get_generic_t<TEvent>>, events_ids_t>::value &&
                                 !aux::is_base_of<aux::wrapper<get_mapped_t<TEvent>>, events_ids_t>::value)>
  bool process_internal_events(const TEvent &event, TDeps &deps, TSubs &subs) {
    log_process_event<sm_t>(aux::type<logger_t>{}, this, deps, event);
#if BOOST_SML_DISABLE_EXCEPTIONS
    return process_event_impl<get_event_mapping_t<get_generic_t<TEvent>, mappings>>(event, deps, subs, states_t{},
                                                                                    aux::make_index_sequence<regions>{});
#else
    return process_event_noexcept<get_event_mapping_t<get_generic_t<TEvent>, mappings>>(event, deps, subs, has_exceptions{});
#endif
  }
  template <class TEvent, class TDeps, class TSubs,
            __BOOST_SML_REQUIRES(aux::is_base_of<aux::wrapper<get_mapped_t<TEvent>>, events_ids_t>::value)>
  bool process_internal_events(const TEvent &event, TDeps &deps, TSubs &subs) {
    log_process_event<sm_t>(aux::type<logger_t>{}, this, deps, event);
#if BOOST_SML_DISABLE_EXCEPTIONS
    return process_event_impl<get_event_mapping_t<get_mapped_t<TEvent>, mappings>>(event, deps, subs, states_t{},
                                                                                   aux::make_index_sequence<regions>{});
#else
    return process_event_noexcept<get_event_mapping_t<get_mapped_t<TEvent>, mappings>>(event, deps, subs, has_exceptions{});
#endif
  }
  template <class TEvent, class TDeps, class TSubs, class... Ts,
            __BOOST_SML_REQUIRES(!aux::is_base_of<aux::wrapper<get_generic_t<TEvent>>, events_ids_t>::value &&
                                 !aux::is_base_of<aux::wrapper<get_mapped_t<TEvent>>, events_ids_t>::value)>
  bool process_internal_event(const TEvent &, TDeps &, TSubs &, Ts &&...) {
    return false;
  }
  template <class TEvent, class TDeps, class TSubs,
            __BOOST_SML_REQUIRES(aux::is_base_of<aux::wrapper<get_generic_t<TEvent>>, events_ids_t>::value &&
                                 !aux::is_base_of<aux::wrapper<get_mapped_t<TEvent>>, events_ids_t>::value)>
  bool process_internal_event(const TEvent &event, TDeps &deps, TSubs &subs, state_t &current_state) {
    log_process_event<sm_t>(aux::type<logger_t>{}, this, deps, event);
#if BOOST_SML_DISABLE_EXCEPTIONS
    return process_event_impl<get_event_mapping_t<get_generic_t<TEvent>, mappings>>(event, deps, subs, states_t{},
                                                                                    current_state);
#else
    return process_event_noexcept<get_event_mapping_t<get_generic_t<TEvent>, mappings>>(event, deps, subs, current_state,
                                                                                        has_exceptions{});
#endif
  }
  template <class TEvent, class TDeps, class TSubs,
            __BOOST_SML_REQUIRES(aux::is_base_of<aux::wrapper<get_mapped_t<TEvent>>, events_ids_t>::value)>
  bool process_internal_event(const TEvent &event, TDeps &deps, TSubs &subs, state_t &current_state) {
    log_process_event<sm_t>(aux::type<logger_t>{}, this, deps, event);
#if BOOST_SML_DISABLE_EXCEPTIONS
    return process_event_impl<get_event_mapping_t<get_mapped_t<TEvent>, mappings>>(event, deps, subs, states_t{},
                                                                                   current_state);
#else
    return process_event_noexcept<get_event_mapping_t<get_mapped_t<TEvent>, mappings>>(event, deps, subs, current_state,
                                                                                       has_exceptions{});
#endif
  }
  template <class TMappings, class TEvent, class TDeps, class TSubs, class TState>
  bool process_event_impl(const TEvent &event, TDeps &deps, TSubs &subs, const aux::type_list<TState> &, aux::index_sequence<>) {
    aux::byte unused{};
    return get_state_mapping_t<TState, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>(event, *this, deps, subs, unused);
  }

  template <class TMappings, class TEvent, class TDeps, class TSubs, class... TStates>
  bool process_event_impl(const TEvent &event, TDeps &deps, TSubs &subs, const aux::type_list<TStates...> &states,
                          aux::index_sequence<0>) {
    return dispatch_t::template dispatch<0, TMappings>(*this, current_state_[0], event, deps, subs, states);
  }
  template <class TMappings, class TEvent, class TDeps, class TSubs, class... TStates, int... Ns>
  bool process_event_impl(const TEvent &event, TDeps &deps, TSubs &subs, const aux::type_list<TStates...> &states,
                          aux::index_sequence<Ns...>) {
    return (dispatch_t::template dispatch<0, TMappings>(*this, current_state_[Ns], event, deps, subs, states) | ...);
  }
  template <class TMappings, class TEvent, class TDeps, class TSubs, class... TStates>
  bool process_event_impl(const TEvent &event, TDeps &deps, TSubs &subs, const aux::type_list<TStates...> &states,
                          state_t &current_state) {
    return dispatch_t::template dispatch<0, TMappings>(*this, current_state, event, deps, subs, states);
  }
#if !BOOST_SML_DISABLE_EXCEPTIONS
  template <class TMappings, class TEvent, class TDeps, class TSubs>
  bool process_event_noexcept(const TEvent &event, TDeps &deps, TSubs &subs, aux::false_type) noexcept {
    return process_event_impl<TMappings>(event, deps, subs, states_t{}, aux::make_index_sequence<regions>{});
  }
  template <class TMappings, class TEvent, class TDeps, class TSubs>
  bool process_event_noexcept(const TEvent &event, TDeps &deps, TSubs &subs, state_t &current_state, aux::false_type) noexcept {
    return process_event_impl<TMappings>(event, deps, subs, states_t{}, current_state);
  }
  template <class TMappings, class TEvent, class TDeps, class TSubs>
  bool process_event_noexcept(const TEvent &event, TDeps &deps, TSubs &subs, state_t &current_state, aux::true_type) noexcept {
    try {
      return process_event_impl<TMappings>(event, deps, subs, states_t{}, current_state);
    } catch (...) {
      return process_exception(deps, subs, exceptions{});
    }
  }
  template <class TMappings, class TEvent, class TDeps, class TSubs>
  bool process_event_noexcept(const TEvent &event, TDeps &deps, TSubs &subs, aux::true_type) {
    try {
      return process_event_impl<TMappings>(event, deps, subs, states_t{}, aux::make_index_sequence<regions>{});
    } catch (...) {
      return process_exception(deps, subs, exceptions{});
    }
  }
  template <class TDeps, class TSubs>
  bool process_exception(TDeps &deps, TSubs &subs, const aux::type_list<> &) {
    return process_internal_events(exception<_>{}, deps, subs);
  }
  template <class TDeps, class TSubs, class E, class... Es>
  bool process_exception(TDeps &deps, TSubs &subs, const aux::type_list<E, Es...> &) {
    try {
      throw;
    } catch (const typename E::type &e) {
      return process_internal_events(E{e}, deps, subs);
    } catch (...) {
      return process_exception(deps, subs, aux::type_list<Es...>{});
    }
  }
#endif
  template <class TDeps, class TSubs, class... TEvents>
  void process_defer_events(TDeps &, TSubs &, const bool, const aux::type<no_policy> &, const aux::type_list<TEvents...> &) {}
  template <class TDeps, class TSubs, class TEvent>
  bool process_event_no_defer(TDeps &deps, TSubs &subs, const void *data) {
    const auto &event = *static_cast<const TEvent *>(data);
    log_process_event<sm_t>(aux::type<logger_t>{}, this, deps, event);
#if BOOST_SML_DISABLE_EXCEPTIONS
    const auto handled = process_event_impl<get_event_mapping_t<TEvent, mappings>>(event, deps, subs, states_t{},
                                                                                   aux::make_index_sequence<regions>{});
#else
    const auto handled = process_event_noexcept<get_event_mapping_t<TEvent, mappings>>(event, deps, subs, has_exceptions{});
#endif
    if (handled) {
      defer_.pop();
    }
    return handled;
  }
  template <class TDeps, class TSubs, class TDeferQueue, class... TEvents>
  void process_defer_events(TDeps &deps, TSubs &subs, const bool handled, const aux::type<TDeferQueue> &,
                            const aux::type_list<TEvents...> &) {
    if (handled) {
      auto size = defer_.size();
      using dispatch_table_t = bool (sm_impl::*)(TDeps &, TSubs &, const void *);
      const static dispatch_table_t dispatch_table[__BOOST_SML_ZERO_SIZE_ARRAY_CREATE(sizeof...(TEvents))] = {
          &sm_impl::process_event_no_defer<TDeps, TSubs, TEvents>...};
      while (size-- && (this->*dispatch_table[defer_.front().id])(deps, subs, defer_.front().data))
        ;
    }
  }
  template <class TVisitor, class... TStates>
  void visit_current_states(const TVisitor &visitor, const aux::type_list<TStates...> &, aux::index_sequence<0>) const {
    using dispatch_table_t = void (*)(const TVisitor &);
    const static dispatch_table_t dispatch_table[__BOOST_SML_ZERO_SIZE_ARRAY_CREATE(sizeof...(TStates))] = {
        &sm_impl::visit_state<TVisitor, TStates>...};
    dispatch_table[current_state_[0]](visitor);
  }
  template <class TVisitor, class... TStates, int... Ns>
  void visit_current_states(const TVisitor &visitor, const aux::type_list<TStates...> &, aux::index_sequence<Ns...>) const {
    using dispatch_table_t = void (*)(const TVisitor &);
    const static dispatch_table_t dispatch_table[__BOOST_SML_ZERO_SIZE_ARRAY_CREATE(sizeof...(TStates))] = {
        &sm_impl::visit_state<TVisitor, TStates>...};
    (void)aux::swallow{0, (dispatch_table[current_state_[Ns]](visitor), 0)...};
  }
  template <class TVisitor, class TState>
  static void visit_state(const TVisitor &visitor) {
    visitor(aux::string<TState>{});
  }
  no_policy create_lock(const aux::type<no_policy> &) { return {}; }
  template <class TLockPolicy>
  auto create_lock(const aux::type<TLockPolicy> &) {
    struct lock_guard {
      explicit lock_guard(thread_safety_t &synch) : thread_safety_(synch) { thread_safety_.lock(); }
      ~lock_guard() { thread_safety_.unlock(); }
      thread_safety_t &thread_safety_;
    };
    return lock_guard{thread_safety_};
  }
  bool is_terminated() const { return is_terminated_impl(aux::make_index_sequence<regions>{}); }
  bool is_terminated_impl(aux::index_sequence<0>) const {
    return current_state_[0] == aux::get_id<state_t, terminate_state>((states_ids_t *)0);
  }
  template <int... Ns>
  bool is_terminated_impl(aux::index_sequence<Ns...>) const {
    auto result = true;
    (void)aux::swallow{
        0, (current_state_[Ns] == aux::get_id<state_t, terminate_state>((states_ids_t *)0) ? result : (result = false))...};
    return result;
  }
  transitions_t transitions_;
  state_t current_state_[regions];
  thread_safety_t thread_safety_;
  defer_t defer_;
};

template <class TSM>
class sm {
  using sm_t = typename TSM::sm;
  using logger_t = typename TSM::logger_policy::type;
  using logger_dep_t =
      aux::conditional_t<aux::is_same<no_policy, logger_t>::value, aux::type_list<>, aux::type_list<logger_t &>>;
  using transitions_t = decltype(aux::declval<sm_t>().operator()());

 public:
  using states = aux::apply_t<aux::unique_t, aux::apply_t<get_states, transitions_t>>;
  using state_machines = aux::apply_t<get_sub_sms, states>;
  using events = aux::apply_t<aux::unique_t, aux::apply_t<get_all_events, transitions_t>>;
  using transitions = aux::apply_t<aux::type_list, transitions_t>;

 private:
  using sm_all_t = aux::join_t<aux::type_list<sm_t>, aux::apply_t<get_sm_t, state_machines>>;
  using sub_sms_t = aux::apply_t<aux::pool, typename convert_to_sm<TSM, aux::apply_t<get_sub_sms, states>>::type>;
  using deps = aux::apply_t<merge_deps, transitions_t>;
  struct events_ids : aux::apply_t<aux::inherit, aux::apply_t<aux::wrap_t, events>> {};

  using deps_t =
      aux::apply_t<aux::pool,
                   aux::apply_t<aux::unique_t, aux::join_t<deps, logger_dep_t, aux::apply_t<merge_deps, sub_sms_t>>>>;

 public:
  using type = sm_t;
  sm(sm &&other)
      : deps_(std::move(other).deps_),
        sub_sms_{std::move(other).sub_sms_},
        sm_{std::move(other).sm_}
  { }
  sm(const sm &) = delete;
  sm &operator=(const sm &) = delete;
  template <class... TDeps, __BOOST_SML_REQUIRES(aux::is_unique_t<TDeps...>::value)>
  explicit sm(sm_t&& sm, TDeps &... deps) : sm_{std::move(sm)}, deps_{deps...}, sub_sms_{aux::init{}, sm_} {
    struct _deps {
      deps_t& deps;
      aux::tuple<> args;
    } ds{deps_};
    aux::get<sm_impl<TSM>>(sub_sms_).start(ds, sub_sms_);
  }

  template <class... TDeps, __BOOST_SML_REQUIRES(aux::is_unique_t<TDeps...>::value)>
  explicit sm(TDeps &&... deps) : deps_{std::forward<TDeps>(deps)...}, sub_sms_{aux::init{}, sm_} {
    struct _deps {
      deps_t& deps;
      aux::tuple<> args{};
    } ds{deps_};
    aux::get<sm_impl<TSM>>(sub_sms_).start(ds, sub_sms_);
  }
  template <class TEvent, class... Ts, __BOOST_SML_REQUIRES(aux::is_base_of<aux::wrapper<TEvent>, events_ids>::value)>
  void process_event(const TEvent &event, Ts &... ts) {
    aux::tuple<Ts &...> args{ts...};
    struct _deps {
      deps_t& deps;
      aux::tuple<Ts &...>& args;
    } ds{deps_, args};
    aux::get<sm_impl<TSM>>(sub_sms_).process_event(event, ds, sub_sms_);
  }
  template <class TEvent, class... Ts, __BOOST_SML_REQUIRES(!aux::is_base_of<aux::wrapper<TEvent>, events_ids>::value)>
  void process_event(const TEvent &event, Ts &... ts) {
    aux::tuple<Ts &...> args{ts...};
    struct _deps {
      deps_t& deps;
      aux::tuple<Ts &...>& args;
    } ds{deps_, args};
    aux::get<sm_impl<TSM>>(sub_sms_).process_event(unexpected_event<_, TEvent>{event}, ds, sub_sms_);
  }
  template <class T = aux::identity<sm_t>, class TVisitor, __BOOST_SML_REQUIRES(concepts::callable<void, TVisitor>::value)>
  void visit_current_states(const TVisitor &visitor) const {
    using type = typename T::type;
    using sm_impl_t = sm_impl<typename TSM::template rebind<type>>;
    using states_t = typename sm_impl_t::states_t;
    constexpr auto regions = sm_impl_t::regions;
    aux::cget<sm_impl_t>(sub_sms_).visit_current_states(visitor, states_t{}, aux::make_index_sequence<regions>{});
  }
  template <class T = aux::identity<sm_t>, class TState>
  bool is(const TState &) const {
    using type = typename T::type;
    using sm_impl_t = sm_impl<typename TSM::template rebind<type>>;
    using state_t = typename sm_impl_t::state_t;
    using states_ids_t = typename sm_impl_t::states_ids_t;
    return aux::get_id<state_t, typename TState::type>((states_ids_t *)0) == aux::cget<sm_impl_t>(sub_sms_).current_state_[0];
  }

  template <class T = aux::identity<sm_t>, class... TStates,
            __BOOST_SML_REQUIRES(sizeof...(TStates) == sm_impl<typename TSM::template rebind<typename T::type>>::regions)>
  bool is(const TStates &...) const {
    auto result = true;
    auto i = 0;
    using type = typename T::type;
    using sm_impl_t = sm_impl<typename TSM::template rebind<type>>;
    using states_ids_t = typename sm_impl_t::states_ids_t;
    using state_t = typename sm_impl_t::state_t;
    state_t state_ids[] = {aux::get_id<state_t, typename TStates::type>((states_ids_t *)0)...};
    visit_current_states<T>([&](auto state) {
      (void)state;
      result &= (aux::get_id<state_t, typename decltype(state)::type>((states_ids_t *)0) == state_ids[i++]);
    });
    return result;
  }
  template <class T = aux::identity<sm_t>, class... TStates>
  void set_current_states(const TStates &...) {
    using type = typename T::type;
    using sm_impl_t = sm_impl<typename TSM::template rebind<type>>;
    using states_ids_t = typename sm_impl_t::states_ids_t;
    using state_t = typename sm_impl_t::state_t;
    auto &sm = aux::get<sm_impl<TSM>>(sub_sms_);
    auto region = 0;
    (void)aux::swallow{0,
                       (sm.current_state_[region++] = aux::get_id<state_t, typename TStates::type>((states_ids_t *)0), 0)...};
  }

  void reset() {
    using T = aux::identity<sm_t>;
    using type = typename T::type;
    using sm_impl_t = sm_impl<typename TSM::template rebind<type>>;
    using states_ids_t = typename sm_impl_t::states_ids_t;
    using state_t = typename sm_impl_t::state_t;
    auto &sm = aux::get<sm_impl<TSM>>(sub_sms_);
    sm.current_state_[0] = aux::get_id<state_t, terminate_state>((states_ids_t *)0);
  }

  void init() { reset(); }

  void start() {
    auto &sm = aux::get<sm_impl<TSM>>(sub_sms_);
    sm.initialize(typename sm_impl<TSM>::initial_states_t{});
  }

  void stop() { reset(); }

  auto is_terminated() const { return aux::cget<sm_impl<TSM>>(sub_sms_).is_terminated(); }

 private:
  deps_t deps_;
  sub_sms_t sub_sms_;
  sm_t sm_;
};
}  // namespace back
namespace concepts {
template <class T>
decltype(aux::declval<T>().operator()()) composable_impl(int);
template <class>
void composable_impl(...);
template <class T>
struct is_pool : aux::false_type {};
template <class... Ts>
struct is_pool<aux::pool<Ts...>> : aux::true_type{};
template <template <class...> class T, class... Ts>
struct is_pool<T<Ts...>> : aux::is_base_of<aux::pool<Ts...>, T<Ts...>> {};
template <class T>
struct composable : is_pool<decltype(composable_impl<T>(0))> {};
template<class T>
concept state_machine = is_pool<T>::value;
}  // namespace concepts
namespace front {
struct operator_base {};
struct action_base {};
struct expr_base {};
template <class, class>
aux::type_list<action_base> args2__(...);
template <class T, class>
auto args2__(int) -> aux::function_traits_t<decltype(&T::operator())>;
template <class T, class E>
auto args1__(int) -> aux::function_traits_t<decltype(&T::__BOOST_SML_TEMPLATE_KEYWORD operator()<back::get_event_t<E>>)>;
template <class T, class E>
auto args1__(...) -> decltype(args2__<T, E>(0));
template <class T, class>
auto args__(int) -> aux::function_traits_t<decltype(&T::__BOOST_SML_TEMPLATE_KEYWORD operator()<>)>;
template <class T, class E>
auto args__(...) -> decltype(args1__<T, E>(0));
template <class T, class E>
using args_t = decltype(args__<T, E>(0));
template <class T, class TEvent, class TDeps>
decltype(auto) get_arg(const aux::type<T> &, const TEvent &, TDeps &deps) {
  return aux::get<T>(deps.deps);
}
template <class T, class TEvent, class TDeps>
decltype(auto) get_arg(const aux::type<const T&> &, const TEvent &, TDeps &deps) {
  if constexpr(requires { static_cast<aux::pool_type<T&> &>(deps.deps).value; }) {
    return aux::get<T&>(deps.deps);
  } else {
    return aux::get<const T&>(deps.deps);
  }
}
template <class TEvent, class TDeps>
decltype(auto) get_arg(const aux::type<TEvent> &, const TEvent &event, TDeps &) {
  return event;
}
template <class TEvent, class TDeps>
decltype(auto) get_arg(const aux::type<const TEvent &> &, const TEvent &event, TDeps &) {
  return event;
}
template <class T, class TEvent, class TDeps>
decltype(auto) get_arg(const aux::type<const TEvent &> &, const back::unexpected_event<T, TEvent> &event, TDeps &) {
  return event.event_;
}
template <class T, class TEvent, class TDeps>
decltype(auto) get_arg(const aux::type<const TEvent &> &, const back::on_entry<T, TEvent> &event, TDeps &) {
  return event.event_;
}
template <class T, class TEvent, class TDeps>
decltype(auto) get_arg(const aux::type<const TEvent &> &, const back::on_exit<T, TEvent> &event, TDeps &) {
  return event.event_;
}
template <class T, class TEvent, class TDeps>
decltype(auto) get_arg(const aux::type<T> &, const back::exception<TEvent> &event, TDeps &) {
  return event.exception_;
}
template <class T, class... Ts, class TDeps>
static auto apply(T object, aux::pool<Ts...> &args, const TDeps&) -> decltype(object(aux::get<Ts>(args)...)) {
  return object(aux::get<Ts>(args)...);
}
template <class T, class TEvent, class TSM, class TDeps, class TSubs>
static auto apply(T object, const TEvent &event, TSM &sm, TDeps &deps, TSubs &subs) -> decltype(object(event, sm, deps, subs)) {
  return object(event, sm, deps, subs);
}
template <class T, class TEvent, class... Ts, class TDeps>
static auto apply(T object, const TEvent &event, aux::tuple<Ts...> &args, const TDeps&) -> decltype(object(event, aux::get_by_type<Ts>(&args)...)) {
  return object(event, aux::get_by_type<Ts>(&args)...);
}
template <auto>
struct priority {};
template <class T, class... TArgs, class TDeps, class... Ts>
static auto apply_impl(priority<1>, T object, aux::tuple<TArgs...> &args, const TDeps&, Ts &&... ts)
    -> decltype(object(std::forward<Ts>(ts)..., aux::get_by_type<TArgs>(&args)...)) {
  return object(std::forward<Ts>(ts)..., aux::get_by_type<TArgs>(&args)...);
}
template <auto X, class T, class... TArgs, class TDeps, class... Ts>
static auto apply_impl(priority<X>, T object, aux::tuple<TArgs...> &, const TDeps& deps, Ts &&... ts) -> decltype(object(std::forward<Ts>(ts)...)) {
  using type = decltype(object(std::forward<Ts>(ts)...));
  if constexpr(std::is_void_v<type>) {
    object(std::forward<Ts>(ts)...);
  } else if constexpr(requires (type& t) { bool(t); } and std::is_base_of_v<aux::pool_type<type&>, TDeps>) {
    return const_cast<type&>(aux::get<type&>(const_cast<TDeps&>(deps))) = object(std::forward<Ts>(ts)...);
  } else {
    return object(std::forward<Ts>(ts)...);
  }
}
template <class T, class... TArgs, class TDeps, class... Ts>
static auto apply(T object, aux::tuple<TArgs...> &args, const TDeps& deps, Ts &&... ts) -> decltype(apply_impl(priority<1>{}, object, args, deps, ts...)) {
  return apply_impl(priority<1>{}, object, args, deps, ts...);
}
template <class, class, class>
struct call;
template <class TEvent>
struct call<TEvent, aux::type_list<>, back::no_policy> {
  template <class T, class TSM, class TDeps, class TSubs>
  static auto execute(T object, const TEvent &, TSM &, TDeps &deps, TSubs &) {
    return apply(object, deps.args, deps.deps);
  }
};
template <class TEvent, class TLogger>
struct call<TEvent, aux::type_list<>, TLogger> {
  template <class T, class TSM, class TDeps, class TSubs>
  static auto execute(T object, const TEvent &event, TSM &sm, TDeps &deps, TSubs &) {
    using result_type = decltype(apply(object, deps.args, deps.deps));
    return execute_impl<typename TSM::sm_t>(aux::type<result_type>{}, &sm, object, event, deps);
  }
  template <class TSM, class T, class TDeps, class Q>
  static auto execute_impl(const aux::type<Q> &, const void *sm, T object, const TEvent &event, TDeps &deps) {
    const auto result = apply(object, deps.args, deps.deps);
    back::log_call<TSM>(aux::type<TLogger>{}, sm, deps, object, event, result);
    return result;
  }
  template <class TSM, class T, class TDeps>
  static auto execute_impl(const aux::type<void> &, const void *sm, T object, const TEvent &event, TDeps &deps) {
    back::log_call<TSM>(aux::type<TLogger>{}, sm, deps, object, event);
    return apply(object, deps.args, deps.deps);
  }
};

template <class TEvent>
struct call<TEvent, aux::type_list<TEvent>, back::no_policy> {
  template <class T, class TSM, class TDeps, class TSubs>
  static auto execute(T object, const TEvent &event, TSM &, TDeps &deps, TSubs &) {
    return apply(object, event, deps.args, deps.deps);
  }
};
template <class TEvent, class TLogger>
struct call<TEvent, aux::type_list<TEvent>, TLogger> {
  template <class T, class TSM, class TDeps, class TSubs>
  static auto execute(T object, const TEvent &event, TSM &sm, TDeps &deps, TSubs &) {
    using result_type = decltype(apply(object, event, deps.args, deps.deps));
    return execute_impl<typename TSM::sm_t>(aux::type<result_type>{}, &sm, object, event, deps);
  }
  template <class TSM, class T, class TDeps, class Q>
  static auto execute_impl(const aux::type<Q> &, const void *sm, T object, const TEvent &event, TDeps &deps) {
    const auto result = apply(object, event, deps.args, deps.deps);
    back::log_call<TSM>(aux::type<TLogger>{}, sm, deps, object, event, result);
    return result;
  }
  template <class TSM, class T, class TDeps>
  static auto execute_impl(const aux::type<void> &, const void *sm, T object, const TEvent &event, TDeps &deps) {
    back::log_call<TSM>(aux::type<TLogger>{}, sm, deps, object, event);
    return apply(object, event, deps.args, deps.deps);
  }
};
template <class TEvent>
struct call<TEvent, aux::type_list<action_base>, back::no_policy> {
  template <class T, class TSM, class TDeps, class TSubs>
  static auto execute(T object, const TEvent &event, TSM &sm, TDeps &deps, TSubs &subs) {
    return apply(object, event, sm, deps, subs);
  }
};
template <class TEvent, class TLogger>
struct call<TEvent, aux::type_list<action_base>, TLogger> {
  template <class T, class TSM, class TDeps, class TSubs>
  static auto execute(T object, const TEvent &event, TSM &sm, TDeps &deps, TSubs &subs) {
    return apply(object, event, sm, deps, subs);
  }
};
template <class TEvent, class... Ts>
struct call<TEvent, aux::type_list<Ts...>, back::no_policy> {
  template <class T, class TSM, class TDeps, class TSubs>
  static auto execute(T object, const TEvent &event, TSM &, TDeps &deps, TSubs &) -> decltype(apply(object, deps.args, deps.deps, get_arg(aux::type<Ts>{}, event, deps)...)) {
    return apply(object, deps.args, deps.deps, get_arg(aux::type<Ts>{}, event, deps)...);
  }
};
template <class TEvent, class... Ts, class TLogger>
struct call<TEvent, aux::type_list<Ts...>, TLogger> {
  template <class T, class TSM, class TDeps, class TSubs>
  static auto execute(T object, const TEvent &event, TSM &sm, TDeps &deps, TSubs &) {
    using result_type = decltype(apply(object, deps.args, deps.deps, get_arg(aux::type<Ts>{}, event, deps)...));
    return execute_impl<typename TSM::sm_t>(aux::type<result_type>{}, &sm, object, event, deps);
  }
  template <class TSM, class T, class TDeps, class Q>
  static auto execute_impl(const aux::type<Q> &, const void *sm, T object, const TEvent &event, TDeps &deps) {
    const auto result = apply(object, deps.args, deps.deps, get_arg(aux::type<Ts>{}, event, deps)...);
    back::log_call<TSM>(aux::type<TLogger>{}, sm, deps, object, event, result);
    return result;
  }
  template <class TSM, class T, class TDeps>
  static auto execute_impl(const aux::type<void> &, const void *sm, T object, const TEvent &event, TDeps &deps) {
    back::log_call<TSM>(aux::type<TLogger>{}, sm, deps, object, event);
    return apply(object, deps.args, deps.deps, get_arg(aux::type<Ts>{}, event, deps)...);
  }
};
template <class... Ts>
class seq_ : operator_base {
 public:
  constexpr explicit seq_(Ts... ts) : a(ts...) {}
  template <class TEvent, class TSM, class TDeps, class TSubs>
  constexpr void operator()(const TEvent &event, TSM &sm, TDeps &deps, TSubs &subs) {
    for_all(aux::make_index_sequence<sizeof...(Ts)>{}, event, sm, deps, subs);
  }

 private:
  template <int... Ns, class TEvent, class TSM, class TDeps, class TSubs>
  constexpr void for_all(const aux::index_sequence<Ns...> &, const TEvent &event, TSM &sm, TDeps &deps, TSubs &subs) {
    (call<TEvent, args_t<Ts, TEvent>, typename TSM::logger_t>::execute(aux::get_by_id<Ns>(&a), event, sm, deps, subs), ...);
  }
  aux::tuple<Ts...> a;
};
template <class... Ts>
class and_ : operator_base {
 public:
  constexpr explicit and_(Ts... ts) : g(ts...) {}
  template <class TEvent, class TSM, class TDeps, class TSubs>
  constexpr auto operator()(const TEvent &event, TSM &sm, TDeps &deps, TSubs &subs) {
    return for_all<Ts...>(aux::make_index_sequence<sizeof...(Ts)>{}, event, sm, deps, subs);
  }

 private:
  template <class T, class... Xs, int N, int... Ns, class TEvent, class TSM, class TDeps, class TSubs>
  constexpr auto for_all(const aux::index_sequence<N, Ns...> &, const TEvent &event, TSM &sm, TDeps &deps, TSubs &subs) {
    using g_t = decltype(call<TEvent, args_t<T, TEvent>, typename TSM::logger_t>::execute(aux::get_by_id<N>(&g), event, sm, deps, subs));
    if constexpr(requires{ g_t::value; }) {
      if constexpr (g_t::value) {
        if constexpr(sizeof...(Xs) == 0u)
          return true;
        else
          return for_all<Xs...>(aux::index_sequence<Ns...>{}, event, sm, deps, subs);
      }
    } else if (call<TEvent, args_t<T, TEvent>, typename TSM::logger_t>::execute(aux::get_by_id<N>(&g), event, sm, deps, subs)) {
      if constexpr(sizeof...(Xs) == 0u)
        return true;
      else
        return for_all<Xs...>(aux::index_sequence<Ns...>{}, event, sm, deps, subs);
    }
    return false;
  }
  aux::tuple<Ts...> g;
};
template <class... Ts>
class or_ : operator_base {
 public:
  constexpr explicit or_(Ts... ts) : g(ts...) {}
  template <class TEvent, class TSM, class TDeps, class TSubs>
  constexpr auto operator()(const TEvent &event, TSM &sm, TDeps &deps, TSubs &subs) {
    return for_all<Ts...>(aux::make_index_sequence<sizeof...(Ts)>{}, event, sm, deps, subs);
  }

 private:
  template <class T, class... Xs, int N, int... Ns, class TEvent, class TSM, class TDeps, class TSubs>
  constexpr auto for_all(const aux::index_sequence<N, Ns...> &, const TEvent &event, TSM &sm, TDeps &deps, TSubs &subs) {
    using g_t = decltype(call<TEvent, args_t<T, TEvent>, typename TSM::logger_t>::execute(aux::get_by_id<N>(&g), event, sm, deps, subs));
    if constexpr(requires{ g_t::value; }) {
      if constexpr (g_t::value) {
          return true;
      } else {
        if constexpr(sizeof...(Xs) == 0u)
          return false;
        else
          return for_all<Xs...>(aux::index_sequence<Ns...>{}, event, sm, deps, subs);
      }
    } else if (call<TEvent, args_t<T, TEvent>, typename TSM::logger_t>::execute(aux::get_by_id<N>(&g), event, sm, deps, subs)) {
      return true;
    } else {
        if constexpr(sizeof...(Xs) == 0u)
          return false;
        else
          return for_all<Xs...>(aux::index_sequence<Ns...>{}, event, sm, deps, subs);
    }
    return false;
  }
  aux::tuple<Ts...> g;
};
template <class T>
class not_ : operator_base {
 public:
  constexpr explicit not_(T t) : g(t) {}
  template <class TEvent, class TSM, class TDeps, class TSubs>
  constexpr auto operator()(const TEvent &event, TSM &sm, TDeps &deps, TSubs &subs) {
    if constexpr (requires{ T::value; }) {
      return !T::value;
    } else {
      return !call<TEvent, args_t<T, TEvent>, typename TSM::logger_t>::execute(g, event, sm, deps, subs);
    }
  }

 private:
  T g;
};
}  // namespace front
template <class T, __BOOST_SML_REQUIRES(concepts::callable<bool, T>::value)>
constexpr auto operator!(const T &t) {
  return front::not_<aux::zero_wrapper<T>>(aux::zero_wrapper<T>{t});
}
template <class T1, class T2, __BOOST_SML_REQUIRES(concepts::callable<bool, T1>::value &&concepts::callable<bool, T2>::value)>
constexpr auto operator&&(const T1 &t1, const T2 &t2) {
  return front::and_<aux::zero_wrapper<T1>, aux::zero_wrapper<T2>>(aux::zero_wrapper<T1>{t1}, aux::zero_wrapper<T2>{t2});
}
template <class T1, class T2, __BOOST_SML_REQUIRES(concepts::callable<bool, T1>::value &&concepts::callable<bool, T2>::value)>
constexpr auto operator||(const T1 &t1, const T2 &t2) {
  return front::or_<aux::zero_wrapper<T1>, aux::zero_wrapper<T2>>(aux::zero_wrapper<T1>{t1}, aux::zero_wrapper<T2>{t2});
}
template <class T1, class T2, __BOOST_SML_REQUIRES(concepts::callable<void, T1>::value &&concepts::callable<void, T2>::value)>
constexpr auto operator,(const T1 &t1, const T2 &t2) {
  return front::seq_<aux::zero_wrapper<T1>, aux::zero_wrapper<T2>>(aux::zero_wrapper<T1>{t1}, aux::zero_wrapper<T2>{t2});
}
namespace front {
namespace actions {
template <class... Ts>
class defer_event {
  using ids_t = aux::type_id<Ts...>;
  static constexpr auto alignment = aux::max<alignof(Ts)...>();
  static constexpr auto size = aux::max<sizeof(Ts)...>();
  template <class T>
  static void dtor_impl(aux::byte *data) {
    (void)data;
    reinterpret_cast<T *>(data)->~T();
  }
  template <class T>
  static void move_impl(aux::byte (&data)[size], defer_event &&other) {
    new (&data) T(static_cast<T &&>(*reinterpret_cast<T *>(other.data)));
  }

 public:
  defer_event(defer_event &&other) : id(other.id), dtor(other.dtor), move(other.move) {
    move(data, static_cast<defer_event &&>(other));
  }
  defer_event(const defer_event &) = delete;
  defer_event &operator=(const defer_event &) = delete;
  template <class T>
  defer_event(T object) {
    id = aux::get_id<int, T>((ids_t *)0);
    dtor = &dtor_impl<T>;
    move = &move_impl<T>;
    new (&data) T(static_cast<T &&>(object));
  }
  ~defer_event() { dtor(data); }
  alignas(alignment) aux::byte data[size];
  int id = -1;

 private:
  void (*dtor)(aux::byte *);
  void (*move)(aux::byte (&)[size], defer_event &&);
};
struct defer : action_base {
  template <class TEvent, class TSM, class TDeps, class TSubs>
  void operator()(const TEvent &event, TSM &sm, TDeps &, TSubs &) {
    sm.defer_.push(event);
  }
};

template<class T>
struct event_traits {
  using type = boost::mp11::mp_list<T>;
};

template<class... Ts>
struct event_traits<std::variant<Ts...>> {
  using type = boost::mp11::mp_list<typename event_traits<Ts>::type...>;
};

template<class... Ts>
struct event_traits<std::pair<Ts...>> {
  using type = boost::mp11::mp_list<typename event_traits<Ts>::type...>;
};

template<class... Ts>
struct event_traits<std::tuple<Ts...>> {
  using type = boost::mp11::mp_list<typename event_traits<Ts>::type...>;
};

template<class T>
struct event_traits<std::vector<T>> {
  using type = typename event_traits<typename event_traits<T>::type>::type;
};

template<class F, class T>
struct event_traits<boost::transformed_range<F, T>> {
  using type = typename event_traits<std::remove_cvref_t<T>>::type;
};

template<class TExpr>
struct emit : action_base, expr_base {
  using expr_t = TExpr;

  constexpr explicit(true) emit(TExpr expr) : expr{expr} {}

  template <class TEvent>
  constexpr auto operator()(const TEvent &, ...) {
    if constexpr(concepts::callable<void, TExpr>::value) {
      return []<template <class...> class TList, class... Ts>(TList<Ts...>) {
        return typename event_traits<std::remove_cvref_t<decltype(std::declval<TExpr>()(std::declval<Ts>()...))>>::type{};
      }(args_t<TExpr, TEvent>{});
    } else {
      return expr;
    }
  }

  template <class TEvent, class TSM, class TDeps, class TSubs>
  constexpr auto operator()(const TEvent & event, const TSM &, TDeps & deps, const TSubs &) -> void {
    if constexpr (requires { deps.args; }) {
      if constexpr(concepts::callable<void, TExpr>::value) {
        [&]<template <class...> class TList, class... Ts, class... TArgs>(const TList<Ts...>&, aux::tuple<TArgs...>& args) {
          on<typename TSM::sm_t, typename TSM::logger_t>(deps.deps, apply(expr, deps.args, deps.deps, get_arg(aux::type<Ts>{}, event, deps)...), args);
        }(args_t<TExpr, TEvent>{}, deps.args);
      } else {
        on<typename TSM::sm_t, typename TSM::logger_t>(deps.deps, expr, deps.args);
      }
    }
  }

 private:
  template<class TSM, class TLogger, class TEvent, class... TArgs>
  constexpr auto on(auto& deps, const TEvent& e, aux::tuple<TArgs...>& args) -> void {
    if constexpr (requires { e(); }) {
      on<TSM, TLogger>(deps, e(), args);
    } else if constexpr (requires { e.has_value(); *e; } and not requires { typename TEvent::name; }) {
      if (e) {
        on<TSM, TLogger>(deps, *e, args);
      }
    } else if constexpr(aux::is_tuple<TEvent>) {
        std::apply([&](const auto&... events) { (on<TSM, TLogger>(deps, events, args), ...); }, e);
    } else if constexpr (requires { e.index(); }) {
      std::visit([ & ](const auto &event) { on<TSM, TLogger>(deps, event, args); }, e);
    } else if constexpr (requires { std::cbegin(e); std::cend(e); } and not requires { e.npos; }) {
      for (const auto& event : e) { on<TSM, TLogger>(deps, event, args); }
    } else {
      if constexpr (requires { static_cast<aux::pool_type<TLogger &> &>(deps).value.template log_send_event<typename TSM::name_t>(e); }) {
        static_cast<aux::pool_type<TLogger &> &>(deps).value.template log_send_event<typename TSM::name_t>(e);
      }
      aux::on(e, aux::get_by_type<TArgs>(&args)...);
    }
  }

  TExpr expr{};
};
}  // namespace actions
}  // namespace front
namespace detail {
template <class T>
  requires concepts::composable<typename T::sm>::value
using state_machine = back::sm<T>;
}
template <class T>
struct thread_safe : aux::pair<back::thread_safety_policy__, thread_safe<T>> {
  using type = T;
};
template <template <class...> class T>
struct defer_queue : aux::pair<back::defer_queue_policy__, defer_queue<T>> {
  template <class U>
  using rebind = T<U>;
  template <class... Ts>
  using defer = front::actions::defer_event<Ts...>;
};
template <class T>
struct logger : aux::pair<back::logger_policy__, logger<T>> {
  using type = T;
};
struct testing : aux::pair<back::testing_policy__, testing> {};
template <class T>
struct dispatch : aux::pair<back::dispatch_policy__, T> {
  using type = T;
};
template <class T, class... TPolicies>
using sm = detail::state_machine<back::sm_policy<T, TPolicies...>>;
namespace concepts {
aux::false_type transitional_impl(...);
template <class T>
auto transitional_impl(T &&t) -> aux::always<typename T::dst_state, typename T::src_state, typename T::event, typename T::deps,
                                             decltype(T::initial), decltype(T::history)>;
template <class T>
struct transitional : decltype(transitional_impl(aux::declval<T>())) {};
}  // namespace concepts
namespace front {
namespace actions {
struct process {
  template <class TEvent, class = void>
  class process_impl : public action_base {
   public:
    explicit process_impl(const TEvent &event) : event(event) {}

    template <class T, class TSM, class TDeps, class TSubs>
    void operator()(const T &, TSM &sm, TDeps &deps, TSubs &subs) {
      sm.process_event(event, deps, subs);
    }

   private:
    aux::conditional_t<aux::is_constructible<TEvent, const TEvent &>::value, TEvent, const TEvent &> event;
  };
  template <class TExpr>
  class process_impl<TExpr, aux::enable_if_t<concepts::callable<void, TExpr>::value>> : public action_base {
   public:
    explicit process_impl(const TExpr &expr) : expr{expr} {}

    template <class TEvent, class TSM, class TDeps, class TSubs>
    void operator()(const TEvent &event, TSM &sm, TDeps &deps, TSubs &subs) {
      [&]<template <class...> class TList, class... Ts, class... TArgs>(const TList<Ts...>&) {
        process(apply(expr, deps.args, deps.deps, get_arg(aux::type<Ts>{}, event, deps)...), sm, deps, subs);
      }(args_t<TExpr, TEvent>{});
    }

   private:
    template<class TEvent>
    constexpr auto process(const TEvent& e, auto& sm, auto& deps, auto& subs) -> void {
      if constexpr (requires { e(); }) {
        process(e(), sm, deps, subs);
      } else if constexpr (requires { e.has_value(); *e; }) {
        if (e) {
          process(*e, sm, deps, subs);
        }
      } else if constexpr(aux::is_tuple<TEvent>) {
          std::apply([&](const auto&... events) { (process(events, sm, deps, subs), ...); }, e);
      } else if constexpr (requires { e.index(); }) {
        std::visit([ & ](const auto &event) { process(event, sm, deps, subs); }, e);
      } else if constexpr (requires { std::cbegin(e); std::cend(e); } and not requires { e.npos; }) {
        for (const auto& event : e) { process(event, sm, deps, subs); }
      } else {
        sm.process_event(e, deps, subs);
      }
    }

    TExpr expr{};
  };
  class process_impl_ {
   public:
    template <class TEvent, class TSM, class TDeps, class TSubs>
    void operator()(const TEvent & event, TSM &sm, TDeps &deps, TSubs &subs) {
      sm.process_event(event, deps, subs);
    }
  };
  template <class TEvent>
  auto operator()(const TEvent &event) {
    return process_impl<TEvent>{event};
  }
  auto operator()() {
    return process_impl_{};
  }
};
}  // namespace actions
}  // namespace front
namespace front {
template <class, class>
struct transition_eg;
template <class, class>
struct transition_ea;
template <class TEvent>
struct event {
  template <class T, __BOOST_SML_REQUIRES(concepts::callable<bool, T>::value)>
  auto operator[](const T &t) const {
    return transition_eg<event, aux::zero_wrapper<T>>{*this, aux::zero_wrapper<T>{t}};
  }
  template <class T, __BOOST_SML_REQUIRES(concepts::callable<void, T>::value)>
  auto operator/(const T &t) const {
    return transition_ea<event, aux::zero_wrapper<T>>{*this, aux::zero_wrapper<T>{t}};
  }
  auto operator()() const { return TEvent{}; }
};
}  // namespace front
namespace front {
struct initial_state {};
struct history_state {};
template <class...>
struct transition;
template <class, class>
struct transition_sa;
template <class, class>
struct transition_sg;
template <class, class>
struct transition_eg;
template <class>
struct state;
template <class TState>
struct state_impl {
  template <class T>
  auto operator<=(const T &t) const {
    return transition<TState, T>{static_cast<const TState &>(*this), t};
  }
  template <class T>
  auto operator+(const T &t) const {
    return transition<TState, T>{static_cast<const TState &>(*this), t};
  }
  template <class T, __BOOST_SML_REQUIRES(concepts::callable<bool, T>::value)>
  auto operator[](const T &t) const {
    return transition_sg<TState, aux::zero_wrapper<T>>{static_cast<const TState &>(*this), aux::zero_wrapper<T>{t}};
  }
  template <class T, __BOOST_SML_REQUIRES(concepts::callable<void, T>::value)>
  auto operator/(const T &t) const {
    return transition_sa<TState, aux::zero_wrapper<T>>{static_cast<const TState &>(*this), aux::zero_wrapper<T>{t}};
  }
};
template <class TState>
struct state : state_impl<state<TState>> {
  using type = TState;
  static constexpr auto initial = false;
  static constexpr auto history = false;
  auto operator*() const { return state<TState(initial_state)>{}; }
  auto operator()(const initial_state &) const { return state<TState(initial_state)>{}; }
  auto operator()(const history_state &) const { return state<TState(history_state)>{}; }
  template <class... Ts>
  auto operator()(const state<Ts> &...) const {
    return state<TState(Ts...)>{};
  }
  template <class T>
  auto operator=(const T &t) const {
    return transition<T, state>{t, *this};
  }
  template <class T>
  auto sm() const {
    return state<back::sm<back::sm_policy<T, aux::identity<TState>>>>{};
  }
};
template <class TState>
struct state<TState(initial_state)> : state_impl<state<TState(initial_state)>> {
  using type = TState;
  static constexpr auto initial = true;
  static constexpr auto history = false;
  template <class T>
  auto operator=(const T &t) const {
    return transition<T, state>{t, *this};
  }
};
template <class TState>
struct state<TState(history_state)> : state_impl<state<TState(history_state)>> {
  using type = TState;
  static constexpr auto initial = true;
  static constexpr auto history = true;
  template <class T>
  auto operator=(const T &t) const {
    return transition<T, state>{t, *this};
  }
};
template <class T, class = void>
struct state_sm {
  using type = state<T>;
};
template <class T>
struct state_sm<T, aux::enable_if_t<concepts::composable<T>::value>> {
  using type = state<back::sm<back::sm_policy<T>>>;
};
}  // namespace front
namespace front {
struct internal {};
template <class, class>
struct ignore;
template <class E, class... Ts>
struct ignore<E, aux::type_list<Ts...>> {
  template <class T>
  struct non_events {
    using type = aux::conditional_t<aux::is_same<back::get_event_t<E>, aux::remove_reference_t<T>>::value ||
                                        aux::is_same<T, action_base>::value,
                                    aux::type_list<>, aux::type_list<aux::remove_const_ref_t<T>>>;
  };
  using type = aux::join_t<typename non_events<Ts>::type...>;
};
template <class T, class E, class = void>
struct get_deps {
  using type = typename ignore<E, args_t<T, E>>::type;
};
template <class T, class E>
using get_deps_t = typename get_deps<T, E>::type;
template <template <class...> class T, class... Ts, class E>
struct get_deps<T<Ts...>, E, aux::enable_if_t<aux::is_base_of<operator_base, T<Ts...>>::value>> {
  using type = aux::join_t<get_deps_t<Ts, E>...>;
};
template <template <class...> class T, class... Ts, class E>
struct get_deps<T<Ts...>, E, aux::enable_if_t<aux::is_base_of<expr_base, T<Ts...>>::value>> {
  using type = typename ignore<E, args_t<typename T<Ts...>::expr_t, E>>::type;
};
struct always {
  bool operator()() const { return true; }
  __BOOST_SML_ZERO_SIZE_ARRAY(aux::byte);
};
struct none {
  void operator()() {}
  __BOOST_SML_ZERO_SIZE_ARRAY(aux::byte);
};
template <class...>
struct transition;
template <class E, class G>
struct transition<front::event<E>, G> {
  template <class T>
  auto operator/(const T &t) const {
    return transition<front::event<E>, G, aux::zero_wrapper<T>>{e, g, aux::zero_wrapper<T>{t}};
  }
  front::event<E> e;
  G g;
};
template <class E, class G, class A>
struct transition<front::event<E>, G, A> {
  static constexpr auto initial = state<void>::initial;
  static constexpr auto history = state<void>::history;
  using src_state = typename state<void>::type;
  using dst_state = typename state<void>::type;
  using event = E;
  using deps = aux::apply_t<aux::unique_t, aux::join_t<get_deps_t<G, E>, get_deps_t<A, E>>>;
  using guard = G;
  using action = A;
  front::event<E> e;
  G g;
  A a;
  template <class TEvent, class SM, class TDeps, class TSubs, class... Ts>
  bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &, Ts &&...) {
    using g_t = decltype(call<TEvent, args_t<G, TEvent>, typename SM::logger_t>::execute(g, event, sm, deps, subs));
    if constexpr(requires{ g_t::value; }) {
      if constexpr(g_t::value) {
        call<TEvent, args_t<A, TEvent>, typename SM::logger_t>::execute(a, event, sm, deps, subs);
        return true;
      }
    } else if (call<TEvent, args_t<G, TEvent>, typename SM::logger_t>::execute(g, event, sm, deps, subs)) {
      call<TEvent, args_t<A, TEvent>, typename SM::logger_t>::execute(a, event, sm, deps, subs);
      return true;
    }
    return false;
  }
};
template <class S2, class G, class A>
struct transition<state<S2>, G, A> : transition<state<internal>, state<S2>, front::event<back::anonymous>, G, A> {
  using transition<state<internal>, state<S2>, front::event<back::anonymous>, G, A>::g;
  using transition<state<internal>, state<S2>, front::event<back::anonymous>, G, A>::a;
  transition(const G &g, const A &a) : transition<state<internal>, state<S2>, front::event<back::anonymous>, G, A>{g, a} {}
  template <class T>
  auto operator=(const T &) const {
    return transition<T, state<S2>, front::event<back::anonymous>, G, A>{g, a};
  }
  template <class TEvent, class TSM, class TDeps, class TSubs>
  auto operator()(const TEvent & event, const TSM & sm, const TDeps & deps, const TSubs & subs) -> void {
    typename TSM::state_t s{};
    this->execute(event, sm, deps, subs, s);
  }
};
template <class S1, class S2>
struct transition<state<S1>, state<S2>> : transition<state<S1>, state<S2>, front::event<back::anonymous>, always, none> {
  transition(const state<S1> &, const state<S2> &)
      : transition<state<S1>, state<S2>, front::event<back::anonymous>, always, none>{always{}, none{}} {}
};
template <class S2, class G>
struct transition_sg<state<S2>, G> : transition<state<internal>, state<S2>, front::event<back::anonymous>, G, none> {
  using transition<state<internal>, state<S2>, front::event<back::anonymous>, G, none>::g;
  transition_sg(const state<S2> &, const G &g)
      : transition<state<internal>, state<S2>, front::event<back::anonymous>, G, none>{g, none{}} {}
  template <class T>
  auto operator/(const T &t) const {
    return transition<state<S2>, G, aux::zero_wrapper<T>>{g, aux::zero_wrapper<T>{t}};
  }
  template <class T>
  auto operator=(const T &) const {
    return transition<T, state<S2>, front::event<back::anonymous>, G, none>{g, none{}};
  }
};
template <class S2, class A>
struct transition_sa<state<S2>, A> : transition<state<internal>, state<S2>, front::event<back::anonymous>, always, A> {
  using transition<state<internal>, state<S2>, front::event<back::anonymous>, always, A>::a;
  transition_sa(const state<S2> &, const A &a)
      : transition<state<internal>, state<S2>, front::event<back::anonymous>, always, A>{always{}, a} {}
  template <class T>
  auto operator=(const T &) const {
    return transition<T, state<S2>, front::event<back::anonymous>, always, A>{always{}, a};
  }
};
template <class S2, class E>
struct transition<state<S2>, front::event<E>> {
  template <class T>
  auto operator=(const T &) const {
    return transition<T, state<S2>, front::event<E>, always, none>{always{}, none{}};
  }
  const state<S2> &s2;
  front::event<E> e;
};
template <class E, class G>
struct transition_eg<front::event<E>, G> {
  template <class T>
  auto operator/(const T &t) const {
    return transition<front::event<E>, G, aux::zero_wrapper<T>>{e, g, aux::zero_wrapper<T>{t}};
  }
  front::event<E> e;
  G g;
};
template <class E, class A>
struct transition_ea<front::event<E>, A> {
  static constexpr auto initial = state<void>::initial;
  static constexpr auto history = state<void>::history;
  using src_state = typename state<void>::type;
  using dst_state = typename state<void>::type;
  using event = E;
  using deps = aux::apply_t<aux::unique_t, get_deps_t<A, E>>;
  using guard = always;
  using action = A;
  front::event<E> e;
  A a;
  template <class TEvent, class SM, class TDeps, class TSubs, class... Ts>
  bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &, Ts &&...) {
    call<TEvent, args_t<A, TEvent>, typename SM::logger_t>::execute(a, event, sm, deps, subs);
    return true;
  }
};
template <class S1, class S2, class G, class A>
struct transition<state<S1>, transition<state<S2>, G, A>>
    : transition<state<S1>, state<S2>, front::event<back::anonymous>, G, A> {
  transition(const state<S1> &, const transition<state<S2>, G, A> &t)
      : transition<state<S1>, state<S2>, front::event<back::anonymous>, G, A>{t.g, t.a} {}
};
template <class S1, class E, class G, class A>
struct transition<state<S1>, transition<front::event<E>, G, A>>
    : transition<state<internal>, state<S1>, front::event<E>, G, A> {
  using transition<state<internal>, state<S1>, front::event<E>, G, A>::g;
  using transition<state<internal>, state<S1>, front::event<E>, G, A>::a;
  transition(const state<S1> &, const transition<front::event<E>, G, A> &t)
      : transition<state<internal>, state<S1>, front::event<E>, G, A>{t.g, t.a} {}
  template <class T>
  auto operator=(const T &) const {
    return transition<T, state<S1>, front::event<E>, G, A>{g, a};
  }
};
template <class S1, class S2, class E>
struct transition<state<S1>, transition<state<S2>, front::event<E>>>
    : transition<state<S1>, state<S2>, front::event<E>, always, none> {
  transition(const state<S1> &, const transition<state<S2>, front::event<E>> &)
      : transition<state<S1>, state<S2>, front::event<E>, always, none>{always{}, none{}} {}
};
template <class S1, class S2, class G>
struct transition<state<S1>, transition_sg<state<S2>, G>>
    : transition<state<S1>, state<S2>, front::event<back::anonymous>, G, none> {
  transition(const state<S1> &, const transition_sg<state<S2>, G> &t)
      : transition<state<S1>, state<S2>, front::event<back::anonymous>, G, none>{t.g, none{}} {}
};
template <class S1, class S2, class A>
struct transition<state<S1>, transition_sa<state<S2>, A>>
    : transition<state<S1>, state<S2>, front::event<back::anonymous>, always, A> {
  transition(const state<S1> &, const transition_sa<state<S2>, A> &t)
      : transition<state<S1>, state<S2>, front::event<back::anonymous>, always, A>{always{}, t.a} {}
};
template <class S2, class E, class G>
struct transition<state<S2>, transition_eg<front::event<E>, G>>
    : transition<state<internal>, state<S2>, front::event<E>, G, none> {
  using transition<state<internal>, state<S2>, front::event<E>, G, none>::g;
  transition(const state<S2> &, const transition_eg<front::event<E>, G> &t)
      : transition<state<internal>, state<S2>, front::event<E>, G, none>{t.g, none{}} {}
  template <class T>
  auto operator=(const T &) const {
    return transition<T, state<S2>, front::event<E>, G, none>{g, none{}};
  }
};
template <class S1, class S2, class E, class G>
struct transition<state<S1>, transition<state<S2>, transition_eg<front::event<E>, G>>>
    : transition<state<S1>, state<S2>, front::event<E>, G, none> {
  transition(const state<S1> &, const transition<state<S2>, transition_eg<front::event<E>, G>> &t)
      : transition<state<S1>, state<S2>, front::event<E>, G, none>{t.g, none{}} {}
};
template <class S2, class E, class A>
struct transition<state<S2>, transition_ea<front::event<E>, A>>
    : transition<state<internal>, state<S2>, front::event<E>, always, A> {
  using transition<state<internal>, state<S2>, front::event<E>, always, A>::a;
  transition(const state<S2> &, const transition_ea<front::event<E>, A> &t)
      : transition<state<internal>, state<S2>, front::event<E>, always, A>{always{}, t.a} {}
  template <class T>
  auto operator=(const T &) const {
    return transition<T, state<S2>, front::event<E>, always, A>{always{}, a};
  }
};
template <class S1, class S2, class E, class A>
struct transition<state<S1>, transition<state<S2>, transition_ea<front::event<E>, A>>>
    : transition<state<S1>, state<S2>, front::event<E>, always, A> {
  transition(const state<S1> &, const transition<state<S2>, transition_ea<front::event<E>, A>> &t)
      : transition<state<S1>, state<S2>, front::event<E>, always, A>{always{}, t.a} {}
};
template <class S1, class S2, class E, class G, class A>
struct transition<state<S1>, transition<state<S2>, transition<front::event<E>, G, A>>>
    : transition<state<S1>, state<S2>, front::event<E>, G, A> {
  transition(const state<S1> &, const transition<state<S2>, transition<front::event<E>, G, A>> &t)
      : transition<state<S1>, state<S2>, front::event<E>, G, A>{t.g, t.a} {}
};
template <class T, class TSubs, class... Ts, class... THs>
void update_composite_states(TSubs &subs, aux::true_type, const aux::type_list<THs...> &) {
  using state_t = typename T::state_t;
  auto &sm = back::sub_sm<T>::get(&subs);
  (void)sm;
  (void)aux::swallow{0, (sm.current_state_[aux::get_id<state_t, THs>((typename T::initial_states_ids_t *)0)] =
                             aux::get_id<state_t, THs>((typename T::states_ids_t *)0),
                         0)...};
}
template <class T, class TSubs, class... Ts>
void update_composite_states(TSubs &subs, aux::false_type, Ts &&...) {
  back::sub_sm<T>::get(&subs).initialize(typename T::initial_states_t{});
}
template <class SM, class TDeps, class TSubs, class TSrcState, class TDstState>
void update_current_state(SM &sm, TDeps &deps, TSubs &, typename SM::state_t &current_state,
                          const typename SM::state_t &new_state, const TSrcState &, const TDstState &) {
  back::log_state_change<typename SM::sm_t>(aux::type<typename SM::logger_t>{}, &sm, deps,
                                            aux::string<typename TSrcState::type>{}, aux::string<typename TDstState::type>{});
  current_state = new_state;
}
template <class SM, class TDeps, class TSubs, class TSrcState, class T>
void update_current_state(SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state,
                          const typename SM::state_t &new_state, const TSrcState &, const state<back::sm<T>> &) {
  back::log_state_change<typename SM::sm_t>(aux::type<typename SM::logger_t>{}, &sm, deps,
                                            aux::string<typename TSrcState::type>{}, aux::string<T>{});
  current_state = new_state;
  update_composite_states<back::sm_impl<T>>(subs, typename back::sm_impl<T>::has_history_states{},
                                            typename back::sm_impl<T>::history_states_t{});
}
template <class S1, class S2, class E, class G, class A>
struct transition<state<S1>, state<S2>, front::event<E>, G, A> {
  static constexpr auto initial = state<S2>::initial;
  static constexpr auto history = state<S2>::history;
  using src_state = typename state<S2>::type;
  using dst_state = typename state<S1>::type;
  using event = E;
  using guard = G;
  using action = A;
  using deps = aux::apply_t<aux::unique_t, aux::join_t<get_deps_t<G, E>, get_deps_t<A, E>>>;
  transition(const G &g, const A &a) : g(g), a(a) {}
  template <class TEvent, class SM, class TDeps, class TSubs>
  bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state, aux::true_type) {
    using g_t = decltype(call<TEvent, args_t<G, TEvent>, typename SM::logger_t>::execute(g, event, sm, deps, subs));
    if constexpr(requires{ g_t::value; }) {
      if constexpr(g_t::value) {
        sm.process_internal_event(back::on_exit<back::_, TEvent>{event}, deps, subs, current_state);
        update_current_state(sm, deps, subs, current_state,
                             aux::get_id<typename SM::state_t, dst_state>((typename SM::states_ids_t *)0), state<src_state>{},
                             state<dst_state>{});
        call<TEvent, args_t<A, TEvent>, typename SM::logger_t>::execute(a, event, sm, deps, subs);
        sm.process_internal_event(back::on_entry<back::_, TEvent>{event}, deps, subs, current_state);
        return true;
      }
    } else if (call<TEvent, args_t<G, TEvent>, typename SM::logger_t>::execute(g, event, sm, deps, subs)) {
      sm.process_internal_event(back::on_exit<back::_, TEvent>{event}, deps, subs, current_state);
      update_current_state(sm, deps, subs, current_state,
                           aux::get_id<typename SM::state_t, dst_state>((typename SM::states_ids_t *)0), state<src_state>{},
                           state<dst_state>{});
      call<TEvent, args_t<A, TEvent>, typename SM::logger_t>::execute(a, event, sm, deps, subs);
      sm.process_internal_event(back::on_entry<back::_, TEvent>{event}, deps, subs, current_state);
      return true;
    }
    return false;
  }
  template <class TEvent, class SM, class TDeps, class TSubs>
  bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state, aux::false_type) {
    using g_t = decltype(call<TEvent, args_t<G, TEvent>, typename SM::logger_t>::execute(g, event, sm, deps, subs));
    if constexpr(requires{ g_t::value; }) {
      if constexpr(g_t::value) {
        update_current_state(sm, deps, subs, current_state,
                             aux::get_id<typename SM::state_t, dst_state>((typename SM::states_ids_t *)0), state<src_state>{},
                             state<dst_state>{});
        call<TEvent, args_t<A, TEvent>, typename SM::logger_t>::execute(a, event, sm, deps, subs);
        return true;
      }
    } else if (call<TEvent, args_t<G, TEvent>, typename SM::logger_t>::execute(g, event, sm, deps, subs)) {
      update_current_state(sm, deps, subs, current_state,
                           aux::get_id<typename SM::state_t, dst_state>((typename SM::states_ids_t *)0), state<src_state>{},
                           state<dst_state>{});
      call<TEvent, args_t<A, TEvent>, typename SM::logger_t>::execute(a, event, sm, deps, subs);
      return true;
    }
    return false;
  }
  G g;
  A a;
};
template <class S2, class E, class G, class A>
struct transition<state<internal>, state<S2>, front::event<E>, G, A> {
  static constexpr auto initial = state<S2>::initial;
  static constexpr auto history = state<S2>::history;
  using src_state = typename state<S2>::type;
  using dst_state = internal;
  using event = E;
  using guard = G;
  using action = A;
  using deps = aux::apply_t<aux::unique_t, aux::join_t<get_deps_t<G, E>, get_deps_t<A, E>>>;
  transition(const G &g, const A &a) : g(g), a(a) {}
  template <class TEvent, class SM, class TDeps, class TSubs, class... Ts>
  bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &, Ts &&...) {
    using g_t = decltype(call<TEvent, args_t<G, TEvent>, typename SM::logger_t>::execute(g, event, sm, deps, subs));
    if constexpr(requires{ g_t::value; }) {
      if constexpr(g_t::value) {
        call<TEvent, args_t<A, TEvent>, typename SM::logger_t>::execute(a, event, sm, deps, subs);
        return true;
      }
    } else if (call<TEvent, args_t<G, TEvent>, typename SM::logger_t>::execute(g, event, sm, deps, subs)) {
      call<TEvent, args_t<A, TEvent>, typename SM::logger_t>::execute(a, event, sm, deps, subs);
      return true;
    }
    return false;
  }
  G g;
  A a;
};
template <class S1, class S2, class E, class A>
struct transition<state<S1>, state<S2>, front::event<E>, always, A> {
  static constexpr auto initial = state<S2>::initial;
  static constexpr auto history = state<S2>::history;
  using src_state = typename state<S2>::type;
  using dst_state = typename state<S1>::type;
  using event = E;
  using guard = always;
  using action = A;
  using deps = aux::apply_t<aux::unique_t, get_deps_t<A, E>>;
  transition(const always &, const A &a) : a(a) {}
  template <class TEvent, class SM, class TDeps, class TSubs>
  bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state, aux::true_type) {
    sm.process_internal_event(back::on_exit<back::_, TEvent>{event}, deps, subs, current_state);
    update_current_state(sm, deps, subs, current_state,
                         aux::get_id<typename SM::state_t, dst_state>((typename SM::states_ids_t *)0), state<src_state>{},
                         state<dst_state>{});
    call<TEvent, args_t<A, TEvent>, typename SM::logger_t>::execute(a, event, sm, deps, subs);
    sm.process_internal_event(back::on_entry<back::_, TEvent>{event}, deps, subs, current_state);
    return true;
  }
  template <class TEvent, class SM, class TDeps, class TSubs>
  bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state, aux::false_type) {
    update_current_state(sm, deps, subs, current_state,
                         aux::get_id<typename SM::state_t, dst_state>((typename SM::states_ids_t *)0), state<src_state>{},
                         state<dst_state>{});
    call<TEvent, args_t<A, TEvent>, typename SM::logger_t>::execute(a, event, sm, deps, subs);
    return true;
  }
  A a;
};
template <class S2, class E, class A>
struct transition<state<internal>, state<S2>, front::event<E>, always, A> {
  static constexpr auto initial = state<S2>::initial;
  static constexpr auto history = state<S2>::history;
  using src_state = typename state<S2>::type;
  using dst_state = internal;
  using event = E;
  using guard = always;
  using action = A;
  using deps = aux::apply_t<aux::unique_t, get_deps_t<A, E>>;
  transition(const always &, const A &a) : a(a) {}
  template <class TEvent, class SM, class TDeps, class TSubs, class... Ts>
  bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &, Ts &&...) {
    call<TEvent, args_t<A, TEvent>, typename SM::logger_t>::execute(a, event, sm, deps, subs);
    return true;
  }
  A a;
};
template <class S1, class S2, class E, class G>
struct transition<state<S1>, state<S2>, front::event<E>, G, none> {
  static constexpr auto initial = state<S2>::initial;
  static constexpr auto history = state<S2>::history;
  using src_state = typename state<S2>::type;
  using dst_state = typename state<S1>::type;
  using event = E;
  using guard = G;
  using action = none;
  using deps = aux::apply_t<aux::unique_t, get_deps_t<G, E>>;
  transition(const G &g, const none &) : g(g) {}
  template <class TEvent, class SM, class TDeps, class TSubs>
  bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state, aux::true_type) {
    using g_t = decltype(call<TEvent, args_t<G, TEvent>, typename SM::logger_t>::execute(g, event, sm, deps, subs));
    if constexpr(requires{ g_t::value; }) {
      if constexpr(g_t::value) {
        sm.process_internal_event(back::on_exit<back::_, TEvent>{event}, deps, subs, current_state);
        update_current_state(sm, deps, subs, current_state,
                             aux::get_id<typename SM::state_t, dst_state>((typename SM::states_ids_t *)0), state<src_state>{},
                             state<dst_state>{});
        sm.process_internal_event(back::on_entry<back::_, TEvent>{event}, deps, subs, current_state);
        return true;
      }
    } else if (call<TEvent, args_t<G, TEvent>, typename SM::logger_t>::execute(g, event, sm, deps, subs)) {
      sm.process_internal_event(back::on_exit<back::_, TEvent>{event}, deps, subs, current_state);
      update_current_state(sm, deps, subs, current_state,
                           aux::get_id<typename SM::state_t, dst_state>((typename SM::states_ids_t *)0), state<src_state>{},
                           state<dst_state>{});
      sm.process_internal_event(back::on_entry<back::_, TEvent>{event}, deps, subs, current_state);
      return true;
    }
    return false;
  }
  template <class TEvent, class SM, class TDeps, class TSubs>
  bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state, aux::false_type) {
    using g_t = decltype(call<TEvent, args_t<G, TEvent>, typename SM::logger_t>::execute(g, event, sm, deps, subs));
    if constexpr(requires{ g_t::value; }) {
      if constexpr(g_t::value) {
        update_current_state(sm, deps, subs, current_state,
                             aux::get_id<typename SM::state_t, dst_state>((typename SM::states_ids_t *)0), state<src_state>{},
                             state<dst_state>{});
        return true;
      }
    } else if (call<TEvent, args_t<G, TEvent>, typename SM::logger_t>::execute(g, event, sm, deps, subs)) {
      update_current_state(sm, deps, subs, current_state,
                           aux::get_id<typename SM::state_t, dst_state>((typename SM::states_ids_t *)0), state<src_state>{},
                           state<dst_state>{});
      return true;
    }
    return false;
  }
  G g;
};
template <class S2, class E, class G>
struct transition<state<internal>, state<S2>, front::event<E>, G, none> {
  static constexpr auto initial = state<S2>::initial;
  static constexpr auto history = state<S2>::history;
  using src_state = typename state<S2>::type;
  using dst_state = internal;
  using event = E;
  using guard = G;
  using action = none;
  using deps = aux::apply_t<aux::unique_t, get_deps_t<G, E>>;
  transition(const G &g, const none &) : g(g) {}
  template <class TEvent, class SM, class TDeps, class TSubs, class... Ts>
  bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &, Ts &&...) {
    return call<TEvent, args_t<G, TEvent>, typename SM::logger_t>::execute(g, event, sm, deps, subs);
  }
  G g;
};
template <class S1, class S2, class E>
struct transition<state<S1>, state<S2>, front::event<E>, always, none> {
  static constexpr auto initial = state<S2>::initial;
  static constexpr auto history = state<S2>::history;
  using src_state = typename state<S2>::type;
  using dst_state = typename state<S1>::type;
  using event = E;
  using guard = always;
  using action = none;
  using deps = aux::type_list<>;
  transition(const always &, const none &) {}
  template <class TEvent, class SM, class TDeps, class TSubs>
  bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state, aux::true_type) {
    sm.process_internal_event(back::on_exit<back::_, TEvent>{event}, deps, subs, current_state);
    update_current_state(sm, deps, subs, current_state,
                         aux::get_id<typename SM::state_t, dst_state>((typename SM::states_ids_t *)0), state<src_state>{},
                         state<dst_state>{});
    sm.process_internal_event(back::on_entry<back::_, TEvent>{event}, deps, subs, current_state);
    return true;
  }
  template <class TEvent, class SM, class TDeps, class TSubs>
  bool execute(const TEvent &, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state, aux::false_type) {
    update_current_state(sm, deps, subs, current_state,
                         aux::get_id<typename SM::state_t, dst_state>((typename SM::states_ids_t *)0), state<src_state>{},
                         state<dst_state>{});
    return true;
  }
  __BOOST_SML_ZERO_SIZE_ARRAY(aux::byte);
};
template <class S2, class E>
struct transition<state<internal>, state<S2>, front::event<E>, always, none> {
  static constexpr auto initial = state<S2>::initial;
  static constexpr auto history = state<S2>::history;
  using src_state = typename state<S2>::type;
  using dst_state = internal;
  using event = E;
  using guard = always;
  using action = none;
  using deps = aux::type_list<>;
  transition(const always &, const none &) {}
  template <class TEvent, class SM, class TDeps, class TSubs, class... Ts>
  bool execute(const TEvent &, SM &, TDeps &, TSubs &, typename SM::state_t &, Ts &&...) {
    return true;
  }
  __BOOST_SML_ZERO_SIZE_ARRAY(aux::byte);
};
}  // namespace front
using _ = back::_;
template <class... TEvents>
front::event<aux::type_list<TEvents...>> event __BOOST_SML_VT_INIT;
template <class... TEvents>
front::event<aux::type_list<TEvents...>> event<boost::mp11::mp_list<TEvents...>> __BOOST_SML_VT_INIT;
template <class TEvent>
__BOOST_SML_UNUSED front::event<back::on_entry<_, TEvent>> on_entry __BOOST_SML_VT_INIT;
template <class TEvent>
__BOOST_SML_UNUSED front::event<back::on_exit<_, TEvent>> on_exit __BOOST_SML_VT_INIT;
template <class TEvent>
front::event<back::unexpected_event<TEvent>> unexpected_event __BOOST_SML_VT_INIT;
template <class T>
front::event<back::exception<T>> exception __BOOST_SML_VT_INIT;
using anonymous = back::anonymous;
using initial = back::initial;
template <class T>
typename front::state_sm<T>::type state __BOOST_SML_VT_INIT;
#if !defined(_MSC_VER)
template <class T, T... Chrs>
auto operator""_s() {
  return front::state<aux::string<T, Chrs...>>{};
}
template <class T, T... Chrs>
auto operator""_e() {
  return event<aux::string<T, Chrs...>>;
}
#endif
__BOOST_SML_UNUSED static front::state<back::terminate_state> X;
__BOOST_SML_UNUSED static front::history_state H;
__BOOST_SML_UNUSED static front::actions::defer defer;
namespace front {
__BOOST_SML_UNUSED static constexpr auto emit = [](auto expr) { return front::actions::emit{expr}; };
__BOOST_SML_UNUSED static constexpr auto eval = state<class SML_EVAL>{};
__BOOST_SML_UNUSED static constexpr auto fwd = []<class TEvent>(const TEvent& event, auto&... args) { aux::on(event, args...); return boost::mp11::mp_list<TEvent>{}; };
}
__BOOST_SML_UNUSED static front::actions::process process;
template <class... Ts, __BOOST_SML_REQUIRES(aux::is_same<aux::bool_list<aux::always<Ts>::value...>,
                                                         aux::bool_list<concepts::transitional<Ts>::value...>>::value)>
auto make_transition_table(Ts... ts) {
  return aux::pool<Ts...>{ts...};
}
template <class... Ts>
auto make_dispatch_table(Ts... ts) {
  return aux::pool<Ts...>{ts...};
}
template <class... Ts>
  requires aux::is_same<aux::bool_list<aux::always<Ts>::value...>, aux::bool_list<concepts::transitional<Ts>::value...>>::value
struct transition_table : aux::pool<Ts...> {
  transition_table(Ts... ts) : aux::pool<Ts...>{ts...} {}
};
template <class... Ts>
  requires aux::is_same<aux::bool_list<aux::always<Ts>::value...>, aux::bool_list<concepts::transitional<Ts>::value...>>::value
struct dispatch_table : aux::pool<Ts...> {
  dispatch_table(Ts... ts) : aux::pool<Ts...>{ts...} {}
};
constexpr auto otherwise = [] { return std::true_type{}; };
namespace dsl {
using boost::sml::event;
using boost::sml::process;
using boost::sml::X;
using boost::sml::otherwise;
using boost::sml::make_dispatch_table;
using boost::sml::make_transition_table;
using boost::sml::dispatch_table;
using boost::sml::transition_table;
using boost::sml::operator,;
using boost::sml::operator!;
using boost::sml::operator||;
using boost::sml::operator&&;
using boost::sml::operator""_s;
using boost::sml::front::emit;
using boost::sml::front::eval;
using boost::sml::front::fwd;
} // namespace dsl
BOOST_SML_NAMESPACE_END
#undef __BOOST_SML_UNUSED
#undef __BOOST_SML_VT_INIT
#undef __BOOST_SML_ZERO_SIZE_ARRAY
#undef __BOOST_SML_ZERO_SIZE_ARRAY_CREATE
#undef __BOOST_SML_TEMPLATE_KEYWORD
#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#undef __has_builtin
#pragma GCC diagnostic pop
#elif defined(_MSC_VER)
#undef __has_builtin
#undef __has_builtin__make_integer_seq
#endif
#endif
