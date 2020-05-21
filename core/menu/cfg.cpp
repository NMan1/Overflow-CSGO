#include <fstream>
#include <ShlObj.h>
#include "cfg.h"
#include "archivex.hpp"
#include "menu.h"

void Menu::run(const char* name) noexcept {
	PWSTR pathToDocuments;
	if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &pathToDocuments))) {
		path = pathToDocuments;
		path /= name;
		path /= "configs";
		CoTaskMemFree(pathToDocuments);
	}

	if (!std::filesystem::is_directory(path)) {
		std::filesystem::remove(path);
		std::filesystem::create_directory(path);
	}

	std::transform(std::filesystem::directory_iterator{ path },
		std::filesystem::directory_iterator{ },
		std::back_inserter(configs),
		[](const auto& entry) { return entry.path().filename().string(); });
	console::log("[setup] default config loaded!\n");
}

void Menu::load(size_t id) noexcept {
	if (!std::filesystem::is_directory(path)) {
		std::filesystem::remove(path);
		std::filesystem::create_directory(path);
	}

	std::ifstream in{ path / configs[id] };

	if (!in.good())
		return;

	ArchiveX<std::ifstream>{ in } >> config;
	in.close();
}

void Menu::save(size_t id) const noexcept {
	if (!std::filesystem::is_directory(path)) {
		std::filesystem::remove(path);
		std::filesystem::create_directory(path);
	}

	std::ofstream out{ path / configs[id] };

	if (!out.good())
		return;

	ArchiveX<std::ofstream>{ out } << config;
	out.close();
}

bool Menu::add(const char* name) noexcept {
	if (*name && std::find(std::cbegin(configs), std::cend(configs), name) == std::cend(configs))
	{
		configs.emplace_back(name);
		return true;
	}
	return false;
}

void Menu::remove(size_t id) noexcept {
	std::filesystem::remove(path / configs[id]);
	configs.erase(configs.cbegin() + id);
}

void Menu::rename(size_t item, const char* newName) noexcept {
	std::filesystem::rename(path / configs[item], path / newName);
	configs[item] = newName;
}

void Menu::reset() noexcept {
	config = { };
}