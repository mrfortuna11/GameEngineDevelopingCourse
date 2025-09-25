#pragma once

#include <Camera.h>
#include <Constants.h>
#include <Window/IWindow.h>
#include <Window.h>
#include "InputConfig.h"

namespace GameEngine::Core
{

   void OnKeyDown(WPARAM key, Camera* camera)
   {
      float speed = 0.5f;

      if (key == Config::GetKey("Forward"))
         camera->MoveForward(speed);
      if (key == Config::GetKey("Backward"))
         camera->MoveForward(-speed);
      if (key == Config::GetKey("Left"))
         camera->MoveRight(speed);
      if (key == Config::GetKey("Right"))
         camera->MoveRight(-speed);
      if (key == Config::GetKey("Up"))
         camera->MoveUp(speed);
      if (key == Config::GetKey("Down"))
         camera->MoveUp(-speed);

      /*switch (key)
      {
      case 'W': camera->MoveForward(speed); break;
      case 'S': camera->MoveForward(-speed); break;
      case 'A': camera->MoveRight(speed); break;
      case 'D': camera->MoveRight(-speed); break;
      case VK_SPACE: camera->MoveUp(speed); break;
      case VK_SHIFT: camera->MoveUp(-speed); break;
      }*/
   }

    void OnKeyUp(WPARAM key, Camera* camera)
    {
        (void)key;
        (void)camera;
    }
    void OnMouseDown(WPARAM btnState, int x, int y, Window* window)
    {
        window->SetMousePos(x, y);

        SetCapture(GetPlatformWindowHandle(window->GetWindowHandle()));
    }

    void OnMouseUp(WPARAM btnState, int x, int y)
    {
        ReleaseCapture();
    }

    void OnMouseMove(WPARAM btnState, int x, int y, Camera* camera, Window* window)
    {
        Math::Vector2i mousePos = window->GetMousePos();
        if ((btnState & MK_LBUTTON) != 0)
        {
            float dx = 0.25 * static_cast<float>(x - mousePos.x) * Math::Constants::PI / 180.f;
            float dy = 0.25 * static_cast<float>(y - mousePos.y) * Math::Constants::PI / 180.f;

            dy = -dy; // To avoid inverse movement

            camera->Rotate(dx, dy);
        }
        else if ((btnState & MK_RBUTTON) != 0)
        {
            float dx = 0.05f * static_cast<float>(x - mousePos.x);
            float dy = 0.05f * static_cast<float>(y - mousePos.y);

            Math::Vector3f offset = camera->GetViewDir() * (dx - dy);

            Math::Vector3f position = camera->GetPosition();
            position = position + offset;

            camera->SetPosition(position);
        }

        window->SetMousePos(x, y);
    }
}