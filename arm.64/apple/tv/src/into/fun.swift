func fun(_ receive: @escaping (String) -> Void) -> FunFn {
    { _, payload, _ in receive(payload) }
}
