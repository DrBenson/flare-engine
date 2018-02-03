/*
Copyright © 2018 Justin Jacobs

This file is part of FLARE.

FLARE is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

FLARE is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
FLARE.  If not, see http://www.gnu.org/licenses/
*/

/**
 * Version number
 */

#include "SharedResources.h"
#include "UtilsParsing.h"
#include "Version.h"

#include <limits.h>
#include <sstream>
#include <iomanip>

Version ENGINE_VERSION(0, 90, 10);
Version VERSION_MIN(0, 0, 0);
Version VERSION_MAX(USHRT_MAX, USHRT_MAX, USHRT_MAX);

Version::Version(unsigned short _x, unsigned short _y, unsigned short _z)
	: x(_x)
	, y(_y)
	, z(_z) {
}

Version::~Version() {
}

bool Version::operator==(const Version& v) {
	return x == v.x && y == v.y && z == v.z;
}

bool Version::operator!=(const Version& v) {
	return !(*this == v);
}

bool Version::operator>(const Version& v) {
	if (x > v.x)
		return true;
	else if (x == v.x && y > v.y)
		return true;
	else if (x == v.x && y == v.y && z > v.z)
		return true;
	else
		return false;
}

bool Version::operator>=(const Version& v) {
	return (*this == v || *this > v);
}

bool Version::operator<(const Version& v) {
	return !(*this >= v);
}

bool Version::operator<=(const Version& v) {
	return (*this == v || *this < v);
}

std::string versionToString(const Version& v) {
	std::stringstream ss;

	// major
	ss << v.x << '.';

	// minor
	if (v.y >= 100 || v.y == 0) {
		ss << v.y;
	}
	else {
		ss << std::setfill('0') << std::setw(2);
		ss << v.y;
	}

	// don't bother printing if there's no patch version
	if (v.z == 0)
		return ss.str();

	ss << '.';

	// patch
	if (v.z >= 100 || v.z == 0) {
		ss << v.z;
	}
	else {
		ss << std::setfill('0') << std::setw(2);
		ss << v.z;
	}

	return ss.str();
}

Version stringToVersion(const std::string& s) {
	std::string val = s + '.';
	Version v;

	v.x = static_cast<unsigned short>(popFirstInt(val, '.'));

	std::string y = popFirstString(val, '.');
	v.y = static_cast<unsigned short>(toInt(y));
	if (y.length() == 1)
		v.y = static_cast<unsigned short>(v.y * 10);

	std::string z = popFirstString(val, '.');
	v.z = static_cast<unsigned short>(toInt(z));
	if (z.length() == 1)
		v.z = static_cast<unsigned short>(v.z * 10);

	return v;
}

std::string createVersionReqString(Version& v1, Version& v2) {
	std::string min_version = (v1 == VERSION_MIN) ? "" : versionToString(v1);
	std::string max_version = (v2 == VERSION_MAX) ? "" : versionToString(v2);
	std::string ret;

	if (min_version != "" || max_version != "") {
		if (min_version == max_version) {
			ret += min_version;
		}
		else if (min_version != "" && max_version != "") {
			ret += min_version + " - " + max_version;
		}
		else if (min_version != "") {
			ret += min_version + ' ' + (msg ? msg->get("or newer") : "or newer");
		}
		else if (max_version != "") {
			ret += max_version + ' ' + (msg ? msg->get("or older") : "or older");
		}
	}

	return ret;
}

