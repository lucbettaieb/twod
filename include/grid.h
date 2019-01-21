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
#include <iterator>
#include <memory>
#include <type_traits>
#include <utility>

// TwoD
#include <twod/bounds.h>
#include <twod/coordinates.h>
#include <twod/crtp.h>
#include <twod/fwd.h>
#include <twod/primitives.h>

namespace twod
{

/**
 * @brief CRTP grid base interface
 *
 * @tparam Derived  CRTP derived grid object
 */
template<typename Derived, typename BoundsT>
class GridBase : public BoundsT
{
public:
  /**
   * @brief Returns grid view of size \p extents
   *
   *        View is anchored at bottom left corning to parent coordinates \p origin
   *
   * @param bounds
   * @return view
   */
  template<typename ViewBoundsT>
  inline View<Derived, ViewBoundsT> view(const ViewBoundsT& bounds)
  {
    return View<Derived, ViewBoundsT>{*derived(), bounds};
  }

  /**
   * @brief Returns grid view of size \p extents
   *
   *        View is anchored at bottom left corning to parent coordinates \p origin
   *
   * @param bounds
   * @return view
   */
  template<typename ViewBoundsT>
  inline View<const Derived, ViewBoundsT> view(const ViewBoundsT& bounds) const
  {
    return View<const Derived, ViewBoundsT>{*derived(), bounds};
  }

  /**
   * @brief Returns view for the entirety of the current grid
   * @return view
   */
  inline auto view()
  {
    return view(bounds());
  }

  /**
   * @brief Returns view for the entirety of the current grid
   * @return view
   */
  inline auto view() const
  {
    return view(bounds());
  }

  /**
   * @brief Generic assignment from <code>GridBase</code> derivative
   *
   * @tparam OtherDerived  CRTP-derived <code>GridBase</code> object
   *
   * @param other  other grid
   * @return <code>*this</code>
   */
  template<typename OtherDerived, typename OtherBoundsT>
  inline Derived& operator=(const GridBase<OtherDerived, OtherBoundsT>& other)
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
  inline Derived& operator+=(const GridBase<OtherDerived, BoundsT>& other)
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
  inline Derived& operator-=(const GridBase<OtherDerived, BoundsT>& other)
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
  inline bool operator!=(const GridBase<OtherDerived, BoundsT>& other) const
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
  inline bool operator==(const GridBase<OtherDerived, BoundsT>& other) const
  {
    return not this->operator!=(other);
  }

  /**
   * @brief Sets all cells to a uniform value
   * @param value  cell value
   * @return <code>*this</code>
   */
  template<typename AssignT>
  inline Derived& fill(const AssignT& value)
  {
    for (auto& c : *derived())
    {
      c = value;
    }
    return *derived();
  }

  /**
   * @brief Returns grid bounds
   */
  constexpr const BoundsT& bounds() const
  {
    return static_cast<const BoundsT&>(*this);
  }

  /**
   * @brief Return mutable reference to element
   * @param pt  index pair associated with grid element
   * @return reference to cell
   */
  constexpr auto& operator[](const Indices& pt)
  {
    return derived()->access_impl(pt + this->origin());
  }

  /**
   * @brief Return immutable reference to element
   * @param pt  index pair associated with grid element
   * @return reference to cell
   */
  constexpr const auto& operator[](const Indices& pt) const
  {
    return derived()->access_impl(pt + this->origin());
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

protected:
  template<typename... Args>
  explicit GridBase(Args&&... args) :
    BoundsT{std::forward<Args>(args)...}
  {}

  template<typename... Args>
  void reset(Args&&... args)
  {
    new (this) GridBase{std::forward<Args>(args)...};
  }

private:
  IMPLEMENT_CRTP_BASE_CLASS(Derived);
};


template<typename GridT>
class GridTraits
{
private:
  static constexpr auto test_cell_access(GridT grid)
  {
    return grid[Indices{0, 0}];
  }

public:
  using cell_type = std::remove_cv_t<std::result_of_t<decltype(&test_cell_access)(GridT)>>;
};


/// End-tag object
struct ViewIteratorEnd {};


/**
 * @brief View iterator
 */
template<typename Derived, typename ViewT>
class ViewIteratorBase
{
public:
  using difference_type = int;
  using value_type = typename GridTraits<ViewT>::cell_type;
  using reference = value_type&;
  using pointer = value_type*;
  using iterator_category = std::forward_iterator_tag;

  /**
   * @brief Pre-increment overload
   */
  inline Derived& operator++()
  {
    return derived()->increment_impl();
  }
  /**
   * @brief Post-increment overload
   */
  inline Derived operator++(int)
  {
    const Derived prev{*derived()};
    derived()->increment_impl();
    return prev;
  }

  /**
   * @brief Mutable cell value dereference operator
   */
  inline auto& operator*()
  {
    return view_->operator[](this->pt_);
  }

  /**
   * @brief Immutable cell value dereference operator
   */
  inline const auto& operator*() const
  {
    return view_->operator[](this->pt_);
  }

  /**
   * @brief Mutable cell value dereference operator
   */
  inline auto* operator->()
  {
    return std::addressof(this->operator*());
  }

  /**
   * @brief Immutable cell value dereference operator
   */
  inline const auto* operator->() const
  {
    return std::addressof(this->operator*());
  }

  /**
   * @brief Equality operator
   */
  inline bool operator==(const Derived& other) const
  {
    return (this->view_ == other.view_) and
           (this->pt_ == other.pt_);
  }

  /**
   * @brief Inequality operator
   */
  inline bool operator!=(const Derived& other) const
  {
    return !this->operator==(other);
  }

  /**
   * @brief Equality operator
   */
  inline bool operator==(ViewIteratorEnd end) const
  {
    return derived()->eq_end_impl(end);
  }

  /**
   * @brief Inequality operator
   */
  inline bool operator!=(ViewIteratorEnd end) const
  {
    return !this->operator==(end);
  }

  /**
   * @brief Returns active view-relative index pair associated with iterator
   */
  const Indices& coords() const
  {
    return pt_;
  }

  /**
   * @brief Returns pointer to parent view
   */
  const ViewT* const view() const
  {
    return view_;
  }

protected:
  /**
   * @brief Initialization constructor
   * @param view  parent grid view
   * @param pt  initial view-relative point
   */
  ViewIteratorBase(ViewT* const view, const Indices& pt) :
    view_{view},
    pt_{pt}
  {}

  /// Parent grid pointer
  ViewT* view_;

  /// View-relative index
  Indices pt_;

  template<typename OtherViewT> friend class ColViewIterator;
  template<typename OtherViewT> friend class RowViewIterator;

private:
  IMPLEMENT_CRTP_BASE_CLASS(Derived);
};


/**
 * @brief Column-major view iterator
 *
 * @tparam GridT  parent grid type
 */
template<typename ViewT>
class ColViewIterator :
  public ViewIteratorBase<ColViewIterator<ViewT>, ViewT>
{
  using Base = ViewIteratorBase<ColViewIterator<ViewT>, ViewT>;
public:
  /**
   * @brief Initialization constructor
   * @param view  parent grid view
   * @param pt  initial view-relative point
   */
  explicit ColViewIterator(ViewT& view, const Indices& pt = Indices::Zero()) :
    Base{std::addressof(view), pt}
  {}

  /**
   * @brief End iterator constructor
   * @param view  parent grid view
   * @param pt  initial view-relative point
   */
  explicit ColViewIterator(ViewT& view, ViewIteratorEnd) :
    Base{std::addressof(view), Indices{0, view.extents().y}}
  {}

private:
  /**
   * @brief Pre-increment overload
   */
  inline ColViewIterator& increment_impl()
  {
    ++this->pt_.x;
    if (this->pt_.x == this->view_->extents().x)
    {
      this->pt_.x = 0;
      ++this->pt_.y;
    }
    return *this;
  }

  /**
   * @brief End tag equality check
   */
  inline bool eq_end_impl(ViewIteratorEnd) const
  {
    return this->pt_.y == this->view_->extents().y;
  }

  friend Base;
};


/**
 * @brief Row-major view iterator
 *
 * @tparam GridT  parent grid type
 */
template<typename ViewT>
class RowViewIterator :
  public ViewIteratorBase<RowViewIterator<ViewT>, ViewT>
{
  using VIBase = ViewIteratorBase<RowViewIterator<ViewT>, ViewT>;
public:
  /**
   * @brief Initialization constructor
   * @param view  parent grid view
   * @param pt  initial view-relative point
   */
  explicit RowViewIterator(ViewT& view, const Indices& pt = Indices::Zero()) :
    VIBase{std::addressof(view), pt}
  {}

  /**
   * @brief End iterator constructor
   * @param view  parent grid view
   * @param pt  initial view-relative point
   */
  explicit RowViewIterator(ViewT& view, ViewIteratorEnd) :
    VIBase{std::addressof(view), Indices{view.extents().x, 0}}
  {}

private:
  /**
   * @brief Pre-increment overload
   */
  inline RowViewIterator& increment_impl()
  {
    ++this->pt_.y;
    if (this->pt_.y == this->view_->extents().y)
    {
      this->pt_.y = 0;
      ++this->pt_.x;
    }
    return *this;
  }

  /**
   * @brief End tag equality check
   */
  inline bool eq_end_impl(ViewIteratorEnd) const
  {
    return this->pt_.x == this->view_->extents().x;
  }

  friend VIBase;
};


template<typename ParentT, typename BoundsT>
class View : public GridBase<View<ParentT, BoundsT>, BoundsT>
{
  using GBase = GridBase<View<ParentT, BoundsT>, BoundsT>;
public:
  using GBase::operator=;

  View(ParentT& parent) :
    parent_{std::addressof(parent)}
  {}

  View(ParentT& parent, BoundsT bounds) :
    GBase{bounds},
    parent_{std::addressof(parent)}
  {}

private:
  /**
   * @brief Return mutable reference to element
   * @param pt  index pair associated with grid element
   * @return reference to cell
   */
  constexpr auto& access_impl(const Indices& pt)
  {
    return parent_->operator[](pt);
  }

  /**
   * @brief Return immutable reference to element
   * @param pt  index pair associated with grid element
   * @return reference to cell
   */
  constexpr const auto& access_impl(const Indices& pt) const
  {
    return parent_->operator[](pt);
  }

  /**
   * @brief Returns iterator (mutable) to first element
   */
  inline auto begin_impl()
  {
    return ColViewIterator<View>{*this};
  }

  /**
   * @brief Returns iterator (immutable) to first element
   */
  inline auto begin_impl() const
  {
    return ColViewIterator<const View>{*this};
  }

  /**
   * @brief Returns iterator (immutable) to one past last element
   */
  inline auto end_impl() const
  {
    return ViewIteratorEnd{};
  }

  /// Pointer to parent grid being viewed
  ParentT* parent_;

  friend GBase;
};


template<typename Derived,
         typename ContainerT,
         typename SizeT = typename ContainerT::size_type,
         typename IteratorT = typename ContainerT::iterator,
         typename ConstIteratorT = typename ContainerT::const_iterator>
class ContainerAccessBase
{
public:
  template<typename... Args>
  explicit ContainerAccessBase(Args&&... args) :
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

  template<typename OtherDerived, typename OtherBoundsT>
  friend class GridBase;
};


template<typename Derived, typename PtrT>
class RawAccessBase
{
  static_assert(
    std::is_pointer_v<PtrT>,
    "RawAccessBase: expecting PtrT to be a pointer type"
  );
public:
  explicit RawAccessBase(PtrT data) :
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

  template<typename OtherDerived, typename OtherBoundsT>
  friend class GridBase;
};


template<typename CellT,
         typename AllocatorT = std::allocator<CellT>>
class Grid :
  public GridBase<Grid<CellT, AllocatorT>, FixedOriginBounds<0, 0>>,
  public RawAccessBase<Grid<CellT, AllocatorT>, CellT*>
{
  using Base = GridBase<Grid, FixedOriginBounds<0, 0>>;
  using StorageBase = RawAccessBase<Grid, CellT*>;
public:
  using Base::operator=;

  Grid(const Extents& extents = Extents::Zero()) :
    Base{extents},
    StorageBase{allocator_.allocate(extents.area())}
  {
    construct();
  }

  Grid(const Extents& extents, const CellT& val) :
    Base{extents},
    StorageBase{allocator_.allocate(extents.area())}
  {
    construct(val);
  }

  Grid(const Extents& extents, const AllocatorT& alloc) :
    Base{extents},
    StorageBase{alloc.allocate(extents.area())},
    allocator_{alloc}
  {
    construct();
  }

  Grid(const Extents& extents, const CellT& val, const AllocatorT& alloc) :
    Base{extents},
    StorageBase{alloc.allocate(extents.area())},
    allocator_{alloc}
  {
    construct(val);
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
    allocator_.deallocate(this->data_, this->extents().area());
  }

  template<typename... CellArgs>
  inline void resize(const Extents& extents, CellArgs&&... cell_args)
  {
    clear();
    Base::reset(extents);
    this->data_ = allocator_.allocate(extents.area());
    this->construct(std::forward<CellArgs>(cell_args)...);
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
  public GridBase<MappedGrid<CellT>, FixedOriginBounds<0, 0>>,
  public RawAccessBase<MappedGrid<CellT>, CellT*>
{
  using GBase = GridBase<MappedGrid, FixedOriginBounds<0, 0>>;
  using StorageBase = RawAccessBase<MappedGrid, CellT*>;
public:
  using GBase::operator=;

  MappedGrid(const Extents& extents, CellT* mem) :
    GBase{extents},
    StorageBase{mem}
  {}

  inline void resize(const Extents& extents)
  {
    this->extents_ = extents;
  }

  inline void resize(const Extents& extents, const CellT& initial_value)
  {
    this->extents_ = extents;
    std::fill(this->begin(), this->end(), initial_value);
  }

  friend GBase;
};


template<typename CellT, int Height, int Width>
class FixedGrid :
  public GridBase<FixedGrid<CellT, Height, Width>, FixedOriginExtentsBounds<0, 0, Height, Width>>,
  public ContainerAccessBase<FixedGrid<CellT, Height, Width>, std::array<CellT, Height * Width>>
{
  using GBase = GridBase<FixedGrid, FixedOriginExtentsBounds<0, 0, Height, Width>>;
  using StorageBase = ContainerAccessBase<FixedGrid, std::array<CellT, Height * Width>>;
public:
  using GBase::operator=;

  FixedGrid() = default;

  constexpr explicit FixedGrid(const CellT& initial_value)
  {
    this->data_.fill(initial_value);
  }
};


template<typename CellT, int Height, int Width>
class FixedMappedGrid :
  public GridBase<FixedMappedGrid<CellT, Height, Width>, FixedOriginExtentsBounds<0, 0, Height, Width>>,
  public RawAccessBase<FixedMappedGrid<CellT, Height, Width>, CellT*>
{
  using GBase = GridBase<FixedMappedGrid, FixedOriginExtentsBounds<0, 0, Height, Width>>;
  using StorageBase = RawAccessBase<FixedMappedGrid, CellT*>;
public:
  using GBase::operator=;

  explicit FixedMappedGrid(CellT* mem) :
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
  public GridBase<FixedTiledGrid<CellT, Height, Width, TileHeight, TileWidth>,
                  FixedOriginExtentsBounds<0, 0, Height, Width>>
{
  static_assert(Height >= TileHeight, "FixedTiledGrid: invalid TileHeight");
  static_assert(Width >= TileWidth, "FixedTiledGrid: invalid TileWidth");

  using GBase =  GridBase<FixedTiledGrid, FixedOriginExtentsBounds<0, 0, Height, Width>>;
public:
  constexpr static const int TileRows = Height / TileHeight;
  constexpr static const int TileCols = Width / TileWidth;
  constexpr static const std::size_t TileCount = TileRows * TileCols;

  using TileGrid = FixedGrid<CellT, TileHeight, TileWidth>;
  using TileType = Tile<TileGrid>;

  constexpr FixedTiledGrid(const CellT& default_value) :
    default_value_{default_value},
    view_{*this}
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

  inline std::size_t active() const
  {
    return std::count_if(tiles_.begin(),
                         tiles_.end(),
                         [](const TileType& t) -> bool
                         {
                           return static_cast<bool>(t.data);
                         });
  }

  inline const TileType& tile(Indices index) const
  {
    return tiles_[index];
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
    return view_.begin();
  }

  inline auto end_impl()
  {
    return view_.end();
  }

  inline const auto begin_impl() const
  {
    return view_.cbegin();
  }

  inline const auto end_impl() const
  {
    return view_.cend();
  }

  /// Cell value to return when tile is compressed
  CellT default_value_;

  /// Grid tiles
  FixedGrid<TileType, TileRows, TileCols> tiles_;

  /// View used to iterate over FixedTiledGrid
  View<FixedTiledGrid, FixedOriginExtentsBounds<0, 0, Height, Width>> view_;

  friend GBase;
};

}  // namespace twod

#endif // TWOD_GRID_H