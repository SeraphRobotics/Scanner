Directory breakdown:
	Builds exists to store temp builds and should never be synced
	Media contains config files and images and other data to be processed
	ScanController contains the program from controlling the scanner system
	OpenCV Tests contains tests to run to check if openCV is working
	
BUG: Stoping
	The system uses forced stop to end a scan early. This means that the VM needs ot be reinitialized each time
	The system can only run one scan per launch of hte program
	The solution is to add a stop functionality to the VM or the cmotion system
	
TODO:
	Unify the 3 axis and 1-2 axis motion in the VM on lines 213-242 of virtualmachines.cpp
