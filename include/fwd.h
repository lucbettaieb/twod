/**
 * @copyright 2018 TwoD
 * @author Brian Cairl
 *
 * @file fwd.h
 * @brief Forward declarations
 */

#ifndef TWOD_FWD_H
#define TWOD_FWD_H

namespace twod
{

// Forward
template<typename CoordT> struct Coordinates;
template<typename GridT> struct Tile;
template<typename Derived, typename CellT> class GridBase;
template<typename GridT> class ViewIterator;
template<typename Derived, typename CellT> class View;
template<typename Derived, typename CellT, int Height, int Width> class FixedExtentsView;
template<typename Derived, typename CellT, int OriginX, int OriginY, int Height, int Width> class FixedOriginExtentsView;
template<typename CellT, int Height, int Width> class FixedGrid;
template<typename CellT, typename AllocatorT> class Grid;
template<typename CellT, int Height, int Width, int TileHeight, int TileWidth> class FixedTiledGrid;

}  // namespace twod

#endif // TWOD_FWD_H