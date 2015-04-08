/*
 * Copyright (c) 2014, webvariants GmbH, http://www.webvariants.de
 *
 * This file is released under the terms of the MIT license. You can find the
 * complete text in the attached LICENSE file or online at:
 *
 * http://www.opensource.org/licenses/mit-license.php
 *
 * @author: Christian Sonderfeld (christian.sonderfeld@webvariants.de)
 */


#ifndef __EVENTMANAGERCOMPONENT__
#define __EVENTMANAGERCOMPONENT__

#include <atomic>
#include "susi/events/EventManager.h"
#include "susi/world/Component.h"
#include "susi/logger/easylogging++.h"

namespace Susi {
    namespace Events {

        class ManagerComponent : public Manager, public Susi::System::Component
        {
        protected:
            std::atomic<bool> noPublish;
            std::mutex mutex;
        public:
            ManagerComponent( size_t workers = 4, size_t buffsize = 32 ) : Manager {workers, buffsize} , noPublish {true} {}

            void publish( EventPtr event, Consumer finishCallback = Consumer {}, bool processorsOnly = false, bool consumersOnly = false, bool highPrio = true) {
                if( noPublish.load() ) {
                    std::shared_ptr<Event> sharedEvent {event.release()};
                    finishCallback( sharedEvent );
                    return;
                }
                Manager::publish( std::move( event ), std::move( finishCallback ), processorsOnly, consumersOnly , highPrio);
            }

            virtual void start() override {
                noPublish.store( false );
                LOG(INFO) <<  "started EventManagerComponent" ;
            }

            virtual void stop() override {
                if( !noPublish.load() ) {
                    noPublish.store( true );
                    std::unique_lock<std::mutex> lk( mutex );
                    if( publishProcesses.size()>0 ) {
                        publishFinished.wait( lk,[this]() {
                            return publishProcesses.size() == 0;
                        } );
                    }
                }
            }

            virtual ~ManagerComponent() {
                stop();
                LOG(INFO) <<  "stopped EventManagerComponent" ;
            }
        };

    }
}

#endif // __EVENTMANAGERCOMPONENT__
