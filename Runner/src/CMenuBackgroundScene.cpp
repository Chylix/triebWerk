#include <CMenuBackgroundScene.h>

#include <CPlayer.h>
#include <CEnvironmentCreator.h>

CMenuBackgroundScene::CMenuBackgroundScene()
{
}

CMenuBackgroundScene::~CMenuBackgroundScene()
{
}

void CMenuBackgroundScene::Start()
{
    m_pCamera = twRenderer->GetCurrentActiveCamera();

    CreateRoad();
    CreateGrid();
    CreateBackground();
    CreateSnakeLoops();
    CreateFeathers();
}

void CMenuBackgroundScene::Update()
{
    m_pCamera->m_Transform.SetPosition(DirectX::XMVectorSet(0.0f, CPlayer::CameraPosY, -CPlayer::CameraMinusPosZ, 0.0f));
    m_pCamera->m_Transform.SetRotationDegrees(CPlayer::CameraRotationX, 0.0f, 0.0f);

    float dt = twTime->GetDeltaTime();
    m_SnakeRotation1.m128_f32[0] += dt * SnakeRotateSpeed1;
    m_SnakeRotation2.m128_f32[0] += dt * SnakeRotateSpeed2;
    m_SnakeRotation3.m128_f32[0] += dt * SnakeRotateSpeed3;
    m_pSnake1->m_Transform.SetRotationDegrees(m_SnakeRotation1);
    m_pSnake2->m_Transform.SetRotationDegrees(m_SnakeRotation2);
    m_pSnake3->m_Transform.SetRotationDegrees(m_SnakeRotation3);
}

void CMenuBackgroundScene::End()
{
}

void CMenuBackgroundScene::CreateRoad()
{
    float position = 0.0f;
    for (size_t i = 0; i < 8; ++i)
    {
        auto entity = twActiveWorld->CreateEntity();
        entity->m_Transform.SetPosition(0.0f, 0.0f, position);
        entity->m_Transform.SetScale(1.0f, 1.0f, 1.0f);

        triebWerk::CMeshDrawable* mesh = twRenderer->CreateMeshDrawable();
        mesh->m_pMesh = twEngine.m_pResourceManager->GetMesh("ms_course_01");
        mesh->m_Material.SetMaterial(twEngine.m_pResourceManager->GetMaterial("StandardTexture"));
        mesh->m_Material.m_pPixelShader.SetTexture(0, twResourceManager->GetTexture2D("T_course_diff"));
        entity->SetDrawable(mesh);

        twActiveWorld->AddEntity(entity);

        position += 50.0f;
    }
}

void CMenuBackgroundScene::CreateGrid()
{
    auto entity = twActiveWorld->CreateEntity();
    entity->m_Transform.SetPosition(0.0f, -5.0f, 100.0f);
    entity->m_Transform.SetScale(50.0f, 1.0f, 50.0f);

    triebWerk::CMeshDrawable* mesh = twRenderer->CreateMeshDrawable();
    mesh->m_pMesh = twEngine.m_pResourceManager->GetMesh("ms_grid");

    mesh->m_Material.SetMaterial(twEngine.m_pResourceManager->GetMaterial("WireframeGrid"));
    mesh->m_Material.m_ConstantBuffer.SetValueInBuffer(4, &DirectX::XMFLOAT3(1.0f, 0.0f, 1.0f));
    mesh->m_Material.m_ConstantBuffer.SetValueInBuffer(5, &DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
    entity->SetDrawable(mesh);

    twActiveWorld->AddEntity(entity);
}

void CMenuBackgroundScene::CreateBackground()
{
    const float planeWidth = 2000.0f;
    const float planeApectRatio = 0.519916f;

    auto entity = twActiveWorld->CreateEntity();
    entity->m_Transform.SetPosition(0.0f, 200.0f, 500.0f);
    entity->m_Transform.SetScale(planeWidth, 0.0f, planeWidth * planeApectRatio);
    entity->m_Transform.SetRotationDegrees(270.0f, 0.0f, 0.0f);

    auto planeMesh = twRenderer->CreateMeshDrawable();
    planeMesh->m_pMesh = twEngine.m_pResourceManager->GetMesh("ms_plane");
    planeMesh->m_Material.SetMaterial(twEngine.m_pResourceManager->GetMaterial("StandardTexture"));
    planeMesh->m_Material.m_pPixelShader.SetTexture(0, twResourceManager->GetTexture2D("t_background"));
    entity->SetDrawable(planeMesh);

    twActiveWorld->AddEntity(entity);
}

void CMenuBackgroundScene::CreateSnakeLoops()
{
    // snake 1
    m_pSnake1 = twActiveWorld->CreateEntity();
    m_pSnake1->m_Transform.SetPosition(-100.0f, -15.0f, 90.0f);
    m_pSnake1->m_Transform.SetScale(0.6f, 0.6f, 0.6f);
    m_SnakeRotation1.m128_f32[0] = 0.0f;
    m_SnakeRotation1.m128_f32[1] = 340.0f;
    m_SnakeRotation1.m128_f32[2] = 0.0f;
    m_pSnake1->m_Transform.SetRotationDegrees(m_SnakeRotation1);

    auto mesh1 = twRenderer->CreateMeshDrawable();
    mesh1->m_pMesh = twEngine.m_pResourceManager->GetMesh("ms_snakeloop_01");
    mesh1->m_Material.SetMaterial(twEngine.m_pResourceManager->GetMaterial("StandardTexture"));
    mesh1->m_Material.m_pPixelShader.SetTexture(0, twResourceManager->GetTexture2D("T_snakeloop_diff"));
    m_pSnake1->SetDrawable(mesh1);

    twActiveWorld->AddEntity(m_pSnake1);

    // snake 2
    m_pSnake2 = twActiveWorld->CreateEntity();
    m_pSnake2->m_Transform.SetPosition(120.0f, -20.0f, 170.0f);
    m_pSnake2->m_Transform.SetScale(0.8f, 0.8f, 0.8f);
    m_SnakeRotation2.m128_f32[0] = 0.0f;
    m_SnakeRotation2.m128_f32[1] = 90.0f;
    m_SnakeRotation2.m128_f32[2] = 350.0f;
    m_pSnake2->m_Transform.SetRotationDegrees(m_SnakeRotation2);

    auto mesh2 = twRenderer->CreateMeshDrawable();
    mesh2->m_pMesh = twEngine.m_pResourceManager->GetMesh("ms_snakeloop_01");
    mesh2->m_Material.SetMaterial(twEngine.m_pResourceManager->GetMaterial("StandardTexture"));
    mesh2->m_Material.m_pPixelShader.SetTexture(0, twResourceManager->GetTexture2D("T_snakeloop_diff"));
    m_pSnake2->SetDrawable(mesh2);

    twActiveWorld->AddEntity(m_pSnake2);

    // snake 3
    m_pSnake3 = twActiveWorld->CreateEntity();
    m_pSnake3->m_Transform.SetPosition(370.0f, 0.0f, 200.0f);
    m_pSnake3->m_Transform.SetScale(1.2f, 1.2f, 1.2f);
    m_SnakeRotation3.m128_f32[0] = 0.0f;
    m_SnakeRotation3.m128_f32[1] = 90.0f;
    m_SnakeRotation3.m128_f32[2] = 350.0f;
    m_pSnake3->m_Transform.SetRotationDegrees(m_SnakeRotation3);

    auto mesh3 = twRenderer->CreateMeshDrawable();
    mesh3->m_pMesh = twEngine.m_pResourceManager->GetMesh("ms_snakeloop_01");
    mesh3->m_Material.SetMaterial(twEngine.m_pResourceManager->GetMaterial("StandardTexture"));
    mesh3->m_Material.m_pPixelShader.SetTexture(0, twResourceManager->GetTexture2D("T_snakeloop_diff"));
    m_pSnake3->SetDrawable(mesh3);

    twActiveWorld->AddEntity(m_pSnake3);
}

void CMenuBackgroundScene::CreateFeathers()
{
    float isSpawnedTo = 0;

    while (isSpawnedTo < 500.0f)
    {
        int randomNumber = twRandom::GetNumber(0, 99);
        if (randomNumber < CEnvironmentCreator::FeatherSpawnProbability)
        {
            auto entity = twActiveWorld->CreateEntity();
            entity->m_Transform.SetPosition(0.0f, 0.0f, isSpawnedTo);
            entity->m_Transform.SetScale(1.0f, 1.0f, 1.0f);

            triebWerk::CMeshDrawable* mesh = twRenderer->CreateMeshDrawable();
            mesh->m_pMesh = twEngine.m_pResourceManager->GetMesh("ms_feathers_01");
            mesh->m_Material.SetMaterial(twEngine.m_pResourceManager->GetMaterial("StandardTexture"));
            mesh->m_Material.m_pPixelShader.SetTexture(0, twResourceManager->GetTexture2D("feather_diff_01"));
            entity->SetDrawable(mesh);

            twActiveWorld->AddEntity(entity);
        }

        isSpawnedTo += 50;
    }
}

