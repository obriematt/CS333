header Main

  uses System, Thread, Synch

  functions
    main ()


  class GamingParlorMonitor
    superclass Object
    fields
	condition: Condition
	DiceCount: int
	DiceLock: Mutex
    methods
	Init ()
	Request (NumberOfDice: int)
	Return (NumberOfDice: int)
	Print(str: String, count: int)
  endClass


endHeader
