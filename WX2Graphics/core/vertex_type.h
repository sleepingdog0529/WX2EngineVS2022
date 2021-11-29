/*********************************************************************
 * @file   vertex_type.h
 * @author Tomomi Murakami
 * @date   2021/11/26 18:51
 * @brief  ���_�^��`
 ********************************************************************/
#pragma once
#include <DirectXMath.h>

namespace wx2::graphics
{
	struct Vertex3D
	{
		DirectX::XMFLOAT3 position;
	};

	struct ModelVertex
	{
		DirectX::XMFLOAT3 position;
	};
}