import PySide
import ScannerInterface
import cv2


class ScannerController(QObject):
	errored = Signal()

	def __init__(self):
		self.interface = ScannerInterface()
		self.interface.connect("COM10",9600)
		self.capwebcam = cv2.VideoCapture(0)
		self.width_ = 1920
		self.height_= 1080
		self.timer = PySide.QtCore.QTimer();
		self.framerate = 5
		self.timer.setInterval(1000.00/self.framrate)
		
		
		
		self.time = 0.0
		self.maxtime = 30.0*1000.0
		self.speed = 10.0 ##mm/s
		self.imformat = ".jpeg"
		
		errored.connect(self.interface.error)
		self.timer.timeout.connect(ontimeout)
		self.__setupCamera()
		
	def __setupCamera(self):
		
		if not self.capwebcam.isOpened():
			self.capwebcam.open()
		if not self.capwebcam.isOpened():
			self.errored.emit()
		
		state=true;
		state = state && capwebcam.set(CV_CAP_PROP_FRAME_WIDTH,self.width_);
		state = state && capwebcam.set(CV_CAP_PROP_FRAME_HEIGHT,self.height_);
		if(not state):
			self.errored.emit()
	
	@Slot()
	def buttonPressed(self):
		self.interface.startscan()
		self.timer.start()
	
	def ontimeout(self):
		self.time = self.time + 1000.00/self.framrate
		dist = self.speed*self.time/1000.0
		ret, frame = cap.read()
		imwrite('%s'%dist+self.imformat,frame)
		
		if self.time>self.maxtime:
			self.timer.stop()
			self.time = 0
			self.interface.home()
		