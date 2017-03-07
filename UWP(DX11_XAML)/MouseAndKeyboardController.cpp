#include "pch.h"
#include "MouseAndKeyboardController.h"

using namespace Windows::UI::Core;
using namespace Windows::UI::Input;
using namespace Windows::UI;
using namespace Windows::Foundation;
using namespace Microsoft::WRL;
using namespace DirectX;
using namespace Windows::Devices::Input;
using namespace Windows::System;


MouseAndKeyboardController::MouseAndKeyboardController(
	_In_ Windows::UI::Core::CoreWindow^ window,
	_In_ Windows::UI::Core::CoreDispatcher^ dispatcher
)
{
	window->KeyDown +=
		ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &MouseAndKeyboardController::OnKeyDown);
	window->KeyUp +=
		ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &MouseAndKeyboardController::OnKeyUp);
}

void MouseAndKeyboardController::Update()
{
	if (m_left)
		m_yaw -= m_deltaYaw;
	if (m_right)
		m_yaw += m_deltaYaw;
}

void MouseAndKeyboardController::OnKeyDown(Windows::UI::Core::CoreWindow ^ sender, Windows::UI::Core::KeyEventArgs ^ args)
{
	Windows::System::VirtualKey Key;
	Key = args->VirtualKey;

	// Figure out the command from the keyboard.
	if (Key == VirtualKey::W)
		m_forward = true;
	if (Key == VirtualKey::S)
		m_back = true;
	if (Key == VirtualKey::A)
		m_left = true;
	if (Key == VirtualKey::D)
		m_right = true;
	if (Key == VirtualKey::Space)
		m_up = true;
	if (Key == VirtualKey::X)
		m_down = true;
	if (Key == VirtualKey::P || Key == VirtualKey::Escape)
		m_pause = true;
}

void MouseAndKeyboardController::OnKeyUp(Windows::UI::Core::CoreWindow ^ sender, Windows::UI::Core::KeyEventArgs ^ args)
{
	Windows::System::VirtualKey Key;
	Key = args->VirtualKey;

	// Figure out the command from the keyboard.
	if (Key == VirtualKey::W)
		m_forward = false;
	if (Key == VirtualKey::S)
		m_back = false;
	if (Key == VirtualKey::A)
		m_left = false;
	if (Key == VirtualKey::D)
		m_right = false;
	if (Key == VirtualKey::Space)
		m_up = false;
	if (Key == VirtualKey::X)
		m_down = false;
	if (Key == VirtualKey::P || Key == VirtualKey::Escape)
		m_pause = false;
}
