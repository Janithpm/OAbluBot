
from pynput import keyboard
import serial
import serial.tools.list_ports

print('''
_______________________________________________________________________ 
***************** RC_Car - [Obstacle Avoiding] ************************

	Exit from application :- esc key or 'q/Q'.
	Control KEYS :-
		# Autopilot Mode 	:- 'a/A'
		# RC Mode 		:- 'r/R'
			Forward 	:- UP Arrow KEY
			Backward 	:- DOWN Arrow KEY
			Turn Left 	:- LEFT Arrow KEY
			Turn Right 	:- RIGHT Arrow KEY
		# Standby		:- Space KEY
	''')
print("Available Serial Ports :-")
ports = serial.tools.list_ports.comports()
for port, desc, hwid in sorted(ports):
        print("{}: {}".format(port, desc))
print("")        
while True:
	port_name = input("___ Select Your Port : ")
	if port_name == 'q' or port_name == 'Q':
		quit()
	try:
		print("___ Connecting ....**")
		ser = serial.Serial(port_name,9600)
		print("___ Sucsessfully Connected. BOT is Ready :)    ")
		print("________________________________________________________________________")
		
		break
	except:
		print("___ Error:inavalied Port. ")
		print("")
		continue


COMBINATIONS = [
		{keyboard.KeyCode(char='q')},
		{keyboard.KeyCode(char='Q')},
		{keyboard.KeyCode(char='a')},
		{keyboard.KeyCode(char='A')},
		{keyboard.KeyCode(char='r')},
		{keyboard.KeyCode(char='R')},
		{keyboard.Key.esc},
		{keyboard.Key.space},
		{keyboard.Key.up},
		{keyboard.Key.down},
		{keyboard.Key.left},
		{keyboard.Key.right},
		{keyboard.Key.up, keyboard.Key.left},
		{keyboard.Key.up, keyboard.Key.right},
		{keyboard.Key.down, keyboard.Key.left},
		{keyboard.Key.down, keyboard.Key.right},

	]

current = set()
value = ""
def on_press(key):
	if any([key in COMBO for COMBO in COMBINATIONS]):
		current.add(key)
		if any(all(k in current for k in COMBO) for COMBO in COMBINATIONS):
			
			if current == COMBINATIONS[0] or current == COMBINATIONS[1] or current == COMBINATIONS[6]: #exit
				quit()
			elif current == COMBINATIONS[7]: # space stop
				value = "s"
			elif current == COMBINATIONS[2] or current == COMBINATIONS[3]: #auto p
				value = "a"
			elif current == COMBINATIONS[4] or current == COMBINATIONS[5]: # rc
				value = "r"
			elif current == COMBINATIONS[8]: # up
				value = "1" 
			elif current == COMBINATIONS[9]: # down
				value = "2"
			elif current == COMBINATIONS[10]: #left
				value = "3"
			elif current == COMBINATIONS[11]: #right
				value = "4"
			elif current == COMBINATIONS[12]: # up + left
				value = "5"
			elif current == COMBINATIONS[13]: # up + right
				value = "6"
			elif current == COMBINATIONS[14]: # down + left
				value = "7"
			elif current == COMBINATIONS[15]: # down + right
				value = "8"
			else:
				value = ""
			#print(value)
			ser.write(value.encode())

def on_release(key):
	if any([key in COMBO for COMBO in COMBINATIONS]):
		current.remove(key)

with keyboard.Listener(on_press = on_press, on_release = on_release) as listener:
		listener.join()
