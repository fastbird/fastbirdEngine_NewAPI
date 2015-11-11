#pragma once
/**
\file FBHelpers.h
Convenent macros for manipulating stl containers.
\author Jungwan Byun
\ingroup FBCommonHeaders
*/

#define ValueExistsInVector(arr, v)	(std::find(arr.begin(), arr.end(), v) != arr.end())

#define DeleteValuesInVector_Size(arr, v) \
	unsigned arr ## SizeBefore = arr.size(); \
	arr.erase(std::remove(arr.begin(), arr.end(), v), arr.end()); \
	unsigned arr ## SizeAfter = arr.size();

#define DeleteValuesInVector(arr, v) \
	auto arr##It = std::remove(arr.begin(), arr.end(), v);\
	bool arr##DeletedAny = arr##It != arr.end();\
	arr.erase(arr##It, arr.end());	

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