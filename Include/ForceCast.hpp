#ifndef FORCE_CAST_HPP
#define FORCE_CAST_HPP

#ifdef ALLOW_FORCE_CAST

namespace util
{
    template<class Orig, class T>
    T ForceCast(Orig p)
    {
        union
        {
            Orig original;
            T ptr;
        };
        original = p;
        return ptr;
    }

    template<class T>
    void* ForceVoid(T p)
    {
        union
        {
            T original;
            void* ptr;
        };
        original = p;
        return ptr;
    }
}

#endif
#endif // FORCE_CAST_HPP
