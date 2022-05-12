#################################################
##              interface for                  ##
##          the delta-robot project            ##
##               for Prof 3C                   ##
##              code writen by                 ##
##             Ruben Middelman                 ##
##               Used by the                   ##
##      HU University of Applied Sciences'     ##
#################################################

import tkinter as tk
import serial
import time
from PIL import ImageTk, Image

#this is for the comminucation from the pc to the arduino
arduino = serial.Serial(port='COM5', baudrate=9600, timeout=.1)

#function that is called when the submit button is pressed
def GetMedicin():
    medicin = entry.get()
    print(medicin)
    arduino.write(bytes(medicin, 'utf-8'))
    time.sleep(0.05)


#makes the frame for the interface
window = tk.Tk()
window.title("pil-pak-inator")

#loads the image
logpoImage = Image.open("logo.png")
imagePythonForm = ImageTk.PhotoImage(logoImage)
logoReadyForDisplay = tk.Label(image=imagePythonForm)
logoReadyForDisplay.image = imagePythonForm

#makes the input form for the medicin number
label = tk.Label(text="Name of medicin?")
entry = tk.Entry()
#makes the submit button that calles the GetMedicin function
submit = tk.Button(text = "submit", width = 20, command=GetMedicin)

#places the logo, the button and the input form
logoReadyForDisplay.place(x=0, y=0)
label.pack()
entry.pack()
submit.pack()

#main loop where the interface works
window.mainloop()
