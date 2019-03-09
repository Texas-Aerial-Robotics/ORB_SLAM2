roslaunch video_stream_opencv stereo.launch &
camera_pid=$!
projectdir=$(readlink -f $(dirname '$0'))
export ROS_PACKAGE_PATH=${ROS_PACKAGE_PATH}:$projectdir/Examples/ROS
echo ${ROS_PACKAGE_PATH}
while ! rostopic list | grep "/left/image_raw"; do
sleep 1
done
rosrun ORB_SLAM2 Stereo $projectdir/Vocabulary/ORBvoc.bin $projectdir/EuRoC.yaml 1
kill -9 $camera_pid
