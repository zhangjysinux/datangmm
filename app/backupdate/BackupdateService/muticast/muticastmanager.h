#ifndef _MUTICAST_MANAGER_H_
#define _MUTICAST_MANAGER_H_
#include <vector>
#include <map>
class MutiCast;
struct MutiCastGroup
{
    int groupId;
    MutiCast *m_pMutiCast;
    MutiCastGroup& operator = (const MutiCastGroup &t)
    {
        groupId=t.groupId;
        m_pMutiCast=t.m_pMutiCast;
        return *this;
    }
};

class MuticastManager
{
public:
    static MuticastManager &instance();
    MuticastManager();
    ~MuticastManager();
    int getNewId();
    int createMuticastGroup(int groupId,MutiCast *pMutiCast);
    bool getMutiCastGroup(int groupId,MutiCastGroup & mutiCastGroup);
    bool removeMutiCastGroup(int groupId);
private:
    void addMuticast(int groupId,MutiCast *pMutiCast);
    std::map<int,MutiCastGroup> m_mapMutiCasts;
   static MuticastManager *m_instance;
};
#endif
