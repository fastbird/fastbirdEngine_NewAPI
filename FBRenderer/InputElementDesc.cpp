#include "stdafx.h"
#include "InputElementDesc.h"
using namespace fastbird;

INPUT_ELEMENT_FORMAT InputElementFromString(const char* sz)
{
	if (!sz)
	{
		Logger::Log(FB_ERROR_LOG_ARG, FormatString(_T("%s is not valid INPUT_ELEMENT_FORMAT"), sz));
		assert(0);
		return INPUT_ELEMET_FORMAT_NUM;
	}

	if (_stricmp(STR_INPUT_ELEMENT_FORMAT[INPUT_ELEMENT_FORMAT_FLOAT4], sz) == 0)
	{
		return INPUT_ELEMENT_FORMAT_FLOAT4;
	}
	if (_stricmp(STR_INPUT_ELEMENT_FORMAT[INPUT_ELEMENT_FORMAT_FLOAT3], sz) == 0)
	{
		return INPUT_ELEMENT_FORMAT_FLOAT3;
	}
	if (_stricmp(STR_INPUT_ELEMENT_FORMAT[INPUT_ELEMENT_FORMAT_UBYTE4], sz) == 0)
	{
		return INPUT_ELEMENT_FORMAT_UBYTE4;
	}
	if (_stricmp(STR_INPUT_ELEMENT_FORMAT[INPUT_ELEMENT_FORMAT_FLOAT2], sz) == 0)
	{
		return INPUT_ELEMENT_FORMAT_FLOAT2;
	}
	if (_stricmp(STR_INPUT_ELEMENT_FORMAT[INPUT_ELEMET_FORMAT_INT4], sz) == 0)
	{
		return INPUT_ELEMET_FORMAT_INT4;
	}

	Logger::Log(FB_ERROR_LOG_ARG, FormatString(_T("%s is not valid INPUT_ELEMENT_FORMAT"), sz));
	assert(0);
	return INPUT_ELEMET_FORMAT_NUM;
}

INPUT_CLASSIFICATION InputClassificationFromString(const char* sz)
{
	if (!sz)
		return INPUT_CLASSIFICATION_PER_VERTEX_DATA;
	if (_stricmp(sz, "VERTEX") == 0)
		return INPUT_CLASSIFICATION_PER_VERTEX_DATA;
	if (_stricmp(sz, "INSTANCE") == 0)
		return INPUT_CLASSIFICATION_PER_INSTANCE_DATA;

	Logger::Log(FB_ERROR_LOG_ARG, FormatString(_T("%s is not valid INPUT_CLASSIFICATION"), sz));
	return INPUT_CLASSIFICATION_PER_VERTEX_DATA;
}