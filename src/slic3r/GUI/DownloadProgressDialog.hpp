#ifndef slic3r_DownloadProgressDialog_hpp_
#define slic3r_DownloadProgressDialog_hpp_

#include <string>
#include <unordered_map>

#include "GUI_Utils.hpp"
#include <wx/dialog.h>
#include <wx/font.h>
#include <wx/bitmap.h>
#include <wx/msgdlg.h>
#include <wx/richmsgdlg.h>
#include <wx/textctrl.h>
#include <wx/statline.h>
#include "Widgets/Button.hpp"
#include "BBLStatusBar.hpp"
#include "BBLStatusBarSend.hpp"
#include "Jobs/UpgradeNetworkJob.hpp"

class wxBoxSizer;
class wxCheckBox;
class wxStaticBitmap;

#define MSG_DIALOG_BUTTON_SIZE wxSize(FromDIP(58), FromDIP(24))
#define MSG_DIALOG_MIDDLE_BUTTON_SIZE wxSize(FromDIP(76), FromDIP(24))
#define MSG_DIALOG_LONG_BUTTON_SIZE wxSize(FromDIP(90), FromDIP(24))


namespace Slic3r {
namespace GUI {

class DownloadProgressDialog : public DPIDialog
{
protected:
    bool Show(bool show) override;
    void on_close(wxCloseEvent& event);

public:
    DownloadProgressDialog(wxString title);
    ~DownloadProgressDialog();

    void on_dpi_changed(const wxRect &suggested_rect) override;
    void update_release_note(std::string release_note, std::string version);

	std::shared_ptr<BBLStatusBarSend> m_status_bar;
    std::shared_ptr<UpgradeNetworkJob> m_upgrade_job { nullptr };
    wxPanel *                         m_panel_download;


};


}
}

#endif
