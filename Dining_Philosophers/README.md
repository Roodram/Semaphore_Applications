### CustomSemaphores
I have implemented the semaphores in a header file "semaphore2.h" and included and used the same in the corresponding .cpp files.

To run a custom_semaphores based implementation make sure that the "semaphore2.h" file is in the same folder as the .cpp file.

#### How to Run
To run the .cpp file, run the following commands:
1. g++ -pthread Dining_Philosophers_CustomSemaphores.cpp
2. ./a.out

### InbuiltSemaphores
These implementations use the inbuilt c++ library "semaphore.h" in the corresponding .cpp files.

#### How to Run
To run the .cpp file, run the following commands:
1. g++ -pthread Dining_Philosophers_InbuiltSemaphores.cpp
2. ./a.out
