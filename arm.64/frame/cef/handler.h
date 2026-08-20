#pragma once

#include "include/cef_client.h"

#include <list>

class WebHandler : public CefClient,
                   public CefDisplayHandler,
                   public CefLifeSpanHandler,
                   public CefLoadHandler {
 public:
  WebHandler();
  ~WebHandler() override;

  static WebHandler* GetInstance();

  CefRefPtr<CefDisplayHandler>  GetDisplayHandler()  override { return this; }
  CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override { return this; }
  CefRefPtr<CefLoadHandler>     GetLoadHandler()     override { return this; }

  void OnTitleChange(CefRefPtr<CefBrowser> browser,
                     const CefString& title) override;

  void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
  bool DoClose(CefRefPtr<CefBrowser> browser) override;
  void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;

  void CloseAllBrowsers(bool force_close);
  bool IsClosing() const { return is_closing_; }

 private:
  using BrowserList = std::list<CefRefPtr<CefBrowser>>;
  BrowserList browser_list_;
  bool        is_closing_ = false;

  IMPLEMENT_REFCOUNTING(WebHandler);
};
