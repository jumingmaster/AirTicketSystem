//
//  Database.cpp
//  AirTicketSystem
//
//  Created by Henry Zeng on 2021/9/30.
//

#include "Database.hpp"


//数据库构造函数
Database::Database() noexcept {
    //航班数据集合
    flight_conn = db_client[databaseProper::defaultDatabaseName][databaseProper::flight_info_conn];
    //用户数据集合
    user_conn = db_client[databaseProper::defaultDatabaseName][databaseProper::user_info_conn];
    //实名认证数据集合
    auth_conn = db_client[databaseProper::defaultDatabaseName][databaseProper::auth_info_conn];
    
}

//插入一个文档
bool Database::insertOne(const InfoType &info_type, string json) {
    
    
    try {
        auto json_data = bsoncxx::from_json(json);  //将 Json 数据转成 Bson 文档
        try {
            if (info_type == User_Info) {
                //result：插入文档函数的返回值
                //插入一个文档
                bsoncxx::stdx::optional<mongocxx::result::insert_one> result = user_conn.insert_one(json_data.view());
                //打印结果
                printResult(result);
                return true;
                
            } else if (info_type == Auth_Info) {
                
                bsoncxx::stdx::optional<mongocxx::result::insert_one> result =
                    auth_conn.insert_one(json_data.view());
                
                printResult(result);
                return true;
                
            } else if (info_type == Flight_Info) {
                
                
                    bsoncxx::stdx::optional<mongocxx::result::insert_one> result =
                        flight_conn.insert_one(json_data.view());
                    
                    printResult(result);
                return true;
                }
            } catch (mongocxx::bulk_write_exception &err) {
                
                    printException("When insert a document throw exception", err);
                    return false;
                }
            
        }//捕获错误
        catch (bsoncxx::exception &err) {
            printException("When crete a document from json string throw exception", err);
            return false;
        }
    
        return true;
}

bool Database::insertMany(const InfoType &info_type, std::vector<string> docs) {
    
    std::vector<bsoncxx::document::value> documents;
    try {
        for (auto ptmp : docs) {
            documents.push_back(bsoncxx::from_json(ptmp));
        }
    } catch (bsoncxx::exception &err) {
        printException("When push_back a document throw exception", err);
        return false;
    }
    try {
        switch (info_type) {
            
            case User_Info:
                user_conn.insert_many(documents);
                return true;
                break;
            
            case Auth_Info:
                auth_conn.insert_many(documents);
                return true;
                break;
            
            case Flight_Info:
                flight_conn.insert_many(documents);
                return true;
                break;
            
            default:
                cout << "unknown info type." << endl;
                return false;
        }
    } catch (mongocxx::bulk_write_exception &err) {
            printException("When insert some documents throw exception", err);
        }
    
    return false;
}


bool Database::updateOne(const InfoType &info_type, string filter, string json) {
    
    auto doc = bsoncxx::from_json(json);
    auto fil = bsoncxx::from_json(filter);
    update_result result;
    try {
        switch (info_type) {
            
            case User_Info:
                result = user_conn.update_one(fil.view(), doc.view());
                printResult(result);
                return true;
            
            case Auth_Info:
                result = auth_conn.update_one(fil.view(), doc.view());
                printResult(result);
                return true;
            
            case Flight_Info:
                result = flight_conn.update_one(fil.view(), doc.view());
                printResult(result);
                return true;
            
            default:
                cout << "unknown info type." << endl;
                return false;
        }
    } catch (mongocxx::logic_error &err) {
        printException("When update a document throw exception", err);
        return false;
    }
    
    
    return false;
}


bool Database::deleteOne(const InfoType &info_type, string filter) {
    
    try {
        auto f_view = bsoncxx::from_json(filter);  //将 Json 数据转成 Bson 文档
        try {
            if (info_type == User_Info) {
                delete_result result = user_conn.delete_one(f_view.view());
                printResult(result);
                return true;
                
            } else if (info_type == Auth_Info) {
                
                delete_result result = auth_conn.delete_one(f_view.view());
                
                printResult(result);
                return true;
                
            } else if (info_type == Flight_Info) {
                delete_result result = flight_conn.delete_one(f_view.view());
                    
                printResult(result);
                return true;
                }
            } catch (mongocxx::bulk_write_exception &err) {
                
                    printException("When delete a document throw exception", err);
                    return false;
                }
            
        }//捕获错误
        catch (bsoncxx::exception &err) {
            printException("When crete a document from json string throw exception", err);
            return false;
        }
    return false;
}

bool Database::deleteMany(const InfoType &info_type, std::vector<string> filter) {
    
    delete_result result;
    
    try {
        for (auto ptmp : filter) {
            try {
                switch (info_type) {
                    
                    case User_Info:
                        result = user_conn.delete_one(bsoncxx::from_json(ptmp).view());
                        printResult(result);
                        return true;
                    
                    case Auth_Info:
                        result = auth_conn.delete_one(bsoncxx::from_json(ptmp).view());
                        printResult(result);
                        return true;
                        
                    
                    case Flight_Info:
                        result = flight_conn.delete_one(bsoncxx::from_json(ptmp).view());
                        printResult(result);
                        return true;
                        
                    default:
                        cout << "unknown info type." << endl;
                        return false;
                }
            } catch (mongocxx::bulk_write_exception &err) {
                    printException("When insert some documents throw exception", err);
                }
        }
    } catch (bsoncxx::exception &err) {
        printException("When push_back a document throw exception", err);
        return false;
    }
    
    return false;
}

string Database::readOne(const InfoType &info_type, string filter) {
    
    bsoncxx::stdx::optional<bsoncxx::document::value> val;
    try {
        auto f_view = bsoncxx::from_json(filter);  //将 Json 数据转成 Bson 文档
        try {
            if (info_type == User_Info) {

                val = user_conn.find_one(f_view.view());

                if (val) {
                    return bsoncxx::to_json(*val);
                } else return nullptr;
                
            } else if (info_type == Auth_Info) {
                
                val = auth_conn.find_one(f_view.view());
                if (val) {
                    return bsoncxx::to_json(*val);
                } else return nullptr;
                
            } else if (info_type == Flight_Info) {
                val = flight_conn.find_one(f_view.view());

                if (val) {
                    return bsoncxx::to_json(*val);
                } else return nullptr;
                }
            } catch (mongocxx::query_exception &err) {
                
                    printException("When delete a document throw exception", err);
                    return nullptr;
                }
            
        }//捕获错误
        catch (bsoncxx::exception &err) {
            printException("When crete a document from json string throw exception", err);
            return nullptr;
        }
    
    return nullptr;
}

std::vector<string> Database::readMany(const InfoType &info_type, string filter) {
    std::vector<string> docs;
    if (filter.empty()) {
        if (info_type == User_Info) {
            mongocxx::cursor cursor = findAllDoc(user_conn);
            for (auto pTmp : cursor) {
                docs.push_back(bsoncxx::to_json(pTmp));
            }
        } else if (info_type == Auth_Info) {
            mongocxx::cursor cursor = findAllDoc(auth_conn);
            for (auto pTmp : cursor) {
                docs.push_back(bsoncxx::to_json(pTmp));
            }
        } else if (info_type == Flight_Info) {
            mongocxx::cursor cursor = findAllDoc(flight_conn);
            for (auto pTmp : cursor) {
                docs.push_back(bsoncxx::to_json(pTmp));
            }
        }
    } else {
        if (info_type == User_Info) {
            mongocxx::cursor cursor = user_conn.find(bsoncxx::from_json(filter));
            for (auto pTmp : cursor) {
                docs.push_back(bsoncxx::to_json(pTmp));
            }
        } else if (info_type == Auth_Info) {
            mongocxx::cursor cursor = auth_conn.find(bsoncxx::from_json(filter));
            for (auto pTmp : cursor) {
                docs.push_back(bsoncxx::to_json(pTmp));
            }
        } else if (info_type == Flight_Info) {
            mongocxx::cursor cursor = flight_conn.find(bsoncxx::from_json(filter));
            for (auto pTmp : cursor) {
                docs.push_back(bsoncxx::to_json(pTmp));
            }
        }
    }
    
    return docs;
}

mongocxx::cursor Database::findAllDoc(mongocxx::collection &conn) {
    try {
        mongocxx::cursor cursor = conn.find({});
        return cursor;
    } catch ( mongocxx::logic_error &err) {
        throw err;
    }
}

void Database::printResult(insert_one_result &result) const noexcept {
    cout << "Insert " << result->result().inserted_count() << " document" << ", object id: "
         << result->inserted_id().get_oid().value.to_string() << endl;
}

void Database::printResult(update_result &result) const noexcept {
    cout << "Update " << result->result().modified_count() << " document" << endl;
    
}

void Database::printResult(delete_result &result) const noexcept {
    cout << "Delete " << result->deleted_count() << " document" << endl;
}

void Database::printException(string failed_string, mongocxx::exception &err) const {
    cout << failed_string << ", err_code: " << err.code() << ": " << err.what() << endl;
}
void Database::printException(string failed_string, mongocxx::logic_error &err) const {
    cout << failed_string << ", err_code: " << err.code() << ": " << err.what() << endl;
}

void Database::printException(std::string failed_string, mongocxx::bulk_write_exception &err) const { 
    cout << failed_string << ", err_code: " << err.code() << ": " << err.what() << endl;
}

void Database::printException(std::string failed_string, bsoncxx::exception &err) const {
    cout << failed_string << ", err_code: " << err.code() << ": " << err.what() << endl;;
}

void Database::printException(std::string failed_string, mongocxx::query_exception &err) const { 
    cout << failed_string << ", err_code: " << err.code() << ": " << err.what() << endl;;
}














