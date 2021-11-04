/*********************************************************************
 * @file	input_layout.h
 * @author	SleepingDog0529
 * @date	2021/07/11 19:54
 ********************************************************************/
#pragma once

namespace wx2
{
	/**
	 * @brief     ID3DBlobの文字列情報をstringに変換する
	 * @param[in] blob 文字列情報
	 * @return    string 文字列
	 */
	std::string BlobMessageToString(ID3DBlob* blob);
}