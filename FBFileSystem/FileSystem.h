/**
\file FBFileSystem.h
Handling file operations. Implemented on the top of boost::filesystem
\author Jungwan Byun
\defgroup FBFileSystem
Handling file operations. Implemented on the top of boost::filesystem
*/
#pragma once
#include "FBCommonHeaders/String.h"
#include <memory>
namespace fastbird{
	class DirectoryIterator;
	typedef std::shared_ptr<DirectoryIterator> DirectoryIteratorPtr;
	typedef std::weak_ptr<DirectoryIterator> DirectoryIteratorWeakPtr;

	/** Collection of file manipulators.
	\ingroup FBFileSystem
	*/
	class FB_DLL_PUBLIC FileSystem
	{
	public:
		/**Start logging.
		You can call this functino several time but only the first call only takes the effect.
		*/
		static void StartLoggingIfNot(const char* path);
		static void StopLogging();

		static bool Exists(const char* path);

		enum {
			NO_ERROR = 0,
			RENAME_NO_SOURCE = 1,
			RENAME_DEST_EXISTS = 2,
		};
		/** Rename a file.
		@remark  If \a path and \a newpath resolve to the same existing file, no action is taken. 
		Otherwise, if \a newpath resolves to an existing non-directory file, it is removed, 
		while if \a newpath resolves to an existing directory, it is removed if empty on ISO/IEC 9945 
		but is an error on Windows. A symbolic link is itself renamed, rather than the file it 
		resolves to being renamed.
		*/
		static int Rename(const char* path, const char* newpath);
		/** Remove a file.
		@return 'false' if \b path did not exist in the first place, otherwise true.
		*/
		static bool Remove(const char* path);
		/** If ext doesn't have \a dot(.), it will be added. */
		static TString ReplaceExtension(const char* path, const char* ext);
		/** If \a dot(.) is not in the \a path, empty string will be returned. */
		static const char* GetExtension(const char* path);

		/** If filepath exists, rename it to preserve. \a numKeeping decides how many backup files
		need to be kept.\n
		*/
		static void BackupFile(const char* filepath, unsigned numKeeping);

		static DirectoryIteratorPtr GetDirectoryIterator(const char* filepath, bool recursive);
	};
}