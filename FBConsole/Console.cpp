/*
 -----------------------------------------------------------------------------
 This source file is part of fastbird engine
 For the latest info, see http://www.jungwan.net/
 
 Copyright (c) 2013-2015 Jungwan Byun
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 -----------------------------------------------------------------------------
*/

#include "stdafx.h"
#include "Console.h"
#include "CandidatesData.h"
#include "FBRenderer/Renderer.h"
#include "FBRenderer/Font.h"
#include "FBSystemLib/StdOutRedirect.h"
#include "FBSystemLib/ClipboardData.h"
#include "FBMathLib/Math.h"
#include "FBStringLib/StringLib.h"
#include "FBStringLib/StringConverter.h"
#include "FBInputManager/InputManager.h"
#include "FBInputManager/IInputInjector.h"
#include "FBLua/LuaUtils.h"
using namespace fastbird;

bool gConsoleInvalidParam = false;
static void InvalidParamHandler(const wchar_t *pszExpression,
	const wchar_t *pszFunction,
	const wchar_t *pszFile,
	unsigned int nLine,
	uintptr_t pReserved){
	gConsoleInvalidParam = true;
}

//---------------------------------------------------------------------------
class Console::Impl{
public:
	static const float sFontSize;
	ConsoleWeakPtr mSelf;
	bool mLuaMode;
	int mCursorPos;
	const int mCursorWidth;
	int mHighlightStart;
	std::string mInputString;
	std::wstring mInputStringw;
	StringVector mBuffer;
	WStringVector mBufferw; // display buffer
	std::mutex mMutex;	
	int mBufferBtmLine; // bottom line
	int mLines;
	bool mOpen;
	const int mLineGap;
	int mHeight;
	std::wstring mPrompt;
	int mPromptStart;
	Vec2I mInputPosition;
	typedef std::vector<CVar*> CVARS;
	CVARS mCVars;

	typedef std::vector<ConsoleCommand*> COMMANDS;
	COMMANDS mCommands;

	CandidatesData* mCandiData;
	bool mACMode; // AutoCompletionMode
	StringVector mCandidates;
	std::string mInputStringBackup; // auto completed input
	int mCandiDepth;
	int mCandiIndex;

	StringVector mHistory;
	std::vector<bool> mValidHistory;
	int mHistoryIndex;
	int mHistoryIndexBackup;
	std::vector<ICVarObserver*> mCVarListeners;
	StdOutRedirect* mStdOutRedirect;
	Vec2I mRTSize;

	struct processQData{
		processQData(const char* str, bool history)
			: mString(str)
			, mHistory(history)
		{

		}
		std::string mString;
		bool mHistory;
	};
	std::vector<processQData> mQueue;
	const char* cacheFile = "consoleCache.tmp";

	//---------------------------------------------------------------------------
	Impl() : mCursorPos(0)
		, mCursorWidth(10)
		, mLines(15)
		, mOpen(false)
		, mLineGap(2)
		, mHighlightStart(-1)
		, mACMode(false), mCandiDepth(0), mCandiIndex(0)
		, mBufferBtmLine(0)
		, mLuaMode(false)
		, mStdOutRedirect(0)
		, mHistoryIndex(0)
		, mHistoryIndexBackup(0)
		, mRTSize(1600, 900)
	{
		mCandiData = FB_NEW(CandidatesData);
		std::ifstream file(cacheFile);
		if (file.is_open())
		{
			do {
				char buf[256] = { 0 };
				file.getline(buf, 256);
				if (strlen(buf) != 0)
				{
					mHistory.push_back(buf);
					mValidHistory.push_back(true);
				}
			} while (!file.eof());
			file.close();
			mHistoryIndex = mHistory.size();
		}
	}

	~Impl(){
		FB_DELETE(mCandiData);
	}

	//--------------------------------------------------------------------------
	bool Init()
	{
		// calc background size
		mHeight = Round((sFontSize + mLineGap) * mLines);

		mPrompt = AnsiToWide(">", 1);
		mPromptStart = 2;
		mInputPosition = Vec2I(20, mHeight - mLineGap);

		return true;
	}

	void SetRenderTargetSize(const Vec2I& size){
		mRTSize = size;
	}

	//--------------------------------------------------------------------------
	void RegisterCommand(ConsoleCommand* pCom)
	{
		for (const auto& c : mCVars)
		{
			if (c->mName == pCom->mName)
			{
				Logger::Log(FB_ERROR_LOG_ARG, FormatString("The same name of CVar(%s) already exists.", pCom->mName).c_str());
				return;
			}
		}

		for (const auto& c : mCommands)
		{
			if (c->mName == pCom->mName)
			{
				Logger::Log(FB_ERROR_LOG_ARG, FormatString("The same name of CFunc(%s) already exists.", pCom->mName).c_str());				
			}
		}
		mCandiData->AddCandidate(pCom->mName.c_str());
		mCommands.push_back(pCom);
	}

	//--------------------------------------------------------------------------
	void UnregisterCommand(ConsoleCommand* pCom)
	{
		mCommands.erase(std::remove(mCommands.begin(), mCommands.end(),
			pCom), mCommands.end());
	}

	//--------------------------------------------------------------------------
	void RegisterVariable(CVar* cvar)
	{
		// check the name;
		for (const auto& c : mCVars)
		{
			if (c->mName == cvar->mName || c == cvar)
			{
				Logger::Log(FB_ERROR_LOG_ARG, FormatString("The same name of CVar(%s) already exists.", cvar->mName).c_str());				
				return;
			}
		}

		for (const auto& c : mCommands)
		{
			if (c->mName == cvar->mName)
			{
				Logger::Log(FB_ERROR_LOG_ARG, FormatString("The same name of CFunc(%s) already exists.", cvar->mName).c_str());
			}
		}

		mCandiData->AddCandidate(cvar->mName.c_str());

		mCVars.push_back(cvar);
	}

	//--------------------------------------------------------------------------
	void UnregisterVariable(CVar* cvar)
	{
		mCVars.erase(std::remove(mCVars.begin(), mCVars.end(), cvar), mCVars.end());
	}

	void AddCandidatesTo(const char* parent, const StringVector& candidates)
	{
		mCandiData->AddCandidatesTo(parent, candidates);
	}

	//--------------------------------------------------------------------------
	void Log(const char* szFmt, ...)
	{
		MutexLock lock(mMutex);		
		char buf[4096];
		auto oldHandler = _set_invalid_parameter_handler(InvalidParamHandler);
		va_list args;
		va_start(args, szFmt);
		vsprintf_s(buf, 4096, szFmt, args);
		va_end(args);
		if (gConsoleInvalidParam){
			strcpy_s(buf, szFmt);
			gConsoleInvalidParam = false;
		}
		_set_invalid_parameter_handler(oldHandler);


		std::cerr << buf << std::endl;

		std::wstring strw = AnsiToWide(buf, strlen(buf));
		auto& renderer = Renderer::GetInstance();
		if (renderer.GetFont(sFontSize))
		{
			FontPtr pFont = renderer.GetFont(sFontSize);
			int textWidth = (int)pFont->GetTextWidth((char*)strw.c_str());
			const auto& size = mRTSize;
			int consoleWidth = size.x;
			if (textWidth > consoleWidth)
			{
				strw = pFont->StripTags(strw.c_str());
				int numCharInRow = 20;
				int len = strw.length();
				int count = 0;
				int curLine = 0;
				WStringVector lines;
				int start = 0;
				int end = 1;
				while (end<len)
				{
					int textWidth = (int)pFont->GetTextWidth((char*)&strw[start], (end - start) * 2);
					if (textWidth>consoleWidth)
					{
						mBuffer.push_back(std::string(buf + start, end - start));
						start = end;
						end += 1;
					}
					else
					{
						end++;
					}
				}
				mBuffer.push_back(std::string(buf + start));
			}
			else
			{
				mBuffer.push_back(buf);
			}
		}
		else
		{
			// no renderer
			mBuffer.push_back(buf);
		}

		mBufferw.clear();
		auto rit = mBuffer.rbegin(), ritEnd = mBuffer.rend();
		int remainedLined = mLines - 1;
		for (; rit != ritEnd && remainedLined; rit++)
		{
			auto strVec = Split(*rit, "\n");
			auto rit2 = strVec.rbegin(), ritEnd2 = strVec.rend();
			for (; rit2 != ritEnd2 && remainedLined; rit2++)
			{
				if (!rit2->empty())
				{
					mBufferw.push_back(AnsiToWide(rit2->c_str()));
					remainedLined--;
				}
			}

		}
	}

	//--------------------------------------------------------------------------
	void ToggleOpen()
	{
		mOpen = !mOpen;
		InputManager::GetInstance().ClearBuffer();
	}

	//--------------------------------------------------------------------------
	void Update()
	{
		if (mStdOutRedirect)
		{
			char buf[1024];
			int len = mStdOutRedirect->GetBuffer(buf, 1024);
			if (len)
			{
				buf[1023] = 0;
				Log(buf);
			}
		}
		for (auto& it : mQueue){
			ProcessCommand(it.mString.c_str(), it.mHistory);
		}
		mQueue.clear();
	}

	//--------------------------------------------------------------------------
	void Render()
	{
		if (!mOpen)
			return;
		auto& renderer = Renderer::GetInstance();
		FontPtr pFont = renderer.GetFont(sFontSize);
		pFont->SetHeight((float)sFontSize);
		const int lineHeight = (int)pFont->GetHeight();

		// Draw Background
		const auto& size = mRTSize;
		renderer.DrawQuad(Vec2I(0, 0), Vec2I(size.x, mHeight),
			Color::DarkGray);

		// DrawHighlight
		if (mHighlightStart != -1)
		{
			Vec2I highlightStartPos = mInputPosition;
			highlightStartPos.x += (int)pFont->GetTextWidth((char*)mInputStringw.c_str(),
				mHighlightStart * 2);
			highlightStartPos.y -= lineHeight;
			Vec2I highlightEndPos = mInputPosition;
			highlightEndPos.x += (int)pFont->GetTextWidth((char*)mInputStringw.c_str(),
				mCursorPos * 2);
			highlightEndPos.y -= lineHeight;

			if (mHighlightStart > mCursorPos)
			{
				std::swap(highlightStartPos, highlightEndPos);
			}

			highlightEndPos.y += lineHeight;

			renderer.DrawQuad(highlightStartPos, highlightEndPos - highlightStartPos,
				Color::Gray);
		}

		// Draw Cursor
		Vec2I cursorPos = mInputPosition;
		cursorPos.x += (int)pFont->GetTextWidth((char*)mInputStringw.c_str(), mCursorPos * 2);
		renderer.DrawQuad(cursorPos, Vec2I(mCursorWidth, 2), Color::Yellow);

		// Draw prompt
		pFont->PrepareRenderResources();
		pFont->SetRenderStates();
		pFont->Write((float)mPromptStart, (float)mInputPosition.y, 0.f, Color::White,
			(char*)mPrompt.c_str(), -1, Font::FONT_ALIGN_LEFT);

		// Draw Input String
		pFont->Write((float)mInputPosition.x, (float)mInputPosition.y, 0.f, Color::White,
			(char*)mInputStringw.c_str(), -1, Font::FONT_ALIGN_LEFT);



		// Draw Buffer
		WStringVector bufferwRender(mBufferw.size());
		{
			MutexLock lock(mMutex);
			std::copy(mBufferw.begin(), mBufferw.end(), bufferwRender.begin());
		}

		int bufferDrawPosY = mInputPosition.y - lineHeight - mLineGap;
		auto it = bufferwRender.begin();
		auto itEnd = bufferwRender.end();
		for (; it < itEnd; it++)
		{
			unsigned numLineFeed = std::count(it->begin(), it->end(), L'\n');
			pFont->Write((float)mInputPosition.x, (float)bufferDrawPosY, 0.f,
				Color::Gray, (char*)it->c_str(), -1, Font::FONT_ALIGN_LEFT);
			bufferDrawPosY -= lineHeight + mLineGap;
		}
		pFont->SetBackToOrigHeight();
	}

	void ConsumeInput(IInputInjectorPtr injector)
	{
		if (!mOpen)
			return;	
		
		auto& renderer = Renderer::GetInstance();
		if (unsigned int chr = injector->GetChar())
		{
			if (chr == 22) // Synchronous idle - ^V
			{
				injector->PopChar();
				std::string data = GetClipboardDataAsString(renderer.GetMainWindowHandle());
				if (!data.empty())
				{
					auto insertionPos = mInputString.begin() + mCursorPos;
					mInputString.insert(insertionPos, data.begin(), data.end());
					mCursorPos += data.size();
				}
			}
			else
			{
				switch (chr)
				{
				case VK_BACK:
				{
					injector->PopChar();
					if (IsHighlighting() && !mInputString.empty())
					{
						auto it = mInputString.begin() + mCursorPos;
						auto end = mInputString.begin() + mHighlightStart;
						if (mCursorPos > mHighlightStart)
						{
							std::swap(it, end);
						}
						mCursorPos = std::distance(mInputString.begin(), it);
						mInputString.erase(it, end);
						EndHighlighting();
					}
					else if (mCursorPos > 0 && !mInputString.empty())
					{
						mInputString.erase(mInputString.begin() + mCursorPos - 1);
						mCursorPos--;
					}
					EndAutoCompletion();
				}
				break;
				case VK_TAB:
				{
					injector->PopChar();
					AutoCompletion();
				}
				break;
				case VK_RETURN:
				{
					injector->PopChar();
					if (!mInputString.empty())
					{
						ProcessCommand(mInputString.c_str());
						mInputString.clear();
						mCursorPos = 0;
						EndHighlighting();
						EndAutoCompletion();
					}
				}
				break;
				default:
				{
					if (IsValidCharForInput(chr))
					{
						injector->PopChar();
						if (IsHighlighting())
						{
							if (!mACMode)
							{
								auto it = mInputString.begin() + mCursorPos;
								auto end = mInputString.begin() + mHighlightStart;
								if (mCursorPos > mHighlightStart)
								{
									std::swap(it, end);
								}
								mCursorPos = std::distance(mInputString.begin(), it);
								// delete selected string
								mInputString.erase(it, end);
							}
							EndHighlighting();
						}

						EndAutoCompletion();

						mInputString.insert(mInputString.begin() + mCursorPos, chr);
						mCursorPos++;
					}
				}
				}
			}
		}

		if (injector->IsKeyPressed(VK_HOME))
		{
			Highlighting(injector->IsKeyDown(VK_SHIFT));
			mCursorPos = 0;
		}
		else if (injector->IsKeyPressed(VK_END))
		{
			Highlighting(injector->IsKeyDown(VK_SHIFT));
			mCursorPos = mInputString.size();
		}
		else if (injector->IsKeyPressed(VK_DELETE))
		{
			if (!mInputString.empty())
			{
				if (IsHighlighting())
				{
					auto it = mInputString.begin() + mCursorPos;
					auto end = mInputString.begin() + mHighlightStart;
					if (mCursorPos > mHighlightStart)
					{
						std::swap(it, end);
					}
					mCursorPos = std::distance(mInputString.begin(), it);
					mInputString.erase(it, end);
					EndHighlighting();
				}
				else
				{
					mInputString.erase(mInputString.begin() + mCursorPos);
				}
			}
			EndAutoCompletion();
		}
		else if (injector->IsKeyPressed(VK_LEFT))
		{
			Highlighting(injector->IsKeyDown(VK_SHIFT));
			if (mCursorPos>0)
			{
				mCursorPos--;
			}
			EndAutoCompletion();
		}
		else if (injector->IsKeyPressed(VK_RIGHT))
		{
			Highlighting(injector->IsKeyDown(VK_SHIFT));
			if (mCursorPos < (int)mInputString.size())
			{
				mCursorPos++;
			}
			EndAutoCompletion();
		}
		else if (injector->IsKeyPressed(VK_UP))
		{
			GetNextHistory();

		}
		else if (injector->IsKeyPressed(VK_DOWN))
		{
			GetPrevHistory();
		}

		mInputStringw = AnsiToWide(mInputString.c_str(), mInputString.size());

		injector->Invalidate(FBInputDevice::AllMask);		
	}

	void EnableInputListener(bool enable)
	{
		//mActivated
	}

	bool IsEnabledInputLIstener() const
	{
		return mOpen;
	}

	void ProcessCommand(const char* command, bool history = true)
	{
		if (!command || strlen(command) == 0 || strlen(command)>512)
		{
			Logger::Log(FB_ERROR_LOG_ARG, "Invalid console command");			
			return;
		}

		if (strcmp("%", command) == 0)
		{
			mLuaMode = true;
			mPrompt = AnsiToWide("%", 1);
			Log("Start Lua interactive mode.");
			return;
		}
		else if (strcmp(">", command) == 0)
		{
			mLuaMode = false;
			mPrompt = AnsiToWide(">", 1);
			Log("Start Game command mode.");
			return;
		}

		bool prefixFound = false;
		const char* found;
		if ((found = strchr(command, '%')) && command == found)
		{
			mLuaMode = true;
			mPrompt = AnsiToWide("%", 1);
			prefixFound = true;
		}
		else if ((found = strchr(command, '>')) && command == found)
		{
			mLuaMode = false;
			mPrompt = AnsiToWide(">", 1);
			prefixFound = true;
		}
		char buf[512];
		if (!prefixFound)
		{
			sprintf_s(buf, "%s%s", mLuaMode ? "%" : ">", command);
		}
		else
		{
			sprintf_s(buf, "%s", command);
		}

		bool pushed = false;
		if (history){
			if (mHistory.empty() || mHistory.back() != buf)
			{
				for (auto it = mHistory.begin(); it != mHistory.end();)
				{
					if (*it == buf)
					{
						unsigned dist = std::distance(mHistory.begin(), it);
						it = mHistory.erase(it);
						mValidHistory.erase(mValidHistory.begin() + dist);
					}
					else
					{
						++it;
					}
				}
				DeleteValuesInVector(mHistory, std::string(buf));
				mHistory.push_back(buf);
				mHistoryIndex = mHistory.size();
				pushed = true;
			}
			else
			{
				mHistoryIndex = mHistoryIndexBackup;
			}
		}

		if (prefixFound)
			command = buf + 1;

		bool processed = false;
		if (mLuaMode)
		{
			std::string newstring = std::string("  ") + command;
			Log(newstring.c_str());
			auto& renderer = Renderer::GetInstance();
			processed =  LuaUtils::ExecuteLua(renderer.GetLua() , command);
			if (history){
				if (pushed)
					mValidHistory.push_back(processed);
				if (processed)
					SaveHistoryToDiskCache();
			}

			return;
		}
		StringVector words = StringConverter::ParseStringVector(command);
		if (words.empty())
			return;

		auto it = words.begin(), itEnd = words.end();
		for (; it != itEnd; it++)
		{
			ToLowerCase(*it);
		}


		//find command
	{
		for (const auto& c : mCommands)
		{
			if (_stricmp(c->mName.c_str(), words[0].c_str()) == 0)
			{
				if (c->mFunc)
				{
					c->mFunc(words);
					processed = true;
				}
			}
		}
	}

	// find cvar
	{
		for (const auto& c : mCVars)
		{
			if (_stricmp(c->mName.c_str(), words[0].c_str()) == 0)
			{
				size_t numWords = words.size();
				if (numWords == 2)
				{
					c->SetData(words[1]);
					OnCVarChanged(c);
				}
				else if (numWords == 3){
					switch (c->mType){
					case CVAR_TYPE_VEC2I:
						c->SetData(FormatString("%s, %s", words[1].c_str(), words[2].c_str()));
						OnCVarChanged(c);
						break;

					default:
						assert(0);

					}

				}
				this->Log("%s %s", c->mName.c_str(), c->GetData().c_str());
				processed = true;
			}
		}
	}
	if (history){
		if (pushed)
			mValidHistory.push_back(processed);

		if (processed)
			SaveHistoryToDiskCache();
	}
	}

	void QueueProcessCommand(const char* command, bool history = true){
		if (!command || strlen(command) == 0)
			return;
		mQueue.push_back(processQData(command, history));
	}

	void SaveHistoryToDiskCache()
	{
		if (!mHistory.empty())
		{
			std::ofstream file(cacheFile);
			if (file.is_open())
			{
				assert(mHistory.size() == mValidHistory.size());
				auto rit = mValidHistory.rbegin(), rend = mValidHistory.rend();
				int index = mValidHistory.size();
				int count = 0;
				for (; rit != rend && count < 10; rit++)
				{
					if (*rit)
					{
						++count;
					}
					--index;

				}
				for (unsigned i = index; i < mHistory.size(); i++)
				{
					if (mValidHistory[i])
						file << mHistory[i] << std::endl;
				}
				file.close();
			}
		}
	}

	void AutoCompletion()
	{
		if (!mACMode)
		{
			mCandidates = mCandiData->GetCandidates(mInputString.c_str(), mCandiDepth);
			if (mCandidates.empty())
				return;

			mACMode = true;
			mCandiIndex = 0;
			mInputStringBackup = mInputString;
		}
		else
		{
			mCandiIndex++;
			if (mCandiIndex >= (int)mCandidates.size())
			{
				mCandiIndex = 0;
			}
		}

		if (!mCandidates.empty())
		{
			if (mCandiDepth == 0)
			{
				// parent
				mInputString = mCandidates[mCandiIndex];
			}
			else
			{
				// child
				size_t loc = mInputStringBackup.find(' ');
				assert(loc != std::string::npos);
				loc++;
				mInputString.assign(mInputStringBackup, 0, loc);
				mInputString.append(mCandidates[mCandiIndex]);
			}
			int aclen = mInputString.length();
			int inlen = mInputStringBackup.length();
			mCursorPos = inlen;
			EndHighlighting();
			StartHighlighting();
			mCursorPos = aclen;

			mInputStringw = AnsiToWide(mInputString.c_str(), mInputString.size());
		}
	}

	bool IsValidCharForInput(unsigned int chr)
	{
		if (chr == 0x60) // `
			return false;

		return true;
	}

	void StartHighlighting()
	{
		assert(!IsHighlighting());
		mHighlightStart = mCursorPos;
	}

	bool IsHighlighting() const { 
		return mHighlightStart != -1; 
	}

	void Highlighting(bool shiftkey)
	{
		if (shiftkey)
		{
			if (!IsHighlighting())
			{
				StartHighlighting();
			}
		}
		else
		{
			EndHighlighting();
		}
	}

	void EndHighlighting() { 
		mHighlightStart = -1; 
	}

	void EndAutoCompletion()
	{
		if (mACMode)
		{
			mACMode = false;
			mCursorPos = mInputString.size();
		}
	}

	void GetNextHistory()
	{
		if (mHistory.empty())
			return;

		mHistoryIndexBackup = mHistoryIndex;
		--mHistoryIndex;
		if (mHistoryIndex >= (int)mHistory.size() || mHistoryIndex<0)
		{
			mHistoryIndex = (int)mHistory.size() - 1;
		}

		mInputString = mHistory[mHistoryIndex].c_str();
		mCursorPos = 0;
		EndHighlighting();
		StartHighlighting();
		mCursorPos = mInputString.size();
		mInputStringw = AnsiToWide(mInputString.c_str(), mInputString.size());
	}

	void GetPrevHistory()
	{
		if (mHistory.empty())
			return;
		mHistoryIndexBackup = mHistoryIndex;
		++mHistoryIndex;
		if (mHistoryIndex >= (int)mHistory.size() || mHistoryIndex<0)
		{
			mHistoryIndex = 0;
		}

		mInputString = mHistory[mHistoryIndex].c_str();
		mCursorPos = 0;
		EndHighlighting();
		StartHighlighting();
		mCursorPos = mInputString.size();
		mInputStringw = AnsiToWide(mInputString.c_str(), mInputString.size());
	}

	void OnCVarChanged(CVar* cvar)
	{
		auto self = mSelf.lock();
		for (auto it : self->mObservers_){
			for (auto weak : it.second){
				auto observer = weak.lock();
				if (observer){
					observer->OnChangeCVar(cvar);
				}
			}
		}
	}

	void RegisterStdout(StdOutRedirect* p)
	{
		mStdOutRedirect = p;
	}

	void Clear()
	{
		mBufferw.clear();
	}
};

const float Console::Impl::sFontSize = 22.f;

//---------------------------------------------------------------------------
static ConsoleWeakPtr sConsole;
ConsolePtr Console::Create(){
	if (sConsole.expired()){
		auto console = ConsolePtr(new Console, [](Console* obj){ delete obj; });
		sConsole = console;
		console->mImpl->mSelf = sConsole;
		auto& inputMgr = InputManager::GetInstance();
		inputMgr.RegisterInputConsumer(console, IInputConsumer::Priority11_Console);
		return console;
	}

	return sConsole.lock();
}

Console& Console::GetInstance(){
	if (sConsole.expired()){
		Logger::Log(FB_ERROR_LOG_ARG, "The Console is deleted. Program will crash...");
	}
	return *sConsole.lock();
}

bool Console::HasInstance(){
	return !sConsole.expired();
}

Console::Console()
	: mImpl(new Impl)
{

}

void Console::SetRenderTargetSize(const Vec2I& size) {
	mImpl->SetRenderTargetSize(size);
}

void Console::RegisterCommand(ConsoleCommand* pCom) {
	mImpl->RegisterCommand(pCom);
}

void Console::UnregisterCommand(ConsoleCommand* pCom) {
	mImpl->UnregisterCommand(pCom);
}

void Console::RegisterVariable(CVar* cvar) {
	mImpl->RegisterVariable(cvar);
}

void Console::UnregisterVariable(CVar* cvar) {
	mImpl->UnregisterVariable(cvar);
}

void Console::AddCandidatesTo(const char* parent, const StringVector& candidates) {
	mImpl->AddCandidatesTo(parent, candidates);
}

void Console::Log(const char* szFmt, ...) {
	mImpl->Log(szFmt);
}

void Console::ProcessCommand(const char* command, bool history) {
	mImpl->ProcessCommand(command, history);
}

void Console::QueueProcessCommand(const char* command, bool history) {
	mImpl->QueueProcessCommand(command, history);
}

void Console::ToggleOpen() {
	mImpl->ToggleOpen();
}

void Console::Update() {
	mImpl->Update();
}

void Console::Render() {
	mImpl->Render();
}

void Console::RegisterStdout(StdOutRedirect* p) {
	mImpl->RegisterStdout(p);
}

void Console::Clear() {
	mImpl->Clear();
}

//---------------------------------------------------------------------------
// IInputConsumer
//---------------------------------------------------------------------------
void Console::ConsumeInput(IInputInjectorPtr injector) {
	mImpl->ConsumeInput(injector);
}

