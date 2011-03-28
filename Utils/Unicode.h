
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

#ifndef NC_CORE_UTILS_UNICODE_H_
#define NC_CORE_UTILS_UNICODE_H_

#include <sstream>
#include <string>
#include "../Define.h"
#include "Convert.h"

namespace Nc
{
    namespace Utils
    {
        /// this namespace provide classes to manipulate any Unicode strings
        namespace Unicode
        {
            template<typename T>
            std::size_t Strlen(const T *str)
            {
                std::size_t len = 0;
                while (*str++)
                    len++;
                return len;
            }

            /// manipulate UTF32 string
            class LCORE UTF32 : public std::basic_string<Nc::UInt32>
            {
                public:
                    UTF32()  {}
                    UTF32(const char *str);
                    UTF32(const wchar_t *str);
                    UTF32(const Nc::UInt32 *str);
                    UTF32(const std::string &str);
                    UTF32(const std::wstring &str);
                    virtual ~UTF32()    {}

                    unsigned int    CharCount(Nc::UInt32 c) const;

                    std::string     ToStdString() const;
                    std::wstring    ToStdWString() const;

                    friend std::ostream& operator << (std::ostream& os, const UTF32& s)
                    {
                        os << s.ToStdString();
                        return os;
                    }
            };
        }
    }
}

#endif
