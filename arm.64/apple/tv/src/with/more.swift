func more(_ def: Def) {
    if let at = appDefs.firstIndex(where: { $0.sid == def.sid }) {
        appDefs[at] = def
    } else {
        appDefs.append(def)
    }
}
