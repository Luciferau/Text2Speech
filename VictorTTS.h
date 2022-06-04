/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\
|        VictorTTS, copyright © Victor Chen, http://www.cppfans.com/        |
\*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
#ifndef VictorTTSH
#define VictorTTSH
//---------------------------------------------------------------------------
#include <vcl.h>
#include <sapi.h>
#include <vector>
//---------------------------------------------------------------------------
namespace VictorTTS {
//---------------------------------------------------------------------------
class TVictorTokenItem
{
public:
  String TokenID;
  String TokenDesc;

  TVictorTokenItem &operator =(const TVictorTokenItem &t){ TokenID=t.TokenID; TokenDesc=t.TokenDesc; return *this; }

  TVictorTokenItem(){}
  TVictorTokenItem(const TVictorTokenItem &t){ TokenID=t.TokenID; TokenDesc=t.TokenDesc; }
};
typedef std::vector<TVictorTokenItem>TVictorTokenList;
//---------------------------------------------------------------------------
class TVictorTTS
{
public:
  __property String           TokenID     = { read = _GetTokenID, write = _SetTokenID };
  __property String           DefTokenID  = { read = _wsDefTokenID                    };
  __property TVictorTokenList TokenList   = { read = _vtlTokenList                    };
  __property ISpObjectToken  *Token       = { read = _pToken                          };
  __property ISpVoice        *Voice       = { read = _pVoice                          };
  __property long             Rate        = { read = _GetRate   , write = _SetRate    }; // The supported values are in the range -10 to 10.
  __property unsigned short   Volume      = { read = _GetVolume , write = _SetVolume  }; // Volume levels are specified in percentage values ranging from 0 to 100.
  __property bool             Speaking    = { read = _GetSpeaking                     };
  __property bool             Paused      = { read = _bPaused   , write = _SetPaused  };

  bool SaveWav(String sText, String sFile);
  bool Speak(String);
  bool SpeakSync(String);
  unsigned long Skip(long);
  bool Stop(void);
  int RefreshTokenList(void);
  bool GetSpeakingWord(unsigned long &ulPos, unsigned long &ulLen);

  TVictorTTS();
  virtual ~TVictorTTS();
private:
  ISpVoice *_pVoice;
  ISpObjectToken *_pToken;
  String _wsDefTokenID;
  TVictorTokenList _vtlTokenList;
  HRESULT _hr;
  bool _bPaused;
  String _GetTokenID(void)const;
  void _SetTokenID(String);
  long _GetRate(void)const;
  void _SetRate(long);
  unsigned short _GetVolume(void)const;
  void _SetVolume(unsigned short);
  bool _GetSpeaking(void)const;
  void _SetPaused(bool);
};
//---------------------------------------------------------------------------
} using namespace VictorTTS;
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
