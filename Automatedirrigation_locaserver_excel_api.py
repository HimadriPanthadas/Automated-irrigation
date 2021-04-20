import gspread
import serial
from oauth2client.service_account import ServiceAccountCredentials
import socket
#connecting and listening to local server
HOST = '192.168.1.114'
PORT = 80
sock = socket.socket()
sock.bind(('0.0.0.0', PORT ))
print("connected")
sock.listen(0) 
print("listening")
'''#connecting to arduino
arduino = serial.Serial('com3', 9600)
print('Established serial connection to Arduino')'''


#connecting client to server of google excel sheet
scope=['https://www.googleapis.com/auth/drive']
credentials= ServiceAccountCredentials.from_json_keyfile_name('credentials.json',scope)
client= gspread.authorize(credentials)
sheet=client.open('StakeCalculator').sheet1

#finding the empty row of the excel sheet
empty_row=len(sheet.get_all_records())+1

while True:
    '''#taking data from the port
    arduino_data = arduino.readline()

    decoded_values = int(arduino_data.decode("utf-8"))
    print(decoded_values)'''

    #getting data from the local server
    client, addr = sock.accept()
    data=(client.recv(1024))
    if(data.decode('utf-8')!=''):
        decoded_values=data.decode('utf-8')
        print(data.decode('utf-8'))
        
    #inputting the value in the sheet
    if(decoded_values!=-1):
        empty_row+=1;
   
        sheet.update_cell(empty_row,1,10)
        
        sheet.update_cell(empty_row,2,decoded_values);
        
    decoded_values=-1


