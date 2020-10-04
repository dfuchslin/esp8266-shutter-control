BOARD = esp8266:esp8266:generic
BAUD = 115200
FQBN = $(BOARD):xtal=80,vt=flash,exception=legacy,ssl=all,ResetMethod=nodemcu,CrystalFreq=26,FlashFreq=40,FlashMode=dout,eesz=1M64,led=2,sdk=nonosdk_190703,ip=lm2f,dbg=Disabled,lvl=None____,wipe=none,baud=$(BAUD)
PORT = /dev/cu.usbserial-1410
TTY = /dev/tty.usbserial-1410
CC = arduino-cli

compile: 
	$(CC) compile --fqbn $(FQBN)

flash: 
	$(CC) upload -p $(PORT) --fqbn $(FQBN) `pwd`

serial:
	screen $(TTY) $(BAUD)

setup:
	$(CC) config init --additional-urls http://arduino.esp8266.com/stable/package_esp8266com_index.json
	$(CC) core update-index
	$(CC) core install esp8266:esp8266
	$(CC) lib install WiFiManager

clean:
	rm *.elf *.bin
