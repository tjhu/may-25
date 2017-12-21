//
// App.xaml.h
// Declaration of the App class.
//

#pragma once

#include "App.g.h"
#include "DirectXPage.xaml.h"

namespace UWP_DX11_XAML_
{
		/// <summary>
	/// Provides application-specific behavior to supplement the default Application class.
	/// </summary>
	ref class App sealed
	{
	public:
		App();
		virtual void OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ e) override;

	private:
		void OnSuspending(Platform::Object^ sender, Windows::ApplicationModel::SuspendingEventArgs^ e);
		void OnResuming(Platform::Object ^sender, Platform::Object ^args);
		DirectXPage^ m_directXPage;
	};
}


// Some debug utilities
#ifndef _MessageBox
#define _MessageBox(x);\
	Windows::UI::Popups::MessageDialog d(x);\
	d.ShowAsync();
#endif // _DEBUGMESSAGEBOX

#ifndef DEBUGMESSAGE
#define DEBUGMESSAGE(x);\
		OutputDebugString(x);
#endif // !DEBUGMESSAGE


