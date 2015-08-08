#pragma once

#ifndef __CDECCORE__
#error cdeccore expected
#endif

#ifndef __STAVL_COMMON__
#define __STAVL_COMMON__

// -------------------------------------------------------------------------- //
// Used in this project only 
// -------------------------------------------------------------------------- //

#ifdef STAVLCOMMON_EXPORTS
#define STAVLCOMMONEXPORT DECLSPEC_EXPORT
#else
#define STAVLCOMMONEXPORT DECLSPEC_IMPORT
#endif

#include <errno.h>

#include "stavlcommon/convertera.h"
#include "stavlcommon/fastlogreader.h"
#include "stavlcommon/logwriter.h"
#include "stavlcommon/stdext.h"
#include "stavlcommon/arrayext.h"

#include "stavlcommon/appenv/u_assert.h"
#include "stavlcommon/appenv/arg_parser.h"

#include "stavlcommon/threading/atom.h"
#include "stavlcommon/threading/lock.h"
#include "stavlcommon/threading/thread.h"

#include "stavlcommon/win32/timer.h"

#endif
