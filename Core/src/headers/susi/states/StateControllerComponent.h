/*
 * Copyright (c) 2014, webvariants GmbH, http://www.webvariants.de
 *
 * This file is released under the terms of the MIT license. You can find the
 * complete text in the attached LICENSE file or online at:
 *
 * http://www.opensource.org/licenses/mit-license.php
 *
 * @author: Thomas Krause (thomas.krause@webvariants.de)
 */

#ifndef __STATE_CONTROLLER_COMPONENT__
#define __STATE_CONTROLLER_COMPONENT__

#include "susi/world/BaseComponent.h"
#include "susi/states/StateController.h"

namespace Susi {
    namespace States {
        class StateControllerComponent : public Susi::System::BaseComponent , public StateController {
        public:
            StateControllerComponent( Susi::System::ComponentManager * mgr, std::string file ) :
                Susi::System::BaseComponent {mgr}, StateController {file} {}

            virtual void start() override {
                subscribe( std::string{"heartbeat::fiveMinute"}, Susi::Events::Processor{[this]( ::Susi::Events::EventPtr evt ) {
                    handleAutoSave( std::move( evt ) );
                }} );

                subscribe( std::string{"state::saveState"}, Susi::Events::Processor{[this]( ::Susi::Events::EventPtr evt ) {
                    handleSave( std::move( evt ) );
                }} );
                subscribe( std::string{"state::setState"}, Susi::Events::Processor{[this]( ::Susi::Events::EventPtr evt ) {
                    handleSetState( std::move( evt ) );
                }} );
                subscribe( std::string{"state::getState"}, Susi::Events::Processor{[this]( ::Susi::Events::EventPtr evt ) {
                    handleGetState( std::move( evt ) );
                }} );
                subscribe( std::string{"state::setPersistentState"}, Susi::Events::Processor{[this]( ::Susi::Events::EventPtr evt ) {
                    handleSetPersistentState( std::move( evt ) );
                }} );
                subscribe( std::string{"state::getPersistentState"}, Susi::Events::Processor{[this]( ::Susi::Events::EventPtr evt ) {
                    handleGetPersistentState( std::move( evt ) );
                }} );
                LOG(INFO) <<  "started StateControllerComponent" ;
            }

            virtual void stop() override {
                handleSave(createEvent("save"));
                unsubscribeAll();
            }

            ~StateControllerComponent(){
                stop();
                LOG(INFO) <<  "stopped StateControllerComponent" ;
            }

        protected:
            void handleAutoSave( Susi::Events::EventPtr event );
            void handleSave( Susi::Events::EventPtr event );
            void handleSetState( Susi::Events::EventPtr event );
            void handleGetState( Susi::Events::EventPtr event );
            void handleSetPersistentState( Susi::Events::EventPtr event );
            void handleGetPersistentState( Susi::Events::EventPtr event );
        };
    }
}

#endif // __STATE_CONTROLLER_COMPONENT__
