#pragma once

#include "../../include/ApiStruct.h"

#ifdef _WIN64
#pragma comment(lib, "../../include/Wind/win64/WAPIWrapperCpp.lib")
#pragma comment(lib, "../../lib/Queue_x64.lib")
#else
#pragma comment(lib, "../../include/Wind/win32/WAPIWrapperCpp.lib")
#pragma comment(lib, "../../lib/Queue_x86.lib")
#endif

#include <set>
#include <string>
#include <atomic>
#include <mutex>
#include <map>
#include <future> 

using namespace std;

class CMsgQueue;

class CMdUserApi
{
    enum RequestType
    {
        Start = 100,
        Stop,
        QueryInstrument,
    };

public:
    CMdUserApi(void);
    virtual ~CMdUserApi(void);

    void Register(void* pCallback, void* pClass);

    void Connect();
    void Disconnect();

    void Subscribe(const string& szInstrumentIDs, const string& szExchangeID);
    void Unsubscribe(const string& szInstrumentIDs, const string& szExchangeID);

private:
    friend void* __stdcall Query(char type, void* pApi1, void* pApi2, double double1, double double2, void* ptr1, int size1, void* ptr2, int size2, void* ptr3, int size3);
    void QueryInThread(char type, void* pApi1, void* pApi2, double double1, double double2, void* ptr1, int size1, void* ptr2, int size2, void* ptr3, int size3);

    void WindStart();
    void WindStop();
    bool WindConnected();
    void WindQryInstrument();

    //��������
    void Subscribe(const set<string>& instrumentIDs, const string& szExchangeID);

private:
    mutex m_csMapInstrumentIDs;
    mutex m_csMapQuoteInstrumentIDs;

    //���ڶ��ĵĺ�Լ
    map<string, set<string> > m_mapInstrumentIDs;
    //���ڶ��ĵĺ�Լ
    map<string, set<string> > m_mapQuoteInstrumentIDs;

    //��Ϣ����ָ��
    CMsgQueue* m_msgQueue;
    CMsgQueue* m_msgQueue_Query;
    void* m_pClass;
    bool m_CoInitialized;
    HANDLE m_ExitEvent;
};
