#ifndef __DATABASE__
#define __DATABASE__

#include <soci.h>
#include <sqlite3/soci-sqlite3.h>
#include <iostream>

#include "bson/Value.h"

namespace Susi {
    namespace DB {
        class Database {
        protected:
            soci::session session;
        public:
            Database( std::string dbtype,std::string connectURI ) : session( dbtype,connectURI ) {};
            BSON::Value query( std::string query );
        };
    }
}

#endif // __DATABASE__
