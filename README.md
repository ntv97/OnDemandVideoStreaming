This is a simple program written in C where the client is able to watch videos by requesting the .mp4 from the server (that the server has stored in their folder). It is on demand so we are able to use TCP and the server transfers the video over the the client and will watch it by calling a python program which will stream using OpenCV.

To run do "make all" then run the server and client.
Make sure to set the PYTHONPATH to "export PYTHONPATH=pathwherethePlayVideo.pyislocated" 
