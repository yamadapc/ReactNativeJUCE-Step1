#pragma once

#include "CJSFunction.h"
#include "CJSValue.h"
#include <JavaScriptCore/JavaScriptCore.h>
#include <boost/callable_traits.hpp>
#include <tuple>
#include <vector>

namespace cpp_javascriptcore
{

template <typename Arg> CJSValue convertValue (JSContextRef context, Arg value)
{
    return CJSValue (context, value);
}

std::vector<JSValueRef> convertValues (JSContextRef context);

template <typename Arg, typename... Args>
std::vector<JSValueRef> convertValues (JSContextRef context, Arg value, Args... tail)
{
    auto ref = convertValue (context, value);
    auto rest = convertValues (context, tail...);
    rest.insert (rest.begin (), ref.getValue ());
    return std::move (rest);
}

template <typename Ret> Ret fromJS (JSContextRef context, JSValueRef value)
{
    auto cvalue = CJSValue (context, value);
    return cvalue.get<Ret> ();
}

template <typename F, typename ArgTypes, std::size_t N, std::size_t... Idx>
decltype (auto) applyImpl (F fn, ArgTypes& args, std::index_sequence<Idx...>)
{
    return fn (std::get<Idx> (args)...);
}

template <typename F, typename ArgTypes, std::size_t N> decltype (auto) apply (F fn, ArgTypes& args)
{
    return applyImpl (fn, args, std::make_index_sequence<N>{});
}

template <typename T, typename F, typename ArgTypes, std::size_t N, std::size_t... Idx>
decltype (auto) applyMethodImpl (T* instance, F fn, ArgTypes& args, std::index_sequence<Idx...>)
{
    return (instance->*fn) (std::get<Idx> (args)...);
}

template <typename T, typename F, typename ArgTypes, std::size_t N>
decltype (auto) applyMethod (T* instance, F fn, ArgTypes& args)
{
    return applyMethodImpl<T, F, ArgTypes, N> (instance, fn, args, std::make_index_sequence<N>{});
}

template <typename TupleType, std::size_t TupleSize, bool isClass = false>
TupleType fromJSArguments (JSContextRef lcontext, size_t, const JSValueRef jsArguments[])
{
    TupleType arguments;

    if constexpr (TupleSize == 0)
    {
    }
    else if constexpr (TupleSize == 1)
    {
        std::get<0> (arguments) = fromJS<typename std::tuple_element<0, TupleType>::type> (lcontext, jsArguments[0]);
    }
    else if constexpr (TupleSize == 2)
    {
        std::get<0> (arguments) = fromJS<typename std::tuple_element<0, TupleType>::type> (lcontext, jsArguments[0]);
        std::get<1> (arguments) = fromJS<typename std::tuple_element<1, TupleType>::type> (lcontext, jsArguments[1]);
    }
    else if constexpr (TupleSize == 3)
    {
        std::get<0> (arguments) = fromJS<typename std::tuple_element<0, TupleType>::type> (lcontext, jsArguments[0]);
        std::get<1> (arguments) = fromJS<typename std::tuple_element<1, TupleType>::type> (lcontext, jsArguments[1]);
        std::get<2> (arguments) = fromJS<typename std::tuple_element<2, TupleType>::type> (lcontext, jsArguments[2]);
    }
    else if constexpr (TupleSize == 4)
    {
        std::get<0> (arguments) = fromJS<typename std::tuple_element<0, TupleType>::type> (lcontext, jsArguments[0]);
        std::get<1> (arguments) = fromJS<typename std::tuple_element<1, TupleType>::type> (lcontext, jsArguments[1]);
        std::get<2> (arguments) = fromJS<typename std::tuple_element<2, TupleType>::type> (lcontext, jsArguments[2]);
        std::get<3> (arguments) = fromJS<typename std::tuple_element<3, TupleType>::type> (lcontext, jsArguments[3]);
    }
    else if constexpr (TupleSize == 5)
    {
        std::get<0> (arguments) = fromJS<typename std::tuple_element<0, TupleType>::type> (lcontext, jsArguments[0]);
        std::get<1> (arguments) = fromJS<typename std::tuple_element<1, TupleType>::type> (lcontext, jsArguments[1]);
        std::get<2> (arguments) = fromJS<typename std::tuple_element<2, TupleType>::type> (lcontext, jsArguments[2]);
        std::get<3> (arguments) = fromJS<typename std::tuple_element<3, TupleType>::type> (lcontext, jsArguments[3]);
        std::get<4> (arguments) = fromJS<typename std::tuple_element<4, TupleType>::type> (lcontext, jsArguments[4]);
    }
    else if constexpr (TupleSize == 6)
    {
        std::get<0> (arguments) = fromJS<typename std::tuple_element<0, TupleType>::type> (lcontext, jsArguments[0]);
        std::get<1> (arguments) = fromJS<typename std::tuple_element<1, TupleType>::type> (lcontext, jsArguments[1]);
        std::get<2> (arguments) = fromJS<typename std::tuple_element<2, TupleType>::type> (lcontext, jsArguments[2]);
        std::get<3> (arguments) = fromJS<typename std::tuple_element<3, TupleType>::type> (lcontext, jsArguments[3]);
        std::get<4> (arguments) = fromJS<typename std::tuple_element<4, TupleType>::type> (lcontext, jsArguments[4]);
        std::get<5> (arguments) = fromJS<typename std::tuple_element<5, TupleType>::type> (lcontext, jsArguments[5]);
    }
    else if constexpr (TupleSize == 7)
    {
        std::get<0> (arguments) = fromJS<typename std::tuple_element<0, TupleType>::type> (lcontext, jsArguments[0]);
        std::get<1> (arguments) = fromJS<typename std::tuple_element<1, TupleType>::type> (lcontext, jsArguments[1]);
        std::get<2> (arguments) = fromJS<typename std::tuple_element<2, TupleType>::type> (lcontext, jsArguments[2]);
        std::get<3> (arguments) = fromJS<typename std::tuple_element<3, TupleType>::type> (lcontext, jsArguments[3]);
        std::get<4> (arguments) = fromJS<typename std::tuple_element<4, TupleType>::type> (lcontext, jsArguments[4]);
        std::get<5> (arguments) = fromJS<typename std::tuple_element<5, TupleType>::type> (lcontext, jsArguments[5]);
        std::get<6> (arguments) = fromJS<typename std::tuple_element<6, TupleType>::type> (lcontext, jsArguments[6]);
    }
    else
    {
        assert (false);
    }

    return arguments;
}

template <typename Fn> CJSFunction::Callback makeCallback (Fn&& callback)
{
    using ArgTypes = boost::callable_traits::args_t<Fn>;          // std::tuple<...>
    using ReturnType = boost::callable_traits::return_type_t<Fn>; // return type
    using Arity = std::tuple_size<ArgTypes>;                      // ::value

    CJSFunction::Callback jsCallback = [&](JSContextRef lcontext,
                                           JSObjectRef,
                                           JSObjectRef,
                                           size_t jsArgumentsSize,
                                           const JSValueRef jsArguments[],
                                           JSValueRef*) {
        ArgTypes arguments = fromJSArguments<ArgTypes, Arity::value, true> (lcontext, jsArgumentsSize, jsArguments);

        if constexpr (std::is_same<ReturnType, void>::value)
        {
            apply (callback, arguments);
            return JSValueMakeNull (lcontext);
        }
        else
        {
            ReturnType ret = apply (callback, arguments);
            return CJSValue (lcontext, ret).getValue ();
        }
    };
    return jsCallback;
}

template <typename T> struct PopFront
{
    typedef std::tuple<> type;
};

template <typename T, typename... Ts> struct PopFront<std::tuple<T, Ts...>>
{
    typedef std::tuple<Ts...> type;
};

template <typename T, typename Fn> CJSFunction::Callback makeMethodCallback (Fn&& callback)
{
    using ArgTypes = typename PopFront<boost::callable_traits::args_t<Fn>>::type; // std::tuple<...>
    using ReturnType = boost::callable_traits::return_type_t<Fn>;                 // return type
    using Arity = std::tuple_size<ArgTypes>;                                      // ::value

    CJSFunction::Callback jsCallback = [&](JSContextRef lcontext,
                                           JSObjectRef,
                                           JSObjectRef thisValue,
                                           size_t jsArgumentsSize,
                                           const JSValueRef jsArguments[],
                                           JSValueRef*) {
        auto* instance = (T*)JSObjectGetPrivate (thisValue);
        ArgTypes arguments = fromJSArguments<ArgTypes, Arity::value> (lcontext, jsArgumentsSize, jsArguments);

        if constexpr (std::is_same<ReturnType, void>::value)
        {
            applyMethod<T, Fn, ArgTypes, Arity::value> (instance, callback, arguments);
            return JSValueMakeNull (lcontext);
        }
        else
        {
            ReturnType ret = applyMethod<T, Fn, ArgTypes, Arity::value> (instance, callback, arguments);
            return CJSValue (lcontext, ret).getValue ();
        }
    };
    return jsCallback;
}

} // namespace cpp_javascriptcore
