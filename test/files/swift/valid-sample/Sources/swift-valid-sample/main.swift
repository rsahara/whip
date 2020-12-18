let stateMachine = StateMachine()
let stateMachineImpl = StateMachineImpl()
let stateMachineDelegate = Delegate()
stateMachine.implementations = stateMachineImpl
stateMachine.delegate = stateMachineDelegate

stateMachine.enterInitialState()
stateMachine.proceedWithEmail()
stateMachine.proceed()
