#include "connectcount.h"
ConnectCount::ConnectCount()
{
    m_ackCount = 0;
    m_bIsConnect = true;
}

ConnectCount::~ConnectCount()
{

}

bool ConnectCount::getIsConnect()
{
    return m_bIsConnect;
}

void ConnectCount::addCount()
{
    m_ackCount++;
}

void ConnectCount::update()
{
    m_dequeNum.push_back(m_ackCount);
    if(m_dequeNum.size()>CHECK_NUM)
    {
        m_dequeNum.pop_front();
    }
    checkIsConnect();
}

void ConnectCount::checkIsConnect()
{
    if(m_dequeNum.size() >= CHECK_NUM)
    {
        std::deque<int>::iterator itdeq = m_dequeNum.begin();
        int beginNum = *itdeq;
        itdeq++;
        for(;itdeq!=m_dequeNum.end();++itdeq)
        {
            if(beginNum!=*itdeq)
            {
                m_bIsConnect = true;
                return;
            }
        }
        m_bIsConnect = false;
    }else
    {
        m_bIsConnect = true;
    }
}
