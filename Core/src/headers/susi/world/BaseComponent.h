/*
 * Copyright (c) 2014, webvariants GmbH, http://www.webvariants.de
 *
 * This file is released under the terms of the MIT license. You can find the
 * complete text in the attached LICENSE file or online at:
 *
 * http://www.opensource.org/licenses/mit-license.php
 *
 * @author: Tino Rusch (tino.rusch@webvariants.de)
 */

#ifndef __BASECOMPONENT__
#define __BASECOMPONENT__

#include <vector>

#include "susi/world/Component.h"
#include "susi/world/ComponentManager.h"
#include "susi/events/EventManagerComponent.h"
#include "susi/events/IEventSystem.h"

namespace Susi {
    namespace System {

        class BaseComponent : public Component {
        protected:
            ComponentManager * componentManager;
            std::shared_ptr<Susi::Events::IEventSystem> eventManager;
            std::vector<long> evtIdPool;

        public:
            BaseComponent( ComponentManager * manager ) : componentManager {manager} {
                eventManager = componentManager->getComponent<Susi::Events::IEventSystem>( "eventsystem" );
            }
            long subscribe( std::string topic, Susi::Events::Processor processor, std::string name="" ) {
                long id = eventManager->subscribe( topic,std::move( processor ),name );
                evtIdPool.push_back( id );
                return id;
            }
            /*long subscribe( Susi::Events::Predicate pred, Susi::Events::Processor processor, std::string name="" ) {
                long id = eventManager->subscribe( pred,std::move( processor ),name );
                evtIdPool.push_back( id );
                return id;
            }*/
            long subscribe( std::string topic, Susi::Events::Consumer consumer, std::string name="" ) {
                long id = eventManager->subscribe( topic,std::move( consumer ),name );
                evtIdPool.push_back( id );
                return id;
            }
            /*long subscribe( Susi::Events::Predicate pred, Susi::Events::Consumer consumer, std::string name="" ) {
                long id = eventManager->subscribe( pred,std::move( consumer ),name );
                evtIdPool.push_back( id );
                return id;
            }*/
            bool unsubscribe( long id ) {
                return eventManager->unsubscribe( id );
            }
            void publish( Susi::Events::EventPtr event, Susi::Events::Consumer finishCallback = Susi::Events::Consumer {} , bool processorsOnly = false, bool consumersOnly= false, bool highPrio = true) {
                eventManager->publish( std::move( event ),finishCallback );
            }
            Susi::Events::EventPtr createEvent( std::string topic ) {
                return eventManager->createEvent( topic );
            }

            void markConfidential(Susi::Events::Event & event, char authlevel){
                event.headers.push_back({"confidential",std::to_string(authlevel)});
            }

            bool unsubscribeAll() {

                bool result = true;

                for( auto id : evtIdPool )
                {
                    if( eventManager->unsubscribe( id ) == false ) {
                        result = false;
                    }
                }

                evtIdPool.clear();

                return result;
            }

        };

    }
}

#endif // __BASECOMPONENT__
