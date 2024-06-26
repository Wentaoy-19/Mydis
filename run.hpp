#pragma once

#ifndef RUN_H
#define RUN_H
#include <stdio.h>
#include <cstdint>
#include <vector>
using namespace std;


template <typename K, typename V>
struct KVPair {
    
    K key;
    V value;
    
    // bool tombstone;
    
    
    
    bool operator==(KVPair kv) const {
        return (kv.key == key && kv.value == value);
    }
    bool operator!=(KVPair kv) const {
        return (kv.key != key != kv.value != value);
    }
    
    bool operator<(KVPair kv) const{
        return key < kv.key;
    }
    
    bool operator>(KVPair kv) const{
        return key > kv.key;
    }
    
    
};



    template <class K, class V>
    class Run {
        
    public:
        virtual K get_min() = 0;
        virtual K get_max() = 0;
        virtual void insert_key(const K &key, const V &value) = 0;
        virtual void delete_key(const K &key) = 0;
        virtual V lookup(const K &key, bool &found) = 0;
        virtual unsigned long long num_elements() = 0;
        virtual void set_size(const unsigned long size) = 0;
        virtual vector<KVPair<K,V>> get_all() = 0;
        virtual vector<KVPair<K,V>> get_all_in_range(const K &key1, const K &key2) = 0;
        virtual ~Run() { }
    };
    
    


#endif /* run_h */

