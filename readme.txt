Hi!
This ROS pkg saves the data from a usb cam as png files under data/train.
The images can then be used for classify the objects and win at life.
To use it change the parameters in the launch-file run.launch (launch/run.launch)
with your pc name and the name of you catkin workspace where the pkg is located.
If you're unsure then right-click on a this file and see the path hereto,
this should look like /home/pc_name/workspace_name.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Parameters:
pc_name is a string, default usr
workspace_name is also a string, default catkin_ws
