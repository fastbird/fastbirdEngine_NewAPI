#include "stdafx.h"
#include "FBDirectoryIterator.h"

namespace fastbird{
	class DirectoryIterator::DirectoryIteratorImpl{
	public:
		TString mLastFile;
		bool mRecursive;
		boost::filesystem::recursive_directory_iterator mRecursiveIterator;
		boost::filesystem::directory_iterator mIterator;

		DirectoryIteratorImpl() :mRecursive(false){
		}

		LPCTSTR GetNext(bool& isDirectory){
			boost::filesystem::path path;
			if (mRecursive){
				if (mRecursiveIterator != boost::filesystem::recursive_directory_iterator()){
					auto entity = *mRecursiveIterator;
					mRecursiveIterator++;
					path = entity.path();					
				}
			}
			else{
				if (mIterator != boost::filesystem::directory_iterator()){
					auto entity = *mIterator;
					mIterator++;
					path = entity.path();										
				}
			}
			if (path.empty())
				return _T("");

			mLastFile = path._tgeneric_string();
			isDirectory = boost::filesystem::is_directory(path);
			return mLastFile.c_str();			
		}
		
	};

	DirectoryIterator::DirectoryIterator(LPCTSTR directoryPath, bool recursive)
		:mImpl(new DirectoryIteratorImpl)
	{
		if (directoryPath && _tstrlen(directoryPath) != 0){			
			mImpl->mRecursive = recursive;
			if (recursive){
				mImpl->mRecursiveIterator = boost::filesystem::recursive_directory_iterator(directoryPath);
			}
			else{
				mImpl->mIterator = boost::filesystem::directory_iterator(directoryPath);
			}
		}
	}

	DirectoryIterator::~DirectoryIterator(){
		delete mImpl;
	}

	LPCTSTR DirectoryIterator::GetNextFileName(bool& isDirectory){
		return mImpl->GetNext(isDirectory);
	}
}