import Foundation

class StateMachineImpl {
    func pushPage(_ page: Page) {
        print("push \(page)")
    }

    func popPage() {
        print("pop")
    }

    func needSecondAuthentication() -> Bool {
        false
    }

    func loginFinished() {
        print("finished")
    }
}
