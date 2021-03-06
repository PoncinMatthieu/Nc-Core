
/*-----------------------------------------------------------------------------

	3dNovac Core
	Copyright (C) 2010-2011, The 3dNovac Team

    This file is part of 3dNovac.

    3dNovac is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    3dNovac is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with 3dNovac.  If not, see <http://www.gnu.org/licenses/>.

    File Created At:        24/03/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

// This file is used only to forcing the compilation in the dll of the classes without .cpp

#include "Clock.h"
#include "Exception.h"
#include "Convert.h"
#include "IntrusiveSmartPointer.h"
#include "NonCopyable.h"
#include "TagManager.h"

using namespace Nc;

#ifdef SYSTEM_WINDOWS
size_t										Utils::TagRegister::counter = 0;
Utils::TagRegister::MapClassNameTagValue	Utils::TagRegister::tagValues;
#endif

const std::size_t	Utils::String::npos = std::string::npos;

void Utils::Getline(std::ifstream &stream, std::string &line)
{
	std::getline(stream, line);
}

bool Utils::Convert::SplitStringTo(std::string &str, const std::string &delimitor, std::string &dest)
{
	std::string::size_type pos = str.find_first_of(delimitor);
	dest = str.substr(0, pos++);
	if (pos != Utils::String::npos && pos < str.size())
	   str = str.substr(pos, Utils::String::npos);
	else
	   str.clear();
	return true;
}

namespace Nc
{
	namespace Utils
	{
		std::ostream &operator << (std::ostream &os, const Utils::Clock &cl)
		{
			os << "time = " << cl.ElapsedTime() << "s";
			return os;
		}
	}
}

const char *Utils::Exception::what() const throw()
{
	return _msg.c_str();
}
