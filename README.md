# CamLink

# Note
-> Create a folder called imageuploads on your flask server directory to keep a place to store the images received by the server.

-> The program's only security measure is based on rejecting all requests except the ones from the ESP-32. This is a security vulnerability as a person can spoof their ip-address to be the address of the esp-32, allowing them access into the server. A measure to prevent this is to create a firewall rule in production.

# Project Description
Upon clicking a button, the ESP-32 camera takes a picture and uploads it onto a flask-server. The user has the option to either "accept" or "reject" the image. This will lead to either the green led (for yes) or the red led to glow. The flask server relays this information to the web-server on the esp-32

# Details about project
I needed to figure out how to use the ESP-32 Wrover kit camera, so I decided that this project could be a good way to understand it. I found the necessary configurations from the datasheet provided by the creators of this particular esp-32, which made it easy to configure it.

-> The flask server provided is pretty self-explanatory as it only has a couple of routes to it.

-> To  open the image and rotate it(the camera captures pictures upside down), I used the Python Imaging Library

-> To further improve this project, one could possibly create a simple AI-model and train it on a face or object. For now, however, it just relies on the user's input.

# Circuit 
Button Connected to GPIO pin 2
Green led connected to GPIO pin 12 and red led connected to GPIO pin 15. (Along with necessary resistors).
