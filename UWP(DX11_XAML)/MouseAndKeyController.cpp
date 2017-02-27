#include "pch.h"
#include "MouseAndKeyController.h"

using namespace Windows::UI::Core;
using namespace Windows::UI::Input;
using namespace Windows::UI;
using namespace Windows::Foundation;
using namespace Microsoft::WRL;
using namespace DirectX;
using namespace Windows::Devices::Input;
using namespace Windows::System;


MouseAndKeyController::MouseAndKeyController(_In_ Windows::UI::Core::CoreWindow^ window)
{
	this->Initialize(window);
}

void MouseAndKeyController::Initialize(Windows::UI::Core::CoreWindow ^ window)
{
	//window->KeyUp +=
	//	ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &MouseAndKeyController::OnKeyDown);
}

void MouseAndKeyController::OnKeyDown(Windows::UI::Core::CoreWindow ^ sender, Windows::UI::Core::KeyEventArgs ^ args)
{
	Windows::UI::Popups::MessageDialog dialog("hge");
	dialog.ShowAsync();
	return;
}
