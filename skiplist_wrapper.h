#pragma once
#include <iostream> 
#include <vector>
#include <algorithm>
#include <map>
#include <sstream>
#include "skiplist.cpp"

/*
    Instruction Type for Query
*/
typedef enum INSTRUCTION {
    SET, GET, DEL, ERR
} INSTRUCTION;

/*
    Query execution return type
*/
typedef enum EXEC_RET{
    EXEC_SUCCESS, EXEC_FAIL
}EXEC_RET;


/*
    Query Package Type
*/
class query_pkg{
    private:
        INSTRUCTION _ins;
        std::string _k;
        std::string _v;
    public:
        query_pkg(INSTRUCTION ins, std::string k, std::string v):_ins(ins),_k(k),_v(v){};
        INSTRUCTION get_ins(){return _ins;};
        std::string get_k(){return _k;};
        std::string get_v(){return _v;};
        ~query_pkg(){}; 
};

/*
    Return package of query execution result for server
*/
class exec_ret_pkg{
    public:
        EXEC_RET _ret_type;
        std::string _buffer;
        exec_ret_pkg(EXEC_RET ret_type, std::string buffer):_ret_type(ret_type),_buffer(buffer){};
        ~exec_ret_pkg(){};
};  

/*
    Wrapper class for Skiplist
*/
class SkipList_Wrapper{
    private:
        /* Skiplist Object */ 
        SkipList<std::string, std::string> _skipList;

        /* Total Level for the SkipList */ 
        int _levels;

    public:
        SkipList_Wrapper(int level=6);
        ~SkipList_Wrapper();

        /* String Slice Function*/
        std::vector<std::string> slice(std::string text, std::string delimiter);

        /* Change char in string to lower case */
        void to_lower(std::string& str);

        /* Parse the input string to query package */
        query_pkg parse(std::string query);

        /* execute query */
        exec_ret_pkg execute_query(query_pkg q_pkg);

        exec_ret_pkg parse_and_execute(std::string query);

};