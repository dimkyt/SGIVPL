#pragma once

#include <unordered_map>

namespace giis
{
  enum class RenderMode
  {
    NORMAL,
    RSM_WCS,
    RSM_NORMAL,
    RSM_FLUX,
    RSM_DEPTH,
    DEPTH
  };

  enum class RenderTarget
  {
    WCS,
    NORMAL,
    FLUX,
    DEPTH_LOW
  };
}