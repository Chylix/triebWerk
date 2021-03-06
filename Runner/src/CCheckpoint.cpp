#include <CCheckpoint.h>

#include <CGameInfo.h>

CCheckpoint::CCheckpoint() :
    m_HasCollected(false),
	m_RotationModifier(0.0f),
	m_Deleted(false)
{
}

CCheckpoint::~CCheckpoint()
{
}

void CCheckpoint::Collected()
{
	m_pEntity->GetDrawable()->SetActive(false);
	m_pCheckpointPlaneButtom->GetDrawable()->SetActive(false);
	m_pCheckpointPlaneTop->GetDrawable()->SetActive(false);
	m_pCheckpointPlaneMiddle->GetDrawable()->SetActive(false);
	m_pGodrayButtom->GetDrawable()->SetActive(false);

	m_pEntity->RemovePhysicEntity();

	// Add the points you collected by flying
	CGameInfo& gameInfo = CGameInfo::Instance();
	gameInfo.m_TotalPoints += gameInfo.m_CurrentPoints * gameInfo.m_Multiplier;
	gameInfo.m_CurrentPoints = 0;
	gameInfo.m_Multiplier = 1.0f;
	gameInfo.m_EffectCheckpointCollected = true;

	m_HasCollected = true;

	twAudio->PlaySFX(m_pCollectSound);
	
}

void CCheckpoint::Start()
{
	m_pMaterial = &reinterpret_cast<triebWerk::CMeshDrawable*>(m_pEntity->GetDrawable())->m_Material;

	DirectX::XMFLOAT3 lineColor = DirectX::XMFLOAT3(0.0f, 1.0f, 1.0f);
	DirectX::XMFLOAT3 faceColor = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	
	m_pCheckpointPlaneButtom = twActiveWorld->CreateEntity();
	m_pCheckpointPlaneButtom->m_Transform.SetPosition(m_pEntity->m_Transform.GetPosition());
	m_pCheckpointPlaneButtom->m_Transform.SetParent(&m_pEntity->m_Transform);

	triebWerk::CMeshDrawable* pMeshDrawableButtom = twRenderer->CreateMeshDrawable();
	pMeshDrawableButtom->m_DrawType = triebWerk::CMeshDrawable::EDrawType::Draw;
	pMeshDrawableButtom->m_RenderMode = triebWerk::CMeshDrawable::ERenderMode::Transparent;
	pMeshDrawableButtom->m_pMesh = twResourceManager->GetMesh("ms_checkpoint_plane3");
	pMeshDrawableButtom->m_Material.SetMaterial(twResourceManager->GetMaterial("Wireframe"));
	pMeshDrawableButtom->m_Material.m_ConstantBuffer.SetValueInBuffer(4, &lineColor);
	pMeshDrawableButtom->m_Material.m_ConstantBuffer.SetValueInBuffer(5, &faceColor);
	m_pCheckpointPlaneButtom->SetDrawable(pMeshDrawableButtom);

	twActiveWorld->AddEntity(m_pCheckpointPlaneButtom);

	m_pCheckpointPlaneMiddle = twActiveWorld->CreateEntity();
	m_pCheckpointPlaneMiddle->m_Transform.SetPosition(m_pEntity->m_Transform.GetPosition());
	m_pCheckpointPlaneMiddle->m_Transform.SetParent(&m_pEntity->m_Transform);

	triebWerk::CMeshDrawable* pMeshDrawableMiddle = twRenderer->CreateMeshDrawable();
	pMeshDrawableMiddle->m_DrawType = triebWerk::CMeshDrawable::EDrawType::Draw;
	pMeshDrawableMiddle->m_RenderMode = triebWerk::CMeshDrawable::ERenderMode::Transparent;
	pMeshDrawableMiddle->m_pMesh = twResourceManager->GetMesh("ms_checkpoint_plane2");
	pMeshDrawableMiddle->m_Material.SetMaterial(twResourceManager->GetMaterial("Wireframe"));
	pMeshDrawableMiddle->m_Material.m_ConstantBuffer.SetValueInBuffer(4, &lineColor);
	pMeshDrawableMiddle->m_Material.m_ConstantBuffer.SetValueInBuffer(5, &faceColor);
	m_pCheckpointPlaneMiddle->SetDrawable(pMeshDrawableMiddle);


	twActiveWorld->AddEntity(m_pCheckpointPlaneMiddle);
	
	m_pCheckpointPlaneTop = twActiveWorld->CreateEntity();
	m_pCheckpointPlaneTop->m_Transform.SetPosition(m_pEntity->m_Transform.GetPosition());
	m_pCheckpointPlaneTop->m_Transform.SetParent(&m_pEntity->m_Transform);

	triebWerk::CMeshDrawable* pMeshDrawableTop = twRenderer->CreateMeshDrawable();
	pMeshDrawableTop->m_DrawType = triebWerk::CMeshDrawable::EDrawType::Draw;
	pMeshDrawableTop->m_RenderMode = triebWerk::CMeshDrawable::ERenderMode::Transparent;
	pMeshDrawableTop->m_pMesh = twResourceManager->GetMesh("ms_checkpoint_plane1");
	pMeshDrawableTop->m_Material.SetMaterial(twResourceManager->GetMaterial("Wireframe"));
	pMeshDrawableTop->m_Material.m_ConstantBuffer.SetValueInBuffer(4, &lineColor);
	pMeshDrawableTop->m_Material.m_ConstantBuffer.SetValueInBuffer(5, &faceColor);
	m_pCheckpointPlaneTop->SetDrawable(pMeshDrawableTop);

	twActiveWorld->AddEntity(m_pCheckpointPlaneTop);

	//Godrays
	m_pGodrayButtom = twActiveWorld->CreateEntity();
	DirectX::XMVECTOR positionGodBegin = m_pEntity->m_Transform.GetPosition();
	m_pGodrayButtom->m_Transform.SetPosition(positionGodBegin.m128_f32[0], 4.0f, positionGodBegin.m128_f32[2]);

	m_pGodrayButtom->m_Transform.SetParent(&m_pEntity->m_Transform);
	
	triebWerk::CMeshDrawable* pGodrayDrawable = twRenderer->CreateMeshDrawable();
	pGodrayDrawable->m_DrawType = triebWerk::CMeshDrawable::EDrawType::Draw;
	pGodrayDrawable->m_RenderMode = triebWerk::CMeshDrawable::ERenderMode::Transparent;
	pGodrayDrawable->m_pMesh = twResourceManager->GetMesh("ms_godray");
	pGodrayDrawable->m_Material.SetMaterial(twResourceManager->GetMaterial("StandardTransparentTexture"));
	pGodrayDrawable->m_Material.m_pPixelShader.SetTexture(0, twResourceManager->GetTexture2D("t_godrays"));
	m_pGodrayButtom->SetDrawable(pGodrayDrawable);
	
	twActiveWorld->AddEntity(m_pGodrayButtom);

	m_pCollectSound = twResourceManager->GetSound("SFX_CollectCheckpoint");
	m_pPassSound = twResourceManager->GetSound("SFX_GainMultiplier");
	m_pFullMultiplierSound = twResourceManager->GetSound("SFX_FullMultiplier");
}

void CCheckpoint::Update()
{
	DirectX::XMVECTOR leftPos = m_pEntity->m_Transform.GetPosition();
	leftPos.m128_f32[1] = (std::sin(twTime->GetTimeSinceStartup()* 2.5f) + 2.0f);
	m_pEntity->m_Transform.SetPosition(leftPos);
	m_pEntity->m_Transform.RotateDegrees(0.0f, 190 * twTime->GetDeltaTime(), 0.0f);

	float distance = m_pEntity->m_Transform.GetPosition().m128_f32[2] - CGameInfo::Instance().m_PlayerPositionZ;

	if (distance < -2.0f && !m_Deleted)
	{
		m_Deleted = true;

		if (!m_HasCollected)
		{
			CGameInfo& gameInfo = CGameInfo::Instance();
			gameInfo.m_Multiplier += AddedMultiplier;
			gameInfo.m_EffectCheckpointPassed = true;

			if (gameInfo.m_Multiplier > HighestMultiplier)
			{
				gameInfo.m_Multiplier = HighestMultiplier;
				twAudio->PlaySFX(m_pFullMultiplierSound);
			}
			else
			{
				if ((float)(gameInfo.m_Multiplier / (int)gameInfo.m_Multiplier) == 1.0f)
				{
					twAudio->PlaySFX(twResourceManager->GetSound("SFX_GainFulMultiplier"));
				}
				else
				{
					twAudio->PlaySFX(m_pPassSound);
				}
			}

		}
	}


}

void CCheckpoint::End()
{
	twActiveWorld->RemoveEntity(m_pCheckpointPlaneButtom);
	twActiveWorld->RemoveEntity(m_pCheckpointPlaneMiddle);
	twActiveWorld->RemoveEntity(m_pCheckpointPlaneTop);
	twActiveWorld->RemoveEntity(m_pGodrayButtom);
}
