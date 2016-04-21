#include <CPhysicWorld.h>

triebWerk::CPhysicWorld::CPhysicWorld() :
    m_CurrentEntityID(0)
{
    m_GravityScale = DirectX::XMVectorSet(0.0f, -9.81f, 0.0f, 0.0f);
}

triebWerk::CPhysicWorld::~CPhysicWorld()
{
}

triebWerk::CPhysicEntity* triebWerk::CPhysicWorld::CreatePhysicEntity()
{
    CPhysicEntity* entity = new CPhysicEntity(m_CurrentEntityID, this);
    m_CurrentEntityID++;

    return entity;
}

triebWerk::CBody* triebWerk::CPhysicWorld::CreateBody()
{
    CBody* body = new CBody();
    return body;
}

triebWerk::CAABBCollider* triebWerk::CPhysicWorld::CreateAABBCollider()
{
    CAABBCollider* aabbCollider = new CAABBCollider();
    return aabbCollider;
}

triebWerk::CSphereCollider* triebWerk::CPhysicWorld::CreateSphereCollider()
{
    CSphereCollider* sphereCollider = new CSphereCollider();
    return sphereCollider;
}

void triebWerk::CPhysicWorld::AddPhysicEntity(CPhysicEntity* a_pPhysicEntity)
{
    // add the entity and all sub categories in it's specific vector

    m_Entities.push_back(a_pPhysicEntity);
    a_pPhysicEntity->SetInPhysicWorldState(true);

    AddBody(a_pPhysicEntity->GetBody());

    for (ICollider* pCollider : a_pPhysicEntity->m_Collider)
    {
        AddCollider(pCollider);
    }
}

void triebWerk::CPhysicWorld::AddBody(CBody* a_pBody)
{
    if (a_pBody != nullptr)
        m_Bodies.push_back(a_pBody);
}

void triebWerk::CPhysicWorld::AddCollider(ICollider* a_pCollider)
{
    if (a_pCollider->m_CheckCollision == false)
        m_StaticCollider.push_back(a_pCollider);
    else
        m_DynamicCollider.push_back(a_pCollider);
}

void triebWerk::CPhysicWorld::RemovePhysicEntity(CPhysicEntity* a_pPhysicEntity)
{
    // remove and delete collider
    for (size_t i = 0; i < a_pPhysicEntity->m_Collider.size(); ++i)
    {
        RemoveCollider(a_pPhysicEntity->m_Collider[i]);
    }

    // remove and delete body
    RemoveBody(a_pPhysicEntity->GetBody());

    // remove and delete entities
    for (size_t i = 0; i < m_Entities.size(); ++i)
    {
        if (m_Entities[i] == a_pPhysicEntity)
        {
            m_Entities.erase(m_Entities.begin() + i);
            delete a_pPhysicEntity;
            break;
        }
    }
}

void triebWerk::CPhysicWorld::RemoveBody(CBody* a_pBody)
{
    // remove and delete body
    if (a_pBody != nullptr)
    {
        for (size_t i = 0; i < m_Bodies.size(); ++i)
        {
            if (m_Bodies[i] == a_pBody)
            {
                m_Bodies.erase(m_Bodies.begin() + i);
                delete a_pBody;
                return;
            }
        }
    }
}

void triebWerk::CPhysicWorld::RemoveCollider(ICollider* a_pCollider)
{
    if (a_pCollider->m_CheckCollision == false)
    {
        for (size_t j = 0; j < m_StaticCollider.size(); ++j)
        {
            if (a_pCollider == m_StaticCollider[j])
            {
                m_StaticCollider.erase(m_StaticCollider.begin() + j);
                delete a_pCollider;
                return;
            }
        }
    }
    else
    {
        for (size_t j = 0; j < m_DynamicCollider.size(); ++j)
        {
            if (a_pCollider == m_DynamicCollider[j])
            {
                m_DynamicCollider.erase(m_DynamicCollider.begin() + j);
                delete a_pCollider;
                return;
            }
        }
    }
}

void triebWerk::CPhysicWorld::Update(const float a_DeltaTime)
{
    DirectX::XMVECTOR deltaGravity = DirectX::XMVectorScale(m_GravityScale, a_DeltaTime);

    for (CBody* pBody : m_Bodies)
    {        
        // apply gravity
        DirectX::XMVECTOR gravity = DirectX::XMVectorScale(deltaGravity, pBody->m_GravityFactor);
        pBody->ApplyImpulse(gravity);

        // apply velocity
        DirectX::XMVECTOR deltaVelocity = DirectX::XMVectorScale(pBody->m_Velocity, a_DeltaTime);
        DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(pBody->m_pTransform->GetPosition(), deltaVelocity);
        pBody->m_pTransform->SetPosition(newPosition);
    }

    CCollision collision;

    for (size_t i = 0; i < m_DynamicCollider.size(); ++i)
    {
        // check for other dynamic collider
        for (size_t j = i + 1; j < m_DynamicCollider.size(); ++j)
            collision.CheckCollision(m_DynamicCollider[i], m_DynamicCollider[j]);

        // and for all static ones
        for (size_t j = 0; j < m_StaticCollider.size(); ++j)
            collision.CheckCollision(m_DynamicCollider[i], m_StaticCollider[j]);
    }

    CheckCollisionEvents();
}

void triebWerk::CPhysicWorld::CheckCollisionEvents()
{
    for (size_t i = 0; i < m_DynamicCollider.size(); ++i)
    {
        ICollider* collider = m_DynamicCollider[i];

        for (int j = collider->m_CollisionEvents.size() - 1; j >= 0; --j)
        {
            CCollisionEvent& collEvent = collider->m_CollisionEvents[j];

            // check if there hasn't been a collision but there is still an open event with another collider
            if (collEvent.m_Updated == false)
            {
                switch (collEvent.m_CollisionState)
                {
                case ECollisionState::Enter:
                    collEvent.m_CollisionState = ECollisionState::Leave;
                    break;
                case ECollisionState::Stay:
                    collEvent.m_CollisionState = ECollisionState::Leave;
                    break;
                case ECollisionState::Leave:
                    collider->m_CollisionEvents.erase(collider->m_CollisionEvents.begin() + j);
                    break;
                }
            }
            else
            {
                collEvent.m_Updated = false;
            }

            // call the collision event from the behaviour
            IBehaviour* behaviour = collider->m_pEntity->GetBehaviour();
            if (behaviour != nullptr)
            {
                switch (collEvent.m_CollisionState)
                {
                case ECollisionState::Enter:
                    behaviour->CollisionEnter(collEvent);
                    break;
                case ECollisionState::Stay:
                    behaviour->CollisionStay(collEvent);
                    break;
                case ECollisionState::Leave:
                    behaviour->CollisionLeave(collEvent);
                    break;
                }
            }
        }
    }
}
