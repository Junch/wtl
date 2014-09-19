#include "stdafx.h"
#include "Utils.h"
#include <Winnetwk.h>

#pragma comment(lib, "mpr.lib")

NetworkDrive::NetworkDrive(TCHAR* szDrive) :m_szDrive(szDrive){

}

CString NetworkDrive::GetConnection(){
    TCHAR buffer[MAX_PATH];
    DWORD len = MAX_PATH;
    DWORD ret = WNetGetConnection(m_szDrive.GetBuffer(), buffer, &len);

    CString sz;
    if (ret == NO_ERROR)
        sz = buffer;

    return sz;
}

CString GetLastErrorAsString(){
    //Get the error message, if any.
    DWORD errorMessageID = ::GetLastError();
    if (errorMessageID == 0)
        return "No error message has been recorded";

    LPTSTR messageBuffer = nullptr;
    size_t size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&messageBuffer, 0, NULL);

    CString message(messageBuffer, size);

    //Free the buffer.
    LocalFree(messageBuffer);

    return message;
}

void NetworkDrive::AddConnection(TCHAR* folder){
    NETRESOURCE nrs;
    memset(&nrs, 0, sizeof(NETRESOURCE));
    nrs.lpLocalName = m_szDrive.GetBuffer();
    nrs.lpRemoteName = folder;

    CString conn = GetConnection();
    if (!conn.IsEmpty()) {
        CString fmt;
        fmt.Format(L"The %s has been mapped to %s. You need unmap it first.", m_szDrive.GetBuffer(), conn.GetBuffer());
        ::MessageBox(NULL, fmt.GetBuffer(), L"Warning", MB_OK | MB_ICONERROR);
        return;
    }

    DWORD dwd = WNetAddConnection2(&nrs, NULL, NULL, CONNECT_UPDATE_PROFILE);
    if (dwd != NO_ERROR){
        CString fmt = GetLastErrorAsString();
        ::MessageBox(NULL, fmt.GetBuffer(), L"Warning", MB_OK | MB_ICONWARNING);
    }
}

void NetworkDrive::CancelConnection(){
    WNetCancelConnection2(m_szDrive, CONNECT_UPDATE_PROFILE, TRUE);
}

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
Setting::getDirs(std::vector<CString>& dirs){
    dirs = m_Dirs;
}

void Setting::addDir(CString dir){
    m_Dirs.push_back(dir);
}

void Setting::removeDir(CString dir){
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

void Setting::load(){
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

void Setting::save(){
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
