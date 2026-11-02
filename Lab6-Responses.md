# Activity 0
- Predictions: 
- - With the higher-priority thread starting delayed and the lower starting first with a semaphore, the higher-priority thread will wait on the semaphore while the medium-priority thread pre-empts the low-priority thread even as it holds the semaphore.

<<<<<<< HEAD
initial Results:

"
Semaphore created succesfully.
Low priority task unable to acquire semaphore...

"


- Reality: Low-priority task was unable to acquire the semaphore at all despite starting first. There is no sign that the medium or high-priority task were able to acquire it.

# Activity 1
- CHanging CreateBinary to CreateMutex results in the low-priority thread successfully obtaining the semaphore.

Change taskYield in Supervisor while(1) to vTaskDelay and successsfully got this output:

'''

Semaphore created succesfully.
Starting scheduler
Created low priority tatsk 
Starting low prioiryt task... attempting to take semaphore
Semaphore taken in low priority task. Delaying...
Created med priority tatsk 
Created high priority tatsk 
High proiorty task attmepting to take sempahore...
med priority starving system attempt
Semaphore taken in high priority task. giving...
Hight priority work done and sempahore released... 

'''
=======
- Reality: Low-priority task was unable to acquire the semaphore at all despite starting first. There is no sign that the medium or high-priority task were able to acquire it.

# Activity 1
- CHanging CreateBinary to CreateMutex results in the low-priority thread successfully obtaining the semaphore.
>>>>>>> 60143242d6f834ee36582f5665ac5d26a1f6c610
