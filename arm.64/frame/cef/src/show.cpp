#include "show.h"
#include "handler.h"

#include "include/cef_browser.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_helpers.h"

namespace {

class WebWindowDelegate : public CefWindowDelegate {
 public:
  explicit WebWindowDelegate(CefRefPtr<CefBrowserView> browser_view)
      : browser_view_(browser_view) {}

  void OnWindowCreated(CefRefPtr<CefWindow> window) override {
    window->AddChildView(browser_view_);
    window->Show();
    browser_view_->RequestFocus();
  }

  void OnWindowDestroyed(CefRefPtr<CefWindow>) override {
    browser_view_ = nullptr;
  }

  bool CanClose(CefRefPtr<CefWindow>) override {
    CefRefPtr<CefBrowser> browser = browser_view_->GetBrowser();
    if (browser) return browser->GetHost()->TryCloseBrowser();
    return true;
  }

  CefSize GetPreferredSize(CefRefPtr<CefView>) override {
    return CefSize(960, 600);
  }

 private:
  CefRefPtr<CefBrowserView> browser_view_;
  IMPLEMENT_REFCOUNTING(WebWindowDelegate);
};

}

void show() {
  CEF_REQUIRE_UI_THREAD();

  CefBrowserSettings browser_settings;
  CefRefPtr<CefBrowserView> browser_view =
      CefBrowserView::CreateBrowserView(
          WebHandler::GetInstance(),
          "efs://html/main/index.html",
          browser_settings,
          nullptr,
          nullptr,
          nullptr);

  CefWindow::CreateTopLevelWindow(new WebWindowDelegate(browser_view));
}
