#include "BaseCollider.h"

CBaseCollider::CBaseCollider(LPDIRECT3DDEVICE9 pGraphicDev, ColliderType colliderType) :
	CComponent(pGraphicDev), m_eColliderType(colliderType)
{
}

CBaseCollider::CBaseCollider(const CBaseCollider& rhs) :
	CComponent(rhs), m_eColliderType(rhs.m_eColliderType)
{
}

CBaseCollider::~CBaseCollider()
{
}

void CBaseCollider::Free()
{
	CComponent::Free();
}
