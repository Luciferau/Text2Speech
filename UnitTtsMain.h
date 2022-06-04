//---------------------------------------------------------------------------

#ifndef UnitTtsMainH
#define UnitTtsMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
    TComboBox *ComboBox1;
    TBitBtn *ButtonSpeak;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label4;
    TTrackBar *TrackBarRate;
    TTrackBar *TrackBarVolume;
    TLabel *LabelRate;
    TLabel *LabelVolume;
    TTimer *Timer1;
    TBitBtn *ButtonStop;
    TMemo *Memo1;
    TBitBtn *ButtonPause;
    TBitBtn *ButtonResume;
    TBitBtn *ButtonSaveWav;
    TSaveDialog *SaveDlgWav;
    TBitBtn *ButtonCaret;
    void __fastcall ButtonSpeakClick(TObject *Sender);
    void __fastcall ComboBox1Select(TObject *Sender);
    void __fastcall TrackBarRateChange(TObject *Sender);
    void __fastcall TrackBarVolumeChange(TObject *Sender);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall ButtonStopClick(TObject *Sender);
    void __fastcall ButtonPauseClick(TObject *Sender);
    void __fastcall ButtonResumeClick(TObject *Sender);
    void __fastcall ButtonSaveWavClick(TObject *Sender);
    void __fastcall ButtonCaretClick(TObject *Sender);
private:	// User declarations
    TVictorTTS TTS;
    int _iFromCaret;
    void SetRate(long r);
    void SetVolume(unsigned short v);
    UnicodeString GetIniFileName(void) const;
    UnicodeString GetIniFilePath(void) const;
public:		// User declarations
    __property UnicodeString IniFilePath = { read = GetIniFilePath };
    __property UnicodeString IniFileName = { read = GetIniFileName };
    __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
