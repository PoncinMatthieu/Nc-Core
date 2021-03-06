
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

    File Created At:        10/11/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_CORE_ENGINE_EVENT_MANAGER_H_
#define NC_CORE_ENGINE_EVENT_MANAGER_H_

#include <typeinfo>
#include <list>
#include <queue>
#include "../Define.h"
#include "../System/Object.h"
#include "../System/API/API.h"

namespace Nc
{
    namespace Engine
    {
        class   EventManager;
        struct  IEvent;

        /**
            Member function pointer used to receive an event.
            \param e could be NULL
        */
        typedef void (EventManager::*CmdFunction)(IEvent *e);

        /// Interface to define an engine event.
        struct LIB_NC_CORE IEvent
        {
            virtual ~IEvent()       {}

            /**
                \return the data of type T of the event.
                Use a dynamic_cast with the template class Event<T> to return the data,
                so the method throw an exception if the event is not of type Event<T>.
            */
            template<typename T>
            T   &GetData();

            /** \return the typeid of the event data, to debug and know what type of event you received. */
            virtual const std::type_info    &GetDataTypeId() = 0;
        };

        /// Define an engine event which simply contain a data of type T.
        template<typename T>
        struct Event : public IEvent
        {
            Event(const T &d)  : data(d)   {}

            /** \return the typeid of the event data, can be usefull to debug and know what type of event you received. */
            virtual const std::type_info    &GetDataTypeId();

            T   data;           ///< data of type T.
        };

        /// Define a Cmd handler which have an id and a member function pointer.
		/**
            Cmds will be used to store command handler to be called at event reception.
			The id is to identify the event, and call the function.
		*/
        struct LIB_NC_CORE Cmd
        {
            Cmd(unsigned int i, CmdFunction f) : id(i), function(f)    {}

            unsigned int        id;             ///< Id of the command.
            CmdFunction         function;       ///< The member function pointer to excute the handler event.
        };

        /// Base class used to manage events, can receive events and execute them.
		/**
            The Event manager allow to create command handlers by using the method 'AddNewCmd' which will take an id corresponding to the event and a pointer to a member function to call.
            Finnaly, events can be pushed from the Manager class to send events and call a command.
		*/
        class LIB_NC_CORE EventManager : public System::Object
        {
            public:
                NC_SYSTEM_DEFINE_OBJECT(Nc::System::Object, Nc::Engine::EventManager);

            private:
                typedef std::queue<std::pair<unsigned int, IEvent*> >       QueueEvent;
                typedef std::list<Cmd>                                      ListCmd;

            public:
                EventManager();
                virtual ~EventManager();

                /** Push an IEvent. */
                void    PushEvent(unsigned int id, IEvent *e = NULL);
                /** Push an Event<T> with an argument. */
                template<typename T>
                void    PushEvent(unsigned int id, const T &arg);

                /** Execute all event present in the event queues. */
                void    ExecuteEvents();

            protected:
                /** Create a new command to receive events. */
                void    AddNewCmd(unsigned int id, CmdFunction function);

                bool    _execEvents;                    ///< to pause the execution of events in the event queue.
                bool    _receiveEvents;                 ///< to bypass events (stop the reception of events).
                bool    _throwErrorAtUnknownCommand;    ///< if true, will throw an error if the event manager receive an unknown command. By default the statement is true.

            private:
                /** Execute an event. */
                void    ExecuteEvent(unsigned int id, IEvent *e);

                QueueEvent              _queueEvent;                ///< Queue of IEvent to execute when we call `ExecuteEvents`.
                ListCmd                 _listCmd;                   ///< List of Event handler to use for the management of events.
                System::Mutex           _mutexQueue;                ///< Protect the mutex queue.

                friend LIB_NC_CORE std::ostream& operator << (std::ostream& Out, const EventManager& o);
        };


// templates implementation
        template<typename T>
        T   &IEvent::GetData()
        {
            if (this == NULL)
                throw Utils::Exception("IEvent:GetData", "Event is Null, Event<" + std::string(typeid(T).name()) + "> expected");
            Event<T> *e = dynamic_cast<Event<T>*>(this);
            if (e == NULL)
                throw Utils::Exception("IEvent:GetData", "Bad param, Event<" + std::string(typeid(T).name()) + "> expected");
            return e->data;
        }

        template<typename T>
        const std::type_info    &Event<T>::GetDataTypeId()
        {
            return typeid(T);
        }

        template<typename T>
        void    EventManager::PushEvent(unsigned int id, const T &arg)
        {
            System::Locker l(&_mutexQueue);
            if (_receiveEvents)
                _queueEvent.push(std::pair<unsigned int, IEvent*>(id, new Event<T>(arg)));
        }
    }
}

#endif
