
//          Copyright Jesse Williamson 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef EXCEPTIONAL_HPP_20150729
 #define EXCEPTIONAL_HPP_20150729 1

#include "attempt/detail.hpp"

#include <functional>

namespace exceptional {

template <typename FnT>
auto attempt(FnT f)
{
 return [f](auto&& ...xs)
	{
		return std::experimental::optional<detail::my_result_of_type<FnT>>(
			f(std::forward<decltype(xs)>(xs)...));
	};
}

template <typename FnT, typename HandlerT0>
auto attempt(FnT f, HandlerT0 h)
{
 return [f, h](auto&& ...xs) 
        {
                try
                {
                        return std::experimental::optional<detail::my_result_of_type<FnT>>(
                                f(std::forward<decltype(xs)>(xs)...));
                }
                catch(detail::arg_at_type<HandlerT0, 0>& e)
                {
                        return detail::invoke_handler<FnT>(h, e);
                }
        };
}

template <typename FnT, typename HandlerT0, typename ...HandlerTS>
auto attempt(FnT f, HandlerT0 h, HandlerTS ...hs)
{
 return [f, h, hs...](auto&& ...xs)
        {
                try
                {
                        return attempt(f, hs...)(std::forward<decltype(xs)>(xs)...);
                }
                catch(detail::arg_at_type<HandlerT0, 0>& e)
                {
                        return detail::invoke_handler<FnT>(h, e);
                }
        };
}

} // namespace exceptional

#endif
