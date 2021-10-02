//
//  Database.hpp
//  AirTicketSystem
//
//  Created by Henry Zeng on 2021/9/30.
//

#ifndef Database_h
#define Database_h
#include <cstdint>
#include <tuple>
#include <iostream>
#include <vector>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>
#include <mongocxx/exception/bulk_write_exception.hpp>
#include <bsoncxx/exception/exception.hpp>
#include <mongocxx/exception/logic_error.hpp>
#include <mongocxx/exception/query_exception.hpp>


using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;
using std::string;
using std::cout;
using std::endl;
typedef bsoncxx::stdx::optional<mongocxx::result::insert_one> insert_one_result;
typedef bsoncxx::stdx::optional<mongocxx::result::insert_many> insert_many_result;
typedef bsoncxx::stdx::optional<mongocxx::result::update> update_result;
typedef bsoncxx::stdx::optional<mongocxx::result::delete_result> delete_result;

namespace databaseProper {

const string defaultDatabaseAddr = "mongodb://localhost:27017";
const string defaultDatabaseName = "AirTicketSystem";
const string flight_info_conn = "flight_info";
const string auth_info_conn = "auth_info";
const string user_info_conn = "user_info";
};




///
/// 与服务器交互，并且操作数据库的类
///
///
/// Databases 通过 @c insertOne, @c insertMany,
/// @c updateOne , @c updateMany, @c deleteOne, @c deleteMany, @c readOne,
/// @c readMany 操作服务器的数据库数据
///
class Database {
    
public:
    ///
    /// 默认构造函数， 创建一个 Database 的实例
    ///
    Database() noexcept;
    
    ///
    /// 默认析构函数， 彻底删除一个 Database 实例前调用
    ///
    ~Database() noexcept {};
    
    /*数据类型*/
    enum InfoType{
        User_Info,      //用户数据
        Flight_Info,    //航班数据
        Auth_Info       //实名认证数据
    };
    
    bool insertOne(const InfoType &info_type, string json);
    bool insertMany(const InfoType &info_type, std::vector<string> docs);
    bool updateOne(const InfoType &info_type, string filter, string json);
    bool deleteOne(const InfoType &info_type, string filter);
    bool deleteMany(const InfoType &info_type, std::vector<string> filter);
    string readOne(const InfoType &info_type, string filter);
    std::vector<string> readMany(const InfoType &info_type, string filter);
    
    
    
private:

    //数据库客户端实例
    mongocxx::client db_client {mongocxx::uri{databaseProper::defaultDatabaseAddr}};
    //航班数据集合
    mongocxx::collection flight_conn;
    //用户数据集合
    mongocxx::collection user_conn;
    //认证数据集合
    mongocxx::collection auth_conn;
    
    mongocxx::cursor findAllDoc(mongocxx::collection &conn);
    
    //打印结果
    void printResult(insert_one_result &result) const noexcept;
    void printResult(update_result &result) const noexcept;
    void printResult(insert_many_result &result) const noexcept;
    void printResult(delete_result &result) const noexcept;
    //打印 exception
    void printException(string failed_string, mongocxx::exception &err) const;
    void printException(string failed_string, mongocxx::logic_error &err) const;
    void printException(string failed_string, mongocxx::bulk_write_exception &err) const;
    void printException(string failed_string, bsoncxx::exception &err) const;
    void printException(string failed_string, mongocxx::query_exception &err) const;
};

#endif /* Database_h */
