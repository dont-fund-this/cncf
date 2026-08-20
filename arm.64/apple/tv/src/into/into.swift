func into(_ sid: String, _ receive: @escaping (String) -> Void) -> Def {
    Def(sid: sid, tag: sid, fit: fit(sid, sid), fun: fun(receive))
}
