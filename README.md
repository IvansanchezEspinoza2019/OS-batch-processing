The batch procesing operating system was one of the earliest OS. It works grouping similar jobs together into batches & execute them secuentially.  

**More about batch processing OS:** [batch_processing_operating_system](https://www.tutorialspoint.com/operating_system/batch_processing_operating_system.htm)



Compile with **gcc**:  
```bash
	 g++ proceso.cpp randomnum.cpp marco.cpp main.cpp -o main
```
Then run (On Windows) `.\main.exe`



<br>
### Program Workflow
---
Jobs will execute an aritmetic operation, wich are: `+, -, \*, /`.

### Initial screen 
---
**No. Procesos**: Enter the number of jobs you want to run  
**Quantum**: is used to calculate how long a job will take in the execution state for a single iteration.

![Inputs:](initial_setup.png)

###  Key options 
---

### Process (or Job) States:
---
### Ready Queue
Procesess to be executed  
### Execution Proccess
The current execution  
### Blocqued Queue
Process that are waiting for an I/O to get back to Ready Queue or to be suspended  
### Finished
Have fibnished their jobs



<br>
###  Classes
---
###  Proceso
Atribuites & methods for simulating a job to be executed
###  Random Num (randomnum.cpp)
Round Robin algorithm for planification

###  Marco, (Actividad 11, 14) (marco.cpp)
Memory paging into equal-size portions. Each "marco" is assigned exclusively to a single process. Thi will simulate memory, for this program the memory will be **45** "Marcos" & each "Marco has *4* slots" for a total of **180** memory units. OS will take initial **3** "Marcos"



<br>
### Refs & errors
---
### c++ error main.cpp:(.text+0x1b): undefined reference to 
[Stack overflow error](https://stackoverflow.com/questions/14620435/c-error-undefined-reference-to-main)
