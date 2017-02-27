#pragma once
ref class MouseAndKeyController sealed
{
public:
	MouseAndKeyController(_In_ Windows::UI::Core::CoreWindow^ window);
	void Initialize(
		_In_ Windows::UI::Core::CoreWindow^ window
	);

protected:
	static void OnKeyDown(
		_In_ Windows::UI::Core::CoreWindow^ sender,
		_In_ Windows::UI::Core::KeyEventArgs^ args
	);
};
