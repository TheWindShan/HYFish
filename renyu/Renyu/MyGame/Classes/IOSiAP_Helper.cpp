//
//  IOSiAP_Helper.cpp
//  MyGame
//
//  Created by appple on 16/5/28.
//
//

#include "IOSiAP_Helper.h"


#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include <map>
#include<iterator>
#include <iostream>
using namespace  rapidjson;
//std::string IOSiAP_Helper::makeJsonData(Map<std::string, std::string> params){
std::string IOSiAP_Helper::makeJsonData(map<std::string,std::string>  &params){
    
    
    if(!params.empty()){
        CCLOG("组装json");
        
        rapidjson::Document doc;
        doc.SetObject();
        rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
        
        map<string,string>::iterator iter = params.begin();
        //rapidjson::Value storage_key(kStringType);
        //rapidjson::Value storage_value(kStringType);
        while(iter != params.end()){
            cout<<iter->first<<endl;
            cout<<iter->second<<endl;
            
            doc.AddMember(iter->first.c_str(),iter->second.c_str(), allocator);
            iter++;
        }
        
        
        
        StringBuffer buf;
        rapidjson::Writer<StringBuffer> writer(buf);
        
        doc.Accept(writer);
        
        CCLOG("生成的json字符串: %s",buf.GetString());
        
        return std::string(buf.GetString());
    }else{
        CCLOG("map为空");
    }
    
    return nullptr;
}


map<std::string,std::string> IOSiAP_Helper::analysisJsonData(std::string &jsonStr){
    
    
    CCLOG("json字符串： %s",jsonStr.c_str());
    
    rapidjson::Document doc;
    doc.Parse<0>(jsonStr.c_str());
    
    if(doc.HasParseError()){
        CCLOG("json解析错误:  %s\n",doc.GetParseError());
    }
    Document::MemberIterator iter = doc.MemberonBegin();
    
    map<std::string,std::string> mapJson;
    while(iter != doc.MemberonEnd()){
        
        CCLOG("%s --  %s",iter->name.GetString(),iter->value.GetString());
        mapJson.insert(pair<string,string>(iter->name.GetString(),iter->value.GetString()));
        ++iter;
    }
    CCLOG("分割线------------");
    map<std::string,std::string>::iterator iter2 = mapJson.begin();
    //rapidjson::Value storage_key(kStringType);
    //rapidjson::Value storage_value(kStringType);
    while(iter2 != mapJson.end()){
        cout<<iter2->first<<endl;
        cout<<iter2->second<<endl;
        iter++;
    }
    
    return mapJson;
}
