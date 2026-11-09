# Activity 0
- Predictions: 
- - With the higher-priority thread starting delayed and the lower starting first with a semaphore, the higher-priority thread will wait on the semaphore while the medium-priority thread pre-empts the low-priority thread even as it holds the semaphore.

initial Results:

"
Semaphore created succesfully.
Low priority task unable to acquire semaphore...

"


- Reality: Low-priority task was unable to acquire the semaphore at all despite starting first. There is no sign that the medium or high-priority task were able to acquire it.

# Activity 1
- CHanging CreateBinary to CreateMutex results in the low-priority thread successfully obtaining the semaphore. The low priority task's "task complete" message was never printed; we suspect that it may have been immediately pre-empted when the semaphore it'd taken was released.

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

# Activity 2
- Both threads with the same priority running busy_busy and busy_yield:
    - I would expect that task 1 and task 2 should take nearly identical amounts of time to run.
    - **Observation**: this is exactly thecase, with both tasks taking roughly 500,000 ticks to complete.

- One runs busy_busy and one runs busy_yield
    - I would expect the thread running busy_yield to take longer to complete, as the ``taskYIELD()`` keyword I believe should result in it waiting until the first thread has completed operation.
    - **Observation**: It was the exact opposite; taskYIELD() upon reading a little more into the documentation rather than just assuming the functionality resulted in task 1 taking significantly longer to complete, while task 2 completed in under 100,000 ticks (under 5,000, even, in fact). The old test has been commented out in response to this.

- Two threads with different priority but running busy_busy and higher starting first
    - With this test, I am expecting that the behavior of thread 1 returns to that of the "busy_busy" test while thread2 remains very short in execution time, as in my mind, the higher priority of thread 1 might result in it completing its operation first due to the inability for thread 2 to pre-empt it
    - **Observation**: It was in fact almost identical to one running busy_busy and one running busy_yield

- Two threads with different priority but running busy_busy and lower starting first
    - I expect it to this time be the opposite, given that the two threads are running the same function. The lower priority thread should be immediately pre-empted, so thread 1's runtime should be close to 0, while thread 2 is close to double.
    - **Observation:** This prediction was correct, although thread 1's runtime is still close to 10,000 ticks. I would presume that this may be due to the small delay introduced between starting the threads.
        - Upon going back and changing that delay from 10 to 0, yes, that is indeed what was the case.

- Two threads with different priority but running busy_yield and higher starting first
    - I expect it to be pretty similar this time to when both were running busy_busy, as with a higher priority and because both are yielding, t1 *should* in my mind end up with most of the execution time.
    - **Observation:** Yup, that's exactly what happened!

- Two threads with different priority but running busy_yield and lower starting first
    - I'm expecting that it will be the opposite of the previous.
    - **Observation**: Yup. Accurate prediction. All done now.