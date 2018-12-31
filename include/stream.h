/**
 * @copyright 2018 TwoD
 * @author Brian Cairl
 *
 * @file stream.h
 */

#ifndef TWOD_STREAM_H
#define TWOD_STREAM_H

// C++ Standard Library
#include <ostream>

// TwoD
#include <twod/fwd.h>

namespace twod
{

template<typename GridT, typename CellT>
struct GridStreamFormatting
{
  friend inline std::ostream& operator<<(std::ostream& os, const GridStreamFormatting<GridT, CellT>& fmt)
  {
    return os << std::setw(4);
  }
};


template<typename GridT>
struct GridStreamFormatting<GridT, bool>
{
  friend inline std::ostream& operator<<(std::ostream& os, const GridStreamFormatting<GridT, bool>& fmt)
  {
    return os << std::setw(2);
  }
};


template<typename CoordT>
inline std::ostream& operator<<(std::ostream& os, const Coordinates<CoordT>& pt)
{
  return os << pt.x << ", " << pt.y;
}


template<typename Derived, typename CellT>
inline std::ostream& operator<<(std::ostream& os, const GridBase<Derived, CellT>& grid)
{
  int new_line_count = 0;
  for (const auto& c : grid)
  {
    os << GridStreamFormatting<Derived, CellT>{} << c;
    if (!(++new_line_count % grid.extents().x))
    {
      os << '\n';
    }
  }
  return os;
}

template<typename GridT>
inline std::ostream& operator<<(std::ostream& os, const Tile<GridT>& tile)
{
  if (tile.data)
  {
    os << "origin: " << tile.origin << '\n';
    os << "tile:\n" << *tile.data;
  }
  else
  {
    os << "tile: <not expanded>";
  }
  return os;
}

}  // namespace twod

#endif // TWOD_STREAM_H