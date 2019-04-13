echo "Configuring and building Thirdparty/DBoW2 ..."

cd Thirdparty/DBoW2
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(($(nproc) - 1))

cd ../../g2o

echo "Configuring and building Thirdparty/g2o ..."

mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(($(nproc) - 1))

cd ../../../

echo "Uncompress vocabulary ..."

cd Vocabulary
tar -xf ORBvoc.txt.tar.gz
cd ..

projectdir=$(readlink -f $(dirname '$0'))
export ROS_PACKAGE_PATH=${ROS_PACKAGE_PATH}:$projectdir/Examples/ROS
export ROS_DISTRO=melodic
export CMAKE_PREFIX_PATH=/opt/ros/${ROS_DISTRO}:/opt/ros/${ROS_DISTRO}/share

echo "Configuring and building ORB_SLAM2 ..."

mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(($(nproc) - 1))

cd ..

cd Vocabulary
echo "Converting vocabulary to binary version"
./bin_vocabulary
cd ..

echo "Building ROS nodes"

cd Examples/ROS/ORB_SLAM2
mkdir build
cd build
cmake .. -DROS_BUILD_TYPE=Release
make -j$(($(nproc) - 1))
