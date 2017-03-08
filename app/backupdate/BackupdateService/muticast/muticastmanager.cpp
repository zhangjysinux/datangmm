#include "muticastmanager.h"
#include "MutiCast.h"
MuticastManager::MuticastManager()
{
    //m_instance = this;
}

MuticastManager::~MuticastManager()
{

    if(m_mapMutiCasts.empty())
        return;
    std::map<int,MutiCastGroup>::iterator itmap = m_mapMutiCasts.begin();
    for(;itmap != m_mapMutiCasts.end();++itmap)
    {

        if(itmap->second.m_pMutiCast)
            delete itmap->second.m_pMutiCast;
    }
    m_mapMutiCasts.clear();
}

MuticastManager *MuticastManager::m_instance = 0;

MuticastManager &MuticastManager::instance()
{
    if(m_instance == 0)
    {
        m_instance = new MuticastManager();
    }
    return *m_instance;
}

int MuticastManager::getNewId()
{
    int groupId = m_mapMutiCasts.size()+1;
    return groupId;
}
int MuticastManager::createMuticastGroup(int groupId,MutiCast *pMutiCast)
{
    addMuticast(groupId,pMutiCast);
    pMutiCast->setGroupId(groupId);
    return groupId;
}

void MuticastManager::addMuticast(int groupId,MutiCast *pMutiCast)
{
    MutiCastGroup mutiCastGroup;
    mutiCastGroup.groupId = groupId;
    mutiCastGroup.m_pMutiCast = pMutiCast;
    m_mapMutiCasts.insert(std::make_pair(groupId,mutiCastGroup));
}

bool MuticastManager::getMutiCastGroup(int groupId,MutiCastGroup & mutiCastGroup)
{
    if(m_mapMutiCasts.empty())
    {
        return false;
    }
    std::map<int,MutiCastGroup>::iterator itmap = m_mapMutiCasts.find(groupId);
    if(itmap != m_mapMutiCasts.end())
    {

        mutiCastGroup = itmap->second;
        return true;
    }
    return false;
}

bool MuticastManager::removeMutiCastGroup(int groupId)
{
    if(m_mapMutiCasts.empty())
    {
        return false;
    }
    bool bIsRemove = false;
    std::map<int,MutiCastGroup> newMap;
    std::map<int,MutiCastGroup>::iterator itmap = m_mapMutiCasts.begin();
    for(;itmap != m_mapMutiCasts.end();++itmap)
    {
        if(itmap->first == groupId)
        {
            bIsRemove = true;
        }else
        {
            newMap.insert(std::make_pair(itmap->first,itmap->second));
        }
    }
    m_mapMutiCasts.clear();
    m_mapMutiCasts = newMap;
    return bIsRemove;
}
