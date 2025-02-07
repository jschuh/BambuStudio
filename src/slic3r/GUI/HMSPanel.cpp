#include "HMS.hpp"
#include "HMSPanel.hpp"
#include <slic3r/GUI/Widgets/Label.hpp>
#include <slic3r/GUI/I18N.hpp>
#include "GUI.hpp"
#include "GUI_App.hpp"

namespace Slic3r {
namespace GUI {

#define HMS_NOTIFY_ITEM_TEXT_SIZE wxSize(FromDIP(730), -1)
#define HMS_NOTIFY_ITEM_SIZE wxSize(-1, FromDIP(80))

HMSNotifyItem::HMSNotifyItem(wxWindow *parent, HMSItem& item)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL) 
    , m_hms_item(item)
    , m_url(get_hms_wiki_url(item.get_long_error_code()))
{
    init_bitmaps();

    this->SetBackgroundColour(*wxWHITE);

    auto main_sizer = new wxBoxSizer(wxVERTICAL);

    m_panel_hms = new wxPanel(this, wxID_ANY, wxDefaultPosition, HMS_NOTIFY_ITEM_SIZE, wxTAB_TRAVERSAL);
    auto m_panel_sizer = new wxBoxSizer(wxVERTICAL);

    auto m_panel_sizer_inner = new wxBoxSizer(wxHORIZONTAL);

    m_bitmap_notify = new wxStaticBitmap(m_panel_hms, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0);
    m_bitmap_notify->SetBitmap(get_notify_bitmap());

    m_hms_content = new wxStaticText(m_panel_hms, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxST_ELLIPSIZE_END);
    m_hms_content->SetSize(HMS_NOTIFY_ITEM_TEXT_SIZE);
    m_hms_content->SetMinSize(HMS_NOTIFY_ITEM_TEXT_SIZE);
    m_hms_content->SetLabelText(_L(wxGetApp().get_hms_query()->query_hms_msg(m_hms_item.get_long_error_code())));
    m_hms_content->Wrap(HMS_NOTIFY_ITEM_TEXT_SIZE.GetX());

    m_bitmap_arrow = new wxStaticBitmap(m_panel_hms, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0);
    if (!m_url.empty())
        m_bitmap_arrow->SetBitmap(m_img_arrow);

    m_panel_sizer_inner->Add(m_bitmap_notify, 0, wxALIGN_CENTER_VERTICAL, 0);
    m_panel_sizer_inner->AddSpacer(FromDIP(8));
    m_panel_sizer_inner->Add(m_hms_content, 0, wxALIGN_CENTER_VERTICAL, 0);
    m_panel_sizer_inner->AddStretchSpacer();
    m_panel_sizer_inner->Add(m_bitmap_arrow, 0, wxALIGN_CENTER_VERTICAL, 0);

    m_panel_sizer->Add(m_panel_sizer_inner, 1, wxEXPAND | wxALL, FromDIP(20));

    m_staticline = new wxPanel(m_panel_hms, wxID_DELETE, wxDefaultPosition, wxSize(-1, FromDIP(1)));
    m_staticline->SetBackgroundColour(wxColour(238, 238, 238));

    m_panel_sizer->Add(m_staticline, 0, wxLEFT | wxRIGHT | wxEXPAND, FromDIP(20));

    m_panel_hms->SetSizer(m_panel_sizer);
    m_panel_hms->Layout();
    m_panel_sizer->Fit(m_panel_hms);

    main_sizer->Add(m_panel_hms, 0, wxEXPAND, 0);

    this->SetSizer(main_sizer);
    this->Layout();

    m_hms_content->Bind(wxEVT_ENTER_WINDOW, [this](wxMouseEvent &e) {
        e.Skip();
        if (!m_url.empty()) {
            auto font = m_hms_content->GetFont();
            font.SetUnderlined(true);
            m_hms_content->SetFont(font);
            Layout();
            SetCursor(wxCURSOR_HAND);
        }
        });
    m_hms_content->Bind(wxEVT_LEAVE_WINDOW, [this](wxMouseEvent &e) {
        e.Skip();
        if (!m_url.empty()) {
            auto font = m_hms_content->GetFont();
            font.SetUnderlined(false);
            m_hms_content->SetFont(font);
            Layout();
            SetCursor(wxCURSOR_ARROW);
        }
        });
    m_hms_content->Bind(wxEVT_LEFT_UP, [this](wxMouseEvent &e) {
        if (!m_url.empty()) wxLaunchDefaultBrowser(m_url);
    });
}
HMSNotifyItem ::~HMSNotifyItem() {
    ;
}

void HMSNotifyItem::init_bitmaps() {
    m_img_notify_lv1 = create_scaled_bitmap("hms_notify_lv1", nullptr, 18);
    m_img_notify_lv2 = create_scaled_bitmap("hms_notify_lv2", nullptr, 18);
    m_img_notify_lv3 = create_scaled_bitmap("hms_notify_lv3", nullptr, 18);
    m_img_arrow      = create_scaled_bitmap("hms_arrow", nullptr, 14);
}

wxBitmap & HMSNotifyItem::get_notify_bitmap()
{
    switch (m_hms_item.msg_level) {
        case (HMS_FATAL): 
            return m_img_notify_lv1;
            break;
        case (HMS_SERIOUS):
            return m_img_notify_lv2;
            break;
        case (HMS_COMMON): 
            return m_img_notify_lv3;
            break;
        case (HMS_INFO): 
            //return m_img_notify_lv4;
            break;
        case (HMS_UNKNOWN): 
        case (HMS_MSG_LEVEL_MAX):
        default: break;
    }
    return wxNullBitmap;
}

HMSPanel::HMSPanel(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style)
    :wxPanel(parent, id, pos, size, style)
{
    this->SetBackgroundColour(wxColour(238, 238, 238));

    auto m_main_sizer = new wxBoxSizer(wxVERTICAL);

    m_scrolledWindow = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL);
    m_scrolledWindow->SetScrollRate(5, 5);

    m_top_sizer = new wxBoxSizer(wxVERTICAL);

    m_top_sizer->AddSpacer(FromDIP(30));

    m_scrolledWindow->SetSizerAndFit(m_top_sizer);

    m_main_sizer->Add(m_scrolledWindow, 1, wxEXPAND, 0);

    this->SetSizerAndFit(m_main_sizer);

    Layout();
}

HMSPanel::~HMSPanel() {
    ;
}

void HMSPanel::append_hms_panel(HMSItem& item) {
    m_notify_item = new HMSNotifyItem(m_scrolledWindow, item);
    m_top_sizer->Add(m_notify_item, 0, wxALIGN_CENTER_HORIZONTAL);
}

void HMSPanel::delete_hms_panels() {
    m_scrolledWindow->DestroyChildren();
}

void HMSPanel::update(MachineObject *obj)
{
    if (obj) {
        this->Freeze();
        delete_hms_panels();
        wxString hms_text;
        for (auto item : obj->hms_list) {
            if (wxGetApp().get_hms_query()) {
                append_hms_panel(item);
            }
        }
        Layout();
        this->Thaw();
    } else {
        delete_hms_panels();
        Layout();
    }
}

bool HMSPanel::Show(bool show)
{
    return wxPanel::Show(show);
}

}}