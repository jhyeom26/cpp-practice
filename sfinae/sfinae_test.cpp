#include "sfinae.h"

#include <gtest/gtest.h>

TEST(sfinae_test, BasicTest)
{
  auto drawableWindow = openDrawableWinwow<DrawableWindow>();
  // openDrawableWinwow<NonDrawableWindow>(); // -> Compile Error!
  drawableWindow.viewWindow();
  drawableWindow.drawPolygon(Polygon::Triangle);

  auto nonDrawableWindow = openNonDrawableWinwow<NonDrawableWindow>();
  // openNonDrawableWinwow<DrawableWindow>(); // -> Compile Error!
  nonDrawableWindow.viewWindow();

  EXPECT_TRUE(
    detail::HasViewWindowMethod<DrawableWindow>::value ? true : false);
  EXPECT_TRUE(
    detail::HasViewWindowMethod<NonDrawableWindow>::value ? true : false);
  EXPECT_TRUE(
    detail::HasDrawPolygonMethod<DrawableWindow>::value ? true : false);
  EXPECT_FALSE(
    detail::HasDrawPolygonMethod<NonDrawableWindow>::value ? true : false);
}
