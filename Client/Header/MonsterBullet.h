#pragma once

#include "Base.h"
#include "GameObject.h"
#include "Player.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CMonsterBullet : public Engine::CGameObject
{
protected:
	explicit CMonsterBullet(LPDIRECT3DDEVICE9 pGraphicDev, BULLETID eBulletID, const _vec3& vFrom, const _vec3& vTo);
	explicit CMonsterBullet(const CMonsterBullet& rhs);
	virtual ~CMonsterBullet();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 override;
public:

	bool						Get_Dead() { return m_bDead; }
protected:
	virtual  HRESULT			Add_Component();
	virtual void				Motion_Change();	//상태 변하면 텍스처 바꿔주는 함수

	_float						DistanceToPlayer(CPlayer* pPlayer); // 플레이어 위치 == 카메라 Eye거리 계산

	//밑 두 함수는 업뎃에서 사용
	void						Chase_Player(CPlayer* pPlayer, const _float& fTimeDelta); //플레이어 추적
	void						Knock_Back(CPlayer* pPlayer, //넉백 : 플레이어 반대 방향으로 밀려남
		const _float& fSpeed, //밀려나는 거리
		const _float& fTimeDelta);

protected:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

protected:
	CPlayer* m_pPlayer;

	BULLETID			m_eBulletID;
	_vec3				m_vFrom;
	_vec3				m_vTo;

	_tchar* m_szTexName[128];	//텍스처 이름

	_float				m_fFrame = 0.f;
	_uint				m_iMaxFrame = 1;
	_float				m_fFrameSpeed = 0.f;

	_int		m_iDamage;
	_float		m_fSpeed;
	_float		m_fAtkRng;

	bool		m_bDead;
	bool		m_bInit;
	bool		m_bHit;
public:
	static CMonsterBullet* Create(LPDIRECT3DDEVICE9	pGraphicDev,  BULLETID eBulletID, const _vec3& vFrom, const _vec3& vTo);

public:
	virtual void Free() override;
};

