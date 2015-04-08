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

#include "susi/enginestarter/EngineStarterComponent.h"

void Susi::EngineStarter::StarterComponent::handleStart( Susi::Events::EventPtr event ) {
    try {
        if(event->payload["path"].isString()){
            std::string path = event->payload["path"];
            execute( path );
        }else{
            execute( _defaultPath );
        }
    }
    catch( const std::exception & e ) {
        std::string msg = "Error in handleStart(): ";
        msg += e.what();
        throw std::runtime_error( msg );
    }
}

void Susi::EngineStarter::StarterComponent::handleRestart( Susi::Events::EventPtr event ) {
    try {
        if(event->payload["path"].isString()){
            std::string path = event->payload["path"];
            killall();
            execute( path );
        }else{
            killall();
            execute( _defaultPath );
        }
    }
    catch( const std::exception & e ) {
        std::string msg = "Error in handleRestart(): ";
        msg += e.what();
        throw std::runtime_error( msg );
    }
}

void Susi::EngineStarter::StarterComponent::handleStop( Susi::Events::EventPtr event ) {
    killall();
}