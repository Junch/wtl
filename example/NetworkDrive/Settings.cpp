#include "stdafx.h"
#include "Settings.h"
#include <rapidjson\document.h>
#include <rapidjson\prettywriter.h>
#include <rapidjson\writer.h>
#include <rapidjson\stringbuffer.h>
#include <rapidjson\encodings.h>
#include "rapidjson\filereadstream.h"
#include "rapidjson\filewritestream.h"
#include "rapidjson\encodedstream.h"
#include <string>
#include <fstream>
using namespace rapidjson;

typedef GenericDocument<UTF16<>> WDocument;
typedef GenericValue<UTF16<>> WValue;

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

//void Utf8ToUnicode(WCHAR** dest, const char* src)
//{
//    ATLASSERT(dest != NULL || src != NULL);
//    int unicodeLen = ::MultiByteToWideChar(CP_UTF8, 0, src, -1, NULL, 0) + 1;
//
//    *dest = new WCHAR[unicodeLen];
//    ::MultiByteToWideChar(CP_UTF8, 0, src, -1, *dest, unicodeLen);
//}

void Settings::load(){
    WDocument d;
    d.SetObject();

    std::string ss;
    std::ifstream testfile;
    testfile.open("d:\\hello.json", std::ios::in | std::ios::binary);
    testfile >> ss;
    testfile.close();

    //The code do the same as as the StringStream below.
    //wchar_t *dest = NULL;
    //Utf8ToUnicode(&dest, ss.c_str());
    //d.Parse<0>(dest);
    //delete[] dest;

    StringStream s(ss.c_str());
    GenericStringBuffer<UTF16<>> buffer;
    Writer<GenericStringBuffer<UTF16<>>, UTF16<>, UTF16<> > writer(buffer);
    GenericReader<UTF8<>, UTF16<>> reader;
    reader.Parse<0>(s, writer);

    d.Parse<0>(buffer.GetString());
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
    d.AddMember(L"drive", L"U:", alloc);
    d.AddMember(L"dirs", arr, alloc);

    StringBuffer buffer;
    Writer<StringBuffer, UTF16<>> writer(buffer);
    //PrettyWriter<StringBuffer, UTF16<>> writer(buffer);
    d.Accept(writer);
    const char* r = buffer.GetString();

    std::wofstream of;
    of.open("d:\\hello.json", std::ios::out | std::ios::binary);
    of << r;
    of.close();
}
