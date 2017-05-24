#pragma once
#ifndef valueptr_h__2009_10_11_13_32_13_begin_guarded_code
#define valueptr_h__2009_10_11_13_32_13_begin_guarded_code

#include <algorithm> 

namespace Qwt3D
{

// Auxiliary traits providing default clone()

template<typename T>
struct ValuePtrTraits  
{
  static  T* clone( const T* p )  
  { 
    //return new T(*p); 
    return p->clone(); 
  }
};

// Copyable smart pointer, taken from Sutter's "More exceptional C++"

template<typename T>
class ValuePtr  
{
public:
  explicit ValuePtr( T* p = 0 ) 
    : rawptr_( p )  
  {}
  ~ValuePtr()  
  { 
    delete rawptr_; 
  }
  ValuePtr( const ValuePtr& orig )
    : rawptr_( createFrom( orig.rawptr_ ) )
  {}

  ValuePtr&  operator= ( const ValuePtr& orig )  
  {
    ValuePtr<T>  temp( orig );
    std::swap(rawptr_, temp.rawptr_);
    return *this;
  }

  //// templated assignment  
  //template< class U >
  //ValuePtr( const ValuePtr<U>& orig )
  //  : rawptr_( createFrom( orig.operator->() ) )
  //{}

  //template< class U >
  //ValuePtr &  operator= ( const ValuePtr<U>& orig )  
  //{
  //  ValuePtr<T>  temp( orig );
  //  std::swap(rawptr_, temp.rawptr_);
  //  return *this;
  //}

  T&  operator*  () const  { return *rawptr_; }
  T*  operator-> () const  { return  rawptr_; }


private:
  template<typename U>
  T*  createFrom( const U* p ) const  
  {
    return (p)
      ? ValuePtrTraits<U>::clone(p)
         : 0;
  }

  T*  rawptr_;
};

} // ns

#endif /* include guard */
