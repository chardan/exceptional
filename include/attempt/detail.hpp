#ifndef ATTEMPT_DETAIL_HPP_20150729
 #define ATTEMPT_DETAIL_HPP_20150729 1

#include <boost/function_types/result_type.hpp>
#include <boost/function_types/function_type.hpp>
#include <boost/function_types/parameter_types.hpp>

#include <experimental/optional>

#include <type_traits>

namespace exceptional { namespace detail {

/* Function-wrangling: */
template <typename FnT>
using my_result_of_type       = typename boost::function_types::result_type<FnT>::type;

template <typename FnT>
using args_of_type         = typename boost::function_types::parameter_types<FnT>::type;

template <typename FnT, int N>
using arg_at_type          = typename boost::mpl::at_c<args_of_type<FnT>, N>::type;

/* SFINAE Maybe: */
template <typename FnT,
          typename HandlerT, typename ExceptionT,
          typename Enable = void>
struct Maybe
{
 static auto invoke_handler(HandlerT& h, ExceptionT& e)
 {
  return std::experimental::optional<my_result_of_type<FnT>>(h(e)); 
 }
};

template <typename FnT,
          typename HandlerT, typename ExceptionT>
struct Maybe<FnT,
             HandlerT, ExceptionT, 
             typename std::enable_if_t <!std::is_constructible<my_result_of_type<FnT>, my_result_of_type<HandlerT>>::value>>
{
 static auto invoke_handler(HandlerT& h, ExceptionT& e)
 {
  return h(e), 
         std::experimental::optional<my_result_of_type<FnT>>();
 }
};

/* SFINAE Maybe dispatcher: */
template <typename FnT, typename HandlerT, typename ExceptionT>
auto invoke_handler(HandlerT& h, ExceptionT& e)
{
 return Maybe<FnT, HandlerT, ExceptionT>::invoke_handler(h, e);
}

}} // namespace exceptional::detail

#endif
