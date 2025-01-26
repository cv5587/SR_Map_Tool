#include "stdafx.h"
#include "Thingy.h"

CThingy::CThingy(LPDIRECT3DDEVICE9 pGraphicDev)
    : CMonster(pGraphicDev), m_iBulletCount(0)
{
}
CThingy::CThingy(const CThingy& rhs)
    : CMonster(rhs), m_iBulletCount(rhs.m_iBulletCount)
{
}

CThingy::~CThingy()
{
}

HRESULT CThingy::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    m_iMaxHealth = m_iCurHealth = 50;
    m_iDamage = 5;
    m_fSpeed = 1.f;
    m_fDtctRng = 8.f;
    m_fAtkRng = 1.5f;
    m_bInit = false;
    m_bHit = false;
    m_bAtk = false;
    m_fAtkTimer = 0.f;
    m_fStiffTimer = 0.f;
    m_fStiffSecond = 2.f;
    m_pTransformCom->Set_Pos(-5.f, 1.5f, 0.f);

    m_eThingyState = THINGY_IDLE;


    //충돌처리해줘야댐

    return S_OK;
}

_int CThingy::Update_GameObject(const _float& fTimeDelta)
{
    CGameObject::Update_GameObject(fTimeDelta);

    Engine::Add_RenderGroup(RENDER_ALPHA, this);

    m_fFrame += _float(m_fFrameSpeed) * fTimeDelta;

    //if (m_bHit)
    //{
    //    Thingy_Take_Dmg(fTimeDelta);
    //}

    //if (m_fFrame > _float(m_iMaxFrame) && (m_eThingyState == THINGY_ATK1 || m_eThingyState == THINGY_ATK2 || m_eThingyState == THINGY_MAGIC))
    //{
    //    m_bAtk = true;
    //    m_fFrame = 0.f;
    //}
    //else if (m_fFrame > _float(m_iMaxFrame) && m_eThingyState != THINGY_DEAD)
    //    m_fFrame = 0.f;


    //switch (m_eThingyState)
    //{
    //case THINGY_IDLE:
    //{
    //    if (!m_bInit)
    //    {
    //        m_pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"GameLogic", L"Player"));
    //        m_bInit = true;
    //    }

    //    if (m_fDtctRng > DistanceToPlayer(m_pPlayer))
    //    {
    //        wstring szSoundFile;
    //        szSoundFile = L"the_thing_detect.wav";

    //        m_eThingyState = THINGY_RUN;
    //    }
    //}
    //break;

    //case THINGY_RUN:
    //{
    //    if (m_fAtkRng > DistanceToPlayer(m_pPlayer))
    //    {
    //        if (rand() % 2 == 1)
    //        {
    //            m_eThingyState = THINGY_ATK1;
    //        }
    //        else
    //        {
    //            m_eThingyState = THINGY_ATK2;
    //        }

    //    }
    //    else if (8.5f > DistanceToPlayer(m_pPlayer) &&
    //        6.5f < DistanceToPlayer(m_pPlayer))
    //    {
    //        m_eThingyState = THINGY_MAGIC;
    //    }
    //    else
    //    {
    //        Chase_Player(m_pPlayer, fTimeDelta);
    //    }
    //}
    //break;
    //case THINGY_ATK1:
    //{
    //    if (m_fAtkRng < DistanceToPlayer(m_pPlayer))
    //    {
    //        m_eThingyState = THINGY_RUN;
    //    }
    //    else
    //    {
    //        //if (m_iMaxFrame <= (_uint)(m_fFrame))
    //        if (m_bAtk)
    //        {
    //            m_pPlayer->Add_Health(-7);
    //            wstring szSoundFile;
    //            szSoundFile = L"the_thing_attack_1.wav";
    //            m_bAtk = false;
    //        }
    //    }
    //}
    //break;
    //case THINGY_ATK2:
    //{
    //    if (m_fAtkRng < DistanceToPlayer(m_pPlayer))
    //    {
    //        m_eThingyState = THINGY_RUN;
    //    }
    //    else
    //    {
    //        if (m_bAtk)
    //        {
    //            m_pPlayer->Add_Health(-8);
    //            wstring szSoundFile;
    //            szSoundFile = L"the_thing_attack_1.wav";
    //            m_bAtk = false;
    //        }
    //    }
    //}
    //break;
    //case THINGY_MAGIC:
    //{

    //    if (m_bAtk)
    //    {
    //        _float d = DistanceToPlayer(m_pPlayer);
    //        _vec3 vFrom, vTo;
    //        m_pTransformCom->Get_Info(INFO_POS, &vFrom);
    //        D3DXVec3Normalize(&vTo, &(m_pPlayer->Get_Player_Eye() - vFrom));

    //        m_pPlayer->Add_Health(-8);
    //        wstring szSoundFile;
    //        szSoundFile = L"the_thing_attack_1.wav";
    //        _vec3 pos = vFrom + vTo * d * 0.9f;
    //        pos.y = 1.8f;
    //        m_bAtk = false;
    //        m_eThingyState = THINGY_RUN;
    //    }

    //}
    //break;
    //case THINGY_HIT:
    //{
    //    m_fStiffTimer += 15 * fTimeDelta;
    //    if (30.f < m_fStiffTimer)
    //    {
    //        m_fStiffTimer = 0.f;
    //        m_eThingyState = THINGY_RUN;
    //    }
    //}
    //break;
    //case THINGY_DEAD:
    //{
    //    if (m_iMaxFrame <= (_uint)(m_fFrame))
    //    {
    //        //TODO : 매끄러운 사망 구현
    //        return OBJ_DEAD;
    //    }

    //}
    //break;
    //}


    return 0;
}

void CThingy::LateUpdate_GameObject( )
{
    Motion_Change();
    __super::LateUpdate_GameObject();

}

void CThingy::Render_GameObject()
{
    if (m_eThingyState == THINGY_DEAD && m_iMaxFrame <= (_uint)(m_fFrame))
    {
        return;
    }
    _matrix   matWorld, matView, matBill;

    m_pTransformCom->Get_WorldMatrix(&matWorld);
    m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
    D3DXMatrixIdentity(&matBill);

    matBill._11 = matView._11;
    matBill._13 = matView._13;
    matBill._31 = matView._31;
    matBill._33 = matView._33;

    D3DXMatrixInverse(&matBill, NULL, &matBill);

    m_pTransformCom->Set_WorldMatrix(&(matBill * matWorld));
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pTextureCom->Set_Texture((_uint)(m_fFrame) % m_iMaxFrame);

    m_pBufferCom->Render_Buffer();
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}




HRESULT CThingy::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

    //Textures
    //Normal
    pComponent = m_pTexCom_Idle = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Thingy_IdleTexture"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Thingy_IdleTexture", pComponent });

    //pComponent = m_pTexCom_Morph = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Thingy_MorphTexture"));
    //NULL_CHECK_RETURN(pComponent, E_FAIL);
    //m_mapComponent[ID_STATIC].insert({ L"Proto_Thingy_MorphTexture", pComponent });

    //pComponent = m_pTexCom_Run = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Thingy_WalkTexture"));
    //NULL_CHECK_RETURN(pComponent, E_FAIL);
    //m_mapComponent[ID_STATIC].insert({ L"Proto_Thingy_WalkTexture", pComponent });

    //pComponent = m_pTexCom_Atk = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Thingy_AttackATexture"));
    //NULL_CHECK_RETURN(pComponent, E_FAIL);
    //m_mapComponent[ID_STATIC].insert({ L"Proto_Thingy_AttackATexture", pComponent });

    //pComponent = m_pTexCom_Atk2 = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Thingy_AttackTexture"));
    //NULL_CHECK_RETURN(pComponent, E_FAIL);
    //m_mapComponent[ID_STATIC].insert({ L"Proto_Thingy_AttackTexture", pComponent });

    //pComponent = m_pTexCom_Magic = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Thingy_AttackBTexture"));
    //NULL_CHECK_RETURN(pComponent, E_FAIL);
    //m_mapComponent[ID_STATIC].insert({ L"Proto_Thingy_AttackBTexture", pComponent });

    //pComponent = m_pTexCom_Hit = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Thingy_HitTexture"));
    //NULL_CHECK_RETURN(pComponent, E_FAIL);
    //m_mapComponent[ID_STATIC].insert({ L"Proto_Thingy_HitTexture", pComponent });

    //pComponent = m_pTexCom_Dead = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Thingy_DeathTexture"));
    //NULL_CHECK_RETURN(pComponent, E_FAIL);
    //m_mapComponent[ID_STATIC].insert({ L"Proto_Thingy_DeathTexture", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

}

CThingy* CThingy::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CThingy* pInstance = DBG_NEW CThingy(pGraphicDev);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Thingy Create Failed");
        return nullptr;
    }

    return pInstance;
}

void CThingy::Free()
{
    CMonster::Free();
}

void CThingy::Motion_Change()
{

    if (m_eThingyState != m_ePreThingyState)
    {
        switch (m_eThingyState)
        {
        case THINGY_IDLE:
        {
            m_pTextureCom = m_pTexCom_Idle;
            m_iMaxFrame = 1;
            m_fFrameSpeed = 1.f;
        }
        break;
        case THINGY_MORPH:
        {
            m_pTextureCom = m_pTexCom_Morph;
            m_iMaxFrame = 18;
            m_fFrameSpeed = 10.f;
        }
        break;
        case THINGY_RUN:
        {
            m_pTextureCom = m_pTexCom_Run;
            m_iMaxFrame = 6;
            m_fFrameSpeed = 10.f;
        }
        break;
        case THINGY_ATK1:
        {
            m_pTextureCom = m_pTexCom_Atk;
            m_iMaxFrame = 18;
            m_fFrameSpeed = 10.f;
        }
        break;
        case THINGY_ATK2:
        {
            m_pTextureCom = m_pTexCom_Atk2;
            m_iMaxFrame = 14;
            m_fFrameSpeed = 10.f;
        }
        break;
        case THINGY_MAGIC:
        {
            m_pTextureCom = m_pTexCom_Magic;
            m_iMaxFrame = 28;
            m_fFrameSpeed = 10.f;
        }
        break;
        case THINGY_HIT:
        {
            m_pTextureCom = m_pTexCom_Hit;
            m_iMaxFrame = 1;
            m_fFrameSpeed = 1.f;
        }
        break;
        case THINGY_DEAD:
        {
            m_pTextureCom = m_pTexCom_Dead;
            m_iMaxFrame = 10;
            m_fFrameSpeed = 10.f;
        }
        break;
        }

        m_ePreThingyState = m_eThingyState;

    }
}

void CThingy::Thingy_Take_Dmg(const _float& fTimeDelta)
{

    wstring szSoundFile = L"";

    if (m_eThingyState != THINGY_DEAD)
    {
        if (0 < m_iCurHealth)
        {
            m_eThingyState = THINGY_HIT;
            Knock_Back(m_pPlayer, m_fSpeed * 0.8f, fTimeDelta);
            //사운드 추가
            szSoundFile = L"the_thing_death_pain_1.wav";

            //피격 이펙트 출력
            CGameObject* pGameObject=nullptr;
            _vec3 vPos;
            m_pTransformCom->Get_Info(INFO_POS, &vPos);

            if (pGameObject != nullptr)
            {
                //이펙트 플레이어 방향으로 조금 이동
                _vec3 vDir;

                Engine::Get_Layer(L"GameLogic")->Add_GameObject(L"YetiEffect", pGameObject);
            }

        }
        else if (0 >= m_iCurHealth)
        {
            m_eThingyState = THINGY_DEAD;
            m_fFrame = 0.f;
            szSoundFile = L"the_thing_death_death_liquid_tail.wav";

            //피격 이펙트 출력
            CGameObject* pGameObject=nullptr;
            _vec3 vPos;
            m_pTransformCom->Get_Info(INFO_POS, &vPos);

            if (pGameObject != nullptr)
            {
                //이펙트 플레이어 방향으로 조금 이동
                _vec3 vDir;
                //이펙트 크기 조정

                Engine::Get_Layer(L"GameLogic")->Add_GameObject(L"YetiDeathEffect", pGameObject);
            }
        }
        m_bHit = false;

    }
}
