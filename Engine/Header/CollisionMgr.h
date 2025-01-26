#pragma once
#include "Base.h"
#include "Engine_Define.h"


BEGIN(Engine)

class Tree;
class CGameObject;

class ENGINE_DLL CCollisionMgr :
    public CBase
{
    DECLARE_SINGLETON(CCollisionMgr)

public:
    HRESULT Ready_CollisionMgr();
    void LateUpte_CollisionMgr();

    unsigned int Add_Collider(CGameObject* pGameObject);

    HRESULT Remove_Collider(unsigned int particleIdx);


private:
    void minimumImage(D3DXVECTOR3& separation,
        const vector<bool>& periodicity, const D3DXVECTOR3& boxSize);

    bool overlaps(D3DXVECTOR3& position1, D3DXVECTOR3& position2,
        const std::vector<bool>& periodicity, const D3DXVECTOR3& boxSize, double cutOff);

private:
    explicit CCollisionMgr();
    virtual ~CCollisionMgr();

private:
    virtual void	Free();


private:
    Tree* _tree;
    unsigned int _particleIdx;
    map<unsigned int, CGameObject*>			_mapObjs;
    std::vector<bool> _periodicity;
    
};

END