import serial

class ScannerInterface:
    def __init__(self):
        self.ser = serial.Serial()
    
	def __del__(self):
        self.ser.close()
    
	def connect(self,port,baud):
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
	
	def error(self):
		self._write("e")
		
	def startscan(self):
		self._write("s")
    def laserOn(self):
		self._write("l")
	def laserOff(self):
		self._write("k")
	def ledOn(self):
		self._write("o")
	def ledOff(self):
		self._write("f")
	def home(self):
		self._write("h")
	

if __name__ == '__main__':
    ir = ScannerInterface()
    ir.connect("COM10",9600)
    print ir.isready()
    loopVar=True
    while(loopVar):
        val = raw_input('Command:')
        ir._write(val)
        if (val == 'N'):
            loopVar = False