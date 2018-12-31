/**
 * @copyright 2018 TwoD
 * @author Brian Cairl
 *
 * @file grid.h
 * @brief Grid container and view implementations
 */
#ifndef TWOD_GRID_H
#define TWOD_GRID_H

// C++ Standard Library
#include <array>
#include <memory>
#include <utility>

// TwoD
#include <twod/coordinates.h>
#include <twod/crtp.h>
#include <twod/fwd.h>

namespace twod
{

/// Grid access index pair
using Indices = Coordinates<int>;

/// Grid sizing
using Extents = Coordinates<int>;

/**
 * @brief CRTP grid base interface
 *
 * @tparam Derived  CRTP derived grid object
 * @tparam CellT  grid element type
 */
template<typename Derived, typename CellT>
class GridBase
{
public:
  /**
   * @brief Returns grid view of size \p extents
   *
   *        View is anchored at bottom left corning to parent coordinates \p origin
   *
   * @param origin  viewing origin
   * @param extents  size of viewing region
   * @return view
   */
  inline View<Derived, CellT> view(const Indices& origin, const Extents& extents)
  {
    return View<Derived, CellT>{derived(), origin, extents};
  }

  /**
   * @brief Returns grid view of size \p extents
   *
   *        View is anchored at bottom left corning to parent coordinates \p origin
   *
   * @param origin  viewing origin
   * @param extents  size of viewing region
   * @return view
   */
  inline View<const Derived, const CellT> view(const Indices& origin, const Extents& extents) const
  {
    return View<const Derived, const CellT>{derived(), origin, extents};
  }

  /**
   * @brief Returns grid view with static sizing
   *
   *        View is anchored at bottom left corning to parent coordinates \p origin
   *
   * @param origin  viewing origin relative to parent grid
   * @return view
   */
  template<int Height, int Width>
  inline FixedExtentsView<Derived, CellT, Height, Width> view(const Indices& origin)
  {
    return FixedExtentsView<Derived, CellT, Height, Width>{derived(), origin};
  }

  /**
   * @brief Returns grid view with static sizing
   *
   *        View is anchored at bottom left corning to parent coordinates \p origin
   *
   * @tparam Height  compile-time view height
   * @tparam Width  compile-time view height
   *
   * @param origin  viewing origin relative to parent grid
   * @return view
   */
  template<int Height, int Width>
  inline FixedExtentsView<const Derived, const CellT, Height, Width> view(const Indices& origin) const
  {
    return FixedExtentsView<const Derived, const CellT, Height, Width>{derived(), origin};
  }

  /**
   * @brief Returns grid view with static sizing and origin
   *
   *        View is anchored at bottom left corning to parent coordinates \p origin
   *
   * @tparam OriginX  compile-time origin x-coordinate relative to parent
   * @tparam OriginY  compile-time origin y-coordinate relative to parent
   * @tparam Height  compile-time view height
   * @tparam Width  compile-time view height
   *
   * @return view
   */
  template<int OriginX, int OriginY, int Height, int Width>
  inline FixedOriginExtentsView<Derived, CellT, OriginX, OriginY, Height, Width> view()
  {
    return FixedOriginExtentsView<Derived, CellT, OriginX, OriginY, Height, Width>{derived()};
  }

  /**
   * @brief Returns grid view with static sizing and origin
   *
   *        View is anchored at bottom left corning to parent coordinates \p origin
   *
   * @tparam OriginX  compile-time origin x-coordinate relative to parent
   * @tparam OriginY  compile-time origin y-coordinate relative to parent
   * @tparam Height  compile-time view height
   * @tparam Width  compile-time view height
   *
   * @return view
   */
  template<int OriginX, int OriginY, int Height, int Width>
  inline FixedOriginExtentsView<const Derived, const CellT, OriginX, OriginY, Height, Width> view() const
  {
    return FixedOriginExtentsView<const Derived, const CellT, OriginX, OriginY, Height, Width>{derived()};
  }

  /**
   * @brief Check if point falls within grid extents
   * @param pt  index pair associated with grid element
   * @retval true  if point lies within grid
   * @retval false  otherwise
   */
  inline bool within(const Indices& pt) const
  {
    return pt.all_ge(Indices::Zero()) and
           pt.all_lt(this->extents());
  }

  /**
   * @brief Return mutable reference to element
   * @param pt  index pair associated with grid element
   * @return reference to cell
   */
  inline CellT& operator[](const Indices& pt)
  {
    return derived()->access_impl(pt);
  }

  /**
   * @brief Return immutable reference to element
   * @param pt  index pair associated with grid element
   * @return reference to cell
   */
  inline const CellT& operator[](const Indices& pt) const
  {
    return derived()->access_impl(pt);
  }

  /**
   * @brief Generic assignment from <code>GridBase</code> derivative
   *
   * @tparam OtherDerived  CRTP-derived <code>GridBase</code> object
   *
   * @param other  other grid
   * @return <code>*this</code>
   */
  template<typename OtherDerived>
  inline Derived& operator=(const GridBase<OtherDerived, CellT>& other)
  {
    auto this_itr = derived()->begin();
    for (auto other_itr = other.begin(); other_itr != other.end(); ++other_itr, ++this_itr)
    {
      *this_itr = *other_itr;
    }
    return *derived();
  }

  /**
   * @brief Compound cell-wise addition from <code>GridBase</code> derivative
   *
   * @tparam OtherDerived  CRTP-derived <code>GridBase</code> object
   *
   * @param other  other grid
   * @return <code>*this</code>
   */
  template<typename OtherDerived>
  inline Derived& operator+=(const GridBase<OtherDerived, CellT>& other)
  {
    auto this_itr = derived()->begin();
    for (auto other_itr = other.begin(); other_itr != other.end(); ++other_itr, ++this_itr)
    {
      *this_itr += *other_itr;
    }
    return *derived();
  }

  /**
   * @brief Compound cell-wise subtraction from <code>GridBase</code> derivative
   *
   * @tparam OtherDerived  CRTP-derived <code>GridBase</code> object
   *
   * @param other  other grid
   * @return <code>*this</code>
   */
  template<typename OtherDerived>
  inline Derived& operator-=(const GridBase<OtherDerived, CellT>& other)
  {
    auto this_itr = derived()->begin();
    for (auto other_itr = other.begin(); other_itr != other.end(); ++other_itr, ++this_itr)
    {
      *this_itr -= *other_itr;
    }
    return *derived();
  }

  /**
   * @brief Compound cell scaling
   *
   * @tparam ScalarT  scalar value type
   *
   * @param scale  scaling value
   * @return <code>*this</code>
   */
  template<typename ScalarT>
  inline Derived& operator*=(const ScalarT scale)
  {
    for (auto& c : *derived())
    {
      c *= scale;
    }
    return *derived();
  }

  /**
   * @brief Compound cell division
   *
   * @tparam ScalarT  scalar value type
   *
   * @param scale  scaling value
   * @return <code>*this</code>
   */
  template<typename ScalarT>
  inline Derived& operator/=(const ScalarT scale)
  {
    for (auto& c : *derived())
    {
      c /= scale;
    }
    return *derived();
  }

  /**
   * @brief Cell wise inequality comparison <code>GridBase</code> derivative
   *
   * @tparam OtherDerived  CRTP-derived <code>GridBase</code> object
   *
   * @param other  other grid
   *
   * @retval true  if any cell of \p other is not equal to corresponding cell in <code>*this</code>
   * @retval false  otherwise
   */
  template<typename OtherDerived>
  inline bool operator!=(const class GridBase<OtherDerived, CellT>& other) const
  {
    auto this_itr = derived()->begin();
    for (auto other_itr = other.begin(); other_itr != other.end(); ++other_itr, ++this_itr)
    {
      if (*this_itr != *other_itr)
      {
        return true;
      }
    }
    return false;
  }

  /**
   * @brief Cell wise equality comparison <code>GridBase</code> derivative
   *
   * @tparam OtherDerived  CRTP-derived <code>GridBase</code> object
   *
   * @param other  other grid
   *
   * @retval true  if all cells of \p other are equal to corresponding cells in <code>*this</code>
   * @retval false  otherwise
   */
  template<typename OtherDerived>
  inline bool operator==(const class GridBase<OtherDerived, CellT>& other) const
  {
    return not this->operator!=(other);
  }

  /**
   * @brief Returns grid coordinate extents
   */
  inline auto extents() const
  {
    return derived()->extents_impl();
  }

  /**
   * @brief Check if grid (effectively) contains no values
   */
  inline bool empty() const
  {
    return derived()->extents() == Extents::Zero();
  }

  /**
   * @brief Sets all cells to a uniform value
   * @param value  cell value
   * @return <code>*this</code>
   */
  inline Derived& fill(const CellT& value)
  {
    for (auto& c : *derived())
    {
      c = value;
    }
    return *derived();
  }

  /**
   * @brief Returns iterator (mutable) to first element
   */
  inline auto begin()
  {
    return derived()->begin_impl();
  }

  /**
   * @brief Returns iterator (mutable) to one past last element
   */
  inline auto end()
  {
    return derived()->end_impl();
  }

  /**
   * @brief Returns iterator (immutable) to first element
   */
  inline auto begin() const
  {
    return derived()->begin_impl();
  }

  /**
   * @brief Returns iterator (immutable) to one past last element
   */
  inline auto end() const
  {
    return derived()->end_impl();
  }

  /**
   * @brief Returns iterator (immutable) to first element
   */
  inline auto cbegin() const
  {
    return derived()->begin_impl();
  }

  /**
   * @brief Returns iterator (immutable) to one past last element
   */
  inline auto cend() const
  {
    return derived()->end_impl();
  }

private:
  IMPLEMENT_CRTP_BASE_CLASS(Derived);
};


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

  template<typename OtherDerived, typename OtherCellT>
  friend class GridBase;
};


/**
 * @brief Helper object which implements runtime determined extent attributes which satisfy GridBase interface
 */
class ResizableExtentsBase
{
protected:
  /// Extents constructor
  explicit ResizableExtentsBase(const Extents& extents) :
    extents_{extents}
  {}

  /// @copydoc GridBase::extents
  inline const Extents& extents_impl() const
  {
    return extents_;
  }

  /// Grid extents
  Extents extents_;

  template<typename OtherDerived, typename OtherCellT>
  friend class GridBase;
};


/**
 * @brief CRTP view iterator base object
 *
 * @tparam Derived  derived iterator type
 * @tparam GridT    associated parent grid type
 */
template<typename Derived, typename GridT>
class ViewIteratorBase
{
public:
  /**
   * @brief Initialization constructor
   * @param gp  parent grid pointer
   * @param pt  initial view-relative point
   */
  ViewIteratorBase(GridT* const gp, const Indices& pt) :
    gp_{gp},
    pt_{pt}
  {}

  /**
   * @brief Pre-increment overload
   */
  inline Derived& operator++()
  {
    ++pt_.x;
    if (pt_.x == derived()->extents_.x)
    {
      pt_.x = 0;
      ++pt_.y;
    }
    return *derived();
  }

  /**
   * @brief Post-increment overload
   */
  inline Derived operator++(int)
  {
    const Derived iterator{*derived()};
    derived()->operator++();
    return iterator;
  }

  /**
   * @brief Mutable cell value dereference operator
   */
  inline auto& operator*()
  {
    return derived()->operator[](pt_);
  }

  /**
   * @brief Immutable cell value dereference operator
   */
  inline const auto& operator*() const
  {
    return derived()->operator[](pt_);
  }

  /**
   * @brief Mutable cell value dereference operator
   */
  inline auto* operator->()
  {
    return std::addressof(derived()->operator[](pt_));
  }

  /**
   * @brief Immutable cell value dereference operator
   */
  inline const auto* operator->() const
  {
    return std::addressof(derived()->operator[](pt_));
  }

  /**
   * @brief Returns active view-relative index pair associated with iterator
   */
  const Indices& coords() const
  {
    return pt_;
  }

  /**
   * @brief Returns pointer to parent grid referred to by associated view
   */
  const GridT* const grid() const
  {
    return gp_;
  }

protected:
  /// Parent grid pointer
  GridT* const gp_;

  /// View-relative index
  Indices pt_;

  IMPLEMENT_CRTP_BASE_CLASS(Derived);
};


/**
 * @brief Special object used to represent <code>ViewIterator end()</code> value
 */
template<typename GridT>
class ViewIteratorEnd
{
public:
  /// Width constructor
  explicit ViewIteratorEnd(int width) :
    width_{width}
  {}

  /**
   * @brief Cast to associated special-value ViewIterator
   */
  inline operator ViewIterator<GridT>() const
  {
    return ViewIterator<GridT>{*this};
  }

private:
  /// Grid view width
  const int width_;

  template<typename OtherGridT>
  friend class ViewIterator;
};


/**
 * @brief Dynamic origin/extents grid view iterator
 *
 * @tparam GridT  parent grid type
 */
template<typename GridT>
class ViewIterator :
  public ResizableExtentsBase,
  public ViewIteratorBase<ViewIterator<GridT>, GridT>
{
  using ViewBase = ViewIteratorBase<ViewIterator<GridT>, GridT>;
public:
  /**
   * @brief Implicit ViewIteratorEnd copy constructor
   *
   *        Constructor ViewIterator with special values equivalent to associated ViewIteratorEnd
   *
   * @param end  end iterator object
   *
   * @note Allows for ViewIterator to work in <code>std::*</code> algorithm functions
   *       which do not deduce separate <code>end()</code> iterator type
   */
  ViewIterator(const ViewIteratorEnd<GridT>& end) :
    ResizableExtentsBase{-1, end.width_},
    ViewBase{nullptr, Indices::Zero()},
    origin_{-1, -1}
  {}

  /**
   * @brief Copy constructor
   *
   * @param other  object to copy
   */
  template<typename OtherGridT>
  ViewIterator(const ViewIterator<OtherGridT>& other) :
    ResizableExtentsBase{other.extents_},
    ViewBase{other.grid(), other.coords()},
    origin_{other.origin()}
  {}

  /**
   * @brief Initialization constructor
   *
   * @param gp  parent grid points
   * @param origin  origin of view, relative to parent
   * @param extents  view extents
   */
  ViewIterator(GridT* const gp, const Indices& origin, const Extents& extents) :
    ResizableExtentsBase{extents},
    ViewBase{gp, Indices::Zero()},
    origin_{origin}
  {}

  /**
   * @brief Returns view origin relative to parent grid
   */
  const Indices& origin() const
  {
    return origin_;
  }

  /**
   * @brief Iterator equality operator
   *
   *        Handles ViewIterator used to represent the more conservative ViewIteratorEnd object
   *
   * @param other  other iterator
   *
   * @retval true  if \p other iterator refer to the same element
   * @retval false  otherwise
   */
  inline bool operator==(const ViewIterator& other) const
  {
    return (other.gp_ == nullptr and this->pt_.y == other.extents_.y) or
           (this->gp_ == other.gp_ and
            this->pt_ == other.pt_ and
            this->origin_ == other.origin_ and
            this->extents_ == other.extents_);
  }

  /**
   * @brief Iterator inequality operator
   *
   *        Handles ViewIterator used to represent the more conservative ViewIteratorEnd object
   *
   * @param other  other iterator
   *
   * @retval true  if \p other iterator do not refer to the same element
   * @retval false  otherwise
   */
  inline bool operator!=(const ViewIterator& other) const
  {
    return !this->operator==(other);
  }

  /**
   * @brief Special end-iterator object equality operator
   *
   * @param other  other iterator
   *
   * @retval true  if \p other iterator refer to one past last element
   * @retval false  otherwise
   */
  inline bool operator==(const ViewIteratorEnd<GridT>& other) const
  {
    return this->pt_.y == other.width_;
  }

  /**
   * @brief Special end-iterator object inequality operator
   *
   * @param other  other iterator
   *
   * @retval true  if \p other iterator refer to one past last element
   * @retval false  otherwise
   */
  inline bool operator!=(const ViewIteratorEnd<GridT>& other) const
  {
    return !this->operator==(other);
  }

private:
  /**
   * @brief Returns mutable reference to grid element relative to view (<code>*this</code>)
   * @param pt  index pair associated with grid view element
   * @return reference to cell
   */
  inline auto& operator[](const Indices& pt)
  {
    return this->gp_->operator[](origin_ + pt);
  }

  /**
   * @brief Returns immutable reference to grid element relative to view (<code>*this</code>)
   * @param pt  index pair associated with grid view element
   * @return reference to cell
   */
  inline auto& operator[](const Indices& pt) const
  {
    return this->gp_->operator[](origin_ + pt);
  }

  /// Origin relative to parent grid
  const Indices origin_;

  template<typename OtherDerived, typename OtherCellT>
  friend class View;

  template<typename OtherGridT>
  friend class ViewIterator;

  template<typename OtherDerived, typename OtherGridT>
  friend class ViewIteratorBase;
};


/**
 * @brief Special object used to represent <code>FixedExtentsViewIterator end()</code> value
 */
template<int Width>
struct FixedViewIteratorEnd {};


/**
 * @brief Dynamic origin/extents grid view iterator
 *
 * @tparam GridT  parent grid type
 * @tparam Height  compile-time grid view height
 * @tparam Width  compile-time grid view width
 */
template<typename GridT, int Height, int Width>
class FixedExtentsViewIterator :
  public FixedExtentsBase<Height, Width>,
  public ViewIteratorBase<FixedExtentsViewIterator<GridT, Height, Width>, GridT>
{
  using ViewBase = ViewIteratorBase<FixedExtentsViewIterator<GridT, Height, Width>, GridT>;
public:
  template<int OtherWidth>
  FixedExtentsViewIterator(const FixedViewIteratorEnd<OtherWidth>& end) :
    ViewBase{nullptr, Indices::Zero()},
    origin_{-1, -1}
  {}

  template<typename OtherGridT>
  FixedExtentsViewIterator(const FixedExtentsViewIterator<OtherGridT, Height, Width>& other) :
    ViewBase{other.grid(), other.coords()},
    origin_{other.origin()}
  {}

  FixedExtentsViewIterator(GridT* const gp, const Indices& origin) :
    ViewBase{gp, Indices::Zero()},
    origin_{origin}
  {}

  /**
   * @brief Returns view origin relative to parent grid
   */
  const Indices& origin() const
  {
    return origin_;
  }

  /**
   * @brief Iterator equality operator
   *
   *        Handles ViewIterator used to represent the more conservative ViewIteratorEnd object
   *
   * @param other  other iterator
   *
   * @retval true  if \p other iterator refer to the same element
   * @retval false  otherwise
   */
  inline bool operator==(const FixedExtentsViewIterator& other) const
  {
    return (other.gp_ == nullptr and this->pt_.y == Width) or
           (this->gp_ == other.gp_ and
            this->pt_ == other.pt_ and
            this->origin_ == other.origin_);
  }

  /**
   * @brief Iterator inequality operator
   *
   *        Handles ViewIterator used to represent the more conservative ViewIteratorEnd object
   *
   * @param other  other iterator
   *
   * @retval true  if \p other iterator do not refer to the same element
   * @retval false  otherwise
   */
  inline bool operator!=(const FixedExtentsViewIterator& other) const
  {
    return !this->operator==(other);
  }

  /**
   * @brief Special end-iterator object equality operator
   *
   * @param other  other iterator
   *
   * @retval true  if \p other iterator refer to one past last element
   * @retval false  otherwise
   */
  inline bool operator==(const FixedViewIteratorEnd<Width>& other) const
  {
    return this->pt_.y == Width;
  }

  /**
   * @brief Special end-iterator object inequality operator
   *
   * @param other  other iterator
   *
   * @retval true  if \p other iterator refer to one past last element
   * @retval false  otherwise
   */
  inline bool operator!=(const FixedViewIteratorEnd<Width>& other) const
  {
    return !this->operator==(other);
  }

private:
  /**
   * @brief Returns mutable reference to grid element relative to view (<code>*this</code>)
   * @param pt  index pair associated with grid view element
   * @return reference to cell
   */
  inline auto& operator[](const Indices& pt)
  {
    return this->gp_->operator[](origin_ + pt);
  }

  /**
   * @brief Returns immutable reference to grid element relative to view (<code>*this</code>)
   * @param pt  index pair associated with grid view element
   * @return reference to cell
   */
  inline auto& operator[](const Indices& pt) const
  {
    return this->gp_->operator[](origin_ + pt);
  }

  /// Origin relative to parent grid
  const Indices origin_;

  template<typename OtherDerived,
           typename OtherCellT,
           int OtherHeight,
           int OtherWidth>
  friend class FixedExtentsView;

  template<typename OtherGridT, int OtherHeight, int OtherWidth>
  friend class FixedExtentsViewIterator;

  template<typename OtherDerived, typename OtherGridT>
  friend class ViewIteratorBase;
};


/**
 * @brief Dynamic origin/extents grid view iterator
 *
 * @tparam GridT  parent grid type
 * @tparam OriginX  compile-time grid view origin x-coordinate, relative to parent
 * @tparam OriginY  compile-time grid view origin y-coordinate, relative to parent
 * @tparam Height  compile-time grid view height
 * @tparam Width  compile-time grid view width
 */
template<typename GridT, int OriginX, int OriginY, int Height, int Width>
class FixedOriginExtentsViewIterator :
  public FixedExtentsBase<Height, Width>,
  public ViewIteratorBase<FixedOriginExtentsViewIterator<GridT, OriginX, OriginY, Height, Width>, GridT>
{
  using ViewBase = ViewIteratorBase<FixedOriginExtentsViewIterator<GridT, OriginX, OriginY, Height, Width>, GridT>;
public:
  template<int OtherWidth>
  FixedOriginExtentsViewIterator(const FixedViewIteratorEnd<OtherWidth>& end) :
    ViewBase{nullptr, Indices::Zero()}
  {}

  template<typename OtherGridT>
  FixedOriginExtentsViewIterator(const FixedOriginExtentsViewIterator<OtherGridT, OriginX, OriginY, Height, Width>& other) :
    ViewBase{other.grid(), other.coords()}
  {}

  constexpr FixedOriginExtentsViewIterator(GridT* const gp) :
    ViewBase{gp, Indices::Zero()}
  {}

  inline bool operator==(const FixedOriginExtentsViewIterator& other) const
  {
    return (other.gp_ == nullptr and this->pt_.y == Width) or
           (this->gp_ == other.gp_ and
            this->pt_ == other.pt_);
  }

  inline bool operator!=(const FixedOriginExtentsViewIterator& other) const
  {
    return !this->operator==(other);
  }

  inline bool operator==(const FixedViewIteratorEnd<Width>& other) const
  {
    return this->pt_.y == Width;
  }

  inline bool operator!=(const FixedViewIteratorEnd<Width>& other) const
  {
    return !this->operator==(other);
  }

private:
  inline auto& operator[](const Indices& pt)
  {
    return this->gp_->operator[](origin_ + pt);
  }

  inline auto& operator[](const Indices& pt) const
  {
    return this->gp_->operator[](origin_ + pt);
  }

  constexpr static const Indices origin_{OriginX, OriginY};

  template<typename OtherDerived,
           typename OtherCellT,
           int OtherOriginX,
           int OtherOriginY,
           int OtherHeight,
           int OtherWidth>
  friend class FixedOriginExtentsView;

  template<typename OtherGridT,
           int OtherOriginX,
           int OtherOriginY,
           int OtherHeight,
           int OtherWidth>
  friend class FixedOriginExtentsIterator;

  template<typename OtherDerived, typename OtherGridT>
  friend class ViewIteratorBase;
};


template<typename Derived, typename CellT>
class View :
  public GridBase<View<Derived, CellT>, CellT>
{
  using GBase = GridBase<View, CellT>;
public:
  using GBase::operator=;

private:
  using iterator = ViewIterator<Derived>;
  using const_iterator = ViewIterator<const Derived>;

  inline View(Derived* p, const Indices& origin, const Extents& extents) :
    itr_{p, origin, extents}
  {}

  inline const CellT& access_impl(const Indices& pt) const
  {
    return itr_[pt];
  }

  inline CellT& access_impl(const Indices& pt)
  {
    return itr_[pt];
  }

  constexpr const Extents& extents_impl() const
  {
    return itr_.extents_;
  }

  inline iterator begin_impl()
  {
    return itr_;
  }

  inline const_iterator begin_impl() const
  {
    return const_iterator{itr_};
  }

  inline ViewIteratorEnd<Derived> end_impl()
  {
    return ViewIteratorEnd<Derived>{itr_.extents_.y};
  }

  inline ViewIteratorEnd<const Derived> end_impl() const
  {
    return ViewIteratorEnd<const Derived>{itr_.extents_.y};
  }

  iterator itr_;

  template<typename OtherDerived, typename OtherCellT>
  friend class GridBase;
};


template<typename Derived, typename CellT, int Height, int Width>
class FixedExtentsView :
  public FixedExtentsBase<Height, Width>,
  public GridBase<FixedExtentsView<Derived, CellT, Height, Width>, CellT>
{
  using GBase = GridBase<FixedExtentsView, CellT>;
public:
  using GBase::operator=;

private:
  using iterator = FixedExtentsViewIterator<Derived, Height, Width>;
  using const_iterator = FixedExtentsViewIterator<const Derived, Height, Width>;

  inline FixedExtentsView(Derived* const p, const Indices& origin) :
    itr_{p, origin}
  {}

  inline const CellT& access_impl(const Indices& pt) const
  {
    return itr_[pt];
  }

  inline CellT& access_impl(const Indices& pt)
  {
    return itr_[pt];
  }

  constexpr static Extents extents_impl()
  {
    return iterator::extents_;
  }

  inline iterator begin_impl()
  {
    return itr_;
  }

  inline const_iterator begin_impl() const
  {
    return itr_;
  }

  constexpr static FixedViewIteratorEnd<Width> end_impl()
  {
    return FixedViewIteratorEnd<Width>{};
  }

  iterator itr_;

  template<typename OtherDerived, typename OtherCellT>
  friend class GridBase;
};


template<typename Derived, typename CellT, int OriginX, int OriginY, int Height, int Width>
class FixedOriginExtentsView :
  public FixedExtentsBase<Height, Width>,
  public GridBase<FixedOriginExtentsView<Derived, CellT, OriginX, OriginY, Height, Width>, CellT>
{
  using GBase = GridBase<FixedOriginExtentsView, CellT>;
public:
  using GBase::operator=;

private:
  using iterator = FixedOriginExtentsViewIterator<Derived, OriginX, OriginY, Height, Width>;
  using const_iterator = FixedOriginExtentsViewIterator<const Derived, OriginX, OriginY, Height, Width>;

  inline FixedOriginExtentsView(Derived* const p) :
    itr_{p}
  {
  }

  inline const CellT& access_impl(const Indices& pt) const
  {
    return itr_[pt];
  }

  inline CellT& access_impl(const Indices& pt)
  {
    return itr_[pt];
  }

  constexpr static Extents extents_impl()
  {
    return iterator::extents_;
  }

  inline iterator begin_impl()
  {
    return itr_;
  }

  inline const_iterator begin_impl() const
  {
    return itr_;
  }

  inline FixedViewIteratorEnd<Width> end_impl() const
  {
    return FixedViewIteratorEnd<Width>{};
  }

  iterator itr_;

  template<typename OtherDerived, typename OtherCellT>
  friend class GridBase;
};


template<typename Derived,
         typename ContainerT,
         typename SizeT = typename ContainerT::size_type,
         typename IteratorT = typename ContainerT::iterator,
         typename ConstIteratorT = typename ContainerT::const_iterator>
class ContainerStorageBase
{
public:
  template<typename... Args>
  explicit ContainerStorageBase(Args&&... args) :
    data_{std::forward<Args>(args)...}
  {}

  inline SizeT toLinearIndex(const Indices& pt) const
  {
    return (derived()->extents().x * pt.y) + pt.x;
  }

protected:
  ContainerT data_;

private:
  IMPLEMENT_CRTP_BASE_CLASS(Derived);

  inline const auto& access_impl(const Indices& pt) const
  {
    return data_[toLinearIndex(pt)];
  }

  inline auto& access_impl(const Indices& pt)
  {
    return data_[toLinearIndex(pt)];
  }

  inline IteratorT begin_impl()
  {
    return data_.begin();
  }

  inline IteratorT end_impl()
  {
    return data_.end();
  }

  inline ConstIteratorT begin_impl() const
  {
    return data_.begin();
  }

  inline ConstIteratorT end_impl() const
  {
    return data_.end();
  }

  template<typename OtherDerived, typename OtherCellT>
  friend class GridBase;
};


template<typename Derived, typename PtrT>
class RawStorageBase
{
  static_assert(
    std::is_pointer_v<PtrT>,
    "RawStorageBase: expecting PtrT to be a pointer type"
  );
public:
  explicit RawStorageBase(PtrT data) :
    data_{data}
  {}

  inline int toLinearIndex(const Indices& pt) const
  {
    return (derived()->extents().x * pt.y) + pt.x;
  }

protected:
  PtrT data_;

private:
  IMPLEMENT_CRTP_BASE_CLASS(Derived);

  inline const auto& access_impl(const Indices& pt) const
  {
    return data_[toLinearIndex(pt)];
  }

  inline auto& access_impl(const Indices& pt)
  {
    return data_[toLinearIndex(pt)];
  }

  inline PtrT begin_impl()
  {
    return data_;
  }

  inline PtrT end_impl()
  {
    return data_ + derived()->extents().area();
  }

  inline const PtrT begin_impl() const
  {
    return data_;
  }

  inline const PtrT end_impl() const
  {
    return data_ + derived()->extents().area();
  }

  template<typename OtherDerived, typename OtherCellT>
  friend class GridBase;
};


template<typename CellT, typename AllocatorT = std::allocator<CellT>>
class Grid :
  public ResizableExtentsBase,
  public GridBase<Grid<CellT, AllocatorT>, CellT>,
  public RawStorageBase<Grid<CellT, AllocatorT>, CellT*>
{
  using GBase = GridBase<Grid, CellT>;
  using StorageBase = RawStorageBase<Grid<CellT, AllocatorT>, CellT*>;
public:
  using GBase::operator=;

  Grid() :
    ResizableExtentsBase{Extents::Zero()},
    StorageBase{nullptr}
  {}

  template<typename... CellArgs>
  Grid(const Extents& extents, const AllocatorT& alloc, CellArgs&&... cell_args) :
    ResizableExtentsBase{extents},
    StorageBase{alloc.allocate(extents.area())},
    allocator_{alloc}
  {
    construct(std::forward<CellArgs>(cell_args)...);
  }

  template<typename... CellArgs>
  explicit Grid(const Extents& extents, CellArgs&&... cell_args) :
    ResizableExtentsBase{extents},
    StorageBase{allocator_.allocate(extents.area())}
  {
    construct(std::forward<CellArgs>(cell_args)...);
  }

  ~Grid()
  {
    clear();
  }

  inline void clear()
  {
    if (!this->data_)
    {
      return;
    }

    for (auto d = this->begin(); d != this->end(); ++d)
    {
      allocator_.destroy(d);
    }
    allocator_.deallocate(this->data_, this->extents_.area());
  }

  template<typename... CellArgs>
  inline void resize(const Extents& extents, CellArgs&&... cell_args)
  {
    clear();
    this->extents_ = extents;
    this->data_ = allocator_.allocate(extents.area());
    construct(std::forward<CellArgs>(cell_args)...);
  }

private:
  template<typename... CellArgs>
  inline void construct(CellArgs&&... cell_args)
  {
    for (auto d = this->begin(); d != this->end(); ++d)
    {
      allocator_.construct(d, std::forward<CellArgs>(cell_args)...);
    }
  }

  /// Allocator
  AllocatorT allocator_;
};


template<typename CellT>
class MappedGrid :
  public ResizableExtentsBase,
  public GridBase<MappedGrid<CellT>, CellT>,
  public RawStorageBase<MappedGrid<CellT>, CellT*>
{
  using GBase = GridBase<MappedGrid, CellT>;
  using StorageBase = RawStorageBase<MappedGrid, CellT*>;
public:
  using GBase::operator=;

  MappedGrid(const Extents& extents, CellT* mem) :
    ResizableExtentsBase{extents},
    StorageBase{mem}
  {}

  inline void resize(const Extents& extents)
  {
    this->extents_ = extents;
  }

  inline void resize(const Extents& extents, CellT initial_value)
  {
    this->extents_ = extents;
    std::fill(this->begin(), this->end(), initial_value);
  }

  friend GBase;
};


template<typename CellT, int Height, int Width>
class FixedGrid :
  public FixedExtentsBase<Height, Width>,
  public GridBase<FixedGrid<CellT, Height, Width>, CellT>,
  public ContainerStorageBase<FixedGrid<CellT, Height, Width>, std::array<CellT, Height * Width>>
{
  using GBase = GridBase<FixedGrid, CellT>;
  using StorageBase = ContainerStorageBase<FixedGrid<CellT, Height, Width>, std::array<CellT, Height * Width>>;
public:
  using GBase::operator=;

  FixedGrid() = default;

  constexpr explicit FixedGrid(CellT initial_value)
  {
    this->data_.fill(initial_value);
  }
};


template<typename CellT, int Height, int Width>
class MappedFixedGrid :
  public FixedExtentsBase<Height, Width>,
  public GridBase<MappedFixedGrid<CellT, Height, Width>, CellT>,
  public RawStorageBase<MappedFixedGrid<CellT, Height, Width>, CellT*>
{
  using GBase = GridBase<MappedFixedGrid, CellT>;
  using StorageBase = RawStorageBase<MappedFixedGrid, CellT*>;
public:
  using GBase::operator=;

  explicit MappedFixedGrid(CellT* mem) :
    StorageBase{mem}
  {}

  friend GBase;
};


template<typename GridT>
struct Tile
{
  std::unique_ptr<GridT> data;
  Indices origin = Indices::Zero();
};


template<typename CellT, int Height, int Width, int TileHeight = Height/2, int TileWidth=Width/2>
class FixedTiledGrid :
  public FixedExtentsBase<Height, Width>,
  public GridBase<FixedTiledGrid<CellT, Height, Width, TileHeight, TileWidth>, CellT>
{
  static_assert(Height >= TileHeight, "FixedTiledGrid: invalid TileHeight");
  static_assert(Width >= TileWidth, "FixedTiledGrid: invalid TileWidth");

  using GBase = GridBase<FixedTiledGrid<CellT, Height, Width, TileHeight, TileWidth>, CellT>;
public:
  constexpr static const int TileRows = Height / TileHeight;
  constexpr static const int TileCols = Width / TileWidth;
  constexpr static const int TileCount = TileRows * TileCols;

  using TileGrid = FixedGrid<CellT, TileHeight, TileWidth>;
  using TileType = Tile<TileGrid>;

  constexpr FixedTiledGrid(CellT default_value) :
    default_value_{default_value}
  {}

  inline FixedGrid<bool, TileRows, TileCols> mask() const
  {
    FixedGrid<bool, TileRows, TileCols> mask{false};

    auto mask_itr = mask.begin();
    for (auto tile_itr = tiles_.begin(); tile_itr != tiles_.end(); ++tile_itr, ++mask_itr)
    {
      *mask_itr = static_cast<bool>(tile_itr->data);
    }
    return mask;
  }

  inline int active() const
  {
    int count{0};
    for (auto& tile : tiles_)
    {
      if (tile.data)
      {
        ++count;
      }
    }
    return count;
  }

  inline const TileType& operator()(int i, int j) const
  {
    return tiles_[Indices{i, j}];
  }

  constexpr static int rows()
  {
    return TileRows;
  }

  constexpr static int cols()
  {
    return TileCols;
  }

private:
  CellT default_value_;

  inline const CellT& access_impl(const Indices& pt) const
  {
    const Indices tile_pt{pt.x / TileHeight, pt.y / TileWidth};

    const TileType& tile = tiles_[tile_pt];
    if (tile.data)
    {
      return tile.data->operator[](pt - tile.origin);
    }
    return default_value_;
  }

  inline CellT& access_impl(const Indices& pt)
  {
    const Indices tile_pt{pt.x / TileHeight, pt.y / TileWidth};

    TileType& tile = tiles_[tile_pt];
    if (!tile.data)
    {
      tile.origin.x = tile_pt.x * TileHeight;
      tile.origin.y = tile_pt.y * TileWidth;
      tile.data = std::make_unique<TileGrid>(default_value_);
    }
    return tile.data->operator[](pt - tile.origin);
  }

  inline auto begin_impl()
  {
    return this->template view<0, 0, Height, Width>().begin();
  }

  inline auto end_impl()
  {
    return this->template view<0, 0, Height, Width>().end();
  }

  inline const auto begin_impl() const
  {
    return this->template view<0, 0, Height, Width>().cbegin();
  }

  inline const auto end_impl() const
  {
    return this->template view<0, 0, Height, Width>().cend();
  }

  FixedGrid<TileType, TileRows, TileCols> tiles_;

  friend GBase;
};

}  // namespace twod

#endif // TWOD_GRID_H