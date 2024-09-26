#pragma once

#include "zep/buffer.h"
#include "zep/editor.h"
#include "zep/syntax.h"
#include <zep/include/zep/mcommon/animation/timer.h>
#include <zep/include/zep/mcommon/file/path.h>

#include <zep/include/zep/filesystem.h>
#include <zep/include/zep/imgui/display_imgui.h>
#include <zep/include/zep/imgui/editor_imgui.h>
#include <zep/include/zep/mode_repl.h>
#include <zep/include/zep/mode_standard.h>
#include <zep/include/zep/mode_vim.h>
#include <zep/include/zep/regress.h>
#include <zep/include/zep/tab_window.h>
#include <zep/include/zep/theme.h>
#include <zep/include/zep/window.h>

Zep::NVec2f GetPixelScale();

struct ZepContainerImGui : public Zep::IZepComponent, public Zep::IZepReplProvider {
	ZepContainerImGui(const std::string& startupFilePath, const std::string& configPath)
	: spEditor(std::make_unique<Zep::ZepEditor_ImGui>(configPath, GetPixelScale())) {
		// ZepEditor_ImGui will have created the fonts for us; but we need to build
		// the font atlas
		/* auto fontPath = std::string("Cousine-Regular.ttf"); */
		auto fontPath = std::string("../../engine/resources/fonts/Roboto-Medium.ttf");
		auto& display = static_cast<Zep::ZepDisplay_ImGui&>(spEditor->GetDisplay());

		// Taken from `../../vendor/zep/src/syntax_providers.cpp` as Zep forgot to setup the Lua syntax factory
		// NOTE: I should submit a PR to Zep to fix it in the Zep codebase
		static std::unordered_set<std::string> lua_keywords = {
			"and", "break", "do", "", "else", "elseif", "end", "false", "for", "function", "if", "in", "", "local", "nil", "not", "or", "repeat", "return", "then", "true", "until", "while"
		};

		static std::unordered_set<std::string> lua_identifiers = {
			"assert", "collectgarbage", "dofile", "error", "getmetatable", "ipairs", "loadfile", "load", "loadstring", "next", "pairs", "pcall", "print", "rawequal", "rawlen", "rawget", "rawset",
			"select", "setmetatable", "tonumber", "tostring", "type", "xpcall", "_G", "_VERSION", "arshift", "band", "bnot", "bor", "bxor", "btest", "extract", "lrotate", "lshift", "replace",
			"rrotate", "rshift", "create", "resume", "running", "status", "wrap", "yield", "isyieldable", "debug", "getuservalue", "gethook", "getinfo", "getlocal", "getregistry", "getmetatable",
			"getupvalue", "upvaluejoin", "upvalueid", "setuservalue", "sethook", "setlocal", "setmetatable", "setupvalue", "traceback", "close", "flush", "input", "lines", "open", "output", "popen",
			"read", "tmpfile", "type", "write", "close", "flush", "lines", "read", "seek", "setvbuf", "write", "__gc", "__tostring", "abs", "acos", "asin", "atan", "ceil", "cos", "deg", "exp", "tointeger",
			"floor", "fmod", "ult", "log", "max", "min", "modf", "rad", "random", "randomseed", "sin", "sqrt", "string", "tan", "type", "atan2", "cosh", "sinh", "tanh",
			"pow", "frexp", "ldexp", "log10", "pi", "huge", "maxinteger", "mininteger", "loadlib", "searchpath", "seeall", "preload", "cpath", "path", "searchers", "loaded", "module", "require", "clock",
			"date", "difftime", "execute", "exit", "getenv", "remove", "rename", "setlocale", "time", "tmpname", "byte", "char", "dump", "find", "format", "gmatch", "gsub", "len", "lower", "match", "rep",
			"reverse", "sub", "upper", "pack", "packsize", "unpack", "concat", "maxn", "insert", "pack", "unpack", "remove", "move", "sort", "offset", "codepoint", "char", "len", "codes", "charpattern",
			"coroutine", "table", "io", "os", "string", "uint8_t", "bit32", "math", "debug", "package"
		};

		// lua
		spEditor.get()->RegisterSyntaxFactory({ ".lua" }, Zep::SyntaxProvider{ "lua", Zep::tSyntaxFactory([](Zep::ZepBuffer* pBuffer) {
			return std::make_shared<Zep::ZepSyntax>(*pBuffer, lua_keywords, lua_identifiers, Zep::ZepSyntaxFlags::CaseInsensitive);
		}) });

		auto& io = ImGui::GetIO();
		ImVector<ImWchar> ranges;
		ImFontGlyphRangesBuilder builder;
		builder.AddRanges(io.Fonts->GetGlyphRangesDefault()); // Add one of the default ranges
		builder.AddRanges(io.Fonts->GetGlyphRangesCyrillic()); // Add one of the default ranges
		builder.BuildRanges(&ranges); // Build the final result (ordered ranges with all the unique characters submitted)

		/* ImFontConfig cfg; */
		/* cfg.OversampleH = 4; */
		/* cfg.OversampleV = 4; */

		/* auto pImFont = ImGui::GetIO().Fonts->AddFontFromFileTTF(fontPath.c_str(), float(16), &cfg, ranges.Data); */
		/* auto fontPixelHeight = 16; */

		/* display.SetFont(Zep::ZepTextType::UI, std::make_shared<Zep::ZepFont_ImGui>(display, pImFont, fontPixelHeight)); */
		/* display.SetFont(Zep::ZepTextType::Text, std::make_shared<Zep::ZepFont_ImGui>(display, pImFont, fontPixelHeight)); */
		/* display.SetFont(Zep::ZepTextType::Heading1, std::make_shared<Zep::ZepFont_ImGui>(display, pImFont, int(fontPixelHeight * 1.75))); */
		/* display.SetFont(Zep::ZepTextType::Heading2, std::make_shared<Zep::ZepFont_ImGui>(display, pImFont, int(fontPixelHeight * 1.5))); */
		/* display.SetFont(Zep::ZepTextType::Heading3, std::make_shared<Zep::ZepFont_ImGui>(display, pImFont, int(fontPixelHeight * 1.25))); */

		/* unsigned int flags = 0; // ImGuiFreeType::NoHinting; */

		spEditor->RegisterCallback(this);

		/* Zep::ZepRegressExCommand::Register(*spEditor); */

		if (!startupFilePath.empty()) {
			spEditor->InitWithFileOrDir(startupFilePath);
		}

		// Create syntax highlighting for lua

		// File watcher not used on apple yet ; needs investigating as to why it doesn't compile/run
		// The watcher is being used currently to update the config path, but clients may want to do more interesting things
		// by setting up watches for the current dir, etc.
		/*fileWatcher.start([=](std::string path, FileStatus status) {
						if (spEditor)
						{
								ZLOG(DBG, "Config File Change: " << path);
								spEditor->OnFileChanged(spEditor->GetFileSystem().GetConfigPath() / path);
						}
				});*/
	}

	~ZepContainerImGui()
	{
	}

	void Destroy()
	{
		spEditor->UnRegisterCallback(this);
		spEditor.reset();
	}

	// Inherited via IZepComponent
	/* virtual void Notify(std::shared_ptr<ZepMessage> message) override */
	/* { */
	/* 	if (message->messageId == Msg::GetClipBoard) */
	/* 	{ */
	/* 		clip::get_text(message->str); */
	/* 		message->handled = true; */
	/* 	} */
	/* 	else if (message->messageId == Msg::SetClipBoard) */
	/* 	{ */
	/* 		clip::set_text(message->str); */
	/* 		message->handled = true; */
	/* 	} */
	/* 	else if (message->messageId == Msg::RequestQuit) */
	/* 	{ */
	/* 		quit = true; */
	/* 	} */
	/* 	else if (message->messageId == Msg::ToolTip) */
	/* 	{ */
	/* 		auto spTipMsg = std::static_pointer_cast<ToolTipMessage>(message); */
	/* 		if (spTipMsg->location.Valid() && spTipMsg->pBuffer) */
	/* 		{ */
	/* 			auto pSyntax = spTipMsg->pBuffer->GetSyntax(); */
	/* 			if (pSyntax) */
	/* 			{ */
	/* 				if (pSyntax->GetSyntaxAt(spTipMsg->location).foreground == ThemeColor::Identifier) */
	/* 				{ */
	/* 					auto spMarker = std::make_shared<RangeMarker>(*spTipMsg->pBuffer); */
	/* 					spMarker->SetDescription("This is an identifier"); */
	/* 					spMarker->SetHighlightColor(ThemeColor::Identifier); */
	/* 					spMarker->SetTextColor(ThemeColor::Text); */
	/* 					spTipMsg->spMarker = spMarker; */
	/* 					spTipMsg->handled = true; */
	/* 				} */
	/* 				else if (pSyntax->GetSyntaxAt(spTipMsg->location).foreground == ThemeColor::Keyword) */
	/* 				{ */
	/* 					auto spMarker = std::make_shared<RangeMarker>(*spTipMsg->pBuffer); */
	/* 					spMarker->SetDescription("This is a keyword"); */
	/* 					spMarker->SetHighlightColor(ThemeColor::Keyword); */
	/* 					spMarker->SetTextColor(ThemeColor::Text); */
	/* 					spTipMsg->spMarker = spMarker; */
	/* 					spTipMsg->handled = true; */
	/* 				} */
	/* 			} */
	/* 		} */
	/* 	} */
	/* } */

	virtual Zep::ZepEditor& GetEditor() const override
	{
		return *spEditor;
	}

	bool quit = false;
	std::unique_ptr<Zep::ZepEditor_ImGui> spEditor;
};
