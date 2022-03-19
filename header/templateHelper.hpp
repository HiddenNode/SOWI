#ifndef TEMPLATEHELPER_HPP
#define TEMPLATEHELPER_HPP

#include <concepts>
#include <vector>

namespace sw
{
    class FuncBase{};

    template<class A, class B> class Func;

    template<class Target, class R, class... Args> 
    struct Func<Target, R (Target::*)(Args...)> : public FuncBase
    {
        using  type = R (Target::*)(Args...);
        using  target = Target*;

    /*     Func(type f):
        tt {f}
        {};
        type tt; */
    };

    template<class F, class ArgType, int I> requires (std::derived_from<F, FuncBase>)
    class MemberCaller
    {
        public:
        static void callMember(typename F::target,typename F::type f, std::vector<ArgType> c);
    };

    template<class F, class ArgType>
    class MemberCaller<F, ArgType, 0>
    {
    public:
        static void callMember(typename F::target t,typename F::type f, std::vector<ArgType> c)
        {
            (t->*f)();
        }
    };

    template<class F, class ArgType>
    class MemberCaller<F, ArgType, 1>
    {
    public:
        static void callMember(typename F::target t,typename F::type f, std::vector<ArgType> c)
        {
            (t->*f)(c[0]);
        }
    };

    template<class F, class ArgType>
    class MemberCaller<F, ArgType, 2>
    {
    public:
        static void callMember(typename F::target t,typename F::type f, std::vector<ArgType> c)
        {
            (t->*f)(c[0], c[1]);
        }
    };

    template<class F, class ArgType>
    class MemberCaller<F, ArgType, 3>
    {
    public:
        static void callMember(typename F::target t,typename F::type f, std::vector<ArgType> c)
        {
            (t->*f)(c[0], c[1], c[2]);
        }
    };

    template<class F, class ArgType>
    class MemberCaller<F, ArgType, 4>
    {
        static void callMember(typename F::target t,typename F::type f, std::vector<ArgType> c)
        {
            (t->*f)(c[0], c[1], c[2], c[3]);
        }
    };

    template<class F, class ArgType>
    class MemberCaller<F, ArgType, 5>
    {
    public:
        static void callMember(typename F::target t,typename F::type f, std::vector<ArgType> c)
        {
            (t->*f)(c[0], c[1], c[2], c[3], c[4]);
        }
    };

}


#endif