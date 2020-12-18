import Foundation

class Delegate: StateMachineDelegate {
    func handleError(stateMachine: StateMachine, error: Error) {
        print(error)
    }

    func willPerformTransition(stateMachine: StateMachine, forEvent event: StateMachineEvent) {
        print("\(event) on \(stateMachine.currentState)")
    }
}
