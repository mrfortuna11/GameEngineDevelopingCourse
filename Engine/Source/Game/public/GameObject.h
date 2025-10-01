#pragma once

#include <RenderObject.h>
#include <Vector.h>

namespace GameEngine
{
   class GameObject
   {
   public:
      GameObject() = default;
      virtual ~GameObject() = default;

      Render::RenderObject** GetRenderObjectRef() { return &m_RenderObject; }

      virtual void Update(float dt, size_t frame) {}

      virtual bool IsPlayerControlled() const { return false; }

      void SetPosition(const Math::Vector3f& position, size_t frame)
      {
         m_Position = position;

         if (m_RenderObject) [[likely]]
            m_RenderObject->SetPosition(position, frame);
      }

      Math::Vector3f GetPosition() const { return m_Position; }

   protected:
      Render::RenderObject* m_RenderObject = nullptr;
      Math::Vector3f m_Position = Math::Vector3f::Zero();
      Math::Vector3f m_MoveDir = Math::Vector3f::Zero();
      Math::Vector3f m_CurrentMoveDir = Math::Vector3f::Zero();
   };
}
