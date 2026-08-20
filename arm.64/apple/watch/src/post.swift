import Foundation

func post(
    _ address: String,
    _ payload: String,
    _ strict: String = "once",
    _ receive: ((String) -> Void)? = nil
) -> [String] {
    guard let jam else { return [] }
    let sid = UUID().uuidString
    var replies = [String]()
    more(into(sid) {
        replies.append($0)
        receive?($0)
    })
    defer { less(sid) }
    let options = "{\"strict\":\"\(strict)\",\"into\":\"\(sid)\"}"
    address.withCString { a in
        payload.withCString { p in
            options.withCString { o in
                _ = jam.invoke(a, p, o)
            }
        }
    }
    return replies
}
