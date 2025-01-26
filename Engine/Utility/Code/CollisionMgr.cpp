#include "CollisionMgr.h"
#include "DynamicAABBTree.h"
#include "GameObject.h"

IMPLEMENT_SINGLETON(CCollisionMgr)



CCollisionMgr::CCollisionMgr(): _tree(nullptr), _particleIdx(0)
{
	if (Ready_CollisionMgr() == E_FAIL)
	{
		Safe_Release(m_pInstance);
		MSG_BOX("CollisionMgr Create Failed");
	}
}

CCollisionMgr::~CCollisionMgr()
{
	Free();
}

HRESULT CCollisionMgr::Ready_CollisionMgr()
{
	double maxDisp = 0.1;
	double density = 0.1;
	unsigned int nSmall = 1000;         // The number of small particles.
	unsigned int nLarge = 100;          // The number of large particles.
	double diameterSmall = 0.1;           // The diameter of the small particles.
	double diameterLarge = 3;          // The diameter of the large particles.

	_periodicity = { true,true,true };

	//double baseLength = std::pow((D3DX_PI * (nSmall * diameterSmall + nLarge * diameterLarge)) / (4.0 * density), 1.0 / 2.0);
	_vec3 boxSize({ (_float)1.f, (_float)1.f, (_float)1.f });

	_tree = DBG_NEW Tree(maxDisp, _periodicity,boxSize,nLarge);


	return S_OK;
}
unsigned int CCollisionMgr::Add_Collider(CGameObject* pGameObject)
{
	_tree->insertParticle(_particleIdx, pGameObject->Get_Pos(), 1.0);
	_mapObjs.emplace(std::pair<unsigned int, CGameObject*>(_particleIdx, pGameObject));
	int particleIdx = _particleIdx;
	_particleIdx++;
	return particleIdx;
}

HRESULT CCollisionMgr::Remove_Collider(unsigned int particleIdx)
{
	_mapObjs.erase(particleIdx);		//오류있는지 확인하기
	_tree->removeParticle(particleIdx);
	return S_OK;
}

void CCollisionMgr::minimumImage(D3DXVECTOR3& separation, const vector<bool>& periodicity, const D3DXVECTOR3& boxSize)
{
	for (unsigned int i = 0; i < 3; i++)
	{
		if (separation[i] < -0.5 * boxSize[i])
		{
			separation[i] += periodicity[i] * boxSize[i];
		}
		else
		{
			if (separation[i] >= 0.5 * boxSize[i])
			{
				separation[i] -= periodicity[i] * boxSize[i];
			}
		}
	}
}

bool CCollisionMgr::overlaps(D3DXVECTOR3& position1, D3DXVECTOR3& position2, const std::vector<bool>& periodicity, const D3DXVECTOR3& boxSize, double cutOff)
{
	// Calculate particle separation.
	D3DXVECTOR3 separation;
	separation.x = (position1[0] - position2[0]);
	separation.y = (position1[1] - position2[1]);
	separation.z = (position1[2] - position2[2]);

	// Calculate minimum image separation.
	minimumImage(separation, periodicity, boxSize);

	double rSqd = separation[0] * separation[0] + separation[1] * separation[1] + separation[2] * separation[2];

	if (rSqd < cutOff) return true;
	else return false;
}




void CCollisionMgr::LateUpte_CollisionMgr()
{
	if (_particleIdx == 0)
		return;

	if (_mapObjs.size() != _tree->nParticles())
	{
		MSG_BOX("list and particles are differnt size");
		return;
	}

	for (auto& iter : _mapObjs)
	{
		_tree->updateParticle(iter.first, iter.second->Get_Pos(), 1.0, true);
	}


	for (auto& iter : _mapObjs)
	{
		D3DXVECTOR3 vPos = iter.second->Get_Pos();
		_vec3 lowerBound = vPos - _vec3(1.f, 1.f, 1.f);
		_vec3 upperBound = vPos + _vec3(1.f, 1.f, 1.f);


		AABB aabb(lowerBound, upperBound);
		std::vector<unsigned int> particles = _tree->query(aabb);

		double cutoff = 1.f;

		for (int i = 0; i < particles.size(); i++)
		{
			if (particles[i] != iter.first)
			{
				if (overlaps(vPos, _mapObjs[particles[i]]->Get_Pos(), _periodicity, D3DXVECTOR3(1.f, 1.f, 1.f), 1))
				{
					float a = 1.f;
				}
			}
		}
	}




}

void CCollisionMgr::Free()
{
	_tree->removeAll();
	delete _tree;
}
