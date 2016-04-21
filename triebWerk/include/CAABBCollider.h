#pragma once
#include <ICollider.h>
#include <CMesh.h>

namespace triebWerk
{
    class CAABBCollider : public ICollider
    {
    public:
        DirectX::XMVECTOR m_Min;
        DirectX::XMVECTOR m_Max;

    public:
        CAABBCollider();
        ~CAABBCollider();

        ECollider::Type GetColliderType() final;
        void CreateFromVertices(const CMesh::SVertex* const a_pVertices, const size_t a_Count);
    };
}