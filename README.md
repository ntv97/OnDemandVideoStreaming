This is a simple program written in C where the client is able to watch videos by requesting the .mp4 from the server (that the server has stored in their folder). It is on demand so we are able to use TCP and the server transfers the video over the the client and will watch it by calling a python program which will stream using OpenCV.

To run do "make all" then run the server and client.
Make sure to set the PYTHONPATH to "export PYTHONPATH=pathwherethePlayVideo.pyislocated" 


<img width="313" alt="Screenshot 2023-09-22 at 7 24 19 PM" src="https://github.com/ntv97/OnDemandVideoStreaming/assets/79057587/250ff4c1-bd54-45ff-8cfe-b01876fd4c39">
<img width="630" alt="Screenshot 2023-09-22 at 7 24 32 PM" src="https://github.com/ntv97/OnDemandVideoStreaming/assets/79057587/43c02bc2-2342-4c44-b8f8-ce7aa9d9d500">

<img width="797" alt="Screenshot 2023-09-22 at 7 26 33 PM" src="https://github.com/ntv97/OnDemandVideoStreaming/assets/79057587/d2b9d1cd-7318-44b7-a242-d501d2fea075">
