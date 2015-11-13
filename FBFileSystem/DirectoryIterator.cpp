#include "stdafx.h"
#include "DirectoryIterator.h"

namespace fastbird{
	class DirectoryIterator::Impl{
	public:
		std::string mLastFile;
		bool mRecursive;
		boost::filesystem::recursive_directory_iterator mRecursiveIterator;
		boost::filesystem::directory_iterator mIterator;

		Impl() :mRecursive(false){
		}

		bool HasNext(){
			if (mRecursive)
				return mRecursiveIterator != boost::filesystem::recursive_directory_iterator();
			return mIterator != boost::filesystem::directory_iterator();
		}

		const char* GetNext(bool* isDirectory){
			if (!HasNext())
				return "";

			boost::filesystem::path path;
			if (mRecursive){
					auto entity = *mRecursiveIterator;
					mRecursiveIterator++;
					path = entity.path();
			}
			else{
					auto entity = *mIterator;
					mIterator++;
					path = entity.path();
			}

			if (path.empty())
				return "";

			mLastFile = path._tgeneric_string();
			if (isDirectory)
				*isDirectory = boost::filesystem::is_directory(path);
			return mLastFile.c_str();			
		}
		
	};

	//---------------------------------------------------------------------------
	DirectoryIterator::DirectoryIterator(const char* directoryPath, bool recursive)
		:mImpl(new Impl)
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

	}

	bool DirectoryIterator::HasNext() const{
		return mImpl->HasNext();
	}
	const char* DirectoryIterator::GetNextFileName(bool* isDirectory){
		return mImpl->GetNext(isDirectory);
	}

	const char* DirectoryIterator::GetNextFileName(){
		return GetNextFileName(0);
	}
}