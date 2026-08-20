@discardableResult
func less(_ sid: String) -> Bool {
    guard let at = appDefs.firstIndex(where: { $0.sid == sid }) else { return false }
    appDefs.remove(at: at)
    return true
}
