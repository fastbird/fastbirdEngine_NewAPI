/**
\file FBFileSystem.h
Handling file operations. Implemented on the top of boost::filesystem
\author fastbird @ fastbird dev studio
\defgroup FBFileSystem
Handling file operations. Implemented on the top of boost::filesystem
*/
#pragma once
#include "../FBCommonHeaders/FBString.h"
namespace fastbird{
	/** Collection of file manipulators.
	\ingroup FBFileSystem
	*/
	class _FBFileSystemDLL FileSystem
	{
	public:
		static bool Exists(LPCTSTR path);

		enum {
			NO_ERROR = 0,
			RENAME_NO_SOURCE = 1,
			RENAME_DEST_EXISTS = 2,
		};
		/** Rename a file.
		@remark  If old_p and new_p resolve to the same existing file, no action is taken. 
		Otherwise, if new_p resolves to an existing non-directory file, it is removed, 
		while if new_p resolves to an existing directory, it is removed if empty on ISO/IEC 9945 
		but is an error on Windows. A symbolic link is itself renamed, rather than the file it 
		resolves to being renamed.
		*/
		static int Rename(LPCTSTR path, LPCTSTR newpath);
		/** Remove a file.
		@return 'false' if \b path did not exist in the first place, otherwise true.
		*/
		static bool Remove(LPCTSTR path);
		/** If ext doesn't have \a dot(.), it will be added. */
		static TString ReplaceExtension(LPCTSTR path, LPCTSTR ext);
		/** If \a dot(.) is not in the \a path, empty string will be returned. */
		static LPCTSTR GetExtension(LPCTSTR path);

		/** If filepath exists, rename it to preserve. \a numKeeping decides how many backup files
		need to be kept.\n
		*/
		static void BackupFile(LPCTSTR filepath, unsigned numKeeping);
	};
}