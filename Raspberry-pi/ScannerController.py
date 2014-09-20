from PySide.QtCore import *
from ScannerInterface import *
import cv2


class ScannerController(QObject):
    
    errored = Signal()

    def __init__(self):
        super(ScannerController, self).__init__()
        self.interface = ScannerInterface()
        self.interface.connectport("COM6",9600)
        self.capwebcam = cv2.VideoCapture(0)
        self.width_ = 1920
        self.height_= 1080
        self.timer = QTimer();
        self.framerate = 5
        self.timer.setInterval(1000.00/self.framerate)
        
        self.time = 0.0
        self.maxtime = 30.0*1000.0
        self.speed = 10.0 ##mm/s
        self.imformat = ".jpeg"
        
        
        self.timer.timeout.connect(self.ontimeout)
        
        self.__setupCamera()
        
    def __setupCamera(self):
        
        if not self.capwebcam.isOpened():
            self.capwebcam.open()
            print "camera didnt open"
        if not self.capwebcam.isOpened():
            self.errored.emit()
            print "camera didnt open twice"
        
        state=True;
        state = state and self.capwebcam.set(cv2.cv.CV_CAP_PROP_FRAME_WIDTH,self.width_);
        state = state and self.capwebcam.set(cv2.cv.CV_CAP_PROP_FRAME_HEIGHT,self.height_);
        if(not state):
            self.errored.emit()
            print "camera didnt get settings"
    
    @Slot()
    def buttonPressed(self):
        self.interface.startscan()
        self.timer.start()
    
    def ontimeout(self):
        self.time = self.time + 1000.00/self.framerate
        dist = self.speed*self.time/1000.0
        ret, frame = self.capwebcam.read()
        cv2.imwrite('%s'%dist+self.imformat,frame)
        if self.time>self.maxtime:
            self.timer.stop()
            self.time = 0
            self.interface.home()

if __name__ == '__main__':
    import sys
    app = QCoreApplication(sys.argv)
    sc = ScannerController()
	QTimer.singleShot(1000,sc.buttonPressed)
    sc.errored.connect(sc.interface.error)
    sys.exit(app.exec_())