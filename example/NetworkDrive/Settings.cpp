#include "stdafx.h"
#include "Settings.h"
#include <rapidjson\document.h>
#include <rapidjson\prettywriter.h>
#include "rapidjson\filereadstream.h"
#include "rapidjson\filewritestream.h"

using namespace rapidjson;

// rapidjson documentation
// http://miloyip.github.io/rapidjson/

typedef GenericDocument<UTF16<>> WDocument;
typedef GenericValue<UTF16<>> WValue;

Settings::Settings(){
    TCHAR buffer[MAX_PATH];
    GetModuleFileName(NULL, buffer, sizeof(buffer));
    CString szFileName(buffer);
    int index = szFileName.ReverseFind('\\');
    buffer[index] = L'\0';

    m_fileName = CString(buffer) + L"\\settings.json";
    m_drive = L"U:";
}

void
Settings::getDirs(std::vector<CString>& dirs){
    dirs = m_Dirs;
}

void Settings::addDir(CString dir){
    m_Dirs.push_back(dir);
}

void Settings::removeDir(CString dir){
    auto iter = std::find(m_Dirs.begin(), m_Dirs.end(), dir);
    if (iter != m_Dirs.end())
        m_Dirs.erase(iter);
}

void Settings::load(){
    WDocument d;
    d.SetObject();

    FILE* fp = NULL;
    errno_t err = _wfopen_s(&fp, m_fileName.GetBuffer(), L"rb");
    if (err != 0)
        return;

    char buffer[1024];
    FileReadStream is(fp, buffer, sizeof(buffer));
    d.ParseStream<0, UTF8<>, FileReadStream>(is);
    fclose(fp);

    if (d.HasMember(L"drive")){
        WValue& v = d[L"drive"];
        CString s = v.GetString();
    }

    if (d.HasMember(L"dirs")){
        WValue& a = d[L"dirs"];
        if (a.IsArray()){
            for (int i = 0, len = a.Size(); i < len; ++i){
                auto s = a[i].GetString();
                m_Dirs.push_back(s);
            }
        }
    }
}

void Settings::save(){
    WDocument d;
    d.SetObject();
    Document::AllocatorType & alloc = d.GetAllocator();

    WValue arr(kArrayType);
    for (CString& dir : m_Dirs) {
        WValue str(dir.GetBuffer(), dir.GetLength());
        arr.PushBack(str, alloc);
    }

    WValue szDrive(m_drive.GetBuffer(), m_drive.GetLength());
    d.AddMember(L"drive", szDrive, alloc);
    d.AddMember(L"dirs", arr, alloc);

    FILE* fp = NULL;
    errno_t err = _wfopen_s(&fp, m_fileName.GetBuffer(), L"wb");
    if (err != 0)
        return;

    char buffer[1024];
    FileWriteStream os(fp, buffer, sizeof(buffer));
    //Writer<FileWriteStream, UTF16<>> writer(os);
    PrettyWriter<FileWriteStream, UTF16<>> writer(os);

    d.Accept(writer);
    fclose(fp);
}
