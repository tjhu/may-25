#pragma once
ref class MouseAndKeyboardController sealed
{
public:
	// Contructor
	MouseAndKeyboardController(
		_In_ Windows::UI::Core::CoreWindow^ window,
		_In_ Windows::UI::Core::CoreDispatcher^ dispatcher
	);



	// Interface
	void Update();
	float Pitch() { return m_pitch; };
	float Yaw() { return m_yaw; };
	bool Pause() { return m_pause; };


protected:
	void OnKeyDown(
		_In_ Windows::UI::Core::CoreWindow^ sender,
		_In_ Windows::UI::Core::KeyEventArgs^ args
	);
	void OnKeyUp(
		_In_ Windows::UI::Core::CoreWindow^ sender,
		_In_ Windows::UI::Core::KeyEventArgs^ args
	);

protected private:
	// Cached pointer to a dispatcher to marshal execution back to the Xaml UI thread.
	Windows::UI::Core::CoreDispatcher^ m_dispatcher;

	// Properties of the controller object.
	const float					m_deltaYaw = 0.03f;
	float                       m_pitch = 0.0f;
	float                       m_yaw = 0.0f;                  // Orientation euler angles in radians.


	// Input states for Keyboard.
	bool                        m_forward = false;
	bool                        m_back = false;                   // States for movement.
	bool                        m_left = false;
	bool                        m_right = false;
	bool                        m_up = false;
	bool                        m_down = false;
	bool                        m_pause = false;
};

