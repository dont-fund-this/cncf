#include "start.h"
#include "show.h"
#include "type.h"

#include "include/cef_browser.h"
#include "include/cef_callback.h"
#include "include/cef_parser.h"
#include "include/cef_request.h"
#include "include/cef_resource_handler.h"
#include "include/cef_response.h"
#include "include/cef_scheme.h"
#include "include/wrapper/cef_helpers.h"

#include <algorithm>
#include <cstring>
#include <string>
#include <vector>

extern Cabi* g_cef_cabi;

namespace {

std::string mime_for(const std::string& path) {
    auto dot = path.rfind('.');
    if (dot == std::string::npos) return "application/octet-stream";
    const std::string ext = path.substr(dot + 1);
    if (ext == "html" || ext == "htm") return "text/html";
    if (ext == "css")                  return "text/css";
    if (ext == "js"   || ext == "mjs") return "application/javascript";
    if (ext == "json")                 return "application/json";
    if (ext == "svg")                  return "image/svg+xml";
    if (ext == "png")                  return "image/png";
    if (ext == "jpg" || ext == "jpeg") return "image/jpeg";
    if (ext == "gif")                  return "image/gif";
    if (ext == "webp")                 return "image/webp";
    if (ext == "wasm")                 return "application/wasm";
    if (ext == "ico")                  return "image/x-icon";
    if (ext == "woff")                 return "font/woff";
    if (ext == "woff2")                return "font/woff2";
    if (ext == "ttf")                  return "font/ttf";
    if (ext == "txt" || ext == "md")   return "text/plain";
    return "application/octet-stream";
}

std::string url_to_path(const std::string& url) {
    constexpr const char* prefix = "efs://";
    constexpr size_t prefix_len  = 6;
    if (url.compare(0, prefix_len, prefix) != 0) return {};
    std::string path = url.substr(prefix_len);
    auto q = path.find_first_of("?#");
    if (q != std::string::npos) path.erase(q);
    while (!path.empty() && path.front() == '/') path.erase(0, 1);
    return path;
}

class EfsResourceHandler : public CefResourceHandler {
 public:
  EfsResourceHandler() = default;

  bool Open(CefRefPtr<CefRequest> request,
            bool& handle_request,
            CefRefPtr<CefCallback>) override {
    handle_request = true;

    const std::string url  = request->GetURL().ToString();
    const std::string path = url_to_path(url);
    if (path.empty()) {
      status_ = 404;
      return true;
    }

    const std::string html = "<html><body><h1>Pat CEF Ready</h1></body></html>";
    data_.assign(html.begin(), html.end());
    mime_   = "text/html";
    status_ = 200;
    return true;
  }

  void GetResponseHeaders(CefRefPtr<CefResponse> response,
                          int64_t& response_length,
                          CefString&) override {
    response->SetStatus(status_);
    response->SetMimeType(mime_);
    CefResponse::HeaderMap headers;
    headers.emplace("Access-Control-Allow-Origin",  "*");
    headers.emplace("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    headers.emplace("Access-Control-Allow-Headers", "Content-Type");
    response->SetHeaderMap(headers);
    response_length = static_cast<int64_t>(data_.size());
  }

  bool Read(void* data_out,
            int   bytes_to_read,
            int&  bytes_read,
            CefRefPtr<CefResourceReadCallback>) override {
    bytes_read = 0;
    if (offset_ >= data_.size()) return false;
    const size_t remaining = data_.size() - offset_;
    const size_t to_copy   = std::min<size_t>(static_cast<size_t>(bytes_to_read), remaining);
    std::memcpy(data_out, data_.data() + offset_, to_copy);
    offset_ += to_copy;
    bytes_read = static_cast<int>(to_copy);
    return true;
  }

  void Cancel() override {}

 private:
  std::vector<unsigned char> data_;
  size_t                     offset_ = 0;
  int                        status_ = 200;
  std::string                mime_   = "application/octet-stream";

  IMPLEMENT_REFCOUNTING(EfsResourceHandler);
};

class EfsSchemeHandlerFactory : public CefSchemeHandlerFactory {
 public:
  EfsSchemeHandlerFactory() = default;

  CefRefPtr<CefResourceHandler> Create(CefRefPtr<CefBrowser>,
                                       CefRefPtr<CefFrame>,
                                       const CefString&,
                                       CefRefPtr<CefRequest>) override {
    return new EfsResourceHandler();
  }

 private:
  IMPLEMENT_REFCOUNTING(EfsSchemeHandlerFactory);
};

}

void declare_efs_scheme(CefRawPtr<CefSchemeRegistrar> registrar) {
  registrar->AddCustomScheme(
      "efs",
      CEF_SCHEME_OPTION_STANDARD |
      CEF_SCHEME_OPTION_SECURE   |
      CEF_SCHEME_OPTION_CORS_ENABLED |
      CEF_SCHEME_OPTION_FETCH_ENABLED);
}

void start() {
  CefRegisterSchemeHandlerFactory("efs", "", new EfsSchemeHandlerFactory());
  show();
}
