//
//  IOSiAP.hpp
//  MyGame
//
//  Created by appple on 16/5/25.
//
//

#ifndef IOSiAP_h
#define IOSiAP_h

#include <stdio.h>


#include <iostream>
#include <vector>

#include "IOSiAP_Define.h"


class IOSiAP
{
public:
    IOSiAP();
    ~IOSiAP();
    void requestProducts(std::vector <std::string> &productIdentifiers);
    IOSProduct *iOSProductByIdentifier(std::string &identifier);
    void paymentWithProduct(IOSProduct *iosProduct, int quantity = 1);
    bool canPayment();
    
    IOSiAPDelegate *delegate;
    void *skProducts;
    void *skTransactionObserver;
    std::vector<IOSProduct *> iOSProducts;
};



#endif /* IOSiAP_hpp */
