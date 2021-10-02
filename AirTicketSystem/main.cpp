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
        << "date" << "2021-10-29"<< finalize;
    

    bsoncxx::document::view f_view = fil_value.view();
    
    
    auto tmp = database.readMany(Database::Flight_Info, bsoncxx::to_json(f_view));
    
    for (auto t : tmp) {
        cout << t << endl;
    }
    
    return 0;
}
