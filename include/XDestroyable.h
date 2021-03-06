#pragma once
#include "XTrait.h"

namespace Eks
{

namespace Trait
{

class Destroyable
  {
public:
  template <typename Derived> class Trait
    {
  public:
    struct Functions
      {
      struct
        {
        void (*assign)(detail::PrivateData *ths, const detail::ConstObjectWrapper &obj);
        void (*destroy)(detail::PrivateData *ths);
        } destroy;

      X_TRAIT_FUNCTION_IMPL(destroy, assign, destroy);
      };

    X_TRAIT_IMPL

    struct Wrapper
      {
      void *data;
      };

  public:
    detail::ConstObjectWrapper object() const
      {
      return const_cast<Trait<Derived>*>(this)->data()->template data<Wrapper>();
      }

    detail::ObjectWrapper object()
      {
      return data()->template data<Wrapper>();
      }

    void clear()
      {
      auto d = data();
      if(d->isValid())
        {
        functions()->destroy.destroy(d);
        }
      }

  protected:
    void assignObject(const Derived &t)
      {
      clear();

      functions()->destroy.assign(data(), t.object());
      }

    template <typename T> void assign(const T &t)
      {
      clear();

      functions()->destroy.assign(data(), &t);
      }
    };

  struct Default
    {
    template <typename T>
        static void assign(detail::PrivateData *ths, const detail::ConstObjectWrapper &t)
      {
      ths->create<T>(*t.as<T>());
      }

    template <typename T>
        static void copy(detail::PrivateData *ths, const detail::PrivateData *src)
      {
      ths->create<T>(*src->data<T>());
      }

    template <typename T>
        static void destroy(detail::PrivateData *ths)
      {
      ths->destroy<T>();
      }
    };
  };
}

}
