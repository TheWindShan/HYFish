//
//  IOSiAP_Helper.hpp
//  MyGame
//
//  Created by appple on 16/5/28.
//
//

#ifndef IOSiAP_Helper_h
#define IOSiAP_Helper_h

#include <stdio.h>
#include "cocos2d.h"
#include <map>
#include "stdio.h"


using namespace std;

USING_NS_CC;

class IOSiAP_Helper{
    
public:
    
    std::string makeJsonData(map<std::string,std::string>  &params);
    
    map<std::string,std::string>  analysisJsonData(std::string &jsonStr);
    
    
    
};


#endif /* IOSiAP_Helper_h */
