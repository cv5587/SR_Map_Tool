#pragma once

#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CCalculator;

END

class CProp : public Engine::CGameObject
{
protected:
	explicit CProp(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CProp(const CProp& rhs);
	virtual ~CProp();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 override;

public:
	static CProp* Create(LPDIRECT3DDEVICE9	pGraphicDev);

protected:
	virtual void				Free() override;
	virtual  HRESULT			Add_Component();
	virtual void				TexNamer(); //한 오브젝트가 서로 다른 텍스처 갖는 경우 Add_Component 맨 위에 추가

protected:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

	//TODO : Prop에 들어가야할 함수
	//텍스처 관련 함수
	//렌더 관련 함수

	_float				m_fFrame = 0.f;
	_float				m_fMaxFrame = 0.f;

	std::wstring m_szTexName;	//텍스처 이름

};

