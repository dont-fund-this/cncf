#include "app.h"
#include "handler.h"
#include "start.h"

#include "include/cef_command_line.h"
#include "include/wrapper/cef_helpers.h"

void WebApp::OnRegisterCustomSchemes(
    CefRawPtr<CefSchemeRegistrar> registrar) {
  declare_efs_scheme(registrar);
}

void WebApp::OnBeforeCommandLineProcessing(
    const CefString&,
    CefRefPtr<CefCommandLine> command_line) {
  command_line->AppendSwitchWithValue("password-store", "basic");
  command_line->AppendSwitch("use-mock-keychain");
  command_line->AppendSwitch("allow-file-access-from-files");
  command_line->AppendSwitch("no-first-run");
  command_line->AppendSwitch("no-default-browser-check");
  command_line->AppendSwitchWithValue("remote-debugging-port", "9222");
  command_line->AppendSwitch("single-process");
  command_line->AppendSwitch("use-alloy-style");
  command_line->AppendSwitchWithValue(
      "disable-features",
      "InfoBar,GlobalMediaControls,Translate,"
      "AutofillEnableAccountWalletStorage,"
      "OptimizationGuideOnDeviceModel,OnDeviceModelService,"
      "OptimizationGuideModelDownloading,OptimizationGuideFetchingForSRP,"
      "AIPromptAPI,AIPromptAPIForExtension,AIRewriterAPI,AISummarizationAPI,"
      "AITranslator,AILanguageDetector");
}

void WebApp::OnContextInitialized() {
  CEF_REQUIRE_UI_THREAD();
  start();
}

CefRefPtr<CefClient> WebApp::GetDefaultClient() {
  return WebHandler::GetInstance();
}
