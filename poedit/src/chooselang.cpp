
/*

    poedit, a wxWindows i18n catalogs editor

    ---------------
      chooselang.cpp
    
      Language chooser
    
      (c) Vaclav Slavik, 2003

*/


#ifdef __GNUG__
#pragma implementation
#endif

#include <wx/wxprec.h>

#include <wx/wx.h>
#include <wx/config.h>

#include "chooselang.h"

static void SaveUILanguage(wxLanguage lang)
{
    if (lang == wxLANGUAGE_UNKNOWN)
        return;
    if (lang == wxLANGUAGE_DEFAULT)
        wxConfig::Get()->Write(_T("ui_language"), _T("default"));
    else
        wxConfig::Get()->Write(_T("ui_language"),
                               wxLocale::GetLanguageInfo(lang)->CanonicalName);
}

wxLanguage GetUILanguage()
{
#ifdef __UNIX__
    return wxLANGUAGE_DEFAULT;
#else
    wxLanguage lang(wxLANGUAGE_DEFAULT);
    wxString lng = wxConfig::Get()->Read(_T("ui_language"));
    if (lng.empty())
    {
        lang = ChooseLanguage();
        if (lang != wxLANGUAGE_UNKNOWN)
            SaveUILanguage(lang);
        else
            lang = wxLANGUAGE_DEFAULT;
    }
    else if (lng != _T("default"))
    {
        const wxLanguageInfo *info = wxLocale::FindLanguageInfo(lng);
        if (info != NULL)
            lang = (wxLanguage)info->Language;
        else
            wxLogError(_("Uknown locale code '%s' in registry."), lng.c_str());
    }
    return lang;
#endif
}

#ifndef __UNIX__
wxLanguage ChooseLanguage()
{
    struct LangInfo
    {
        const wxChar *name;
        wxLanguage code;
    };

    LangInfo langs[] = 
    {
        { _("(Use default language)"), wxLANGUAGE_DEFAULT },

        { _T("Bulgarian"), wxLANGUAGE_BULGARIAN },
        { _T("Catalan"), wxLANGUAGE_CATALAN },
        { _T("Chinese (Traditional)"), wxLANGUAGE_CHINESE_TRADITIONAL },
        { _T("Chinese (Simplified)"), wxLANGUAGE_CHINESE_SIMPLIFIED },
        { _T("Croatian"), wxLANGUAGE_CROATIAN },
        { _T("Czech"), wxLANGUAGE_CZECH },
        { _T("Dutch"), wxLANGUAGE_DUTCH },
        { _T("English"), wxLANGUAGE_ENGLISH },
        { _T("Estonian"), wxLANGUAGE_ESTONIAN },
        { _T("French"), wxLANGUAGE_FRENCH },
        { _T("Georgian"), wxLANGUAGE_GEORGIAN },
        { _T("German"), wxLANGUAGE_GERMAN },
        { _T("Greek"), wxLANGUAGE_GREEK },
        { _T("Islandic"), wxLANGUAGE_ICELANDIC },
        { _T("Italian"), wxLANGUAGE_ITALIAN },
        { _T("Japanese"), wxLANGUAGE_JAPANESE },
        { _T("Latvian"), wxLANGUAGE_LATVIAN },
        { _T("Norwegian Nynorsk"), wxLANGUAGE_NORWEGIAN_NYNORSK },
        { _T("Norwegian Bokm�l"), wxLANGUAGE_NORWEGIAN_BOKMAL },
        { _T("Polish"), wxLANGUAGE_POLISH },
        { _T("Romanian"), wxLANGUAGE_ROMANIAN },
        { _T("Russian"), wxLANGUAGE_RUSSIAN },
        { _T("Slovak"), wxLANGUAGE_SLOVAK },
        { _T("Swedish"), wxLANGUAGE_SWEDISH },
        { _T("Turkish"), wxLANGUAGE_TURKISH },
        { _T("Tamil"), wxLANGUAGE_TAMIL },
        { _T("Portuguese (Brazilian)"), wxLANGUAGE_PORTUGUESE_BRAZILIAN },
        { _T("Spanish"), wxLANGUAGE_SPANISH },

        { NULL, wxLANGUAGE_UNKNOWN }
    };

    wxArrayString arr;
    for (int i = 0; langs[i].name; i++)
        arr.Add(langs[i].name);

    int choice = wxGetSingleChoiceIndex(
            _("Select your prefered language"),
            _("Language selection"),
            arr);
    if (choice == -1)
        return wxLANGUAGE_UNKNOWN;
    else
        return langs[choice].code;
}

void ChangeUILanguage()
{
    wxLanguage lang = ChooseLanguage();
    if (lang == wxLANGUAGE_UNKNOWN)
        return;
    SaveUILanguage(lang);
    wxMessageBox(_("You must restart poEdit for this change to take effect."),
                 _T("poEdit"),
                 wxOK | wxCENTRE | wxICON_INFORMATION);
}

#endif
