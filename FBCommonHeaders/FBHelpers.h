#pragma once
/**
\file FBHelpers.h
Convenent macros for manipulating stl containers.
\author Jungwan Byun
\ingroup FBCommonHeaders
*/

#define ValueExistsInVector(arr, v)	(std::find(arr.begin(), arr.end(), v) != arr.end())
#define DeleteValuesInVector(arr, v) \
	unsigned arr ## SizeBefore = arr.size(); \
	arr.erase(std::remove(arr.begin(), arr.end(), v), arr.end()); \
	unsigned arr ## SizeAfter = arr.size();
#define DeleteValuesInList(arr, v) \
	unsigned arr ## SizeBefore = arr.size(); \
	for (auto it = arr.begin(); it != arr.end();)\
		{\
		if ((*it) == v)\
				{\
			it = mChildren.erase(it);\
				}\
				else\
				{\
			++it;\
				}\
		}\
	unsigned arr ## SizeAfter = arr.size();