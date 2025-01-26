#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTexture : public CComponent
{
protected:
	explicit CTexture(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTexture(const CTexture& rhs);
	virtual ~CTexture();

public:
	HRESULT		Ready_Texture(TEXTUREID eType, const _tchar* pPath, const _uint& iCnt);
	HRESULT CTexture::Ready_Texture(TEXTUREID eType, const _tchar* pPath, const _uint& iCnt, const _uint& mulWidth, const _uint& mulHeight);
	void		Set_Texture(const _uint& iIndex = 0);
	_uint* Get_Texture() { return &textureIndex; }
	vector<IDirect3DBaseTexture9*> Get_TextureVec() { return m_vecTexture; }
private:
	vector<IDirect3DBaseTexture9*>		m_vecTexture;
	_uint textureIndex;
public:
	virtual CComponent*	Clone();
	static CTexture*	Create(LPDIRECT3DDEVICE9 pGraphicDev, TEXTUREID eType, const _tchar* pPath, const _uint& iCnt = 1);

public:
	virtual void		Free();

};

END