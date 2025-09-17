from flask import Flask   
from markupsafe import escape
from flask import *
import time
import requests
from PIL import Image

app = Flask(__name__)

#Not the safest option but it is what it is
@app.before_request
def limit_access():
    if request.remote_addr != "add ip of esp32":
        abort(403)


@app.route("/videos/",methods = ['POST','GET'])
def saveVid():
    if request.method == 'POST':
        data = request.data
        with open("imageuploads/uploadedImage.jpeg","wb") as file:
            file.write(data)
        time.sleep(5)
        detectImage()
        #downloadVid()
        return "<p>UPLOADED IMAGE</p>"
    else:
        return "<p>FAILED</p>"
        abort(401)



@app.route("/imageuploads/<path:image>")
def returnIMG(image):
    return send_from_directory('imageuploads',image)





def detectImage():
    with Image.open("imageuploads/uploadedImage.jpeg") as image:
        image.rotate(180).show()    
        option = input("Enter yes / no").lower()
        
        while option!="yes" and option !="no":
            option = input("Incorrect input").lower()
        print("im here")
        sendSignal(option)
        image.close()
       

def sendSignal(value):
    url = "http://ip addr of esp32/data/"
    try:
        response = requests.post(url,data=value,headers={'Content-Type': 'text/plain'})
        print("sent data")
    except:
        print("Unable to send request")


if __name__ == "__main__":
    app.run(host="0.0.0.0",port=5061)
#remove this line and run with flask if you want localhost 
#or just change to 127.0.0.1


