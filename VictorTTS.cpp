/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\
|        VictorTTS, copyright © Victor Chen, http://www.cppfans.com/        |
\*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
#pragma hdrstop
#include "VictorTTS.h"
//---------------------------------------------------------------------------
namespace VictorTTS {
//---------------------------------------------------------------------------
TVictorTTS::TVictorTTS()
{
  _pVoice = NULL;
  _pToken = NULL;

  _bPaused = false;

  if(SUCCEEDED(_hr=CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void**)&_pVoice)))
   {
     if(SUCCEEDED(_hr=CoCreateInstance(CLSID_SpObjectToken, NULL, CLSCTX_ALL, IID_ISpObjectToken, (void**)&_pToken)))
       _hr=_pVoice->SetVoice(_pToken);
   }
  RefreshTokenList();
}
//---------------------------------------------------------------------------
TVictorTTS::~TVictorTTS()
{
  if(_pToken)
   {
     _pToken->Release();
     _pToken = NULL;
   }
  if(_pVoice)
   {
     _pVoice->Release();
     _pVoice = NULL;
   }
}
//---------------------------------------------------------------------------
String TVictorTTS::_GetTokenID(void)const
{
  String ws;
  if(_pToken)
   {
     wchar_t *lpws = NULL;
     if(SUCCEEDED(_pToken->GetId(&lpws)))
      {
        ws = lpws;
        CoTaskMemFree(lpws);
      }
   }
  return ws;
}
//---------------------------------------------------------------------------
void TVictorTTS::_SetTokenID(String id)
{
  if(_GetTokenID()!=id)
   {
     if(_pToken)
      {
        _pToken->Release();
        _pToken = NULL;
        if(SUCCEEDED(_hr=CoCreateInstance(CLSID_SpObjectToken, NULL, CLSCTX_ALL, IID_ISpObjectToken, (void **)&_pToken)))
          _hr=_pVoice->SetVoice(_pToken);
        _pToken->SetId(SPCAT_VOICES, id.c_str(), false);
      }
   }
}
//---------------------------------------------------------------------------
long TVictorTTS::_GetRate(void)const
{
  if(_pVoice)
   {
     long rt = 0;
     _pVoice->GetRate(&rt);
     return rt;
   }
  return 0;
}
//---------------------------------------------------------------------------
void TVictorTTS::_SetRate(long rt)
{
  if(_pVoice)_pVoice->SetRate(rt);
}
//---------------------------------------------------------------------------
unsigned short TVictorTTS::_GetVolume(void)const
{
  if(_pVoice)
   {
     unsigned short v = 100;
     _pVoice->GetVolume(&v);
     return v;
   }
  return 100;
}
//---------------------------------------------------------------------------
void TVictorTTS::_SetVolume(unsigned short v)
{
  if(_pVoice)_pVoice->SetVolume(v);
}
//---------------------------------------------------------------------------
bool TVictorTTS::_GetSpeaking(void)const
{
  if(_pVoice)
    return _pVoice->WaitUntilDone(0) == S_FALSE;
  return false;
}
//---------------------------------------------------------------------------
unsigned long TVictorTTS::Skip(long n)
{
  unsigned long v = 0;
  if(_pVoice)
    _pVoice->Skip(L"SENTENCE",n,&v);
  return v;
}
//---------------------------------------------------------------------------
bool TVictorTTS::Stop(void)
{
  if(_pVoice)
    return SUCCEEDED(_pVoice->Speak(L"", SPF_PURGEBEFORESPEAK, NULL));
  return false;
}
//---------------------------------------------------------------------------
bool TVictorTTS::SaveWav(String sText, String sFile)
{
  if(_pVoice)
   {
     ISpStream *pISpStream;
     HRESULT hResult = CoCreateInstance(CLSID_SpStream,NULL,CLSCTX_INPROC_SERVER, IID_ISpStream, (void**)&pISpStream);
     if(SUCCEEDED(hResult))
      {
        ISpStreamFormat *pISpStreamFormat;
        hResult = Voice->GetOutputStream(&pISpStreamFormat);
        if(SUCCEEDED(hResult))
         {
           GUID guidFormatId;
           WAVEFORMATEX *pCoMemWaveFormatEx;
           hResult = pISpStreamFormat->GetFormat(&guidFormatId,&pCoMemWaveFormatEx);
           if(SUCCEEDED(hResult))
            {
              hResult = pISpStream->BindToFile(sFile.c_str(), SPFM_CREATE_ALWAYS, &guidFormatId, pCoMemWaveFormatEx,0);
              if(SUCCEEDED(hResult))
               {
                 Voice->SetOutput(pISpStream, TRUE);
                 Voice->Speak(sText.c_str(), SPF_DEFAULT, NULL);
                 Voice->SetOutput(NULL, TRUE);
               }
              CoTaskMemFree(pCoMemWaveFormatEx);
            }
           pISpStreamFormat->Release();
         }
        pISpStream->Release();
      }
     return SUCCEEDED(hResult);
   }
  return false;
}
//---------------------------------------------------------------------------
bool TVictorTTS::Speak(String s)
{
  if(_pVoice)
   {
     Paused = false;
     return SUCCEEDED(_pVoice->Speak(s.c_str(), SPF_ASYNC|SPF_PURGEBEFORESPEAK, NULL));
   }
  return false;
}
//---------------------------------------------------------------------------
bool TVictorTTS::SpeakSync(String s)
{
  if(_pVoice)
   {
     Paused = false;
     return SUCCEEDED(_pVoice->Speak(s.c_str(), SPF_PURGEBEFORESPEAK, NULL));
   }
  return false;
}
//---------------------------------------------------------------------------
void TVictorTTS::_SetPaused(bool b)
{
  if(_pVoice)
   {
     if(b)
      {
        if(!_bPaused)
         {
           if(SUCCEEDED(_pVoice->Pause()))
             _bPaused = true;
         }
      }
     else
      {
        if(_bPaused)
         {
           if(SUCCEEDED(_pVoice->Resume()))
             _bPaused = false;
         }
      }
   }
}
//---------------------------------------------------------------------------
bool TVictorTTS::GetSpeakingWord(unsigned long &ulPos, unsigned long &ulLen)
{
  bool bOK = false;
  if(_pVoice)
   {
     SPVOICESTATUS vs;
     if(SUCCEEDED(_pVoice->GetStatus(&vs,NULL)))
      {
        ulPos = vs.ulInputWordPos;
        ulLen = vs.ulInputWordLen;
        bOK = true;
      }
   }
  return bOK;
}
//---------------------------------------------------------------------------
int TVictorTTS::RefreshTokenList(void)
{
  _wsDefTokenID = L"";
  _vtlTokenList.clear();
  if(_pVoice)
   {
     ISpObjectTokenCategory *pCategory = NULL;
     if(SUCCEEDED(CoCreateInstance(CLSID_SpObjectTokenCategory, NULL, CLSCTX_ALL, IID_ISpObjectTokenCategory, (void**)&pCategory)))
      {
        pCategory->SetId(SPCAT_VOICES, false);
        IEnumSpObjectTokens *pEnum = NULL;
        if(SUCCEEDED(pCategory->EnumTokens(NULL, NULL, &pEnum)))
         {
           ULONG ulCount = 0;
           if(SUCCEEDED(pEnum->GetCount(&ulCount))) //Get the number of voices
            {
              for(ULONG i=0; i<ulCount; i++)
               {
                 ISpObjectToken *pVoiceToken = NULL;
                 if(SUCCEEDED(pEnum->Next(1, &pVoiceToken, NULL)))
                  {
                    TVictorTokenItem vti;
                    wchar_t *lpws = NULL;
                    if(SUCCEEDED(pVoiceToken->GetId(&lpws)))
                     {
                       vti.TokenID = lpws;
                       CoTaskMemFree(lpws);
                     }
                    lpws = NULL;
                    if(SUCCEEDED(pVoiceToken->GetStringValue(NULL, &lpws)));
                     {
                       vti.TokenDesc = lpws;
                       CoTaskMemFree(lpws);
                     }
                    _vtlTokenList.push_back(vti);
                    pVoiceToken->Release();
                  }
               }
            }
           pEnum->Release();
         }
        wchar_t *lpws = NULL;
        if(SUCCEEDED(pCategory->GetDefaultTokenId(&lpws)))
         {
           _wsDefTokenID = lpws;
           CoTaskMemFree(lpws);
         }
        pCategory->Release();
      }
   }
  return _vtlTokenList.size();
}
//---------------------------------------------------------------------------
} // namespace VictorTTS
//---------------------------------------------------------------------------
