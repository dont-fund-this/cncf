import Foundation

var running = "primary"
var switchTo: String?

func tapToggle() {
    if useFb {
        DispatchQueue.main.async { grid.showText.toggle() }
    } else {
        switchTo = (running == "rata") ? "primary" : "rata"
    }
}
