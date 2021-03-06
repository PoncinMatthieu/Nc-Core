
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

#ifndef NC_CORE_SYSTEM_INPUT_MANAGER_H_
#define NC_CORE_SYSTEM_INPUT_MANAGER_H_

#include <list>
#include <queue>
#include "../../Define.h"
#include "Input.h"

namespace Nc
{
    namespace System
    {
        /// Manage the inputs, like WindowInput.
        /**
			An input listener, contain a list of input in which they are attached, and an event queue to receive events.

			The event queue is protected with a mutex.

            \sa
                - Input
        */
        class LIB_NC_CORE InputListener : public Utils::NonCopyable
        {
            private:
                typedef std::list<Input*>           ListInput;

            public:
                InputListener();
                virtual ~InputListener();

                /** Add the given input and set the eventQueue pointer of the input. */
                void                    AddInput(Input *input);
                /** Remove the given input from the list of inputs. */
                void                    RemoveInput(Input *input);

                /** Fill the given event by the event which was in the eventQueue. */
                bool                    PollEvent(Event &e);

                /** \return the mutex which protect the event queue. */
                inline System::Mutex    &GetMutex()                         {return _mutexQueue;}
                /** \return the event queue of the listener. */
                inline EventQueue       &GetEventQueue()                    {return _eventQueue;}

            private:
                ListInput       _inputList;         ///< the list of inputs.
                EventQueue      _eventQueue;        ///< the event queue which store the event received by the inputs.
                Mutex           _mutexQueue;        ///< the mutex used to protect the event queue.
        };
    }
}

#endif
