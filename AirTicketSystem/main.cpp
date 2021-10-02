//
//  main.cpp
//  AirTicketSystem
//
//  Created by Henry Zeng on 2021/9/30.
//

#include "Database.hpp"


int main(int argc, const char * argv[]) {
    
    mongocxx::instance instance{};
    Database database{};
    
    bsoncxx::builder::stream::document filter;
    bsoncxx::builder::stream::document builder;
    
    bsoncxx::document::value fil_value = filter
        << "number" << "MU6302"<< finalize;
    
    bsoncxx::document::value doc_value = builder
      << "name" << "MongoDB"
      << "type" << "database"
      << "count" << 1
      << "versions" << bsoncxx::builder::stream::open_array
        << "v3.2" << "v3.0" << "v2.6"
      << close_array
      << "info" << bsoncxx::builder::stream::open_document
        << "x" << 203
        << "y" << 102
      << bsoncxx::builder::stream::close_document
      << bsoncxx::builder::stream::finalize;

    
    bsoncxx::document::view view = doc_value.view();
    bsoncxx::document::view f_view = fil_value.view();
    
    cout << database.readOne(Database::Flight_Info, bsoncxx::to_json(f_view)) << endl;

    
    return 0;
}
