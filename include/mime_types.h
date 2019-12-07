#include <iostream>
#include <sstream>
#include <fstream>
#include <exception>
#include <mutex>
#include <thread>

#include <string>
#include <string_view>
#include <unordered_map>

#include "rapidjson/document.h"

namespace pmc{

    struct mimetypes{
        rapidjson::Document document;
        std::unordered_map<std::string, std::string> map;
        std::mutex m;
        
        mimetypes(std::string jsonPath){
            std::ifstream txt(jsonPath);
            std::vector<char> json((std::istreambuf_iterator<char>(txt)), std::istreambuf_iterator<char>());
            if(document.Parse(&json[0]).HasParseError())throw std::exception();
            std::thread t([&]{
            for (rapidjson::Value::ConstMemberIterator itr = document.MemberBegin();itr != document.MemberEnd(); ++itr)
            {
                rapidjson::Value::ConstMemberIterator itr2 = itr->value.FindMember("extensions");
                if (itr2 != document.MemberEnd() && itr2->value.IsArray()){
                    for (rapidjson::SizeType i = 0; i < itr2->value.Size(); i++){
                        const std::lock_guard<std::mutex> lock(m);
                        map[std::string(itr2->value[i].GetString())]=std::string(itr->name.GetString());
                    }
                    std::this_thread::yield();
                }
            }
                
            });
            t.detach();
        };
        
        std::string_view lookup(std::string_view file){
            size_t index=file.find_last_of('.');
            if(index!=std::string::npos)file=file.substr(index+1);
            //std::cout<<"mimetypes "<<file<<std::endl;
            std::string key(file.begin(), file.end());
            if(map.count(key)){
                const std::lock_guard<std::mutex> lock(m);
                return std::string_view(map[key]);
            }
            else{
                for (rapidjson::Value::ConstMemberIterator itr = document.MemberBegin();itr != document.MemberEnd(); ++itr)
                {
                    rapidjson::Value::ConstMemberIterator itr2 = itr->value.FindMember("extensions");
                    if (itr2 != document.MemberEnd() && itr2->value.IsArray()){
                        for (rapidjson::SizeType i = 0; i < itr2->value.Size(); i++){
                            if(key.compare(itr2->value[i].GetString())==0){
                                //std::cout<<"mime "<<itr->name.GetString()<<std::endl;
                                {
                                    const std::lock_guard<std::mutex> lock(m);
                                    map[key]=std::string(itr->name.GetString());
                                }
                                return std::string_view(map[key]);
                            }
                        }
                    }
                }
                return std::string_view("");
            }
        }
        
        std::string_view extension(std::string_view type){
            const rapidjson::Value& entry = document[type.data()];
            const rapidjson::Value& extensions = entry["extensions"];
            if(extensions.IsArray() && extensions.Size()>0){
                return std::string_view(std::string(extensions[0].GetString()));
            }
            else return std::string_view("");
        }
        
        std::string_view charset(std::string_view type){
            const rapidjson::Value& entry = document[type.data()];
            const rapidjson::Value& charset = entry["charset"];
            if(charset.IsString()){
                return std::string_view(std::string(charset.GetString()));
            }
            else return std::string_view("");
        }
        
        
    };
}
