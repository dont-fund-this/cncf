#include "handler.h"

#include "include/base/cef_callback.h"
#include "include/cef_app.h"
#include "include/cef_command_line.h"
#include "include/cef_frame.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"

#include <string>

namespace {
WebHandler* g_instance = nullptr;
}

WebHandler::WebHandler() {
  g_instance = this;
}

WebHandler::~WebHandler() {
  g_instance = nullptr;
}

WebHandler* WebHandler::GetInstance() {
  return g_instance;
}

void WebHandler::OnTitleChange(CefRefPtr<CefBrowser> browser,
                               const CefString& title) {
  CEF_REQUIRE_UI_THREAD();
  if (auto browser_view = CefBrowserView::GetForBrowser(browser)) {
    if (auto window = browser_view->GetWindow()) {
      window->SetTitle(title);
    }
  }
}

void WebHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
  CEF_REQUIRE_UI_THREAD();
  browser_list_.push_back(browser);

  auto cmd = CefCommandLine::GetGlobalCommandLine();
  std::string url = cmd->GetSwitchValue("url").ToString();
  if (!url.empty() && url != "about:blank") {
    browser->GetMainFrame()->LoadURL(url);
  }
}

bool WebHandler::DoClose(CefRefPtr<CefBrowser>) {
  CEF_REQUIRE_UI_THREAD();
  if (browser_list_.size() == 1) is_closing_ = true;
  return false;
}

void WebHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
  CEF_REQUIRE_UI_THREAD();
  for (auto it = browser_list_.begin(); it != browser_list_.end(); ++it) {
    if ((*it)->IsSame(browser)) {
      browser_list_.erase(it);
      break;
    }
  }
  if (browser_list_.empty()) {
    CefQuitMessageLoop();
  }
}

void WebHandler::CloseAllBrowsers(bool force_close) {
  if (!CefCurrentlyOn(TID_UI)) {
    CefPostTask(TID_UI,
                base::BindOnce(&WebHandler::CloseAllBrowsers, this,
                               force_close));
    return;
  }
  for (const auto& browser : browser_list_) {
    browser->GetHost()->CloseBrowser(force_close);
  }
}
