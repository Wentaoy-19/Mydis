#include "skiplist_wrapper.h"

/*
    Map from instruction op string to enumerate type
*/
std::map<std::string, INSTRUCTION> INS_MAP = {
    {"set", SET},
    {"get", GET},
    {"del", DEL}
};

SkipList_Wrapper::SkipList_Wrapper(int level):_levels(level),_skipList(SkipList<std::string,std::string>(level)){}

SkipList_Wrapper::~SkipList_Wrapper(){};

std::vector<std::string> SkipList_Wrapper::slice(std::string text, std::string delimiter){
    std::vector<std::string> ret;
    std::string _str = text + delimiter;
    while(1){
        size_t pos = _str.find(delimiter);
        if(pos == _str.npos) break;
        std::string tmp = _str.substr(0,pos);
        ret.push_back(tmp);
        _str = _str.substr(pos+1,_str.size());
    }
    return ret;
}

void SkipList_Wrapper::to_lower(std::string& str){
    std::transform(str.begin(),str.end(),str.begin(),::tolower);
};

query_pkg SkipList_Wrapper::parse(std::string query){
    std::vector<std::string> str_list = this->slice(query," ");
    if(str_list.size()!=3) return query_pkg(ERR,"-1","");
    std::string instruction = str_list[0];
    std::string key_str = str_list[1];
    std::string val = str_list[2];

    // change instruction to lower case
    this->to_lower(instruction);

    // create map iterator
    std::map<std::string, INSTRUCTION>::iterator iter = INS_MAP.find(instruction);

    // Not found in the map, return end 
    if(iter == INS_MAP.end()) return query_pkg(ERR,"-1","");

    // found, get the instruction enumerate value 
    INSTRUCTION ins = iter->second;

    std::string key = key_str;

    // if nothing fail, all fail case pass, return pkg
    return query_pkg(ins, key, val); 
};

exec_ret_pkg SkipList_Wrapper::execute_query(query_pkg q_pkg){

    // If query is invalid, return fail
    if(q_pkg.get_ins() == ERR) return exec_ret_pkg(EXEC_FAIL,"INVALID");
    std::string ret_buf;

    if(q_pkg.get_ins() == SET){
        int tmp = _skipList.insert_element(q_pkg.get_k(),q_pkg.get_v());
        if(tmp == 1) {printf("[MYDIS] SET FAIL\n");return exec_ret_pkg(EXEC_FAIL,"SET");}
        printf("[MYDIS] SET SUCCESS\n");
        return exec_ret_pkg(EXEC_SUCCESS,"SET");
    } else if(q_pkg.get_ins() == GET){
        std::string* ret_buf = _skipList.get_element(q_pkg.get_k());
        if(ret_buf == NULL) {printf("[MYDIS] GET FAIL\n");return exec_ret_pkg(EXEC_FAIL, "GET");}
        printf("[MYDIS] GET SUCCESS\n");
        return exec_ret_pkg(EXEC_SUCCESS, *ret_buf);
    } else if(q_pkg.get_ins() == DEL){
        _skipList.delete_element(q_pkg.get_k());
        printf("[MYDIS] DEL SUCCESS\n");
        return exec_ret_pkg(EXEC_SUCCESS, "DEL");
    } else{
        printf("[MYDIS] INSTRUCTION INVALID\n");
        return exec_ret_pkg(EXEC_FAIL,"INVALID");
    }

};


exec_ret_pkg SkipList_Wrapper::parse_and_execute(std::string query){
    query_pkg qpkg = this->parse(query);
    return this->execute_query(qpkg);
};