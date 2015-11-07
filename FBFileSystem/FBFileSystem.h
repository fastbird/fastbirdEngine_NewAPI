#pragma once
#include "../FBCommonHeaders/FBString.h"
namespace fastbird{
	class _FBFileSystemDECL FileSystem
	{
	public:
		static bool Exists(LPCTSTR path);

		enum {
			NO_ERROR = 0,
			RENAME_NO_SOURCE = 1,
			RENAME_DEST_EXISTS = 2,
		};
		/* Rename a file.
		@remark  If old_p and new_p resolve to the same existing file, no action is taken. 
		Otherwise, if new_p resolves to an existing non-directory file, it is removed, 
		while if new_p resolves to an existing directory, it is removed if empty on ISO/IEC 9945 
		but is an error on Windows. A symbolic link is itself renamed, rather than the file it 
		resolves to being renamed.
		**/
		static int Rename(LPCTSTR path, LPCTSTR newpath);
		/* Remove a file.
		@return 'false' if \b path did not exist in the first place, otherwise true.
		**/
		static bool Remove(LPCTSTR path);
		/** \b If ext doesn't have '.', it will be added. */
		static TString ReplaceExtension(LPCTSTR path, LPCTSTR ext);
		/** '.' included. If '.' is not in the \b path, empty string will be returned. */
		static LPCTSTR GetExtension(LPCTSTR path);
	};
}