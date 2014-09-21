import serial
from PySide.QtCore import *

class ScannerInterface(QObject):
    
    pressed = Signal()
    scanCompleted = Signal()
    errored = Signal()
    
    def __init__(self):
        super(ScannerInterface, self).__init__()
        self.ser = serial.Serial()
        self.timer = QTimer()
        self.timer.setInterval(100)
        self.timer.start()
	self.timer.timeout.connect(self._checkBuffer)
    
    def __del__(self):
        self.ser.close()
    
    @Slot()
    def connectport(self,port,baud):
        if (self.ser.isOpen()): self.ser.close()
        self.ser.setPort(port)
        self.ser.baudrate = baud
        self.ser.open()
    def isready(self):
        return self.ser.isOpen()
    
    def _write(self,towrite):
        if (not self.isready()):
            return False
        else:
            self.ser.write(towrite)
        return True
    
    @Slot()
    def _checkBuffer(self):
        if(self.isready()):
            while(self.ser.inWaiting()>0):
                char = self.ser.read();
                if char == "B":
                    self.pressed.emit();
                    print "recevied: B"
                elif char =="D":
                    self.scanCompleted.emit();
                elif char =="E":
                    self.errored.emit();
                else:
                    print "received "+char
    
    @Slot()
    def error(self):
        self._write("e")
    
    @Slot()
    def startscan(self):
        self._write("s")
    
    @Slot()
    def laserOn(self):
        self._write("l")
    
    @Slot()
    def laserOff(self):
        self._write("k")
    
    @Slot()
    def ledOn(self):
        self._write("o")
    
    @Slot()
    def ledOff(self):
        self._write("f")
    
    @Slot()
    def home(self):
        self._write("h")
    

if __name__ == '__main__':
    import sys
    app = QCoreApplication(sys.argv)
    ir = ScannerInterface()
    ir.connectport("/dev/ttyACM0",9600) 
    print ir.isready()
    loopVar=True
    while(loopVar):
        val = raw_input('Command:')
        ir._write(val)
        if (val == 'N'):
            loopVar = False
            #app.exec_()
