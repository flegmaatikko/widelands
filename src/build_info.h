/*
 * Copyright (C) 2006-2015 by the Widelands Development Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#ifndef WL_BUILD_INFO_H
#define WL_BUILD_INFO_H

#include <string>

constexpr uint16_t kWidelandsCopyrightStart = 2002;
constexpr uint16_t kWidelandsCopyrightEnd = 2015;

///\return the build id which is automagically created from the checkout's
///revision number or the VERSION file
const std::string & build_id();

///\return the build type, which is set during compile time (either manually
///or to a default value)
const std::string & build_type();

#endif  // end of include guard: WL_BUILD_INFO_H
