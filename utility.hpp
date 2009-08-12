#ifndef UTILITY_H__
#define UTILITY_H__
//////////////////////////////////////////////////////////////////////////
/* A collection of utility functions to simplify the use of commonly used 
   STL algorithms. 
   Also, an independent algorithm copy_if*/
//////////////////////////////////////////////////////////////////////////
//#define ENABLE_TOPOLOGICALSORT_DEBUGPRINTS
//////////////////////////////////////////////////////////////////////////
template<typename tContainer, typename tFunctor>
tFunctor for_all( tContainer& c, const tFunctor& f)
{
    return std::for_each(c.begin(), c.end(), f);
}
//////////////////////////////////////////////////////////////////////////
template<typename tContainer, typename tFunctor>
typename tContainer::const_iterator find_if_all( const tContainer& c, const tFunctor& f )
{
    return std::find_if(c.begin(), c.end(), f);
}
//////////////////////////////////////////////////////////////////////////
template<typename tContainer, typename tFunctor>
typename tContainer::const_iterator find_all( const tContainer& c, const tFunctor& f )
{
    return std::find(c.begin(), c.end(), f);
}
//////////////////////////////////////////////////////////////////////////
template< typename Inputlterator, typename Outputlterator, typename Predicate>
Outputlterator copy_if(Inputlterator begin, Inputlterator end, Outputlterator destBegin, Predicate p)
{
    while (begin != end) {
        if (p(*begin))
            *destBegin++ = *begin;
        ++begin;
    }
    return destBegin;
}
//////////////////////////////////////////////////////////////////////////
#endif // UTILITY_H__
