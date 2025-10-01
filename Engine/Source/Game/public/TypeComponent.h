#pragma once
#include <Vector.h>
#include "GameObject.h"
#include <Input/InputHandler.h>

namespace GameEngine
{
   class JumpingCube : public GameObject
   {
   public:
      void Update(float dt, size_t frame) override
      {
         m_Time += dt * speed;
         m_Position.y = jumpHeight * std::abs(std::sin(m_Time));

         SetPosition(m_Position, frame);
      }
   private:
      float m_Time = 0.0f;
      float jumpHeight = 3.0f;
      float speed = 5.0f;
   };

   class MovingCube : public GameObject
   {
      void Update(float dt, size_t frame) override
      {
         m_Position.x += m_Speed * m_Direction * dt;


         if (m_Position.x > m_MaxX)
         {
            m_Position.x = m_MaxX;
            m_Direction = -1.0f; 
         }
         else if (m_Position.x < m_MinX)
         {
            m_Position.x = m_MinX;
            m_Direction = 1.0f;
         }

         SetPosition(m_Position, frame);
      }

   private:
      float m_Speed = 2.0f;     
      float m_Direction = 1.0f; 
      float m_MinX = -5.0f;     
      float m_MaxX = 5.0f;     
   };


   class PlayerCube : public GameObject
   {
   public:
      bool IsPlayerControlled() const override { return true; }

      void Update(float dt, size_t frame) override
      {

         if (m_MoveDir != Math::Vector3f::Zero())
         {
            m_Position = m_Position + m_MoveDir.Normalized() * m_Speed * dt;
            m_MoveDir = Math::Vector3f::Zero(); 
         }
         SetPosition(m_Position, frame);
      }

      void Move(Math::Vector3f dir)
      {
         m_MoveDir = m_MoveDir + dir;
      }
   private:

      float m_Speed = 5.0f;
   };

}