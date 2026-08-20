import Foundation

func fit(_ sid: String, _ tag: String) -> FitFn {
    { address, _, _ in
        address == sid || tag.split(separator: ",").contains {
            $0.trimmingCharacters(in: .whitespaces) == address
        }
    }
}
