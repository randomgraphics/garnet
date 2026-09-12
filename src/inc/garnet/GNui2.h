#ifndef __GN_UI2_H__
#define __GN_UI2_H__

/// \namespace GN::ui2 — immediate-mode UI integration for window input and gpu2.
namespace GN::ui2 {};

#include "GNengine2.h"
#include "GNwin.h"
#include <imgui.h>

#define __GN_INSIDE_UI2_H__ 1
#include "ui2/imgui-backend.h"
#undef __GN_INSIDE_UI2_H__

#endif
