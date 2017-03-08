#ifndef _CONNECT_COUNT_H_
#define _CONNECT_COUNT_H_
#define CHECK_NUM 5
#include <deque>
class ConnectCount
{
public:
    ConnectCount();
    ~ConnectCount();
    bool getIsConnect();
    void addCount();
    void update();
private:
    void checkIsConnect();
    bool m_bIsConnect;
    int m_ackCount;
    std::deque<int> m_dequeNum ;
};
#endif
