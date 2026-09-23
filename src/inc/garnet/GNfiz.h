#pragma once
#ifndef __GN_FIZ_GNFIZ_H__
#define __GN_FIZ_GNFIZ_H__

/// \namespace GN::fiz — Physics and continuous media simulation module
namespace GN::fiz {}

#include "GNbase.h"

// GNfiz subheaders below must only be included through this file.
#define __GN_INSIDE_FIZ_H__ 1
#include "fiz/common.h"
#include "fiz/temper.h"
#include "fiz/hull.h"
#include "fiz/solid.h"
#undef __GN_INSIDE_FIZ_H__

#endif // __GN_FIZ_GNFIZ_H__
