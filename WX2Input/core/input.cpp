#include "input.h"
#include <algorithm>
#include <iostream>
#include <ranges>

namespace wx2
{
	Input::Input() :
		directInput_()
	{
	}

	void Input::Initialize(HWND hwnd)
	{
		HRESULT hr;
		hr = DirectInput8Create(
			GetModuleHandle(nullptr),
			DIRECTINPUT_VERSION,
			IID_IDirectInput8,
			&directInput_,
			nullptr);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("DirectInput8の作成に失敗しました。");
			exit(EXIT_FAILURE);
		}

		keyboard_.Initialize(directInput_, hwnd);
		mouse_.Initialize(directInput_, hwnd);

		keyboard_.Regist();
		mouse_.Regist();
	}

	void Input::Update()
	{
		keyboard_.Update();
		mouse_.Update();
		gamepad_.Update();
	}
}