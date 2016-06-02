//
//  SocketRecvData.h
//  BeefView
//
//  Created by wh on 13-5-31.
//
//

#ifndef BeefView_SocketRecvData_h
#define BeefView_SocketRecvData_h

#include "cocos2d.h"

enum NetDataType
{
    Send_Data = 0,
    Recv_Data,
};

class RecvData : public cocos2d::Ref
{
public:
    RecvData(void* pData, WORD wSize)
    :m_dataType(Recv_Data)
    {
        m_data = new char[wSize + 1];
        memset(m_data, 0, wSize + 1);
        memcpy(m_data, pData, wSize);
        m_datasize = wSize;
    }
    
    ~RecvData()
    {
        delete []m_data;
    }
    
    WORD getDatasize()
    {
        //return m_recvData.size();
        return m_datasize;
    }
    
    char *getRecvData()
    {
        return m_data;
    }
    
    
protected:
    char *m_data;//接受到的数据
    WORD m_datasize;//数据长度
    
    CC_SYNTHESIZE(bool, m_IsInBack, IsInBack)//是否是退到后台接受的数据
    
    CC_SYNTHESIZE(NetDataType, m_dataType , dataType)
    
};

#endif
