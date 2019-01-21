/**
 * @copyright 2019 TwoD
 * @author Brian Cairl
 *
 * @file primitives.h
 * @brief Grid primitive base definitions
 */
#ifndef TWOD_PRIMITIVES_H
#define TWOD_PRIMITIVES_H

// TwoD
#include <twod/coordinates.h>
#include <twod/fwd.h>

namespace twod
{

/**
 * @brief Helper object which implements compile-time determined extent attributes which satisfy GridBase interface
 *
 * @tparam Height  compile-time grid height
 * @tparam Width  compile-time grid width
 */
template<int Height, int Width>
class FixedExtentsBase
{
protected:
  /// @copydoc GridBase::extents
  constexpr static Extents extents_impl()
  {
    return extents_;
  }

  /// Grid extents
  constexpr static const Extents extents_{Height, Width};

  template<typename OtherDerived> friend class BoundsBase;
  template<typename OtherDerived, typename OtherCellT> friend class GridBase;
};


/**
 * @brief Helper object which implements runtime determined extent attributes which satisfy GridBase interface
 */
class ResizableExtentsBase
{
protected:
  ResizableExtentsBase(const ResizableExtentsBase&) = default;

  /// Extents constructor
  explicit ResizableExtentsBase(const Extents& extents) :
    extents_{extents}
  {}

  /// @copydoc GridBase::extents
  inline Extents extents_impl() const
  {
    return extents_;
  }

  /// Grid extents
  Extents extents_;

  template<typename OtherDerived> friend class BoundsBase;
  template<typename OtherDerived, typename OtherCellT> friend class GridBase;
};


/**
 * @brief Helper object which
 *
 * @tparam X  compile-time origin X-coordinate
 * @tparam Y  compile-time origin T-coordinate
 */
template<int X, int Y>
class FixedOriginBase
{
protected:
  /// @copydoc GridBase::origin
  constexpr static Indices origin_impl()
  {
    return origin_;
  }

  /// Origin location
  constexpr static const Indices origin_{X, Y};

  template<typename OtherDerived> friend class BoundsBase;
  template<typename OtherDerived, typename OtherCellT> friend class GridBase;
};


/**
 * @brief Helper object which
 */
class DynamicOriginBase
{
protected:
  DynamicOriginBase(const DynamicOriginBase&) = default;

  /// Indices constructor
  explicit DynamicOriginBase(const Indices& origin) :
    origin_{origin}
  {}

  /// @copydoc GridBase::origin
  inline Indices origin_impl() const
  {
    return origin_;
  }

  /// Origin location
  Indices origin_;

  template<typename OtherDerived> friend class BoundsBase;
  template<typename OtherDerived, typename OtherCellT> friend class GridBase;
};

}  // namespace twod

#endif // TWOD_PRIMITIVES_H