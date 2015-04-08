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

#ifndef __JSONTCPCLIENT__
#define __JSONTCPCLIENT__

#include "susi/apiserver/TCPClient.h"
#include "susi/apiserver/JSONStreamCollector.h"
#include "bson/Value.h"
namespace Susi {
    namespace Api {

        class JSONTCPClient : public TCPClient {
        public:
            JSONTCPClient( std::string addr ) :
                TCPClient {addr},
            collector {[this]( std::string & msg ) {
                auto message = BSON::Value::fromJSON( msg );
                //std::cout<<"got message in json client"<<std::endl;
                this->onMessage( message );
            }
                                } {}
            virtual ~JSONTCPClient() {
                close();
            }
            void send( BSON::Value & message ) {
                std::string msg = message.toJSON();
                TCPClient::send( msg );
            }
            void close() {
                try{
                    TCPClient::close();
                }catch(...){}
            }
        protected:
            JSONStreamCollector collector;
            virtual void onData( std::string & data ) override {
                //std::cout<<"JSONTCPClient::onData()"<<std::endl;
                collector.collect( data );
            }

            virtual void onMessage( BSON::Value & message ) {};

        };

    }
}


#endif // __JSONTCPCLIENT__
