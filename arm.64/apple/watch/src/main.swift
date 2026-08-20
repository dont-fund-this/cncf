import SwiftUI
import Combine

@main
struct PatApp: App {
    @Environment(\.scenePhase) private var phase
    private let clock = Timer.publish(every: 1.0 / 60.0, on: .main, in: .common).autoconnect()

    init() {
        _ = prep()
        start()
    }

    var body: some Scene {
        WindowGroup {
            Show(grid: grid)
                .onAppear { start() }
                .onReceive(clock) { _ in step() }
                .onChange(of: phase) { _, value in
                    if value == .active { start() }
                }
        }
    }
}
