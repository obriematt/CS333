code Main

  -- OS Class: Project 3
  --
  -- Matthew O'Brien
  --

-----------------------------  Main  ---------------------------------

  function main ()
      InitializeScheduler ()
      --SleepingBarber ()
      GamingParlor ()
      
      ThreadFinish ()

      --FatalError ("Need to implement")
    endFunction


----------------------------- Gaming Parlor --------------------------
  --Gaming parlor stuff. Using previous code from Dining Philosopher and 
  --the producer and consumer to allow for a framework of what I'm doing here. 
  --Trying to start from scratch is difficult, and having some sort of previous framework to look at
  --is extremely helpful.

  --Gaming Parlor Monitor and the 8 groups/threads take will be in the parlor.
  var
    gamingPortal: GamingParlorMonitor = new GamingParlorMonitor
    groups: array [8] of Thread = new array of Thread {8 of new Thread}

  --Borrowed this portion from the Dining Philo and Pro/Con to Init and Fork my threads
  function GamingParlor ()
    gamingPortal.Init ()


    groups[0].Init ("A")
    groups[0].Fork (ToPlay, 4)

    groups[1].Init ("B")
    groups[1].Fork (ToPlay, 4)

    groups[2].Init ("C")
    groups[2].Fork (ToPlay, 5)

    groups[3].Init ("D")
    groups[3].Fork (ToPlay, 5)

    groups[4].Init ("E")
    groups[4].Fork (ToPlay, 2)

    groups[5].Init ("F")
    groups[5].Fork (ToPlay, 2)

    groups[6].Init ("G")
    groups[6].Fork (ToPlay, 1)

    groups[7].Init ("H")
    groups[7].Fork (ToPlay, 1)

  endFunction

  --This function is what fork will call to attempt to play a game. They'll wait if there
  --isn't enough die for them to play, or they'll play for a time of 15.
  --Each group tries to play 5 times.

  function ToPlay(AmountOfDice: int)
    var
      GamesToPlay: int
      WaitTime: int

    --Attempts to play 5 times
    for GamesToPlay = 1 to 5
      gamingPortal.Request (AmountOfDice)

      --Random 15 picked for the wait time of the games
      for WaitTime = 1 to 15
	currentThread.Yield ()
      endFor

      --Give back the die
      gamingPortal.Return (AmountOfDice)
    endFor
  endFunction


  --Parlor monitor initialization
  behavior GamingParlorMonitor
    method Init ()
      DiceCount = 8
      DiceLock = new Mutex
      DiceLock.Init ()
      condition = new Condition
      condition.Init ()
    endMethod

    --Request the dice from the parlor
    method Request (NumberOfDice: int)
      --CRITICAL SECTION
      DiceLock.Lock ()
      self.Print ("requests", NumberOfDice)
      
      --If they number of dice they want is too large from the available die count
      while NumberOfDice > DiceCount
        condition.Wait (&DiceLock)
      endWhile
  
      --Group gets the dice. Change the amount of dice the parlor has
      DiceCount = DiceCount - NumberOfDice
      self.Print("proceeds with", NumberOfDice)

      --EXIT THE CRITICAL SECTION!
      DiceLock.Unlock ()

    endMethod
   
    --Group gives back the dice
    method Return (NumberOfDice: int)

      --ENTER THE CRITICAL SECTION
      DiceLock.Lock ()
      --Group gives back the dice, change the parlor's count
      DiceCount = DiceCount + NumberOfDice
      self.Print ("releases and adds back", NumberOfDice)
      
      --Signal that dice have been freed up for others
      --I used both signal and broadcast here for my trials.
      --They both seemed to work correctly, but with the 
      --MESA semantics, I feel like broadcast made the most sense
      --and I ran with it instead.
      condition.Broadcast (&DiceLock)
   
      --EXIT THE CRITICAL SECTION!
      DiceLock.Unlock ()
    endMethod	   


    --This portion of the code was given from the professor.
    --No need for commenting within this method.
    method Print (str: String, count: int)

      print (currentThread.name)
      print (" ")
      print (str)
      print (" ")
      printInt (count)
      nl ()
      print ("---------------------------------Number of dice now avail = ")
      printInt (DiceCount)
      nl ()
    endMethod



  endBehavior

---------------------------- SleepingBarber --------------------------
  --NOTE: I am using the implementation from the previous project of dining philosophers and pro/con.
  --I figured since it was allowed to look ahead for the mutex implementation, it would also be 
  --allowed to look back on the previous project to get a helpful starting part for a nice output.


  --MAX_CHAIRS for the amount of chairs in the waiting room
  --MAX_CUST the amount of customers visiting the barber
  --LIMIT is the amount of attempts each customer will make to visit the barber

  const
      MAX_CHAIRS = 5
      MAX_CUST = 15
      LIMIT = 5

  --BarberMonitor, the monitor for the barber cutting customer's hair
  --A single thread for the barber, could adjust this and allow for multiple barbers. 
  --An array of customer threads to sit in the barber's chairs.

  var
      mon: StoreMonitor
      BarberThreads: Thread = new Thread
      CustomerThreads: array [MAX_CUST] of Thread = new array of Thread {MAX_CUST of new Thread}
      CurrentCustomer: int = -1
      

  --STARTCUT and ENDCUT are for the barber. Starting to cut hair and ending a haircut
  --EXIT and ENTER are for customers leaving the shop and entering the shop.
  --OUT and WAITING are for customers outside the shop and waiting in the shop.
  --GETCUT and DONECUT are for customers getting their haircut for the start and end of the cut.
  --SLEEPING for taking a nap while no customer wants a haircut. This was my original idea, but 
  --I later changed my mind and used this as a default status to allow for better displays.

  enum STARTCUT, ENDCUT, OUT, ENTER, WAITING, GETCUT, DONECUT, EXIT, SLEEPING
      


  --The basic output function for the barbershop.
  function SleepingBarber ()
      var i: int
      print ("Chairs  Customers: ")


      for i = 1 to MAX_CUST
 	printInt (i)
        print ("   ")
      endFor
      nl ()


       
      mon = new StoreMonitor 
      mon.Init ()


      BarberThreads.Init ("Barber")
      BarberThreads.Fork (BarberBegin, 1)

      CustomerThreads[0].Init ("Customer")
      CustomerThreads[0].Fork (CustomerStatus, 0)


      CustomerThreads[1].Init ("Customer")
      CustomerThreads[1].Fork (CustomerStatus, 1)


      CustomerThreads[2].Init ("Customer")
      CustomerThreads[2].Fork (CustomerStatus, 2)


      CustomerThreads[3].Init ("Customer")
      CustomerThreads[3].Fork (CustomerStatus, 3)
 

      CustomerThreads[4].Init ("Customer")
      CustomerThreads[4].Fork (CustomerStatus, 4)


      CustomerThreads[5].Init ("Customer")
      CustomerThreads[5].Fork (CustomerStatus, 5)


      CustomerThreads[6].Init ("Customer")
      CustomerThreads[6].Fork (CustomerStatus, 6)


      CustomerThreads[7].Init ("Customer")
      CustomerThreads[7].Fork (CustomerStatus, 7)


      CustomerThreads[8].Init ("Customer")
      CustomerThreads[8].Fork (CustomerStatus, 8)


      CustomerThreads[9].Init ("Customer")
      CustomerThreads[9].Fork (CustomerStatus, 9)


      CustomerThreads[10].Init ("Customer")
      CustomerThreads[10].Fork (CustomerStatus, 10)


      CustomerThreads[11].Init ("Customer")
      CustomerThreads[11].Fork (CustomerStatus, 11)
  

      CustomerThreads[12].Init ("Customer")
      CustomerThreads[12].Fork (CustomerStatus, 12)


      CustomerThreads[13].Init ("Customer")
      CustomerThreads[13].Fork (CustomerStatus, 13)


      CustomerThreads[14].Init ("Customer")
      CustomerThreads[14].Fork (CustomerStatus, 14)
  endFunction



  --Previous project, PhilosophizeAndEat was used as a framework for this function.
  function CustomerStatus (p: int)
      var
	visits: int
	yield: int
	temp: int

      CurrentCustomer = CurrentCustomer + 1
      temp = CurrentCustomer

      --I am trying to line up the barber visits time with haircuts.
      --There is some mistake here where the customer and the barber don't line 
      --up quite perfect for time of haircut.
      --Based on what I've done in gaming parlor, I figured this would be the spot
      --to correct the problem. But they are still off. I've ran out of time to correct my problem.
      --But I'm pretty sure this is the area that is wrong.

      for visits = 0 to LIMIT
	for yield = 1 to 9
	  currentThread.Yield ()
	endFor
	mon.Customer (temp)
      endFor
  endFunction

  function BarberBegin ()
	mon.Barber ()
  endFunction


  --Framework from ForkMonitor was used for this. KPL is confusing.
  class StoreMonitor
      superclass Object

      --Fields of the BarberMonitor.
      --status for the status of each customer at the barbers.
      --BarberStatus, what the barber is currently doing. SLEEPING for if no one is in his chair.
      --CustomerSem, semaphore for the waiting customers.
      --BarberSem, semaphore for the barber.
      --Queue, the customers waiting for their haircut.
      fields
	Status: array [MAX_CUST] of int
	BarberStatus: int
	CustomerSem: Semaphore
	BarberSem: Semaphore
	Control: Mutex
	Queue: int

      methods
	Init ()
	Barber ()
	Customer (CustID: int)
	Start_Haircut ()
	Get_Haircut (CustID: int)
	PrintAllStatus ()
  endClass

  --The behavior of the class.
  behavior StoreMonitor

      --Initialization method for BarberMonitor
      method Init ()

	Status = new array of int {MAX_CUST of OUT}
	BarberStatus = 0
	Queue = 0

	CustomerSem = new Semaphore
	BarberSem = new Semaphore
	Control = new Mutex

	CustomerSem.Init (0)
	BarberSem.Init (0)
	Control.Init ()

      endMethod

      method Barber ()
	--Going to sleep if no customers are wanting a haircut
	while true
	  CustomerSem.Down ()

	  --Control allows for access to shared data, down is allowing for access
	  Control.Lock ()

	  --Change the shared data
	  Queue = Queue - 1
	
	  --Barber is awake from sleep, and ready to cut some hair
	  BarberSem.Up ()

	  --Releases control of the shared Queue data.
	  Control.Unlock ()

	  --The customer is getting their hair cut.
	  self.Start_Haircut ()
        endWhile
      endMethod

      method Customer (CustID: int)

	--Customer is entering the barber shop
	--Allow for access to shared data
	Control.Lock ()
	Status[CustID] = ENTER
	self.PrintAllStatus ()
	Status[CustID] = SLEEPING

	--Checking for empty chairs in the shop
	--Waiting in the shop if allowed, increasing the amount of customers waiting	
	if (Queue < MAX_CHAIRS)
	  Status[CustID] = WAITING
	  self.PrintAllStatus ()
	  Queue = Queue + 1

	  --Wakes up the barber if need be
	  CustomerSem.Up ()
	  Control.Unlock ()
	  BarberSem.Down ()

          --My alternative to fixing my problem is right here. To yield after the self calls Get_haircut.
	  --This wasn't really a good fix and caused way more problems.

	  self.Get_Haircut (CustID)
	  
	--No empty seats for the customer, and they leave.
	else
	  Status[CustID] = EXIT
	  self.PrintAllStatus ()
	  Status[CustID] = SLEEPING
	  Control.Unlock ()
	endIf
      endMethod
 

      method Start_Haircut ()

	var
	  temp: int
	
	Control.Lock ()
	BarberStatus = STARTCUT
	self.PrintAllStatus ()
	BarberStatus = SLEEPING
	Control.Unlock ()

        --This is the wait time for the barber to cut some hair.
        --My main issues are happening here and with my customer.
	for temp = 1 to 9
	  currentThread.Yield ()
	endFor

	Control.Lock ()
	BarberStatus = ENDCUT
	self.PrintAllStatus ()
	BarberStatus = SLEEPING
	Control.Unlock ()

      endMethod

      method Get_Haircut (CustID: int)
	

	
	Control.Lock ()
	Status[CustID] = GETCUT
	self.PrintAllStatus ()
	Control.Unlock ()


	Control.Lock ()
	Status[CustID] = DONECUT
	self.PrintAllStatus ()
	Status[CustID] = SLEEPING
	Control.Unlock ()

	Control.Lock ()
	Status[CustID] = EXIT
	self.PrintAllStatus ()
	Status[CustID] = SLEEPING
	Control.Unlock ()

      endMethod


      --I used the PDF from the project file about an output for this part of the project
      --I know the professor said to make your own, but the PDF was in the project files
      --so I simply used what was available to me.
      --It is also modelled slightly after the ideas from Dino Philo I noticed.
      --I changed a few of the symbols just because I thought they looked nicer.
      method PrintAllStatus ()
	var
	  QueuedCust: int
	  ChairCount: int
	  CustCount: int
	  TotalCust: int = MAX_CUST - 1
	  EmptyChairs: int = MAX_CHAIRS - Queue

	for QueuedCust = 1 to Queue
	  print ("#")
	endFor

	for ChairCount = 1 to EmptyChairs
	  print ("_")
	endFor

	switch BarberStatus
	  case STARTCUT:
	    print("Cutting Hair  ")
	    break
	  case ENDCUT:
	    print("Done          ")
	    break
	  default:
	    print(" -            ")
	endSwitch



	for CustCount = 0 to TotalCust
	  if CustCount > 9
	    print(" ")
	  endIf

	  switch Status[CustCount]
	    case OUT:
	      print("_   ")
	      break
	    case ENTER:
	      print("E   ")
	      break
	    case WAITING:
	      print("W   ")
	      break
	    case GETCUT:
	      print("G   ")
	      break
	    case DONECUT:
	      print("D   ")
	      break
	    case EXIT:
	      print("L   ")
  	      break
	    default:
	      print("_   ")
	  endSwitch
	endFor
      nl ()
      endMethod
  endBehavior 
endCode
