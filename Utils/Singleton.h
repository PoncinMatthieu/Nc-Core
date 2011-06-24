
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

#ifndef NC_CORE_UTILS_SINGLETON_H_
#define NC_CORE_UTILS_SINGLETON_H_

#include "../Define.h"
#include "../System/API/API.h"

namespace Nc
{
    namespace Utils
    {
        /// Implement a template singleton class
        /**
            The singleton class is thread safe.

            <h3>To create a singleton class, create a public heritance like this :  </h3>
 \code
    class MySingleton : public Utils::Singleton<MySingleton>
    {
        void function()
        {
            cout << "hello" << endl;
        }
    }
 \endcode

            <h3>To acces to MySingleton instance anywere in your code :          </h3>
 \code
    MySingleton::Instance()->function();
 \endcode
        */
        template <typename T>
        class   Singleton
        {
            public:
            /**
                Static function wich return the instance of the singleton T.
                \return the instance of the singleton of type T
            */
            static T &Instance()
              {
                if (_instance == NULL)
                {
                    // synchronize the threads access, only once for creation, so in a second if != NULL
                    _mutex.Lock();
                    if (_instance == NULL)
                    {
                        try // in a try catch block, to avoid the mutex be block forever
                        {
                            _instance = new T();
                        }
                        catch (...) // unlock and rethrow
                        {
                            _mutex.Unlock();
                            throw;
                        }
                    }
                    _mutex.Unlock();
                }
                return *_instance;
              }

            /**
                Delete the instance.
                If this function is not called before the program exit, the data will be not released and appears in valgrind.
                It's totally normal and not ugly with a static witch exist during the all execution of the program
            */
              static void DeleteInstance()  {if (_instance != NULL) delete _instance; _instance = NULL;}

            /** \return true if the instance is already created */
              static bool Exist()           {return (_instance != NULL);}

            protected:
                Singleton()                 {_instance = NULL;}
                virtual ~Singleton()        {_instance = NULL;}

                static T                *_instance;     ///< instance of type T
                static System::Mutex    _mutex;         ///< protect the instance for multithread context
        };

        template<typename T>
        T               *Singleton<T>::_instance = NULL;

        template<typename T>
        System::Mutex   Singleton<T>::_mutex;
    }
}

#endif
