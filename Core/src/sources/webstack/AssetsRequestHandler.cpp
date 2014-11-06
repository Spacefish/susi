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

#include "webstack/AssetsRequestHandler.h"

void Susi::AssetsRequestHandler::handleRequest( Poco::Net::HTTPServerRequest& request,
        Poco::Net::HTTPServerResponse& response ) {
    LOG(DEBUG) <<  "Assets request from " + request.clientAddress().toString()+" "+request.getURI() ;
    try {
        std::string fileLocation = _rootDirectory.path()+"/"+request.getURI().substr( 8 );
        LOG(DEBUG) <<  "Target file: "+fileLocation ;
        Poco::File f( fileLocation );

        Poco::Timestamp dateTime    = f.getLastModified();
        Poco::File::FileSize length = f.getSize();
        response.set( "Last-Modified", Poco::DateTimeFormatter::format( dateTime, Poco::DateTimeFormat::HTTP_FORMAT ) );
        response.setContentLength( static_cast<int>( length ) );

        if( fileLocation.find( ".svg" ) == fileLocation.size()-4 ) {
            response.setContentType( "image/svg+xml" );
        }

        response.setChunkedTransferEncoding( false );

        Poco::FileInputStream istr( fileLocation );
        if( istr.good() ) {
            std::ostream& ostr = response.send();
            Poco::StreamCopier::copyStream( istr, ostr );
        }
        else {
            throw std::runtime_error {"cannot find file"};
        }
        LOG(DEBUG) << "correctly leaving assets handler";
    }
    catch( const std::exception & e ) {
        LOG(DEBUG) <<  "got error "+std::string {e.what()} ;
        response.setChunkedTransferEncoding( true );
        response.setContentType( "text/html" );
        response.setStatus( Poco::Net::HTTPServerResponse::HTTPStatus::HTTP_NOT_FOUND );
        std::ostream& ostr = response.send();
        ostr << "<html><head><title>SUSI</title></head> ";
        ostr << "<body><p style=\"text-align: center;font-size: 48px;\"></br></br></br>";
        ostr << "(404) Not found: "+std::string( e.what() );
        ostr << "</p></body></html>";
    }
}
