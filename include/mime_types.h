#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include <exception>
#include <atomic>
#include <mutex>
#include <thread>

#include <string>
#include <string_view>
#include <unordered_map>

#include "io/json/Binder.h"

namespace pmc{

    struct mimetypes{
        sylvanmats::io::json::Binder jsonBinder;
        std::unordered_map<std::string, std::string> map;
        std::atomic_flag lock = ATOMIC_FLAG_INIT;
        std::mutex m;
        std::thread t;
        
        mimetypes(std::string jsonPath){
            std::ifstream txt(jsonPath);
            std::string jsonContent((std::istreambuf_iterator<char>(txt)), std::istreambuf_iterator<char>());
            jsonBinder(jsonContent);
            sylvanmats::io::json::Path jp="*"_jp;
            t=std::thread([&]{
                std::vector<std::string> mimeNames;
                jsonBinder(jp, [&](std::string_view& key, std::any& v){
//                    std::cout<<key<<std::endl;
                    mimeNames.push_back(std::string(key.begin(), key.end()));
                    if(!lock.test_and_set(std::memory_order_acquire)) return;
                    std::this_thread::yield();
                });
                for(std::vector<std::string>::iterator it=mimeNames.begin();it!=mimeNames.end();++it){
                    std::string jp2Str=(*it);
                    sylvanmats::io::json::Path jp2;
                    jp2/jp2Str.c_str()/"extensions";
                    jsonBinder(jp2, [&](std::string_view& key, std::any& v){
//                        std::cout<<(*it)<<" "<<key<<std::endl;
                        const std::lock_guard<std::mutex> lock(m);
                        map[std::string(key.begin(), key.end())]=(*it);
                    });
                    if(!lock.test_and_set(std::memory_order_acquire)) return;
                    std::this_thread::yield();
                }
            });
            t.detach();
        };
        
        virtual ~mimetypes(){
            lock.clear(std::memory_order_release);
        }
        
        std::string_view lookup(std::string_view file){
            size_t index=file.find_last_of('.');
            if(index!=std::string::npos)file=file.substr(index+1);
            //std::cout<<"mimetypes "<<file<<std::endl;
            std::string key(file.begin(), file.end());
            const std::lock_guard<std::mutex> lock(m);
            if(map.count(key)){
                return std::string_view(map[key]);
            }
            else{
                sylvanmats::io::json::Path jp="*"_jp;
                std::vector<std::string> mimeNames;
                jsonBinder(jp, [&](std::string_view& key, std::any& v){
//                    std::cout<<key<<std::endl;
                    mimeNames.push_back(std::string(key.begin(), key.end()));
                });
                for(std::vector<std::string>::iterator it=mimeNames.begin();it!=mimeNames.end();++it){
                    std::string jp2Str=(*it);
                    sylvanmats::io::json::Path jp2;
                    jp2/jp2Str.c_str()/"extensions";
                    jsonBinder(jp2, [&](std::string_view& key2, std::any& v){
//                        std::cout<<(*it)<<" "<<key<<std::endl;
                        if(key.compare(key2)==0){
                            map[std::string(key2.begin(), key2.end())]=(*it);
                        }
                    });
                    if(map.count(key))
                        return std::string_view(map[key]);
                }
                
                return std::string_view("");
            }
        }
        
        std::string_view extension(std::string_view type){
            std::string jp2Str(type.begin(), type.end());
            sylvanmats::io::json::Path jp2;
            jp2/jp2Str.c_str()/"extensions";
            std::string_view key("");
            jsonBinder(jp2, [&](std::string_view& key2, std::any& v){
                if(key.empty())key=key2;
            });
            return key;
        }
        
        std::string_view charset(std::string_view type){
            std::string jp2Str(type.begin(), type.end());
            sylvanmats::io::json::Path jp2;
            jp2/jp2Str.c_str()/"charset";
            std::string_view key("");
            jsonBinder(jp2, [&](std::string_view& key2, std::any& v){
                if(key.empty())key=key2;
            });
            return key;
        }
        
        
    };
}
