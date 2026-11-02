# Activity 0
- Predictions: 
- - With the higher-priority thread starting delayed and the lower starting first with a semaphore, the higher-priority thread will wait on the semaphore while the medium-priority thread pre-empts the low-priority thread even as it holds the semaphore.

- Reality: Low-priority task was unable to acquire the semaphore at all despite starting first. There is no sign that the medium or high-priority task were able to acquire it.

# Activity 1
- CHanging CreateBinary to CreateMutex results in the low-priority thread successfully obtaining the semaphore.