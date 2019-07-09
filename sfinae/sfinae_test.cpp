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
}
