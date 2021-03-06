
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

#ifndef NC_CORE_ENGINE_MANAGER_H_
#define NC_CORE_ENGINE_MANAGER_H_

#include "../Define.h"
#include "../Utils/FileName.h"
#include "../Utils/Mask.h"
#include "../System/API/API.h"

namespace Nc
{
    /// Provide a creation and management system to use threaded engine, like a GraphicalEngine, GameEngine, AudioEngine, and NetworkEngine, or whatever...
    /**
        \todo Add an option to avoid creation of threads or to synchronize an engine with an other(s) (for instance it would be very intersting to make some benchmark with and without multithreaded opengl contexts - to see if it's really good to use a dedicated rendering thread with an other shared context).
    */
    namespace Engine
    {
        class   IEngine;
        struct  IEvent;

        /// Manage Engines.
        /**
            The Manager allow to manage a list of engines, and schedule the launching of the threads (engines) and will dispatch events between them.
            It Stores a map of engine associated to a permission mask.

            The Manager act as an intermediate bettween engines and therefore can dispatch events through engines. To send an event to an engine, use the method 'PushEvent' with the engine's name and commmand id.
            The name of an engine correspond to the result of the method 'ResolvedClassName', so when you define your own engine, you should always redefine this method.

            \sa
                - IEngine
                - MainEngine
        */
        class LIB_NC_CORE Manager : public Utils::NonCopyable
        {
            public:
                /// Define the engines permissions.
                enum Permission
                {
                    Nop,
                    Exit,       ///< Allow to exit the manager.
                };
                typedef Utils::Mask<Permission>             Permissions;

				/// store an engine with it's permissions.
                struct AllowedEngine
                {
                    AllowedEngine()                                     {}
                    AllowedEngine(IEngine *e, const Permissions &p)
                        : engine(e), permissions(p)                     {}

                    IEngine         *engine;
                    Permissions     permissions;
                };

                typedef std::map<std::string, AllowedEngine>        MapEngine;

            public:
                /**
                    \param confFile allow you to load automatically the 3dNovac System::Config file, it will be closed in the destructor. If the param stay empty, it won't load the confFile.
                    \param initCrashReporter if true, will create and init the default crashreporter, this one will be deleted in the destructor.
                */
                Manager(const Utils::FileName &confFile = "", bool initCrashReporter = true);
                virtual ~Manager();

                /** Add an engine. */
                virtual void            AddEngine(IEngine *engine, const Permissions &permissions = Exit);
                /** Remove the engine with the given name, delete it if del is true. */
                virtual void            RemoveEngine(const std::string &name, bool del = true);
                /** \return the corresponding engine. */
                static IEngine          *GetEngine(const std::string &name);
                /** \return the engines managed by the Manager. */
                static MapEngine        &GetEngines()                   {return _mapEngine;}

                /** \return true if the engines is launch. */
                inline bool             IsLaunched()                    {return _isLaunched;}

                /** Start the engines. */
                virtual void            Start();
                /** Stop the engines if the current thread has been allowed to do it. */
                void                    Stop();
                /** Wait the engines until they are stoped. */
                virtual void            Wait();

                /** \return the mutex which is used to protect and synchronize the engines. */
                inline System::Mutex    &MutexGlobal()                  {return _mutexGlobal;}

                /** Wait until all the engine as been started. */
                void                    WaitAllEngineStarted();
                /** Wait until the given \p priority is good for loading the engine context. */
                void                    WaitLoadingContextPriority(unsigned char priority);
                /** Wait until the given \p priority is good for loading the engine. */
                void                    WaitLoadingPriority(unsigned char priority);
                /** Wait until all engine contexts are loaded. */
                void                    WaitEnginesContextLoading();
                /** Wait until all engines are loaded. */
                void                    WaitEnginesLoading();
				/** Wait until the engine as the priority to delete it's contents. */
				void					WaitReleasePriority(unsigned int priority);

                /** Dispatch events to the corresponding engine. */
                template<typename T>
                static void             PushEvent(const std::string &engineName, unsigned int id, const T &arg);
                /** Dispatch events to the corresponding engine. */
                static void             PushEvent(const std::string &engineName, unsigned int id);
                /** Dispatch events to the corresponding engine. */
                static void             PushEvent(const std::string &engineName, unsigned int id, IEvent *e);

            protected:
                static MapEngine        _mapEngine;             ///< Engine map container.

                bool                    _isLaunched;            ///< true, if the engines are launched.
                static System::Mutex    _mutexGlobal;           ///< global mutex used to synchronize threads if needed.

            private:
                unsigned int        _mainThreadId;
                bool                _confFileOpened;            ///< true, if the Config file of 3dNovac has been loaded by the Manager, in this case it will be closed be the Manager.
                bool                _crashReporterInitialized;  ///< true, if the crash reporter has been initialized in the constructor, this one will be deleted in the destructor.
        };
    }
}

#include "IEngine.h"

namespace Nc
{
    namespace Engine
    {
        template<typename T>
        void Manager::PushEvent(const std::string &engineName, unsigned int id, const T &arg)
        {
            try
            {
                GetEngine(engineName)->PushEvent(id, arg);
            }
            catch(const std::exception &e)
            {
                LOG_ERROR << "Error: " << e.what() << std::endl;
            }
        }
    }
}

#endif // GAMEMANAGER_H_INCLUDED

