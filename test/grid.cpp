/**
 * @copyright 2018 TwoD
 * @author Brian Cairl
 */

// C++ Standard Library
#include <vector>

// GTest
#include <gtest/gtest.h>

// TwoD
#include <twod/grid.h>
#include <twod/stream.h>


using namespace twod;


TEST(Grid, DefaultConstructor)
{
  Grid<int> grid;

  ASSERT_EQ(grid.extents(), Extents::Zero());
  ASSERT_TRUE(grid.empty());
}


TEST(Grid, InitSizeConstructor)
{
  Grid<int> grid{Extents{20, 10}};

  ASSERT_EQ(grid.extents(), (Extents{20, 10}));
  ASSERT_FALSE(grid.empty());
}


TEST(Grid, UniformInitialValueConstructor)
{
  Grid<int> grid{Extents{20, 10}, 1};

  ASSERT_EQ(grid.extents(), (Extents{20, 10}));
  ASSERT_FALSE(grid.empty());

  for (const auto& v : grid)
  {
    ASSERT_EQ(v, 1);
  }
}


TEST(Grid, Within)
{
  const Grid<int> grid{Extents{20, 10}};
  ASSERT_TRUE(grid.within(Indices{1, 1}));
}


TEST(Grid, NotWithin)
{
  const Grid<int> grid{Extents{20, 10}};
  ASSERT_FALSE(grid.within(Indices{21, 11}));
}


TEST(Grid, NonTrivialCell)
{
  Grid<std::vector<int>> grid{Extents{20, 10}};

  ASSERT_EQ(grid.extents(), (Extents{20, 10}));
  ASSERT_FALSE(grid.empty());
}


TEST(FixedGrid, DefaultConstructor)
{
  FixedGrid<int, 20, 10> grid;

  ASSERT_EQ(grid.extents(), (Extents{20, 10}));
}


TEST(FixedGrid, UniformInitialValueConstructor)
{
  FixedGrid<int, 20, 10> grid{1};

  ASSERT_EQ(grid.extents(), (Extents{20, 10}));

  for (const auto& v : grid)
  {
    ASSERT_EQ(v, 1);
  }
}


TEST(FixedGrid, Within)
{
  const FixedGrid<int, 20, 10> grid{1};
  ASSERT_TRUE(grid.within(Indices{1, 1}));
}


TEST(FixedGrid, NotWithin)
{
  const FixedGrid<int, 20, 10> grid{1};
  ASSERT_FALSE(grid.within(Indices{21, 11}));
}


TEST(FixedGridDynamicView, Fill)
{
  FixedGrid<int, 20, 10> grid{1};

  grid.view(Indices{1, 1}, Indices{2, 2}).fill(5);

  EXPECT_EQ((grid[Indices{0, 0}]), 1);
  EXPECT_EQ((grid[Indices{1, 1}]), 5);
  EXPECT_EQ((grid[Indices{1, 2}]), 5);
  EXPECT_EQ((grid[Indices{2, 1}]), 5);
  EXPECT_EQ((grid[Indices{2, 2}]), 5);
  EXPECT_EQ((grid[Indices{3, 3}]), 1);
}


TEST(FixedGridDynamicView, Const)
{
  FixedGrid<int, 20, 10> grid{1};

  grid.view(Indices{1, 1}, Indices{3, 3})[Indices{1, 1}] = 5;

  const FixedGrid<int, 20, 10> const_grid{grid};

  const auto value = const_grid.view(Indices{1, 1}, Indices{3, 3})[Indices{1, 1}];

  ASSERT_EQ(value, 5);
}


TEST(FixedGridDynamicView, AssignElement)
{
  FixedGrid<int, 20, 10> grid{1};

  grid.view(Indices{1, 1}, Indices{3, 3})[Indices{1, 1}] = 5;

  const auto value = grid[Indices{2, 2}];

  ASSERT_EQ(value, 5);
}


TEST(FixedGridDynamicView, AssignGrid)
{
  FixedGrid<int, 20, 10> grid{1};

  grid.view(Indices{1, 1}, Indices{2, 2}) = FixedGrid<int, 2, 2>{5};

  EXPECT_EQ((grid[Indices{0, 0}]), 1);
  EXPECT_EQ((grid[Indices{1, 1}]), 5);
  EXPECT_EQ((grid[Indices{1, 2}]), 5);
  EXPECT_EQ((grid[Indices{2, 1}]), 5);
  EXPECT_EQ((grid[Indices{2, 2}]), 5);
  EXPECT_EQ((grid[Indices{3, 3}]), 1);
}


TEST(FixedGridDynamicView, CompoundAddGrid)
{
  FixedGrid<int, 20, 10> grid{1};

  grid.view(Indices{1, 1}, Indices{2, 2}) += FixedGrid<int, 2, 2>{4};

  EXPECT_EQ((grid[Indices{0, 0}]), 1);
  EXPECT_EQ((grid[Indices{1, 1}]), 5);
  EXPECT_EQ((grid[Indices{1, 2}]), 5);
  EXPECT_EQ((grid[Indices{2, 1}]), 5);
  EXPECT_EQ((grid[Indices{2, 2}]), 5);
  EXPECT_EQ((grid[Indices{3, 3}]), 1);
}


TEST(FixedGridDynamicView, Equality)
{
  FixedGrid<int, 20, 10> grid{1};

  grid.view(Indices{1, 1}, Indices{2, 2}) = FixedGrid<int, 2, 2>{5};

  EXPECT_EQ((grid.view(Indices{1, 1}, Indices{2, 2})), (FixedGrid<int, 2, 2>{5}));
}


TEST(FixedGridDynamicView, Inequality)
{
  FixedGrid<int, 20, 10> grid{1};

  ASSERT_NE((grid.view(Indices{1, 1}, Indices{2, 2})), (FixedGrid<int, 2, 2>{5}));
}


TEST(FixedGridFixedExentsView, Fill)
{
  FixedGrid<int, 20, 10> grid{1};

  grid.view<2, 2>(Indices{1, 1}).fill(5);

  EXPECT_EQ((grid[Indices{0, 0}]), 1);
  EXPECT_EQ((grid[Indices{1, 1}]), 5);
  EXPECT_EQ((grid[Indices{1, 2}]), 5);
  EXPECT_EQ((grid[Indices{2, 1}]), 5);
  EXPECT_EQ((grid[Indices{2, 2}]), 5);
  EXPECT_EQ((grid[Indices{3, 3}]), 1);
}


TEST(FixedGridFixedExentsView, Const)
{
  FixedGrid<int, 20, 10> grid{1};

  grid.view<3, 3>(Indices{1, 1})[Indices{1, 1}] = 5;

  const FixedGrid<int, 20, 10> const_grid{grid};

  const auto value = const_grid.view<3, 3>(Indices{1, 1})[Indices{1, 1}];

  ASSERT_EQ(value, 5);
}


TEST(FixedGridFixedExentsView, AssignElement)
{
  FixedGrid<int, 20, 10> grid{1};

  grid.view<3, 3>(Indices{1, 1})[Indices{1, 1}] = 5;

  const auto value = grid[Indices{2, 2}];

  ASSERT_EQ(value, 5);
}


TEST(FixedGridFixedExentsView, AssignGrid)
{
  FixedGrid<int, 20, 10> grid{1};

  grid.view<2, 2>(Indices{1, 1}) = FixedGrid<int, 2, 2>{5};

  EXPECT_EQ((grid[Indices{0, 0}]), 1);
  EXPECT_EQ((grid[Indices{1, 1}]), 5);
  EXPECT_EQ((grid[Indices{1, 2}]), 5);
  EXPECT_EQ((grid[Indices{2, 1}]), 5);
  EXPECT_EQ((grid[Indices{2, 2}]), 5);
  EXPECT_EQ((grid[Indices{3, 3}]), 1);
}


TEST(FixedGridFixedExentsView, CompoundAddGrid)
{
  FixedGrid<int, 20, 10> grid{1};

  grid.view<2, 2>(Indices{1, 1}) += FixedGrid<int, 2, 2>{4};

  EXPECT_EQ((grid[Indices{0, 0}]), 1);
  EXPECT_EQ((grid[Indices{1, 1}]), 5);
  EXPECT_EQ((grid[Indices{1, 2}]), 5);
  EXPECT_EQ((grid[Indices{2, 1}]), 5);
  EXPECT_EQ((grid[Indices{2, 2}]), 5);
  EXPECT_EQ((grid[Indices{3, 3}]), 1);
}


TEST(FixedGridFixedExentsView, Equality)
{
  FixedGrid<int, 20, 10> grid{1};

  grid.view<2, 2>(Indices{1, 1}) = FixedGrid<int, 2, 2>{5};

  EXPECT_EQ((grid.view<2, 2>(Indices{1, 1})), (FixedGrid<int, 2, 2>{5}));
}


TEST(FixedGridFixedExentsView, Inequality)
{
  FixedGrid<int, 20, 10> grid{1};

  ASSERT_NE((grid.view<2, 2>(Indices{1, 1})), (FixedGrid<int, 2, 2>{5}));
}


TEST(FixedGridFixedOriginExentsView, Fill)
{
  FixedGrid<int, 20, 10> grid{1};

  grid.view<1, 1, 2, 2>().fill(5);

  EXPECT_EQ((grid[Indices{0, 0}]), 1);
  EXPECT_EQ((grid[Indices{1, 1}]), 5);
  EXPECT_EQ((grid[Indices{1, 2}]), 5);
  EXPECT_EQ((grid[Indices{2, 1}]), 5);
  EXPECT_EQ((grid[Indices{2, 2}]), 5);
  EXPECT_EQ((grid[Indices{3, 3}]), 1);
}


TEST(FixedGridFixedOriginExentsView, Const)
{
  FixedGrid<int, 20, 10> grid{1};

  grid.view<1, 1, 3, 3>()[Indices{1, 1}] = 5;

  const FixedGrid<int, 20, 10> const_grid{grid};

  const auto value = const_grid.view<3, 3>(Indices{1, 1})[Indices{1, 1}];

  ASSERT_EQ(value, 5);
}


TEST(FixedGridFixedOriginExentsView, AssignElement)
{
  FixedGrid<int, 20, 10> grid{1};

  grid.view<1, 1, 3, 3>()[Indices{1, 1}] = 5;

  const auto value = grid[Indices{2, 2}];

  ASSERT_EQ(value, 5);
}


TEST(FixedGridFixedOriginExentsView, AssignGrid)
{
  FixedGrid<int, 20, 10> grid{1};

  grid.view<1, 1, 2, 2>() = FixedGrid<int, 2, 2>{5};

  EXPECT_EQ((grid[Indices{0, 0}]), 1);
  EXPECT_EQ((grid[Indices{1, 1}]), 5);
  EXPECT_EQ((grid[Indices{1, 2}]), 5);
  EXPECT_EQ((grid[Indices{2, 1}]), 5);
  EXPECT_EQ((grid[Indices{2, 2}]), 5);
  EXPECT_EQ((grid[Indices{3, 3}]), 1);
}


TEST(FixedGridFixedOriginExentsView, CompoundAddGrid)
{
  FixedGrid<int, 20, 10> grid{1};

  grid.view<1, 1, 2, 2>() += FixedGrid<int, 2, 2>{4};

  EXPECT_EQ((grid[Indices{0, 0}]), 1);
  EXPECT_EQ((grid[Indices{1, 1}]), 5);
  EXPECT_EQ((grid[Indices{1, 2}]), 5);
  EXPECT_EQ((grid[Indices{2, 1}]), 5);
  EXPECT_EQ((grid[Indices{2, 2}]), 5);
  EXPECT_EQ((grid[Indices{3, 3}]), 1);
}


TEST(FixedGridFixedOriginExentsView, Equality)
{
  FixedGrid<int, 20, 10> grid{1};

  grid.view<1, 1, 2, 2>() = FixedGrid<int, 2, 2>{5};

  EXPECT_EQ((grid.view<1, 1, 2, 2>()), (FixedGrid<int, 2, 2>{5}));
}


TEST(FixedGridFixedOriginExentsView, Inequality)
{
  FixedGrid<int, 20, 10> grid{1};

  ASSERT_NE((grid.view<1, 1, 2, 2>()), (FixedGrid<int, 2, 2>{5}));
}


TEST(MappedGrid, FromArray)
{
  int segment[200];
  std::fill(segment, segment+200, 1);

  MappedGrid<int> grid{Extents{20, 10}, segment};

  grid.view<2, 2>(Indices{1, 1}) = FixedGrid<int, 2, 2>{5};

  EXPECT_EQ((grid[Indices{0, 0}]), 1);
  EXPECT_EQ((grid[Indices{1, 1}]), 5);
  EXPECT_EQ((grid[Indices{1, 2}]), 5);
  EXPECT_EQ((grid[Indices{2, 1}]), 5);
  EXPECT_EQ((grid[Indices{2, 2}]), 5);
  EXPECT_EQ((grid[Indices{3, 3}]), 1);
}


TEST(MappedFixedGrid, FromArray)
{
  int segment[200];
  std::fill(segment, segment+200, 1);

  MappedFixedGrid<int, 20, 10> grid{segment};

  grid.view<2, 2>(Indices{1, 1}) = FixedGrid<int, 2, 2>{5};

  EXPECT_EQ((grid[Indices{0, 0}]), 1);
  EXPECT_EQ((grid[Indices{1, 1}]), 5);
  EXPECT_EQ((grid[Indices{1, 2}]), 5);
  EXPECT_EQ((grid[Indices{2, 1}]), 5);
  EXPECT_EQ((grid[Indices{2, 2}]), 5);
  EXPECT_EQ((grid[Indices{3, 3}]), 1);
}


TEST(FixedTiledGrid, DefaultValueConstructor)
{
  FixedTiledGrid<int, 20, 20, 10, 10> grid{5};

  for (const auto& v : grid)
  {
    ASSERT_EQ(v, 5);
  }
}


TEST(FixedTiledGrid, SingleTileAssign)
{
  FixedTiledGrid<int, 20, 20, 20, 20> grid{5};

  grid[Indices{5, 5}] = 6;

  EXPECT_EQ((grid[Indices{5, 5}]), 6);
}


TEST(FixedTiledGrid, Assign)
{
  FixedTiledGrid<int, 20, 20, 5, 5> grid{5};

  grid[Indices{5, 5}] = 6;
  grid[Indices{18, 19}] = 9;

  ASSERT_TRUE((grid.mask()[Indices{1, 1}]));
  ASSERT_TRUE((grid.mask()[Indices{3, 3}]));
  ASSERT_EQ(grid.active(), 2);

  EXPECT_EQ((grid[Indices{5, 5}]), 6);
  EXPECT_EQ((grid[Indices{18, 19}]), 9);
}


TEST(FixedTiledGrid, AssignGrid)
{
  FixedTiledGrid<int, 20, 20, 5, 5> grid{1};

  grid.view<2, 2>(Indices{1, 1}) = FixedGrid<int, 2, 2>{5};

  EXPECT_EQ((grid[Indices{0, 0}]), 1);
  EXPECT_EQ((grid[Indices{1, 1}]), 5);
  EXPECT_EQ((grid[Indices{1, 2}]), 5);
  EXPECT_EQ((grid[Indices{2, 1}]), 5);
  EXPECT_EQ((grid[Indices{2, 2}]), 5);
  EXPECT_EQ((grid[Indices{3, 3}]), 1);
}


TEST(FixedTiledGrid, AccessTile)
{
  FixedTiledGrid<int, 20, 20, 5, 5> grid{1};

  grid.view<2, 2>(Indices{1, 1}) = FixedGrid<int, 2, 2>{5};
}


//
// ASSIGNMENT "STRESS" TESTS
//


TEST(Grid, AssignIterated)
{
  Grid<int> grid{Extents{2000, 2000}};

  for (auto& c : grid)
  {
    c = 2;
  }
}


TEST(FixedGrid, AssignIterated)
{
  FixedGrid<int, 200, 200> grid{1};

  for (auto& c : grid)
  {
    c = 2;
  }
}


TEST(FixedTiledGrid, AssignIterated)
{
  FixedTiledGrid<int, 2000, 2000, 500, 500> grid{1};

  for (auto& c : grid)
  {
    c = 2;
  }

  ASSERT_EQ(grid.active(), (FixedTiledGrid<int, 2000, 2000, 500, 500>::TileCount));
}


int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
