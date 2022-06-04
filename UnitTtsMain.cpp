//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "VictorTTS.h"
#include "UnitTtsMain.h"
#include <IniFiles.hpp>
#include <SysUtils.hpp>
#include <IOUtils.hpp>
#include <shlobj.h>
#include <memory>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
  _iFromCaret = 0;
  std::auto_ptr<TMemIniFile>ini(new TMemIniFile(IniFileName));
  TTS.TokenID = ini->ReadString(L"OPTIONS",L"TokenID",TTS.DefTokenID);
  ComboBox1->Items->Clear();
  int nTokenCount = TTS.TokenList.size();
  for(int i=0; i<nTokenCount; i++)
   {
     ComboBox1->Items->Add(TTS.TokenList[i].TokenDesc);
     if(TTS.TokenList[i].TokenID==TTS.TokenID)
       ComboBox1->ItemIndex = i;
   }
  SetRate(ini->ReadInteger(L"OPTIONS",L"SpeakRate",0));
  SetVolume(ini->ReadInteger(L"OPTIONS",L"SpeakVolume",100));
}
//---------------------------------------------------------------------------
void TForm1::SetRate(long r)
{
  TrackBarRate->Position = r;
  LabelRate->Caption = UnicodeString().sprintf(L"%+ld",r);
  if(TTS.Rate != r)
   {
     TTS.Rate = r;
     TDirectory::CreateDirectory(IniFilePath);
     std::auto_ptr<TMemIniFile>ini(new TMemIniFile(IniFileName));
     ini->WriteInteger(L"OPTIONS",L"SpeakRate",TTS.Rate);
     ini->UpdateFile();
   }
}
//---------------------------------------------------------------------------
void TForm1::SetVolume(unsigned short v)
{
  TrackBarVolume->Position = v;
  LabelVolume->Caption = UnicodeString().sprintf(L"%d%%",v);
  if(TTS.Volume != v)
   {
     TTS.Volume = v;
     TDirectory::CreateDirectory(IniFilePath);
     std::auto_ptr<TMemIniFile>ini(new TMemIniFile(IniFileName));
     ini->WriteInteger(L"OPTIONS",L"SpeakVolume",TTS.Volume);
     ini->UpdateFile();
   }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ComboBox1Select(TObject *Sender)
{
  int i = ComboBox1->ItemIndex;
  int nTokenCount = TTS.TokenList.size();
  if(i>=0 && i<nTokenCount)
   {
     TTS.TokenID = TTS.TokenList[i].TokenID;
     TDirectory::CreateDirectory(IniFilePath);
     std::auto_ptr<TMemIniFile>ini(new TMemIniFile(IniFileName));
     ini->WriteString(L"OPTIONS",L"TokenID",TTS.TokenID);
     ini->UpdateFile();
   }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::TrackBarRateChange(TObject *Sender)
{
  static bool bChanging = false;
  if(!bChanging)
   {
     try
      {
        bChanging = true;
        SetRate(TrackBarRate->Position);
      }
     __finally
      {
        bChanging = false;
      }
   }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::TrackBarVolumeChange(TObject *Sender)
{
  static bool bChanging = false;
  if(!bChanging)
   {
     try
      {
        bChanging = true;
        SetVolume(TrackBarVolume->Position);
      }
     __finally
      {
        bChanging = false;
      }
   }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
  if(!TTS.Speaking)
   {
     if(!ButtonSaveWav->Enabled) ButtonSaveWav->Enabled = true;
     if(!ButtonSpeak->Enabled) ButtonSpeak->Enabled = true;
     if(!ButtonCaret->Enabled) ButtonCaret->Enabled = true;
     if(ButtonStop->Enabled) ButtonStop->Enabled = false;
     if(ButtonPause->Enabled) ButtonPause->Enabled = false;
     if(ButtonResume->Enabled) ButtonResume->Enabled = false;
   }
  else
   {
     if(ButtonSaveWav->Enabled) ButtonSaveWav->Enabled = false;
     if(ButtonSpeak->Enabled) ButtonSpeak->Enabled = false;
     if(ButtonCaret->Enabled) ButtonCaret->Enabled = false;
     if(!ButtonStop->Enabled) ButtonStop->Enabled = true;
     unsigned long iPos, nCount;
     if(TTS.GetSpeakingWord(iPos, nCount))
      {
        iPos += _iFromCaret;
        if(Memo1->SelStart != (int)iPos || Memo1->SelLength != (int)nCount)
         {
           Memo1->SelStart = iPos;
           Memo1->SelLength = nCount;
         }
      }
   }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ButtonSpeakClick(TObject *Sender)
{
  _iFromCaret = 0;
  ButtonSaveWav->Enabled = false;
  ButtonSpeak->Enabled = false;
  ButtonCaret->Enabled = false;
  ButtonStop->Enabled = true;
  ButtonPause->Enabled = true;
  ButtonResume->Enabled = false;
  TTS.Speak(Memo1->Text);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ButtonCaretClick(TObject *Sender)
{
  _iFromCaret = Memo1->SelStart;
  ButtonSaveWav->Enabled = false;
  ButtonSpeak->Enabled = false;
  ButtonCaret->Enabled = false;
  ButtonStop->Enabled = true;
  ButtonPause->Enabled = true;
  ButtonResume->Enabled = false;
  TTS.Speak(Memo1->Text.SubString(_iFromCaret+1,Memo1->Text.Length()));
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ButtonStopClick(TObject *Sender)
{
  TTS.Stop();
  ButtonSaveWav->Enabled = true;
  ButtonSpeak->Enabled = true;
  ButtonCaret->Enabled = true;
  ButtonStop->Enabled = false;
  ButtonPause->Enabled = false;
  ButtonResume->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ButtonPauseClick(TObject *Sender)
{
  ButtonPause->Enabled = false;
  ButtonResume->Enabled = true;
  TTS.Paused = true;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ButtonResumeClick(TObject *Sender)
{
  ButtonPause->Enabled = true;
  ButtonResume->Enabled = false;
  TTS.Paused = false;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ButtonSaveWavClick(TObject *Sender)
{
  if(SaveDlgWav->Execute())
   {
     UnicodeString sFileName = SaveDlgWav->FileName;
     if(TTS.SaveWav(Memo1->Text, sFileName))
       ::MessageBox(Handle,(L"保存 wav 文件成功:\r\n"+sFileName).c_str(),Caption.c_str(),MB_OK|MB_ICONINFORMATION);
     else
       ::MessageBox(Handle,L"保存 wav 文件失败！",Caption.c_str(),MB_OK|MB_ICONEXCLAMATION);
   }
}
//---------------------------------------------------------------------------
UnicodeString TForm1::GetIniFilePath(void) const
{
  UnicodeString sHomePath;
#if(__BORLANDC__>=0x0720) // C++Builder 10.1 Berlin or Later
  sHomePath = TPath::GetHomePath();
#else // Earlier Versions
  TCHAR szhp[MAX_PATH];
  if(SHGetFolderPath(0, CSIDL_APPDATA, 0, 0, szhp)==S_OK)
    sHomePath = szhp;
#endif
  UnicodeString sIniPath;
  if(!sHomePath.IsEmpty())
   {
     sIniPath = sHomePath      + TPath::DirectorySeparatorChar +
                L"Victor Chen" + TPath::DirectorySeparatorChar +
                TPath::GetFileNameWithoutExtension(Application->ExeName);
   }
  else
   {
     sIniPath = TPath::GetDirectoryName(Application->ExeName);
   }
  return sIniPath;
}
//---------------------------------------------------------------------------
UnicodeString TForm1::GetIniFileName(void) const
{
  UnicodeString sIniName = TPath::GetFileNameWithoutExtension(Application->ExeName) + L".ini";
  return IniFilePath + TPath::DirectorySeparatorChar + sIniName;
}
//---------------------------------------------------------------------------
