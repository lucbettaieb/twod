/**
 * @copyright 2019 TwoD
 * @author Brian Cairl
 *
 * @file bounds.h
 * @brief Grid bounds definitions
 */
#ifndef TWOD_BOUNDS_H
#define TWOD_BOUNDS_H

// TwoD
#include <twod/coordinates.h>
#include <twod/crtp.h>
#include <twod/fwd.h>
#include <twod/primitives.h>

namespace twod
{

/**
 * @brief
 */
template<typename Derived>
class BoundsBase
{
public:
  /**
   * @brief Equality comparison overload
   */
  constexpr bool operator==(const Derived& other) const
  {
    return this->origin() == other.origin() and
           this->extents() == other.extents();
  }

  /**
   * @brief Returns grid coordinate origin
   */
  constexpr bool operator!=(const Derived& other) const
  {
    return !this->operator==(other);
  }

  /**
   * @brief Returns grid coordinate origin
   */
  constexpr Indices origin() const
  {
    return derived()->origin_impl();
  }

  /**
   * @brief Returns grid coordinate extents
   */
  constexpr Extents extents() const
  {
    return derived()->extents_impl();
  }

  /**
   * @brief Returns center point of bounds
   */
  constexpr Indices center() const
  {
    return origin() + extents() / 2;
  }

  /**
   * @brief Check if grid (effectively) contains no values
   */
  constexpr bool empty() const
  {
    return this->extents() == Extents::Zero();
  }

  /**
   * @brief Check if point falls within grid extents
   * @param pt  index pair associated with grid element
   * @retval true  if point lies within grid
   * @retval false  otherwise
   */
  constexpr bool within(const Indices& pt) const
  {
    return pt.all_ge(this->origin()) and
           pt.all_lt(this->origin() + this->extents());
  }

  /**
   * @brief Check if bounds overlap
   * @param other  other bounds object
   * @retval true  if bounds overlap
   * @retval false  otherwise
   */
  template<typename OtherDerived>
  constexpr bool overlaps(const BoundsBase<OtherDerived>& other) const
  {
    return (this->origin() - other.origin()).abs().all_le(this->extents() +
                                                          other.extents());
  }

private:
  IMPLEMENT_CRTP_BASE_CLASS(Derived);
};


struct Bounds :
  public BoundsBase<Bounds>,
  public DynamicOriginBase,
  public ResizableExtentsBase
{
  template<typename Derived>
  Bounds(const BoundsBase<Derived>& bounds) :
    DynamicOriginBase{bounds.origin()},
    ResizableExtentsBase{bounds.extents()}
  {}

  Bounds(const Indices& origin, const Extents& extents) :
    DynamicOriginBase{origin},
    ResizableExtentsBase{extents}
  {}
};


template<int OriginX, int OriginY>
struct FixedOriginBounds :
  public BoundsBase<FixedOriginBounds<OriginX, OriginY>>,
  public FixedOriginBase<OriginX, OriginY>,
  public ResizableExtentsBase
{
  template<typename Derived>
  FixedOriginBounds(const BoundsBase<Derived>& bounds) :
    ResizableExtentsBase{bounds.extents()}
  {}

  FixedOriginBounds(const Extents& extents) :
    ResizableExtentsBase{extents}
  {}
};


template<int Height, int Width>
struct FixedExtentsBounds :
  public BoundsBase<FixedExtentsBounds<Height, Width>>,
  public DynamicOriginBase,
  public FixedExtentsBase<Height, Width>
{
  template<typename Derived>
  FixedExtentsBounds(const BoundsBase<Derived>& bounds) :
    DynamicOriginBase{bounds.origin()}
  {}

  FixedExtentsBounds(const Indices& origin) :
    DynamicOriginBase{origin}
  {}
};


template<int OriginX, int OriginY, int Height, int Width>
struct FixedOriginExtentsBounds :
  public BoundsBase<FixedOriginExtentsBounds<OriginX, OriginY, Height, Width>>,
  public FixedOriginBase<OriginX, OriginY>,
  public FixedExtentsBase<Height, Width>
{
};

}  // namespace twod

#endif // TWOD_BOUNDS_H